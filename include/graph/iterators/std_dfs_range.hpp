//
// based on new_dfs_range.hpp from:
//
//	This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
//	(c) Pacific Northwest National Laboratory 2018
//
//	Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
//	https://creativecommons.org/licenses/by-nc-sa/4.0/
//
//	Author: Andrew Lumsdaine
//

#ifndef GRAPH_DFS_RANGE_HPP
#define GRAPH_DFS_RANGE_HPP

#include "../graph_fwd.hpp"
#include <cassert>
#include <stack>
#include <vector>

namespace std::graph {

enum three_colors : int8_t {
  white, // undiscovered
  grey,  // discovered, with adjacent white vertices
  black  // discovered, without adjacent white vertices
};


template <typename G, typename A = allocator<char>>
class dfs_vertex_range {
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

    vertex_t<G> const&         operator*() const { return *elem_.u; }
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
    dfs_vertex_range* dfs_ = nullptr; // always non-null; allow default ctor
    stack_elem        elem_;
  };

  class iterator : public const_iterator {
  public:
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

    vertex_t<G>&         operator*() { return *this->elem_.u; }
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
  iterator       begin() { return iterator(*this); } // begin() shouldn't be called after iterating starts on range
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
        stack_.push(stack_elem{u, uv}); // remember edge where we stopped so we can continue later
        vertex_edge_iterator_t<G> vw = std::graph::begin(graph_, *v); // vw may ==end(graph_,*v)
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


template <typename G, typename A = allocator<char>>
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

    size_t depth() const { return dfs_->stack_.size(); }

  protected:
    dfs_edge_range* dfs_ = nullptr; // always non-null; allow default ctor
    stack_elem      elem_;
  };

  class iterator : public const_iterator {
  public:
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
  iterator       begin() { return iterator(*this); } // begin() shouldn't be called after iterating starts on range
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
        stack_.push(stack_elem{u, uv}); // remember edge where we stopped so we can continue later
        vertex_edge_iterator_t<G> vw = std::graph::begin(graph_, *v); // vw may ==end(graph_,*v)
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


#ifdef _old_version
template <typename Graph>
struct dfs_value {
  bool                first_visit;
  vertex_key_t<Graph> out_key;
  edge_t<Graph>&      edge;
  size_t              depth;
};

template <typename Graph, typename Stack = std::stack<vertex_key_t<Graph>>>
class dfs_range {

public:
  dfs_range(Graph& graph, vertex_key_t<Graph> seed = 0)
        : the_graph_(graph), colors_(graph.end() - graph.begin(), white) {
    Q_.push(seed);
    colors_[seed] = grey;

    dfs_visit(the_graph_.begin(), Q_, colors_, seed);
  }

  dfs_range(const dfs_range&)  = delete;
  dfs_range(const dfs_range&&) = delete;

  bool empty() { return Q_.empty(); }

private:
  template <typename GraphRange>
  static void dfs_visit(const GraphRange& G, Stack& Q, std::vector<three_colors>& colors, vertex_key_t<Graph> v) {
    auto u     = G[v].begin();
    auto u_end = G[v].end();

    while (u != u_end) {
      if (colors[std::get<0>(*u)] == white) {
        Q.push(v);
        colors[v] = grey;

        v     = std::get<0>(*u);
        u     = G[v].begin();
        u_end = G[v].end();
      } else {
        ++u;
      }
      Q.push(v);
      colors[v] = black;
    }
  }

public:
  class dfs_range_iterator {
  public:
    dfs_range_iterator(dfs_range<Graph>& range) : the_range_(range), cursor_(0) {}

    dfs_range_iterator& operator++() {
      auto  G      = the_range_.the_graph_.begin();
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      while (!Q.empty() && colors[Q.top()] == black)
        Q.pop();

      if (!Q.empty()) {
        dfs_visit(G, Q, colors, Q.top());
      } else {
        while (colors[++cursor_] != white && cursor_ != the_range_.the_graph_.size())
          ;
        if (cursor_ != the_range_.the_graph_.size()) {
          Q.push(cursor_);
          colors[cursor_] = grey;
          dfs_visit(G, Q, colors, cursor_);
        }
      }

      return *this;
    }

    auto operator*() { return the_range_.Q_.top(); }
    // auto operator*() { return active_; }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return the_range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.empty(); }

  private:
    dfs_range<Graph>&   the_range_;
    vertex_key_t<Graph> cursor_;
  };

  typedef dfs_range_iterator iterator;

  auto begin() { return dfs_range_iterator(*this); }
  auto end() { return typename dfs_range_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  Stack                     Q_;
  std::vector<three_colors> colors_;
};


template <typename Graph, typename Stack = std::stack<vertex_key_t<Graph>>>
class dfs_edge_range {

public:
  dfs_edge_range(Graph& graph, vertex_key_t<Graph> seed = 0)
        : the_graph_(graph), colors_(graph.end() - graph.begin(), white) {
    Q_.push(seed);
    colors_[seed] = grey;
  }

