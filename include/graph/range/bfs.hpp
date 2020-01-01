//
//	Author: J. Phillip Ratzloff
//
// inspired by bfs_range.hpp from: BGL17
//
// breadth-first search graph algorithms for vertices and edges.
//
// The algorithms are designed to work with both directed & undirected graphs
// by using general functions such as vertex() and edges() instead of out_vertex()
// and out_edges().
//
// The ranges have the following assumptions and behavior
//  1.  The graph structure is assumed to remain stable during the duration of iteration.
//  2.  The depth is a 1-based integer. A value of 0 indicates there is nothing visited.
//  3.  The state of the traversal is in the range object. Calling begin() returns the
//      current state, not the beginning of the range.
//
// The bfs_edge_range iterators have is_back_edge() and back_vertex() which are used
// on terminal vertices in the search. A terminal is a vertex that hasn't been visited
// yet and has no edges to search. When this is true, a pseudo edge is used which is just
// the end(g,u) edge for the vertex. When is_back_edge() is true, back_vertex() should
// be used to retreive the vertex that was traversed from most recentely. This must
// be used as the edge returned is invalid.
//
// TODO
//  1x  Add designation for forward range
//  2x  Add concepts for requirements
//      ax  integer vertex_key
//      bx  vertices in contiguous space (vertex or array)
//  3.  Test with const graph
//  5.  Add range functions to create the range
//  6.  [SG19] Check integer-based performance vs existing iterator-based performance
//  7.  [SG19] Support graphs with non-consecutive integers
//  8.  Test with array<>
//  9.  Test with unordered graph
//
// ISSUES / QUESTIONS
//  1.  Range holds the state, not the iterator. is there a better design?
//      a.  begin() returns current state of iteration (non-standard)
//      b.  range is potentially heavy-weight, based on the number of vertices in the graph
//          and the longest DFS path.
//      c.  the current design could be useful for multi-threading, assuming the queue &
//          visited members are guarded with locks.
//  2.  Will BFS ranges will work with centrality, or is a visitor needed? Similar for
//      Weighted Betweeness. - Matt Galati
//  3.  Is there any additional work require to make this more compatible with Ranges?
//

#include "../graph_fwd.hpp"
#include <queue>
#include <vector>

#if !defined(GRAPH_BFS_HPP)
#  define GRAPH_BFS_HPP

namespace std::graph {

//----------------------------------------------------------------------------------------
/// breadth-first search range for vertices, given a single seed vertex.
///
template <searchable_graph_c G, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ::ranges::contiguous_range<vertex_range_t<G>> class bfs_vertex_range {
  enum three_colors : int8_t {
    white, // undiscovered
    grey,  // discovered, but not visited (in queue)
    black  // visited
  };

  struct queue_elem {
    vertex_iterator_t<G> u;
    size_t               depth = 0;
  };
  using queue_alloc = typename allocator_traits<typename A>::template rebind_alloc<queue_elem>;
  using queue_type  = queue<queue_elem, deque<queue_elem, queue_alloc>>;

public:
  bfs_vertex_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph), queue_(alloc), visited_(vertices_size(graph), white, alloc), alloc_(alloc) {
    if (seed != std::graph::end(graph_)) {
      queue_.push(queue_elem{seed, 1});
      visited_[vertex_key(graph_, *seed)] = grey;
    }
  }

  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const_iterator const&) = default;
    const_iterator(bfs_vertex_range& bfs, bool end_iter = false) : bfs_(&bfs), elem_{std::graph::end(bfs.graph_)} {
      if (!end_iter && !bfs.queue_.empty())
        elem_ = bfs.queue_.front();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    vertex_t<G> const&         operator*() const { return *elem_.u; }
    const_vertex_iterator_t<G> operator->() const { return elem_.u; }

    const_iterator& operator++() {
      queue_elem elem_ = bfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const_iterator const& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const_iterator const& rhs) const { return !operator==(rhs); }

    size_t depth() const { return bfs_->queue_.empty() ? 0 : bfs_->queue_.front().depth; }

  protected:
    bfs_vertex_range* bfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    queue_elem        elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;

    iterator() = default;
    iterator(const_iterator&& iter) : const_iterator(move(iter)) {}
    iterator(const_iterator const& iter) : const_iterator(iter) {}
    iterator(bfs_vertex_range& bfs) : const_iterator(bfs) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(iterator const& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    vertex_t<G>&         operator*() { return *this->elem_.u; }
    vertex_iterator_t<G> operator->() const { return this->elem_.u; }

    iterator& operator++() {
      this->elem_ = this->bfs_->advance();
      return *this;
    }

    iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }
  };

