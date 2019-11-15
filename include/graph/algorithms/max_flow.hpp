//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#pragma once

#include <tuple>
#include <algorithm>
#include <reverse.hpp>
#include <util.hpp>
#include <filtered_bfs_range.hpp>
#include <util.hpp>
static int DEFAULT_MAX = 1000;
enum class default_dict {capacity_idx = 1, flow_idx = 2};

//****************************************************************************
template<typename Dict = default_dict, typename Graph>
double max_flow(Graph &A, vertex_index_t source, vertex_index_t sink,
		size_t max_iters=DEFAULT_MAX)
{
  //typedef static_cast<size_t>(Dict::capacity_idx) test;
  struct tree_edge {
    double *capacity;
    double *flow;
    size_t predecessor;
  };

  
  size_t n_vtx = A.size();
  std::vector<tree_edge> search_tree(n_vtx);

  double maxflow = 0;

  for(size_t i=0; i <= max_iters; ++i) {
    auto mf_filter = [](vertex_index_t vtx1, decltype((A.begin())[0].begin()) edge)
      {return property<idx(Dict::flow_idx)>(edge) >= property<idx(Dict::capacity_idx)>(edge);};
    filtered_bfs::filtered_bfs_edge_range ranges(A, source, sink, mf_filter);
    auto ite = ranges.begin();
    
    for(; ite!= ranges.end(); ++ite) {
      auto v = get_source(ite);
      auto u = get_target(ite);
      auto c = property_ptr<idx(Dict::capacity_idx)>(ite);
      auto f = property_ptr<idx(Dict::flow_idx)>(ite);

      search_tree[u].capacity = c;
      search_tree[u].flow = f;
      search_tree[u].predecessor = v;
    }

    if(!ite.found())
      break;

    double min_cap = 100000;
    size_t path_length = 0;
    for(auto y: reverse_path(search_tree, sink, source)) {
      min_cap = std::min(min_cap,*(search_tree[std::get<0>(y)].capacity) -
			 *(search_tree[std::get<0>(y)].flow));
      ++path_length;
    }

    maxflow += min_cap;

    for(auto y: reverse_path(search_tree, sink, source)) {
      *(search_tree[std::get<0>(y)].flow) += min_cap;
    }
  }

  return maxflow;
}



