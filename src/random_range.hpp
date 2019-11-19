//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __RANDOM_RANGE_HPP
#define __RANDOM_RANGE_HPP

#include "util.hpp"

#include <cmath>
#include <random>
#include <vector>

template<typename Graph>
class random_range {
public:
  random_range(Graph& g, size_t length = size_t(0xffffffffffffffffULL), vertex_index_t first = 0, unsigned seed = 2049)
      : the_graph_(g), starting_vertex(first), length_(length), distribution(0, 1.0), generator(seed),
        dice(std::bind(distribution, generator)) {}

  class edge_range_iterator {
  private:
    random_range<Graph>&           the_range_;
    typename Graph::outer_iterator G;
    vertex_index_t                 current_vertex;
    size_t                         length_;

  public:
    edge_range_iterator(random_range<Graph>& range)
        : the_range_(range), G(the_range_.the_graph_.begin()), current_vertex(the_range_.starting_vertex),
          length_(the_range_.length_) {}

    edge_range_iterator& operator++() {
      --length_;
      typename Graph::inner_iterator u_begin = G[current_vertex].begin();
      typename Graph::inner_iterator u_end   = G[current_vertex].end();
      size_t                         N       = u_end - u_begin;
      vertex_index_t                 pick    = std::floor(N * the_range_.dice());

      current_vertex = std::get<0>(u_begin[pick]);

      return *this;
    }

    auto operator*() const { return current_vertex; }

    struct end_sentinel_type {};

    auto operator==(const end_sentinel_type&) const { return length_ == 0; }
    bool operator!=(const end_sentinel_type&) const { return length_ != 0; }
  };

  typedef edge_range_iterator iterator;

  auto begin() { return edge_range_iterator(*this); }
  auto end() { return typename edge_range_iterator::end_sentinel_type(); }

private:
  Graph& the_graph_;

  vertex_index_t starting_vertex;
  size_t         length_;

  std::uniform_real_distribution<double> distribution;
  std::default_random_engine             generator;

  decltype(std::bind(distribution, generator)) dice;
};

#endif    // __RANDOM_RANGE_HPP
