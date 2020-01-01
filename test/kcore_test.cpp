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
#include <graph/range/new_dfs_range.hpp>
#include <graph/algorithm/k_core.hpp>
#include <mmio.hpp>
#include <vector>

#include "common/test_header.hpp"

using namespace bgl17;

TEST(k_core, k_core) {
  int k = 3;

  size_t n_vtx = 10;

  edge_list<undirected> A_list(n_vtx);
  A_list.push_back(0,1);
  A_list.push_back(1,2);
  A_list.push_back(2,3);
  A_list.push_back(3,4);
  A_list.push_back(4,5);
  A_list.push_back(6,7);
  A_list.push_back(7,8);
  A_list.push_back(8,9);
  A_list.push_back(2,6);
  A_list.push_back(3,7);
  A_list.push_back(4,8);
  A_list.push_back(5,9);
  A_list.push_back(2,7);
  A_list.push_back(3,6);

  adjacency<0> A(A_list);
  auto core = k_core(A, k);
  auto filter = std::get<0>(core);
  auto remain = std::get<1>(core);
  EXPECT_TRUE(remain == 4);
  
  std::vector<int> degree(n_vtx,0);
  auto first = A.begin();
  auto last = A.end();
  for(auto G=first; first !=last; ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      Neighbors edge = make_my_pair(first-G,std::get<0>(*v));
      if(filter.find(edge) == filter.end())
	degree[std::get<0>(*v)]++;
    }
    
  }
  EXPECT_TRUE(degree[0]==0);
  EXPECT_TRUE(degree[1]==0);
  EXPECT_TRUE(degree[2]==3);
  EXPECT_TRUE(degree[3]==3);
  EXPECT_TRUE(degree[4]==0);
  EXPECT_TRUE(degree[5]==0);
  EXPECT_TRUE(degree[6]==3);
  EXPECT_TRUE(degree[7]==3);
  EXPECT_TRUE(degree[8]==0);
  EXPECT_TRUE(degree[9]==0);
}

TEST(disconnected, disconnected) {
  int k = 3;
  size_t n_vtx = 9;
  
  edge_list<undirected> A_list(n_vtx);
  A_list.push_back(0,1);
  A_list.push_back(1,2);
  A_list.push_back(2,3);
  A_list.push_back(3,4);
  A_list.push_back(5,6);
  A_list.push_back(7,8);
  A_list.push_back(0,5);
  A_list.push_back(1,6);
  A_list.push_back(3,7);
  A_list.push_back(4,8);
  A_list.push_back(0,6);
  A_list.push_back(1,5);
  A_list.push_back(3,8);
  A_list.push_back(4,7);
  
  adjacency<0> A(A_list);
  auto core = k_core(A, k);
  auto filter = std::get<0>(core);
  auto remain = std::get<1>(core);     

  EXPECT_TRUE(remain == 8);

  std::vector<int> degree(n_vtx,0);
  auto first = A.begin();
  auto last = A.end();
  for(auto G=first; first !=last; ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      Neighbors edge = make_my_pair(first-G,std::get<0>(*v));
      if(filter.find(edge) == filter.end())
	degree[std::get<0>(*v)]++;
    }
  }
  
  EXPECT_TRUE(degree[0]==3);
  EXPECT_TRUE(degree[1]==3);
  EXPECT_TRUE(degree[2]==0);
  EXPECT_TRUE(degree[3]==3);
  EXPECT_TRUE(degree[4]==3);
  EXPECT_TRUE(degree[5]==3);
  EXPECT_TRUE(degree[6]==3);
  EXPECT_TRUE(degree[7]==3);
  EXPECT_TRUE(degree[8]==3);
}