public:
  iterator       begin() { return iterator(*this); }
  const_iterator begin() const { return const_iterator(*this); }
  const_iterator cbegin() const { return const_iterator(*this); }

  iterator       end() { return const_iterator(*this, true); }
  const_iterator end() const { return const_iterator(*this, true); }
  const_iterator cend() const { return const_iterator(*this, true); }

protected:
  queue_elem advance() {
    if (!queue_.empty()) {
      // get current vertex & add neighbors
      // neighbor addition is deferred until we've visited the vertex so other
      // vertices connected to the neighbor have already identified it, to keep
      // the queue smaller
      auto [u, depth]       = queue_.front();
      vertex_key_t<G> u_key = vertex_key(graph_, *u);
      visited_[u_key]       = black;
      push_neighbors(u, depth + 1);
      queue_.pop();

      while (!queue_.empty()) {
        auto [u, depth]       = queue_.front();
        vertex_key_t<G> u_key = vertex_key(graph_, *u);

        if (visited_[u_key] == grey)
          return queue_.front(); // visit it

        // already visited from diff neighbor?
        // (will this condition ever occur?)
        assert(visited_[u_key] == black);
        queue_.pop();
      }
    }
    return queue_elem{std::graph::end(graph_), 0};
  }

  void push_neighbors(vertex_iterator_t<G> u, size_t depth) {
    vertex_edge_iterator_t<G> uv_end = std::graph::end(graph_, *u);
    for (vertex_edge_iterator_t<G> uv = std::graph::begin(graph_, *u); uv != uv_end; ++uv) {
      vertex_iterator_t<G> v     = vertex(graph_, *uv, *u);
      vertex_key_t<G>      v_key = vertex_key(graph_, *v);
      if (visited_[v_key] == white) {
        queue_.push(queue_elem{v, depth});
        visited_[v_key] = grey;
      }
    }
  }

private:
  G&                   graph_;
  queue_type           queue_;
  vector<three_colors> visited_;
  A                    alloc_;
};


//----------------------------------------------------------------------------------------
/// breadth-first search range for edges, given a single seed vertex.
///
/// requires bi-directional edges to get last edge on a vertex
///
template <searchable_graph_c G, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ::ranges::contiguous_range<vertex_range_t<G>>&& ::ranges::bidirectional_range<
      vertex_edge_range_t<G>> class bfs_edge_range {
  enum colors : int8_t {
    white, // undiscovered
    grey,  // discovered (in-process)
    black  // visited; all edges have been visited
  };

  struct queue_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
    size_t                    depth = 0;
  };
  using queue_alloc = typename allocator_traits<typename A>::template rebind_alloc<queue_elem>;
  using queue_type  = queue<queue_elem, deque<queue_elem, queue_alloc>>;

public:
  bfs_edge_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph), queue_(alloc), visited_(vertices_size(graph), white, alloc), alloc_(alloc) {
    if (seed != std::graph::end(graph_)) {
      push_neighbors(std::graph::end(graph_), seed, 1);
    }
  }

  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = vertex_t<G>;
    using reference         = value_type const&;

    using vertex_type           = const_vertex_t<G>;
    using vertex_reference_type = vertex_type&;
    using vertex_iterator_type  = const_vertex_iterator_t<G>;

    using edge_type                 = const_edge_t<G>;
    using edge_reference_type       = edge_type&;
    using vertex_edge_iterator_type = const_vertex_edge_iterator_t<G>;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const_iterator const&) = default;
    const_iterator(bfs_edge_range& bfs, bool end_iter = false)
          : bfs_(&bfs), elem_{std::graph::end(bfs.graph_), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !bfs.queue_.empty())
        elem_ = bfs.queue_.front();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    reference                 operator*() const { return *elem_.uv; }
    vertex_edge_iterator_type operator->() const { return elem_.uv; }

    const_iterator& operator++() {
      queue_elem elem_ = bfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const_iterator const& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const_iterator const& rhs) const { return !operator==(rhs); }

    size_t               depth() const { return bfs_->queue_.empty() ? 0 : bfs_->queue_.front().depth; }
    bool                 is_back_edge() const { return bfs_->is_back_edge(elem_); }
    bool                 is_path_end() const { return !out_exists(); }
    vertex_iterator_type out_vertex() const { return vertex(this->bfs_->graph_, *this->elem_.uv, *this->elem_.u); }
    vertex_iterator_type back_vertex() const { return out_exists() ? out_vertex() : elem_.u; }

  protected:
    bool out_exists() const { return bfs_->out_exists(elem_); }
    bool is_out_visited() const { return bfs_->is_out_visited(elem_); }

  protected:
    bfs_edge_range* bfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    queue_elem      elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = vertex_t<G>;
    using reference         = value_type&;

    using vertex_type           = vertex_t<G>;
    using vertex_reference_type = vertex_type&;
    using vertex_iterator_type  = vertex_iterator_t<G>;

    using edge_type                 = edge_t<G>;
    using edge_reference_type       = edge_type&;
    using vertex_edge_iterator_type = vertex_edge_iterator_t<G>;

    iterator() = default;
    iterator(const_iterator&& iter) : const_iterator(move(iter)) {}
    iterator(const_iterator const& iter) : const_iterator(iter) {}
    iterator(bfs_edge_range& bfs) : const_iterator(bfs) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(iterator const& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    edge_type&                operator*() { return *this->elem_.uv; }
    vertex_edge_iterator_type operator->() const { return this->elem_.uv; }

    iterator& operator++() {
      this->elem_ = this->bfs_->advance();
      return *this;
    }

    iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    vertex_iterator_type out_vertex() const { return vertex(this->bfs_->graph_, *this->elem_.uv, *this->elem_.u); }
    vertex_iterator_type back_vertex() const { return this->out_exists() ? out_vertex() : this->elem_.u; }
  };

