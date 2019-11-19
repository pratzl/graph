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

#include "compressed.hpp"
#include "edge_list.hpp"

#include "mmio.hpp"

int main(int argc, char* argv[]) {

  indexed_struct_of_arrays<size_t> pre_A(7);

  {
    std::cout << "Nu" << std::endl;
    size_t i = 0;
    for (auto&& x : pre_A) {
      std::cout << "i: " << i++ << std::endl;
      for (auto&& [u] : x) {
        std::cout << "    " << u << std::endl;
      }
    }
  }

  pre_A.open_for_push_back();
  {
    std::cout << "A" << std::endl;
    size_t i = 0;
    for (auto&& x : pre_A) {
      std::cout << "i: " << i++ << std::endl;
      for (auto&& [u] : x) {
        std::cout << "    " << u << std::endl;
      }
    }
  }

  pre_A.push_back(0, 1);
  pre_A.push_back(0, 2);
  pre_A.push_back(0, 3);

  pre_A.push_back(1, 2);
  pre_A.push_back(1, 3);
  pre_A.push_back(1, 4);

  if (false) {
    std::cout << "B" << std::endl;
    size_t i = 0;
    for (auto&& x : pre_A) {
      std::cout << "i: " << i++ << std::endl;
      for (auto&& [u] : x) {
        std::cout << "    " << u << std::endl;
      }
    }
  }

  //  pre_A.push_back(0, 2);
  pre_A.close_for_push_back();
  {
    std::cout << "C" << std::endl;
    size_t i = 0;
    for (auto&& x : pre_A) {
      std::cout << "i: " << i++ << std::endl;
      for (auto&& [u] : x) {
        std::cout << "    " << u << std::endl;
      }
    }
  }

  // Should this work?
  {
    std::cout << "D" << std::endl;
    size_t i = 0;
    for (auto&& x : pre_A) {
      std::cout << "i: " << i++ << std::endl;
      for (auto&& [u] : x) {
        std::cout << "    " << u << std::endl;
      }
    }
  }
  // compressed_sparse<0, undirected> B(aos_a);
  // std::cout << "edge list size of undirected graph is " << aos_a.size() << std::endl;

  return 0;
}
