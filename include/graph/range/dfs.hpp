//
//	Author: J. Phillip Ratzloff
//
// inspired by new_dfs_range.hpp from: BGL17
//
// depth-first search graph algorithms for vertices and edges.
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
// The dfs_edge_range iterators have is_back_edge() and back_vertex() which are used
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
//  4.  Make more compatible with Ranges
//  5.  Add range functions to create the range
//  6.  [SG19] Check integer-based performance vs existing iterator-based performance
//  7.  [SG19] Support graphs with non-consecutive integers
//  8.  Test with array<>
//  9.  Test with unordered graph
//  10. const_iterators should return const_vertex_iterator<G> for back_vertex() & out_vertex()
//
// ISSUES / QUESTIONS
//  1.  Range holds the state, not the iterator. is there a better design?
//      a.  begin() returns current state of iteration (non-standard)
//      b.  range is potentially heavy-weight, based on the number of vertices in the graph
//          and the longest DFS path.
//      c.  the current design could be useful for multi-threading, assuming the stack &
//          visited members are guarded with locks.
//  2. Becase indexes are used to access vertices, a deque would also be acceptable in
//     addition to a vector & array. Is there a way to distinguish between a deque and
//     map[int] to avoid using a map using concepts?
//  3.  Is there any additional work require to make this more compatible with Ranges?
//

#include "../graph.hpp"
#include <stack>
#include <vector>

#ifndef GRAPH_DFS_HPP
#  define GRAPH_DFS_HPP

namespace std::graph {


//---------------------------------------------------------------------------------------
/// depth-first search range for vertices, given a single seed vertex.
///

template <searchable_graph_c G, typename A = allocator<char>>
requires uniform_graph_c<G>&&
      integral<vertex_key_t<G>>&& ::ranges::contiguous_range<vertex_range_t<G>> class dfs_vertex_range {

  enum colors : int8_t { white, grey, black };

  struct stack_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
  };
  using stack_alloc = typename allocator_traits<A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

  using visited_alloc = typename allocator_traits<A>::template rebind_alloc<colors>;
  using visited_type  = vector<colors, visited_alloc>;