public:
  iterator       begin() { return iterator(*this); }
  const_iterator begin() const { return const_iterator(*this); }
  const_iterator cbegin() const { return const_iterator(*this); }

  iterator       end() { return const_iterator(*this, true); }
  const_iterator end() const { return const_iterator(*this, true); }
  const_iterator cend() const { return const_iterator(*this, true); }

protected:
  bool out_exists(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    return uv != std::graph::end(graph_, *u);
  }

  bool is_out_visited(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    const_vertex_iterator_t<G> v     = vertex(graph_, *uv, *u);
    vertex_key_t<G>            v_key = vertex_key(graph_, *v);
    return visited_[v_key] >= grey;
  }

  bool is_back_edge(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    return !out_exists(u, uv) || (out_exists(u, uv) && is_out_visited(u, uv));
  }

  bool out_exists(queue_elem const& se) const { return out_exists(se.u, se.uv); }
  bool is_out_visited(queue_elem const& se) const { return is_out_visited(se.u, se.uv); }
  bool is_back_edge(queue_elem const& se) const { return is_back_edge(se.u, se.uv); }

  void visit(vertex_iterator_t<G> u, colors color) {
    vertex_key_t<G> u_key = vertex_key(graph_, *u);
    visited_[u_key]       = color;
  }

  void push_neighbors(vertex_iterator_t<G> u, vertex_iterator_t<G> v, size_t depth) {
    vertex_edge_iterator_t<G> vw     = std::graph::begin(graph_, *v);
    vertex_edge_iterator_t<G> vw_end = std::graph::end(graph_, *v);
    if (vw == vw_end || (edges_size(graph_,*v)==1 && vertex(graph_, *vw, *v) == u)) {
      queue_.push({v, vw_end, depth});
    } else {
      for (; vw != vw_end; ++vw) {
        vertex_iterator_t<G> w = vertex(graph_, *vw, *v);
        if (w != u)
          queue_.push({v, vw, depth});
      }
    }
    visit(v, grey);
  }

  queue_elem advance() {
    while (!queue_.empty()) {
      auto [u, uv, d] = queue_.front();
#    ifdef _DEBUG
      vertex_key_t<G> u_key = vertex_key(graph_, *u);
#    endif
      queue_.pop();
      if (queue_.empty())
        break;

      if (!out_exists(u, uv)) { // orphan vertex, or no out edges?
        visit(u, black);
        return queue_.front(); // next sibling/cousin of uv, or next generation (whatever is in the queue)
      } else {
#    ifdef _DEBUG
        vertex_key_t<G> v_key = vertex_key(graph_, *uv, *u);
#    endif
        if (!is_out_visited(u, uv)) {
          vertex_iterator_t<G> v = vertex(graph_, *uv, *u);
          push_neighbors(u, v, d + 1);
        }

        vertex_edge_iterator_t<G> uv_last = --std::graph::end(graph_, *u);
        if (uv == uv_last) // last edge of u?
          visit(u, black);

        return queue_.front(); // next sibling/cousin of uv, or next generation (whatever is in the queue)
      }
    }

    return queue_elem{std::graph::end(graph_), vertex_edge_iterator_t<G>(), 0};
  }

private:
  G&             graph_;
  queue_type     queue_;
  vector<colors> visited_;
  A              alloc_;
};

} // namespace std::graph

#endif // GRAPH_BFS_HPP
