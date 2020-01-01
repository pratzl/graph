//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//
#pragma once

#include "pch.h"
#include "graph/range/bfs_range.hpp"

/*BFS-based connected component algorithm*/

template<typename Graph>
void compute_connected_components(Graph A,
				  std::vector<size_t>& component_ids) {
  size_t N = A.size();
  vertex_index_t global_component_counter = -1;
  for (size_t vtx = 0; vtx < N; vtx++) {
    if (component_ids[vtx] == std::numeric_limits<std::uint32_t>::max()) {
      global_component_counter++;
      component_ids[vtx] = global_component_counter;
      bfs_range ranges(A, vtx);
      for (auto ite = ranges.begin(); ite!= ranges.end(); ++ite) {
	auto u = std::get<0>(*ite);
	auto v = std::get<1>(*ite);
	// std::cout << u << " " << v << std::endl; 
	component_ids[v] = component_ids[u];
      }
    }
  }
}

