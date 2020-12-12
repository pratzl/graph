//
//	Author: J. Phillip Ratzloff
//
// inspired by bfs_range.hpp from: BGL17
//
// breadth-first search graph algorithms for vertices and edges.
//
// The algorithms are designed to work with both directed & undirected graphs
// by using general functions such as vertex() and edges() instead of outward_vertex()
// and outward_edges().
//
// The ranges have the following assumptions and behavior
//  1.  The graph structure is assumed to remain stable during the duration of iteration.
//  2.  The depth is a 1-based integer. A value of 0 indicates there is nothing visited.
//  3.  The state of the traversal is in the range object. Calling begin() returns the
//      current state, not the beginning of the range.
//
// The breadth_first_search_edge_range iterators have is_back_edge() and back_vertex() which are used
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

#include "../graph.hpp"
#include <queue>
#include <vector>

#if !defined(GRAPH_BFS_HPP)
#  define GRAPH_BFS_HPP

namespace std::graph {

//----------------------------------------------------------------------------------------
/// breadth-first search range for vertices, given a single seed vertex.
///
// clang-format off
template <searchable_graph G, typename A = allocator<char>>
  requires integral<vertex_key_t<G>> 
        && ranges::random_access_range<vertex_range_t<G>> 
class breadth_first_search_vertex_range
// clang-format on
{
  enum three_colors : int8_t {
    white, // undiscovered
    grey,  // discovered, but not visited (in queue)
    black  // visited
  };

  struct queue_elem {
    vertex_iterator_t<G> u;
    size_t               depth = 0;
  };
  using queue_alloc = typename allocator_traits<A>::template rebind_alloc<queue_elem>;
  using queue_type  = queue<queue_elem, deque<queue_elem, queue_alloc>>;

public:
  breadth_first_search_vertex_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph), queue_(alloc), visited_(vertices_size(graph), white, alloc), alloc_(alloc) {
    if (seed != ranges::end(vertices(graph_))) {
      queue_.push(queue_elem{seed, 1});
      visited_[vertex_key(graph_, seed)] = grey;
    }
  }