  using parent_alloc = typename allocator_traits<A>::template rebind_alloc<vertex_iterator_t<G>>;
  using parent_type  = vector<vertex_iterator_t<G>, parent_alloc>;

public:
  dfs_vertex_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph)
        , stack_(alloc)
        , visited_(vertices_size(graph_), white, alloc)
        , parent_(vertices_size(graph_), vertex_end(graph_), alloc) {
    if (seed != vertex_end(graph_)) {
      stack_.push(stack_elem{seed, edge_begin(graph_, *seed)});
      visited_[vertex_key(graph_, *seed)] = grey;
    }
  }

  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = vertex_t<G>;
    using reference         = const value_type&;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const_iterator const&) = default;
    const_iterator(dfs_vertex_range& dfs, bool end_iter = false)
          : dfs_(&dfs), elem_{vertex_end(dfs.graph_), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !dfs.stack_.empty())
        elem_ = dfs.stack_.top();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    reference                  operator*() const { return *elem_.u; }
    const_vertex_iterator_t<G> operator->() const { return elem_.u; }

    const_iterator& operator++() {
      elem_ = dfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const_iterator const& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const_iterator const& rhs) const { return !operator==(rhs); }

    size_t               depth() const { return dfs_->stack_.size(); }
    vertex_iterator_t<G> parent() const { return parent_[vertex_key(graph_, *elem_.u)]; }

  protected:
    dfs_vertex_range* dfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    stack_elem        elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = vertex_t<G>;
    using reference         = value_type&;

    iterator() = default;
    iterator(const_iterator&& iter) : const_iterator(move(iter)) {}
    iterator(const_iterator const& iter) : const_iterator(iter) {}
    iterator(dfs_vertex_range& dfs) : const_iterator(dfs) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(iterator const& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    reference            operator*() { return *this->elem_.u; }
    vertex_iterator_t<G> operator->() const { return this->elem_.u; }

    iterator& operator++() {
      this->elem_ = this->dfs_->advance();
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
  stack_elem advance() {
    while (!stack_.empty()) {
      auto [u, uv] = stack_.top();
      stack_.pop();

      // advance uv to next unvisited v vertex
      vertex_edge_iterator_t<G> uv_end = edge_end(graph_, *u);
      vertex_iterator_t<G>      v      = vertex_iterator_t<G>();
      vertex_key_t<G>           v_key  = vertex_key_t<G>();
      for (; uv != uv_end; ++uv) {
        v     = vertex(graph_, *uv, *u);
        v_key = vertex_key(graph_, *uv, *u);
        if (visited_[v_key] == white)
          break;
      }

      if (uv == uv_end) {
        vertex_key_t<G> u_key = vertex_key(graph_, *u);
        visited_[u_key]       = black;
      } else {                                                 // (visited_[v_key] == white)
        stack_.push(stack_elem{u, uv});                        // remember next edge to resume on for u
        vertex_edge_iterator_t<G> vw = edge_begin(graph_, *v); // may ==end(graph_,*v)
        stack_.push(stack_elem{v, vw});                        // go level deaper in traversal
        visited_[v_key] = grey;
        parent_[v_key]  = u;
        return stack_.top();
      }
    }
    return stack_elem{vertex_end(graph_), vertex_edge_iterator_t<G>()};
  }

private:
  G&           graph_;
  stack_type   stack_;   // stack<stack_elem, >
  visited_type visited_; // vector<colors, >
  parent_type  parent_;  // vector<vertex_iterator_t<G>, >
};


//---------------------------------------------------------------------------------------
/// depth-first search range for edges, given a single seed vertex.
///
template <searchable_graph_c G, typename A = allocator<char>>
requires uniform_graph_c<G> /*directed_graph_c<G> */&&
      integral<vertex_key_t<G>>&& ::ranges::contiguous_range<vertex_range_t<G>> class dfs_edge_range {

  enum colors : int8_t { white, grey, black };

  struct stack_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
  };

  using stack_alloc = typename allocator_traits<A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

  using visited_alloc = typename allocator_traits<A>::template rebind_alloc<colors>;
  using visited_type  = vector<colors, visited_alloc>;

  using parent_alloc = typename allocator_traits<A>::template rebind_alloc<vertex_iterator_t<G>>;
  using parent_type  = vector<vertex_iterator_t<G>, parent_alloc>;

public:
  dfs_edge_range(G& graph, vertex_iterator_t<G> seed, A alloc = A())
        : graph_(graph)
        , stack_(alloc)
        , visited_(vertices_size(graph), white, alloc)
        , parent_(vertices_size(graph), vertex_end(graph), alloc) {
    if (seed != vertex_end(graph_)) {
      stack_.push({seed, edge_begin(graph_, *seed)});
      visit(seed, grey);
    }
  }

public:
  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = edge_t<G>;
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
    const_iterator(dfs_edge_range& dfs, bool end_iter = false)
          : dfs_(&dfs), elem_{vertex_end(dfs.graph_), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !dfs.stack_.empty())
        elem_ = dfs.stack_.top();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    edge_reference_type       operator*() const { return *elem_.uv; }
    vertex_edge_iterator_type operator->() const { return elem_.uv; }

    const_iterator& operator++() {
      elem_ = dfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const_iterator const& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const_iterator const& rhs) const { return !operator==(rhs); }

    vertex_iterator_type in_vertex() const { return elem_.u; }
    vertex_iterator_type out_vertex() const { return vertex(dfs_->graph_, *elem_.uv, *elem_.u); }
    vertex_iterator_type back_vertex() const { return out_vertex(); }

    size_t depth() const { return dfs_->stack_.size(); }

    bool is_path_end() const { return !out_exists(); }
    bool is_back_edge() const { // No outgoing edge, or outgoing vertex has been visised (undirected, or directed cycle)
      return !out_exists() || (out_exists() && is_out_visited());
    }

  protected:
    bool out_exists() const { return dfs_->out_exists(elem_); }
    bool is_out_visited() const { return dfs_->is_out_visited(elem_); }

  protected:
    dfs_edge_range* dfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    stack_elem      elem_;
  };


  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = edge_t<G>;
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
    iterator(dfs_edge_range& dfs) : const_iterator(dfs) {}

    iterator& operator=(iterator&& rhs) {
      const_iterator::operator=(move(rhs));
      return *this;
    }
    iterator& operator=(iterator const& rhs) {
      const_iterator::operator=(rhs);
      return *this;
    }

    edge_reference_type       operator*() { return *this->elem_.uv; }
    vertex_edge_iterator_type operator->() const { return this->elem_.uv; }

    iterator& operator++() {
      this->elem_ = this->dfs_->advance();
      return *this;
    }

    iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    vertex_iterator_type in_vertex() const { return this->elem_.u; }
    vertex_iterator_type out_vertex() const { return vertex(this->dfs_->graph_, *this->elem_.uv, *this->elem_.u); }
    vertex_iterator_type back_vertex() const { return out_vertex(); }
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
    return uv != edge_end(graph_, *u);
  }

  bool is_out_visited(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    const_vertex_iterator_t<G> v     = vertex(graph_, *uv, *u);
    vertex_key_t<G>            v_key = vertex_key(graph_, *v);
    return visited_[v_key] >= grey;
  }

  bool is_back_edge(const_vertex_iterator_t<G> u, const_vertex_edge_iterator_t<G> uv) const {
    return !out_exists(u, uv) || (out_exists(u, uv) && is_out_visited(u, uv));
  }

  bool out_exists(stack_elem const& se) const { return out_exists(se.u, se.uv); }
  bool is_out_visited(stack_elem const& se) const { return is_out_visited(se.u, se.uv); }
  bool is_back_edge(stack_elem const& se) const { return is_back_edge(se.u, se.uv); }

  bool is_parent(vertex_iterator_t<G> u, vertex_edge_iterator_t<G> uv) const {
    vertex_key_t<G>      u_key = vertex_key(graph_, *u);
    vertex_iterator_t<G> v     = vertex(graph_, *uv, *u);
    return v == parent_[u_key];
  }

  stack_elem advance() {
    while (!stack_.empty()) {
      if (!out_exists(stack_.top())) { // orphan vertex, or no out edges?
        visit(stack_.top().u, black);
        stack_.pop();
        continue;
      } else if (is_out_visited(stack_.top())) { // back edge?
        auto [u, uv]                     = stack_.top();
        vertex_edge_iterator_t<G> uv_end = edge_end(graph_, *u);
        stack_.pop();
        for (++uv; uv != uv_end && is_parent(u, uv); ++uv)
          ;
        if (uv == uv_end) { // no more edges?
          visit(u, black);
          continue;
        } else {                // next sibling of uv
          stack_.push({u, uv}); // sibling to uv
          break;
        }
      } else { // go down a level
        auto [u, uv]                     = stack_.top();
        vertex_iterator_t<G>      v      = vertex(graph_, *uv, *u);
        vertex_edge_iterator_t<G> vw     = edge_begin(graph_, *v);
        vertex_edge_iterator_t<G> vw_end = edge_end(graph_, *v);

        // scan past parent of v (we have enough info so parent_[] isn't needed)
        for (; vw != vw_end && vertex(graph_, *vw, *v) == u; ++vw)
          ;

        stack_.push({v, vw});
        visit(u, v, grey);
        break;
      }
    }

    if (!stack_.empty())
      return stack_.top();
    else
      return empty_elem();
  }

  void visit(vertex_iterator_t<G> u, colors color) {
    vertex_key_t<G> u_key = vertex_key(graph_, *u);
    visited_[u_key]       = color;
  }

  void visit(vertex_iterator_t<G> parent, vertex_iterator_t<G> u, colors color) {
    vertex_key_t<G> u_key = vertex_key(graph_, *u);
    visited_[u_key]       = color;
    parent_[u_key]        = parent;
  }

  stack_elem empty_elem() const { return stack_elem{vertex_end(graph_), vertex_edge_iterator_t<G>()}; }


private:
  G&           graph_;
  stack_type   stack_;   // stack<stack_elem, >
  visited_type visited_; // vector<colors, >
  parent_type  parent_;  // vector<vertex_iterator_t<G>, >; only needed for undirected graphs
};

} // namespace std::graph

#endif // GRAPH_DFS_HPP
