//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#include <algorithm>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <mmio.hpp>
#include <vector>
#include <graph/algorithms/boykov_kolmogorov.hpp>
#include "common/test_header.hpp"

using namespace bgl17;

TEST(boykov_kolmogorov_max_flow, bkmf) {
  
    
  auto   aos_a = read_mm<directed, double>("../data/bktest1.mtx");
  
  size_t n_vtx = aos_a.size();
    
  edge_list<directed, double, double*> A_list(n_vtx);
  for (auto y : aos_a) {
    A_list.push_back(std::get<0>(y), std::get<1>(y), std::get<2>(y), NULL);
  }
  
  adjacency<0, double, double*> A(A_list);
  
  std::vector<double> capacities(n_vtx);
  
  for(auto it = capacities.begin(); it != capacities.end(); ++it) {
    *it=1;
  }
  capacities[1]=-1;
  capacities[3]=-1;
  capacities[7]=-1;
  
  auto result = bk_maxflow(A, capacities);
  EXPECT_TRUE(std::get<0>(result) == 0);
}
