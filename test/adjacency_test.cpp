//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include "pch.h"
#include <algorithm>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <graph/range/bfs_range.hpp>
#include <mmio.hpp>
#include <vector>

#include "common/test_header.hpp"

TEST(adjacency, adjacency_test) {
  size_t n_vtx = 5;

  bgl17::edge_list<bgl17::undirected, int> A_list(n_vtx);
  A_list.push_back(0,1,1);
  A_list.push_back(1,2,2);
  A_list.push_back(2,3,3);
  A_list.push_back(3,4,4);
  
  bgl17::adjacency<0, int> A(A_list);
  
  bfs_edge_range ranges(A, 0);
  for (auto ite = ranges.begin(); ite!= ranges.end(); ++ite) {
    auto u = std::get<0>(*ite);
    auto v = std::get<1>(*ite);
    auto w = std::get<2>(*ite);
    std::cout << "edge " << u << " to " << v << " change weight " << w << " to "
	      << -w << std::endl;
    w=-w;
  }

  bfs_edge_range reverse_ranges(A, 4);
  for (auto ite = reverse_ranges.begin(); ite!= ranges.end(); ++ite) {
    auto u = std::get<0>(*ite);
    auto v = std::get<1>(*ite);
    auto w = std::get<2>(*ite);
    std::cout << "edge " << u << " to " << v << " with weight " << w
	      << std::endl;
  }
}