  class const_iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_t<G>;
    using pointer           = const_vertex_iterator_t<G>;
    using reference         = const value_type&;
    using difference_type   = typename iterator_traits<const_vertex_iterator_t<G>>::difference_type;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const const_iterator&) = default;
    const_iterator(breadth_first_search_vertex_range& bfs, bool end_iter = false)
          : bfs_(&bfs), elem_{ranges::end(vertices(bfs.graph_))} {
      if (!end_iter && !bfs.queue_.empty())
        elem_ = bfs.queue_.front();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const const_iterator&) = default;

    reference operator*() const { return *elem_.u; }
    pointer   operator->() const { return elem_.u; }

    const_iterator& operator++() {
      elem_ = bfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const const_iterator& rhs) const { return !operator==(rhs); }

    size_t depth() const { return bfs_->queue_.empty() ? 0 : bfs_->queue_.front().depth; }

  protected:
    breadth_first_search_vertex_range* bfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    queue_elem                         elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = vertex_t<G>;
    using pointer           = vertex_iterator_t<G>;
    using reference         = value_type&;
    using difference_type   = typename iterator_traits<vertex_iterator_t<G>>::difference_type;

    iterator() = default;
    iterator(const_iterator&& iter) : const_iterator(move(iter)) {}
    iterator(const const_iterator& iter) : const_iterator(iter) {}
    iterator(breadth_first_search_vertex_range& bfs, bool end_iter = false) : const_iterator(bfs, end_iter) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(const iterator& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    reference operator*() { return *this->elem_.u; }
    pointer   operator->() const { return this->elem_.u; }

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
      vertex_key_t<G> u_key = vertex_key(graph_, u);
      visited_[u_key]       = black;
      push_neighbors(u, depth + 1);
      queue_.pop();

      while (!queue_.empty()) {
        u     = queue_.front().u;
        u_key = vertex_key(graph_, u);

        if (visited_[u_key] == grey)
          return queue_.front(); // visit it

        // already visited from diff neighbor?
        // (will this condition ever occur?)
        assert(visited_[u_key] == black);
        queue_.pop();
      }
    }
    return queue_elem{ranges::end(vertices(graph_)), 0};
  }

  void push_neighbors(vertex_iterator_t<G> u, size_t depth) {
    vertex_edge_iterator_t<G> uv_end = edges_end(graph_, u);
    for (vertex_edge_iterator_t<G> uv = edges_begin(graph_, u); uv != uv_end; ++uv) {
      vertex_iterator_t<G> v     = vertex(graph_, uv, u);
      vertex_key_t<G>      v_key = vertex_key(graph_, v);
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
// clang-format off
template <searchable_graph G, typename A = allocator<char>>
  requires integral<vertex_key_t<G>>
        && ranges::random_access_range<vertex_range_t<G>>
        && ranges::bidirectional_range<vertex_edge_range_t<G>> 
class breadth_first_search_edge_range
// clang-format on
{
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
  using queue_alloc = typename allocator_traits<A>::template rebind_alloc<queue_elem>;
  using queue_type  = queue<queue_elem, deque<queue_elem, queue_alloc>>;

public:
  breadth_first_search_edge_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph), queue_(alloc), visited_(vertices_size(graph), white, alloc), alloc_(alloc) {
    if (seed != ranges::end(vertices(graph_))) {
      push_neighbors(ranges::end(vertices(graph_)), seed, 1);
    }
  }

  class const_iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_t<G>;
    using pointer           = const_vertex_edge_iterator_t<G>;
    using reference         = const value_type&;

    using vertex_type           = vertex_t<G> const;
    using vertex_reference_type = vertex_type&;
    using vertex_iterator_type  = const_vertex_iterator_t<G>;

    using edge_type                 = edge_t<G> const;
    using edge_reference_type       = edge_type&;
    using vertex_edge_iterator_type = const_vertex_edge_iterator_t<G>;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const const_iterator&) = default;
    const_iterator(breadth_first_search_edge_range& bfs, bool end_iter = false)
          : bfs_(&bfs), elem_{ranges::end(vertices(bfs.graph_)), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !bfs.queue_.empty())
        elem_ = bfs.queue_.front();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const const_iterator&) = default;

    reference operator*() const { return *elem_.uv; }
    pointer   operator->() const { return elem_.uv; }

    const_iterator& operator++() {
      elem_ = bfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const const_iterator& rhs) const { return !operator==(rhs); }

    size_t depth() const { return bfs_->queue_.empty() ? 0 : bfs_->queue_.front().depth; }
    bool   is_back_edge() const { return bfs_->is_back_edge(elem_); }
    bool   is_path_end() const { return !outward_exists(); }

    vertex_iterator_type outward_vertex() const { return vertex(this->bfs_->graph_, *this->elem_.uv, *this->elem_.u); }
    vertex_iterator_type inward_vertex() const { return vertex(this->bfs_->graph_, *this->elem_.uv, outward_vertex()); }
    vertex_iterator_type back_vertex() const { return outward_exists() ? outward_vertex() : elem_.u; }

  protected:
    bool outward_exists() const { return bfs_->outward_exists(elem_); }
    bool is_outward_visited() const { return bfs_->is_outward_visited(elem_); }

  protected:
    breadth_first_search_edge_range* bfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    queue_elem                       elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = input_iterator_tag;
    using value_type        = edge_t<G>;
    using pointer           = vertex_edge_iterator_t<G>;
    using reference         = value_type&;

    using vertex_type           = vertex_t<G>;
    using vertex_reference_type = vertex_type&;
    using vertex_iterator_type  = vertex_iterator_t<G>;

    using edge_type                 = edge_t<G>;
    using edge_reference_type       = edge_type&;
    using vertex_edge_iterator_type = vertex_edge_iterator_t<G>;

    iterator() = default;
    iterator(const_iterator&& iter) : const_iterator(move(iter)) {}
    iterator(const const_iterator& iter) : const_iterator(iter) {}
    iterator(breadth_first_search_edge_range& bfs) : const_iterator(bfs) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(const iterator& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    reference operator*() { return *this->elem_.uv; }
    pointer   operator->() const { return this->elem_.uv; }

    iterator& operator++() {
      this->elem_ = this->bfs_->advance();
      return *this;
    }

    iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    vertex_iterator_type outward_vertex() const { return vertex(this->bfs_->graph_, this->elem_.uv, this->elem_.u); }
    vertex_iterator_type inward_vertex() const { return vertex(this->bfs_->graph_, this->elem_.uv, outward_vertex()); }
    vertex_iterator_type back_vertex() const { return this->outward_exists() ? outward_vertex() : this->elem_.u; }
  };

