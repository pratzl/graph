//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#pragma once

//template<typename Graph, typename PriorityQueue>
//class bfs_edge_range {

// template <typename T> class bfs_edge_range;
// template<template <typename...> class Graph, typename PriorityQueue, typename...Attributes>
// class bfs_edge_range<Graph<Attributes...>> {


enum three_colors : int8_t { black, white, grey };

//****************************************************************************
// This range used by dijkstra
/// @todo resolve differences between this and bfs_edge_range in bfs_range.hpp
template<typename Graph, typename PriorityQueue>
class bfs_edge_range2 {
private:
public:
  bfs_edge_range2(Graph& graph, PriorityQueue& Q, std::tuple<size_t, size_t> seed = {0, 0})
      : the_graph_(graph), Q_(Q), colors_(graph.end() - graph.begin(), white) {
    Q_.push(seed);
    colors_[std::get<0>(seed)] = grey;
  }

  bfs_edge_range2(const bfs_edge_range2&)  = delete;
  bfs_edge_range2(const bfs_edge_range2&&) = delete;

  bool empty() { return Q_.empty(); }

  class bfs_edge_range2_iterator {
  private:
    bfs_edge_range2<Graph, PriorityQueue>& the_range_;
    typename Graph::outer_iterator        G;
    vertex_index_t                        v_;
    typename Graph::inner_iterator        u_begin, u_end;

    // Graph -> v, u, w
    // Q -> v, d

  public:
    bfs_edge_range2_iterator(bfs_edge_range2<Graph, PriorityQueue>& range)
        : the_range_(range), G(the_range_.the_graph_.begin()), v_(std::get<0>(the_range_.Q_.top())), u_begin(G[v_].begin()),
          u_end(G[v_].end()) {}

    bfs_edge_range2_iterator& operator++() {
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      Q.push({std::get<0>(*u_begin), size_t(0xffffffffffffffffULL)});
      colors[std::get<0>(*u_begin)] = grey;

      ++u_begin;
      while (u_begin != u_end && colors[std::get<0>(*u_begin)] != white) {
        ++u_begin;
      }

      while (u_begin == u_end) {
        colors[v_] = black;

        while (colors[std::get<0>(Q.top())] == black && !Q.empty())
          Q.pop();

        if (Q.empty()) break;

        v_      = std::get<0>(Q.top());
        u_begin = G[v_].begin();
        u_end   = G[v_].end();

        while (u_begin != u_end && colors[std::get<0>(*u_begin)] != white) {
          ++u_begin;
        }
      }

      return *this;
    }

    auto operator*() {
      return std::tuple<vertex_index_t, vertex_index_t, size_t>(v_, std::get<0>(*u_begin), std::get<1>(*u_begin));
    }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return the_range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.empty(); }
  };

  typedef bfs_edge_range2_iterator iterator;

  auto begin() { return bfs_edge_range2_iterator(*this); }
  auto end() { return typename bfs_edge_range2_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  PriorityQueue&            Q_;
  std::vector<three_colors> colors_;
};
