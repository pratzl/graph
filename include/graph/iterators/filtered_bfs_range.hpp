// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#ifndef __FILTERED_BFS_RANGE_HPP
#define __FILTERED_BFS_RANGE_HPP

#include <cassert>
#include <functional>
#include <queue>
#include <tuple>
#include <util.hpp>
#include <vector>

namespace filtered_bfs {

enum three_colors : int8_t { black, white, grey };

template<typename Graph, typename Queue = std::queue<vertex_index_t>, typename Filter = std::function<bool()>>
class filtered_bfs_edge_range {

public:
  filtered_bfs_edge_range(Graph& graph, vertex_index_t source, vertex_index_t target,
                          Filter filter = [](vertex_index_t v, typename Graph::inner_iterator iter) { return false; })
      : the_graph_(graph), colors_(graph.end() - graph.begin(), white), target_(target), filter_(filter) {
    Q_.push(source);
    colors_[source] = grey;
  }

  filtered_bfs_edge_range(const filtered_bfs_edge_range&)  = delete;
  filtered_bfs_edge_range(const filtered_bfs_edge_range&&) = delete;

  bool empty() { return Q_.empty(); }
  bool found() { return colors_[target_] == grey; }
  bool done() { return (found() || unreachable); }
  bool unreachable = false;
  /*template<typename Graph::dict_t Eval>
  auto property(filtered_bfs_edge_range &iter) const {
    return std::get<Eval>(*iter);
  }*/
  class filtered_bfs_edge_range_iterator {

  public:
    filtered_bfs_edge_range_iterator(filtered_bfs_edge_range<Graph, Queue, Filter>& range)
        : the_range_(range), G(the_range_.the_graph_.begin()), v_(the_range_.Q_.front()), u_begin(G[v_].begin()),
          u_end(G[v_].end()), iterator_target_(the_range_.target_), filter_(the_range_.filter_) {

      while (filter_(v_, u_begin)) {
        ++u_begin;
      }
    }

    filtered_bfs_edge_range_iterator(const filtered_bfs_edge_range_iterator& ite)
        : the_range_(ite.the_range_), G(ite.G), v_(ite.v_), u_begin(u_begin), u_end(u_end), filter_(ite.filter_) {}

    filtered_bfs_edge_range_iterator& operator++() {
      auto& Q      = the_range_.Q_;
      auto& colors = the_range_.colors_;

      Q.push(std::get<0>(*u_begin));
      colors[std::get<0>(*u_begin)] = grey;

      if (iterator_target_ == std::get<0>(*u_begin)) {
        colors[std::get<0>(*u_begin)] = grey;
        return *this;
      }
      ++u_begin;

      while (u_begin != u_end) {
        if (colors[std::get<0>(*u_begin)] == white && !filter_(v_, u_begin)) break;

        ++u_begin;
      }

      while (u_begin == u_end) {
        colors[v_] = black;
        Q.pop();
        if (Q.empty()) {
          the_range_.unreachable = true;
          break;
        }

        v_ = Q.front();
        assert(colors[v_] == grey);

        u_begin = G[v_].begin();
        u_end   = G[v_].end();

        while (u_begin != u_end) {
          if (colors[std::get<0>(*u_begin)] == white && !filter_(v_, u_begin)) break;

          ++u_begin;
        }
      }

      return *this;
    }

    auto operator*() { return *u_begin; }
    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };
    /*auto source() { return v_; };
    auto target() { return std::get<0>(*u_begin); }*/
    auto get_index() {return v_;};

    auto operator==(const end_sentinel_type&) const { return the_range_.done(); }
    bool operator!=(const end_sentinel_type&) const { return !the_range_.done(); }

    bool found() { return the_range_.found(); }

  private:
    filtered_bfs_edge_range<Graph, Queue, Filter>& the_range_;
    typename Graph::outer_iterator                 G;
    vertex_index_t                                 v_;
    typename Graph::inner_iterator                 u_begin, u_end;
    vertex_index_t                                 iterator_target_;
    Filter                                         filter_;
  };

  typedef filtered_bfs_edge_range_iterator iterator;

  auto begin() { return filtered_bfs_edge_range_iterator(*this); }
  auto end() { return typename filtered_bfs_edge_range_iterator::end_sentinel_type(); }

private:
  Graph&                    the_graph_;
  Queue                     Q_;
  std::vector<three_colors> colors_;
  vertex_index_t            target_;
  Filter                    filter_;
};
}    // namespace filtered_bfs
#endif    // __FILTERED_BFS_RANGE_HPP