public:
  iterator       begin() { return iterator(*this); }
  const_iterator begin() const { return const_iterator(*this); }
  const_iterator cbegin() const { return const_iterator(*this); }

  iterator       end() { return const_iterator(*this, true); }
  const_iterator end() const { return const_iterator(*this, true); }
  const_iterator cend() const { return const_iterator(*this, true); }

protected:
  bool outward_exists(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    return uv != edges_end(graph_, u);
  }

  bool is_outward_visited(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    const_vertex_iterator_t<G> v     = vertex(graph_, uv, u);
    vertex_key_t<G>            v_key = vertex_key(graph_, v);
    return visited_[v_key] >= grey;
  }

  bool is_back_edge(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    return !outward_exists(u, uv) || (outward_exists(u, uv) && is_outward_visited(u, uv));
  }

  bool outward_exists(const queue_elem& se) const { return outward_exists(se.u, se.uv); }
  bool is_outward_visited(const queue_elem& se) const { return is_outward_visited(se.u, se.uv); }
  bool is_back_edge(const queue_elem& se) const { return is_back_edge(se.u, se.uv); }

  void visit(vertex_iterator_t<G> u, colors color) {
    vertex_key_t<G> u_key = vertex_key(graph_, u);
    visited_[u_key]       = color;
  }

  void push_neighbors(vertex_iterator_t<G> u, vertex_iterator_t<G> v, size_t depth) {
    vertex_edge_iterator_t<G> vw     = edges_begin(graph_, v);
    vertex_edge_iterator_t<G> vw_end = edges_end(graph_, v);
    if (vw == vw_end || (edges_size(graph_, v) == 1 && vertex(graph_, vw, v) == u)) {
      queue_.push({v, vw_end, depth});
    } else {
      for (; vw != vw_end; ++vw) {
        vertex_iterator_t<G> w = vertex(graph_, vw, v);
        if (w != u)
          queue_.push({v, vw, depth});
      }
    }
    visit(v, grey);
  }

  queue_elem advance() {
    while (!queue_.empty()) {
      auto [u, uv, d] = queue_.front();
#  ifdef _DEBUG
      vertex_key_t<G> u_key = vertex_key(graph_, u);
#  endif
      queue_.pop();
      if (queue_.empty())
        break;

      if (!outward_exists(u, uv)) { // orphan vertex, or no out edges?
        visit(u, black);
        return queue_.front(); // next sibling/cousin of uv, or next generation (whatever is in the queue)
      } else {
#  ifdef _DEBUG
        vertex_key_t<G> v_key = vertex_key(graph_, uv, u);
#  endif
        if (!is_outward_visited(u, uv)) {
          vertex_iterator_t<G> v = vertex(graph_, uv, u);
          push_neighbors(u, v, d + 1);
        }

        vertex_edge_iterator_t<G> uv_last = --edges_end(graph_, u);
        if (uv == uv_last) // last edge of u?
          visit(u, black);

        return queue_.front(); // next sibling/cousin of uv, or next generation (whatever is in the queue)
      }
    }

    return queue_elem{ranges::end(vertices(graph_)), vertex_edge_iterator_t<G>(), 0};
  }

private:
  G&             graph_;
  queue_type     queue_;
  vector<colors> visited_;
  A              alloc_;
};

} // namespace std::graph

#endif // GRAPH_BFS_HPP
