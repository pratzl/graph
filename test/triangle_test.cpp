//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <queue>

#include <algorithms/triangle_count.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <mmio.hpp>

#include "common/test_header.hpp"

typedef compressed_sparse<0, undirected> csr_graph;

//****************************************************************************
TEST(triangle_counting, tc) {

  size_t num_threads = 2;

  auto aos_a = read_mm<undirected>("../data/karate.mtx");
  aos_a.triangularize<predecessor>();
  aos_a.sort_by<1>();
  aos_a.stable_sort_by<0>();

  csr_graph A(aos_a);

  /*SECTION("v1")*/ {
    size_t triangles = triangle_count_v1(A);
    std::cout << triangles << " triangles (v1)" << std::endl;
    EXPECT_TRUE(triangles == 45);
  }

  /*SECTION("v2")*/ {
    // Should this work?
    size_t triangles = triangle_count_v2(A);
    std::cout << triangles << " triangles (v2)" << std::endl;
    EXPECT_TRUE(triangles == 45);
  }

  /*SECTION("v3")*/ {
    size_t triangles = triangle_count_v3(A);
    std::cout << triangles << " triangles (v3)" << std::endl;
    EXPECT_TRUE(triangles == 45);
  }
}
