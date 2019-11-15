//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __BFS_RANGE_HPP
#define __BFS_RANGE_HPP

#include "util.hpp"
#include <cassert>
#include <queue>
#include <vector>

enum three_colors { black, white, grey };

//****************************************************************************
template<typename Graph, typename Queue = std::queue<vertex_index_t>>
class bfs_range {
public:
  bfs_range(Graph& graph, vertex_index_t seed = 0) : the_graph_(graph), colors_(graph.end() - graph.begin(), white), Q_index_(0) {
    Q_[Q_index_].push(seed);
    colors_[seed] = grey;
    //After adding seed, add the neighbors of seed to next queue
    for (auto ite = the_graph_.begin()[seed].begin(); ite != the_graph_.begin()[seed].end(); ++ite) {    // Explore neighbors
      auto u = std::get<0>(*ite);
      if (colors_[u] == white) {
        colors_[u] = grey;
        Q_[!Q_index_].push(u);
      }
    }
  }

  bfs_range(const bfs_range&)  = delete;
  bfs_range(const bfs_range&&) = delete;

  bool empty() { return Q_[Q_index_].empty(); }

  class bfs_range_iterator {
  public:
    bfs_range_iterator(bfs_range<Graph>& range) : the_range_(range)  {}

    bfs_range_iterator& operator++() {
      auto  G       = the_range_.the_graph_.begin();
      Queue (&Q)[2] = the_range_.Q_;
      auto& Q_index = the_range_.Q_index_;
      auto& colors  = the_range_.colors_;

      if (!Q[!Q_index].empty()) {
        auto u = Q[!Q_index].front();
        Q[!Q_index].pop();
        colors[u] = black;
        Q[Q_index].push(u);
        while (Q[!Q_index].empty()) {//keep filling neighbors until the
          //neighboring queue is not empty
          if (Q[Q_index].empty()) break; //break if both queue are empty

          auto v = Q[Q_index].front(); //get vertex

          for (auto ite = G[v].begin(); ite != G[v].end(); ++ite) {    // Explore neighbors
            u = std::get<0>(*ite);
            if (colors[u] == white) {
              colors[u] = grey;
              Q[!Q_index].push(u);//add all the white neighbors of v
            }
          }
          if (Q[!Q_index].empty()) {//if vertex v has no white neighbors
            Q[Q_index].pop(); //pop it out
            colors[v] = black; //be done with it
          }
        }
      }

      return *this;
    }

    auto operator*() {
      Queue (&Q)[2]      = the_range_.Q_;
      auto& Q_index   = the_range_.Q_index_;
      return std::make_tuple(Q[Q_index].front(), Q[!Q_index].front()); }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return the_range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.empty(); }

  private:
    bfs_range<Graph>& the_range_;
  };

  typedef bfs_range_iterator iterator;

  auto begin() { return bfs_range_iterator(*this); }
  auto end() { return typename bfs_range_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  Queue                     Q_[2];//Q_[0] is processing queue, Q_[1] is neighbor queue
  bool                      Q_index_;
  std::vector<three_colors> colors_;
};

//****************************************************************************
/// @todo move to its own header file
template<typename Graph, typename Queue = std::queue<vertex_index_t>>
class bfs_edge_range {

public:
  bfs_edge_range(Graph& graph, vertex_index_t seed = 0) : the_graph_(graph), colors_(graph.size(), white) {
    Q_.push(seed);
    colors_[seed] = grey;
  }

  bfs_edge_range(const bfs_edge_range&)  = delete;
  bfs_edge_range(const bfs_edge_range&&) = delete;

  bool empty() {
    bool b = Q_.empty();
    return b;
  }

  class bfs_edge_range_iterator {
  private:
    bfs_edge_range<Graph, Queue>&  the_range_;
    typename Graph::outer_iterator G;
    vertex_index_t                 v_;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    bfs_edge_range_iterator(bfs_edge_range<Graph, Queue>& range)
        : the_range_(range), G(the_range_.the_graph_.begin()), v_(the_range_.Q_.front()), u_begin(G[v_].begin()),
          u_end(G[v_].end()) {}

    bfs_edge_range_iterator(const bfs_edge_range_iterator& ite):
      the_range_(ite.the_range_),
      G(ite.G),
      v_(ite.v_),
      u_begin(u_begin),
      u_end(u_end)
    {}


    bfs_edge_range_iterator& operator++() {
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      Q.push(std::get<0>(*u_begin));
      colors[std::get<0>(*u_begin)] = grey;

      ++u_begin;
      while (u_begin != u_end && colors[std::get<0>(*u_begin)] != white) {
        ++u_begin;
      }

      while (u_begin == u_end) {
        colors[v_] = black;
        Q.pop();
        if (Q.empty()) break;

        v_ = Q.front();
        assert(colors[v_] == grey);
        u_begin = G[v_].begin();
        u_end   = G[v_].end();

        while (u_begin != u_end && colors[std::get<0>(*u_begin)] != white) {
          ++u_begin;
        }
      }

      return *this;
    }

    auto operator*() {
      return std::tuple_cat(std::make_tuple(v_), *u_begin);
    }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return the_range_.empty(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.empty(); }
  };

  typedef bfs_edge_range_iterator iterator;

  auto begin() {
    return bfs_edge_range_iterator(*this);
  }
  auto end() { return typename bfs_edge_range_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  Queue                     Q_;
  std::vector<three_colors> colors_;
};

#endif    // __BFS_RANGE_HPP
