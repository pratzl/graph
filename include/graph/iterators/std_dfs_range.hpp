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

//#include "util.hpp"
#include "../graph_fwd.hpp"
#include <cassert>
#include <stack>
#include <vector>

namespace std::graph {

enum three_colors : int8_t { black, white, grey };
//using vertex_index_t = size_t;

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
    vertex_iterator_t<Graph>      G;
    vertex_edge_iterator_t<Graph> u_begin, u_end, u_parent;
    dfs_edge_range<Graph, Stack>& the_range_;
    vertex_key_t<Graph>           v_;
    bool                          has_parent_, back_edge_;

  public:
    dfs_edge_range_iterator(dfs_edge_range<Graph, Stack>& range)
          : the_graph_(range.the_graph_)
          , the_range_(range)
          , G(::ranges::begin(vertices(the_graph_)))
          , v_(the_range_.Q_.top())
          , u_begin(std::graph::begin(the_graph_,*G))
          , u_end(std::graph::end(the_graph_, *G))
          , back_edge_(false) {}

    vertex_key_t<Graph>           in_key(edge_t<Graph> const& uv) const { return in_vertex_key(the_graph_, uv); }

    vertex_edge_iterator_t<Graph> get_u_begin(vertex_key_t<Graph> v) {
      return ::std::graph::begin(the_graph_, *(G + v));
    }
    vertex_edge_iterator_t<Graph> get_u_end(vertex_key_t<Graph> v) {
      return ::std::graph::end(the_graph_, *(G + v));
    }

    dfs_edge_range_iterator& operator++() {
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      back_edge_ = false;
      if (u_begin != u_end) {
        if (colors[in_key(*u_begin)] == white) {
          Q.push(v_);
          colors[v_] = grey;

          v_      = in_key(*u_begin);
          u_begin = get_u_begin(v_);
          u_end   = get_u_end(v_);

          while (in_key(*u_begin) == Q.top()) {
            has_parent_ = true;
            u_parent    = u_begin;
            ++u_begin;
          }
        } else {

          if (in_key(*u_begin) == Q.top()) {
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

          if (u_begin != u_end && in_key(*u_begin) == Q.top()) {
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

        while (in_key(*u_begin) == Q.top()) {
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
        return std::make_tuple(true, v_, *u_parent);
      }
      //return std::tuple_cat(std::make_tuple(the_range_.colors_[in_key(*u_begin)] == white, v_), *u_begin);
      return std::make_tuple(the_range_.colors_[in_vertex_key(the_graph_, *u_begin)] == white, v_, *u_begin);
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

} // namespace std::graph

#endif // GRAPH_DFS_RANGE_HPP
