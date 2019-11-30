//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __ROW_RANGE_HPP
#define __ROW_RANGE_HPP

#include "util.hpp"
#include <queue>
#include <vector>

namespace bgl17 {

template <typename Graph>
class neighbor_range {
public:
  neighbor_range(Graph& g) : the_graph_(g) {}

  class edge_range_iterator {
  private:
    neighbor_range<Graph>&         the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::outer_iterator G;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    edge_range_iterator(neighbor_range<Graph>& range)
          : the_range_(range)
          , first(the_range_.the_graph_.begin())
          , last(the_range_.the_graph_.end())
          , G(first)
          , u_begin((*first).begin())
          , u_end((*first).end()) {}

    edge_range_iterator& operator++() {
      ++u_begin;
      while (u_begin == u_end) {
        if (++first == last)
          break;
        u_begin = (*first).begin();
        u_end   = (*first).end();
      }

      return *this;
    }

    auto operator*() { return std::tuple<vertex_index_t, vertex_index_t&&>(first - G, std::get<0>(*u_begin)); }

    // return std::tuple<typename Graph::sub_view, typename Graph::sub_view>(*first, G[std::get<0>(*u_begin)]);

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return first == last; }
    bool operator!=(const end_sentinel_type&) const { return (first != last); }
  };

  typedef edge_range_iterator iterator;

  auto begin() { return edge_range_iterator(*this); }
  auto end() { return typename edge_range_iterator::end_sentinel_type(); }

private:
  Graph& the_graph_;
};

} // namespace bgl17

#endif    // __NEIGHBOR_RANGE_HPP
