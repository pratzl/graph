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
// TODO
//  1.  Add designation for forward range
//  2.  Add concepts for requirements
//      a.  integer vertex_key
//      b.  vertices in contiguous space? (vertex or deque)
//      c.  max vertex key is number of vertices
//  3.  Test with const graph
//  4.  Make more compatible with Ranges
//  5.  Add range functions to create the range
//  6.  [SG19] Check integer-based performance vs existing iterator-based performance
//  7.  [SG19] Support graphs with non-consecutive integers
//
// ISSUES / QUESTIONS
//  1.  Range holds the state, not the iterator. is there a better design?
//      a.  begin() returns current state of iteration (non-standard)
//      b.  range is potentially heavy-weight, based on the number of vertices in the graph
//          and the longest DFS path.
//      c.  the current design could be useful for multi-threading, assuming the stack &
//          visited members are guarded with locks.
//

#include "../graph_fwd.hpp"
#include <stack>
#include <vector>

#ifndef GRAPH_DFS_HPP
#  define GRAPH_DFS_HPP

namespace std::graph {


/// depth-first search range for vertices, given a single seed vertex.
///
template <searchable_graph_c G, typename A = allocator<char>>
requires integral<vertex_key_t<G>> && ::ranges::contiguous_range<vertex_range_t<G>> 
class dfs_vertex_range 
{
  struct stack_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
  };
  using stack_alloc = typename allocator_traits<typename A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

public:
  dfs_vertex_range(G& graph, vertex_iterator_t<G> seed) : graph_(graph), visited_(vertices_size(graph)) {
    if (seed != std::graph::end(graph_)) {
      stack_.push(stack_elem{seed, std::graph::begin(graph_, *seed)});
      visited_[vertex_key(graph_, *seed)] = true;
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
          : dfs_(&dfs), elem_{std::graph::end(dfs.graph_), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !dfs.stack_.empty())
        elem_ = dfs.stack_.top();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    reference                  operator*() const { return *elem_.u; }
    const_vertex_iterator_t<G> operator->() const { return elem_.u; }

    const_iterator& operator++() {
      stack_elem elem_ = dfs_->advance();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const_iterator const& rhs) const { return elem_.u == rhs.elem_.u; }
    bool operator!=(const_iterator const& rhs) const { return !operator==(rhs); }

    size_t depth() const { return dfs_->stack_.size(); }

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
      vertex_edge_iterator_t<G> uv_end = std::graph::end(graph_, *u);
      vertex_iterator_t<G>      v      = vertex_iterator_t<G>();
      vertex_key_t<G>           v_key  = vertex_key_t<G>();
      for (; uv != uv_end; ++uv) {
        v     = vertex(graph_, *uv);
        v_key = vertex_key(graph_, *v);
        if (!visited_[v_key])
          break;
      }

      if (uv != uv_end) {
        stack_.push(stack_elem{u, uv});                               // remember next edge to resume on for u
        vertex_edge_iterator_t<G> vw = std::graph::begin(graph_, *v); // may ==end(graph_,*v)
        stack_.push(stack_elem{v, vw});                               // go level deaper in traversal
        visited_[v_key] = true;
        return stack_.top();
      }
    }
    return stack_elem{std::graph::end(graph_), vertex_edge_iterator_t<G>()};
  }

private:
  G&           graph_;
  stack_type   stack_;
  vector<bool> visited_;
};


/// depth-first search range for edges, given a single seed vertex.
///
template <searchable_graph_c G, typename A = allocator<char>>
requires integral<vertex_key_t<G>> && ::ranges::contiguous_range<vertex_range_t<G>> 
class dfs_edge_range {
  struct stack_elem {
    vertex_iterator_t<G>      u;
    vertex_edge_iterator_t<G> uv;
  };
  using stack_alloc = typename allocator_traits<typename A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

public:
  dfs_edge_range(G& graph, vertex_iterator_t<G> seed) : graph_(graph), visited_(vertices_size(graph)) {
    if (seed != std::graph::end(graph_)) {
      stack_.push(stack_elem{seed, std::graph::begin(graph_, *seed)});
      visited_[vertex_key(graph_, *seed)] = true;
    }
  }

  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;

    const_iterator()                      = default;
    const_iterator(const_iterator&&)      = default;
    const_iterator(const_iterator const&) = default;
    const_iterator(dfs_edge_range& dfs, bool end_iter = false)
          : dfs_(&dfs), elem_{std::graph::end(dfs.graph_), vertex_edge_iterator_t<G>()} {
      if (!end_iter && !dfs.stack_.empty())
        elem_ = dfs.stack_.top();
    }

    const_iterator& operator=(const_iterator&&) = default;
    const_iterator& operator=(const_iterator const&) = default;

    edge_t<G> const&                operator*() const { return *elem_.uv; }
    const_vertex_edge_iterator_t<G> operator->() const { return elem_.uv; }

    const_iterator& operator++() {
      stack_elem elem_ = dfs_->advance();
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
    bool                 is_back_edge() const { return elem_.uv == std::graph::end(dfs_->graph_, *elem_.u); }

    bool is_vertex_visited() const { // Has the [out] vertex been visited yet?
      if (is_back_edge())
        return false;

      vertex_iterator_t<G> v     = vertex(dfs_->graph_, *elem_.uv);
      vertex_key_t<G>      v_key = vertex_key(graph_, *v);
      return dfs_->visited_[v_key];
    }

  protected:
    dfs_edge_range* dfs_ = nullptr; // always non-null & valid; ptr allows default ctor
    stack_elem      elem_;
  };

  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;

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

    edge_t<G>&                operator*() { return *this->elem_.uv; }
    vertex_edge_iterator_t<G> operator->() const { return this->elem_.uv; }

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
    bool resume = false;
    while (!stack_.empty()) {
      if (resume)
        return stack_.top();

      auto [u, uv] = stack_.top();

      vertex_iterator_t<G>      v      = vertex(graph_, *uv);
      vertex_key_t<G>           v_key  = vertex_key(graph_, *v);
      vertex_edge_iterator_t<G> uv_end = std::graph::end(graph_, *u);

      stack_.pop();
      resume = true;

      // no more edges for u? (will occur if no [out] edges)
      if (uv == uv_end)
        continue;

      // remember next edge to resume on for u
      if (++uv != uv_end)
        stack_.push(stack_elem{u, uv});

      // visit v vertex if it hasn't been visited yet
      if (!visited_[v_key]) {
        vertex_edge_iterator_t<G> vw = std::graph::begin(graph_, *v); // may ==end(graph_,*v)
        stack_.push(stack_elem{v, vw});                               // go level deeper in traversal
        visited_[v_key] = true;
      }
    }
    return stack_elem{std::graph::end(graph_), vertex_edge_iterator_t<G>()};
  }

private:
  G&           graph_;
  stack_type   stack_;
  vector<bool> visited_;
};


} // namespace std::graph

#endif // GRAPH_DFS_HPP
