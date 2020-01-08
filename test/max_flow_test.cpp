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
#include <graph/algorithm/max_flow.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <graph/range/filtered_bfs_range.hpp>
#include <mmio.hpp>
#include <reverse.hpp>
#include <vector>

#include "common/test_header.hpp"

using namespace bgl17;

TEST(max_flow, mf) {
  size_t source = 0, sink = 7;

  auto   aos_a = read_mm<directed, double>("../data/flowtest.mtx");
  size_t n_vtx = aos_a.size();

  edge_list<directed, double, double> A_list(n_vtx);
  for (auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y), 0);
  }
  
  compressed_sparse<0, directed, double, double> A(A_list);
  double maxflow = max_flow(A, source, sink);
}

TEST(max_flow2, mf) {
  size_t source = 0, sink = 7;

  auto   aos_a = read_mm<directed, double>("../data/flowtest.mtx");
  size_t n_vtx = aos_a.size();

  edge_list<directed, double, double, double> A_list(n_vtx);
  for (auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y), 3*std::get<2>(y), 0);
  }
  
  compressed_sparse<0, directed, double, double, double> A(A_list);
  
  enum class dict {capacity_idx = 1, flow_idx = 3};
  
  double maxflow = max_flow<dict>(A, source, sink);
  
  enum class dict2 {capacity_idx = 2, flow_idx = 3};
  
  double maxflow2 = max_flow<dict2>(A,source, sink);

  EXPECT_TRUE(maxflow*2 == maxflow2);
}

TEST(max_flow_bfs, mf_bfs) {
  struct tree_edge {
    double* capacity;
    double* flow;
    size_t  predecessor;
  };

  size_t source = 0, sink = 7;

  auto   aos_a = read_mm<directed, double>("../data/flowtest.mtx");
  size_t n_vtx = aos_a.size();

  edge_list<directed, double, double> A_list(n_vtx);
  for (auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y), 0);
  }

  compressed_sparse<0, directed, double, double> A(A_list);
  std::vector<tree_edge>                search_tree(n_vtx);

  double maxflow = 0;

  for (int i = 0; i <= 5; ++i) {
    auto mf_filter = [](vertex_index_t vtx, decltype((A.begin())[0].begin()) edge)
      {return std::get<2>(*edge) >= std::get<1>(*edge);};
    filtered_bfs::filtered_bfs_edge_range ranges(A, source, sink, mf_filter);
    auto              ite = ranges.begin();
    for (; ite != ranges.end(); ++ite) {
      auto v = get_source(ite);
      auto u = get_target(ite);
      auto c = &std::get<1>(*ite);
      auto f = &std::get<2>(*ite);

      search_tree[u].capacity    = c;
      search_tree[u].flow        = f;
      search_tree[u].predecessor = v;
    }

    if (i < 5)
      EXPECT_TRUE(ite.found());
    else {
      EXPECT_TRUE(!ite.found());
      break;
    }

    double min_cap     = 100000;
    size_t path_length = 0;
    for (auto y : reverse_path(search_tree, sink, source)) {
      min_cap = std::min(min_cap, *(search_tree[std::get<0>(y)].capacity) -
			 *(search_tree[std::get<0>(y)].flow));
      ++path_length;
    }

    maxflow += min_cap;

    if (i < 4)
      EXPECT_TRUE(path_length == 3);
    else if (i == 4)
      EXPECT_TRUE(path_length == 5);

    for (auto y : reverse_path(search_tree, sink, source)) {
      EXPECT_TRUE(*(search_tree[std::get<0>(y)].capacity) -
	      *(search_tree[std::get<0>(y)].flow) >= min_cap);
    }

    for (auto y : reverse_path(search_tree, sink, source)) {
      *(search_tree[std::get<0>(y)].flow) += min_cap;
    }
    if (i == 0) {
      for (auto y : reverse_path(search_tree, sink, source)) {
        EXPECT_TRUE(*(search_tree[std::get<0>(y)].flow) == min_cap);
      }
    }
  }
  EXPECT_TRUE(maxflow == 28);
}
