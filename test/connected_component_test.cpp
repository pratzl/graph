//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//

/**/

#include <iostream>
#include <algorithm>
#include <list>
#include <cstdint>
#include <limits>
#include "aos.hpp"
#include "compressed.hpp"
#include "mmio.hpp"
#include "util.hpp"
#include "graph/algorithms/connected_component.hpp"
#include "common/test_header.hpp"


typedef compressed_sparse<0> csr_graph;

TEST(connected_component, cc) {
  /*Read the edgelist*/
  auto aos_a = read_mm<undirected>("../data/coloringData.mmio");
  aos_a.triangularize<predecessor>();
  aos_a.sort_by<1>();
  aos_a.stable_sort_by<0>();
  
  /*Construct the graph*/
  compressed_sparse<0, undirected> A(aos_a);
  
  size_t N = A.size();
  std::vector<size_t> component_ids(N, std::numeric_limits<std::uint32_t>::max());
  
  std::vector<size_t> result = {0, 0, 0, 0, 0, 0, 0, 0};
  compute_connected_components(A, component_ids);
  for (auto component_id: component_ids) {
    std::cout << component_id << std::endl;
  }
  EXPECT_TRUE(component_ids == result);
}