  dfs_edge_range(const dfs_edge_range&)  = delete;
  dfs_edge_range(const dfs_edge_range&&) = delete;

  bool empty() { return Q_.empty(); }

  class dfs_edge_range_iterator {
  private:
    //typename Graph::outer_iterator G;
    //typename Graph::inner_iterator u_begin, u_end, u_parent;
    Graph&                        the_graph_;
    dfs_edge_range<Graph, Stack>& the_range_;
    vertex_iterator_t<Graph>      G;
    vertex_key_t<Graph>           v_;
    vertex_edge_iterator_t<Graph> u_begin, u_end, u_parent;
    bool                          has_parent_, back_edge_;

  public:
    dfs_edge_range_iterator()                               = default;
    dfs_edge_range_iterator(dfs_edge_range_iterator const&) = default;
    dfs_edge_range_iterator(dfs_edge_range<Graph, Stack>& range)
          : the_graph_(range.the_graph_)
          , the_range_(range)
          , G(::ranges::begin(vertices(the_graph_)))
          , v_(the_range_.Q_.top())
          , u_begin(std::graph::begin(the_graph_, *(G + v_)))
          , u_end(std::graph::end(the_graph_, *(G + v_)))
          , back_edge_(false) {}

    dfs_edge_range_iterator& operator=(dfs_edge_range_iterator const&) = default;

    vertex_key_t<Graph> in_key(edge_t<Graph> const& uv) const { return in_vertex_key(the_graph_, uv); }
    vertex_key_t<Graph> out_key(edge_t<Graph> const& uv) const { return out_vertex_key(the_graph_, uv); }

    vertex_edge_iterator_t<Graph> get_u_begin(vertex_key_t<Graph> v) {
      return ::std::graph::begin(the_graph_, *(G + v));
    }
    vertex_edge_iterator_t<Graph> get_u_end(vertex_key_t<Graph> v) { return ::std::graph::end(the_graph_, *(G + v)); }

    dfs_edge_range_iterator& operator++() {
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      back_edge_ = false;
      if (u_begin != u_end) {
        if (colors[out_key(*u_begin)] == white) {
          Q.push(v_);
          colors[v_] = grey;

          v_      = out_key(*u_begin);
          u_begin = get_u_begin(v_);
          u_end   = get_u_end(v_);

          while (out_key(*u_begin) == Q.top()) {
            has_parent_ = true;
            u_parent    = u_begin;
            ++u_begin;
          }
        } else {

          if (out_key(*u_begin) == Q.top()) {
            u_parent    = u_begin;
            has_parent_ = true;
          }
          ++u_begin;
          if (u_begin == u_end && !has_parent_) {
            colors[v_]  = black;
            v_          = Q.top();
            has_parent_ = false;
            Q.pop();
            if (Q.empty())
              return *this;
          }

          if (u_begin != u_end && out_key(*u_begin) == Q.top()) {
            u_parent    = u_begin;
            has_parent_ = true;
            ++u_begin;
          }
        }

        if (u_begin == u_end) {
          back_edge_ = true;
        }
      } else {
        colors[v_]  = black;
        v_          = Q.top();
        has_parent_ = false;
        Q.pop();
        if (Q.empty())
          return *this;

        assert(colors[v_] == grey);
        u_begin = get_u_begin(v_);
        u_end   = get_u_end(v_);

        while (out_key(*u_begin) == Q.top()) {
          u_parent    = u_begin;
          has_parent_ = true;
          ++u_begin;
        }
      }
      //Think this is just used to cleanly exit from root
      //because root is pushed onto queue twice?
      if (u_begin == u_end && !has_parent_) {
        colors[v_]  = black;
        v_          = Q.top();
        has_parent_ = false;
        Q.pop();
        if (Q.empty())
          return *this;
      }

      return *this;
    }

    auto operator*() {
      if (back_edge_) {
        //return std::tuple_cat(std::make_tuple(true, v_), *u_parent);
        //return std::make_tuple(true, v_, *u_parent);
        return dfs_value<Graph>{true, v_, *u_parent, the_range_.Q_.size()};
      }
      //return std::tuple_cat(std::make_tuple(the_range_.colors_[in_key(*u_begin)] == white, v_), *u_begin);
      //return std::make_tuple(the_range_.colors_[in_vertex_key(the_graph_, *u_begin)] == white, v_, *u_begin);
      return dfs_value<Graph>{the_range_.colors_[out_vertex_key(the_graph_, *u_begin)] == white, v_, *u_begin,
                              the_range_.Q_.size()};
    }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    //This is also a result of double pushing root node
    auto operator==(const end_sentinel_type&) const { return u_begin == u_end && the_range_.Q_.size() == 1; }
    //the_range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.empty(); }
  };

  typedef dfs_edge_range_iterator iterator;

  auto begin() { return dfs_edge_range_iterator(*this); }
  auto end() { return typename dfs_edge_range_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  Stack                     Q_;
  std::vector<three_colors> colors_;
};

#endif //_old_version

} // namespace std::graph

#endif // GRAPH_DFS_RANGE_HPP
