//
//	Author: J. Phillip Ratzloff
//
// breadth-first search graph algorithms for vertices and edges.
//
// The algorithms are designed to work with both directed & undirected graphs
// by using general functions such as vertex() and edges() instead of out_vertex()
// and out_edges().
//
// Assumes the graph structure remains stable during the duration of iteration.
//
// TODO
//  1.  Add designation for forward range
//  2.  Add concepts for requirements
//      a.  integer vertex_key
//      b.  vertices in contiguous space? (vertex or deque)
//      c.  max vertex key is number of vertices
//  3.  Test with const graph
//  4.  Make more compatible with Ranges
//  5.  Add range functions to create the range
//
// ISSUES / QUESTIONS
//  1.  Range holds the state, not the iterator. is there a better design?
//      a.  begin() returns current state of iteration (non-standard)
//      b.  range is potentially heavy-weight, based on the number of vertices in the graph
//          and the longest DFS path.
//      c.  the current design could be useful for multi-threading, assuming the queue &
//          visited members are guarded with locks.
//

#include "../graph_fwd.hpp"
#include <queue>
#include <vector>

#ifndef GRAPH_BFS_HPP
#  define GRAPH_BFS_HPP

namespace std::graph {

/// breadth-first search range for vertices, given a single seed vertex.
/// begin() returns the current state kept in the range; it should only be called when starting the iteration.
///
template <typename G, typename A = allocator<char>>
class bfs_vertex_range {
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
  bfs_vertex_range(G& graph, vertex_iterator_t<G> seed) : graph_(graph), visited_(vertices_size(graph), white) {
    if (seed != std::graph::end(graph_)) {
      queue_.push(queue_elem{seed, 0});
      visited_[vertex_key(graph_, *seed)] = grey;
    }
  }

  class const_iterator {
  public:
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
  iterator       begin() { return iterator(*this); } // begin() shouldn't be called after iterating starts on range
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
      push_neighbors(u, depth);
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
      vertex_iterator_t<G> v     = vertex(graph_, *uv);
      vertex_key_t<G>      v_key = vertex_key(graph_, *v);
      if (visited_[v_key] == white) {
        queue_.push(queue_elem{v, depth + 1});
        visited_[v_key] = grey;
      }
    }
  }

private:
  G&                   graph_;
  queue_type           queue_;
  vector<three_colors> visited_;
};


/// breadth-first search range for edges, given a single seed vertex.
/// begin() returns the current state kept in the range; it should only be called when starting the iteration.
///
template <typename G, typename A = allocator<char>>
class bfs_edge_range {
  struct queue_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
  };
  using queue_alloc = typename allocator_traits<typename A>::template rebind_alloc<queue_elem>;
  using queue_type  = queue<queue_elem, deque<queue_elem, queue_alloc>>;

public:
  bfs_edge_range(G& graph, vertex_iterator_t<G> seed) : graph_(graph), visited_(vertices_size(graph)) {
    if (seed != std::graph::end(graph_)) {
      queue_.push(queue_elem{seed, std::graph::begin(graph_, *seed)});
      visited_[vertex_key(graph_, *seed)] = true;
    }
  }

  class const_iterator {
  public:
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

    edge_t<G> const&                operator*() const { return *elem_.uv; }
    const_vertex_edge_iterator_t<G> operator->() const { return elem_.uv; }

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

    size_t               depth() const { return bfs_->queue_.size(); }
    vertex_iterator_t<G> in_vertex() const { return elem_.u; }
    bool                 is_back_edge() const { return elem_.uv == std::graph::end(bfs_->graph_, *elem_.u); }

    bool is_vertex_visited() const { // Has the [out] vertex been visited yet?
      if (is_back_edge())
        return false;

      vertex_iterator_t<G> v     = vertex(bfs_->graph_, *elem_.uv);
      vertex_key_t<G>      v_key = vertex_key(graph_, *v);
      return bfs_->visited_[v_key];
    }

  protected:
    bfs_edge_range* bfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    queue_elem      elem_;
  };

  class iterator : public const_iterator {
  public:
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

    edge_t<G>&                operator*() { return *this->elem_.uv; }
    vertex_edge_iterator_t<G> operator->() const { return this->elem_.uv; }

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
  iterator       begin() { return iterator(*this); } // begin() shouldn't be called after iterating starts on range
  const_iterator begin() const { return const_iterator(*this); }
  const_iterator cbegin() const { return const_iterator(*this); }

  iterator       end() { return const_iterator(*this, true); }
  const_iterator end() const { return const_iterator(*this, true); }
  const_iterator cend() const { return const_iterator(*this, true); }

protected:
  queue_elem advance() {
    bool resume = false;
    while (!queue_.empty()) {
      if (resume)
        return queue_.front();

      auto [u, uv] = queue_.front();

      vertex_iterator_t<G>      v      = vertex(graph_, *uv);
      vertex_key_t<G>           v_key  = vertex_key(graph_, *v);
      vertex_edge_iterator_t<G> uv_end = std::graph::end(graph_, *u);

      queue_.pop();
      resume = true;

      // no more edges for u? (will occur if no [out] edges)
      if (uv == uv_end)
        continue;

      // remember next edge to resume on for u
      if (++uv != uv_end)
        queue_.push(queue_elem{u, uv});

      // visit v vertex if we haven't visited it yet
      if (!visited_[v_key]) {
        vertex_edge_iterator_t<G> vw = std::graph::begin(graph_, *v); // may ==end(graph_,*v)
        queue_.push(queue_elem{v, vw});                               // go level deeper in traversal
        visited_[v_key] = true;
      }
    }
    return queue_elem{std::graph::end(graph_), vertex_edge_iterator_t<G>()};
  }

private:
  G&           graph_;
  queue_type   queue_;
  vector<bool> visited_;
};

} // namespace std::graph

#endif // GRAPH_BFS_HPP
