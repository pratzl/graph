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

#include "common/abstract_test.hpp"
#include "aos.hpp"
#include "bfs_range.hpp"

//****************************************************************************
template <typename EdgeListT>
bool validate(EdgeListT                 &aos,
              size_t                     seed,
              std::vector<size_t> const &distance,
              std::vector<size_t> const &predecessor)
{
  bool pass(true);

  /// @note Should we parse the orginal mmio file instead?
  for (auto [u, v] : aos.storage_)
  {
    int localdiff = static_cast<int>(distance[u]) - static_cast<int>(distance[v]);
    size_t diff = abs(localdiff);
#ifdef PRINT_OUT
    std::cout << "Checking edge: "
	      << u << " (" << distance[u] << ", " << predecessor[u] << ") -> "
	      << v << " (" << distance[v] << ", " << predecessor[v] << "), diff = "
	      << diff << std::endl;
#endif
    if (diff > 1)
      {
#ifdef PRINT_OUT
	std::cerr << "ERROR: too far: dist[" << u << "] = " << distance[u]
		  << " too far from dist[" << v << "] = " << distance[v]
		  << std::endl;
#endif
	pass = false;
	break;
      }
  }

  //std::cout << "vertex\tdepth\tparent\tpass-fail" << std::endl;
  for (size_t vid = 0; vid < distance.size(); ++vid)
  {
      bool passed =
          ((vid == seed) && (distance[vid] == 0)) ||
          (distance[vid] == (1 + distance[predecessor[vid]]));
      pass &= passed;
      //std::cout << vid+1
      //          << ":\t" << distance[vid]
      //          << "\t" << predecessor[vid]+1
      //          << "\t" << (passed ? "PASS" : "FAIL") << std::endl;
  }

  return pass;
}

TEST( BFS_traversal, bfs ) {

  test_util util;
  auto aos_a = util.generate_directed_aos();
  directed_csr_graph_t A(aos_a);

  size_t              N = A.size();
  std::vector<size_t> distance(N);
  std::vector<size_t> predecessor(N);

  /*SECTION( "default seed" )*/ {
    vertex_index_t seed = 0;
    distance[seed] = 0;
    bfs_edge_range ranges(A, seed);
    auto ite = ranges.begin();
    for(; ite != ranges.end(); ++ite) {
      auto v = std::get<0>(*ite);
      auto u = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    EXPECT_TRUE(validate(aos_a, seed, distance, predecessor));
  }

  /*SECTION( "another seed" )*/ {
    vertex_index_t seed = 1;
    distance[seed] = 0;
    bfs_edge_range ranges(A, seed);
    auto ite = ranges.begin();
    for(; ite != ranges.end(); ++ite) {
      auto v = std::get<0>(*ite);
      auto u = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    EXPECT_TRUE(validate(aos_a, seed, distance, predecessor));
  }

  /*SECTION( "Top-down BFS default seed using bfs_range")*/ {
    vertex_index_t seed = 0;
    distance[seed] = 0;
    bfs_range ranges(A, seed);
    auto ite = ranges.begin();
    for(; ite != ranges.end(); ++ite) {
      auto v = std::get<0>(*ite);
      auto u = std::get<1>(*ite);
      distance[u]    = distance[v] + 1;
      predecessor[u] = v;
    }

    EXPECT_TRUE(validate(aos_a, seed, distance, predecessor));
  }
}
