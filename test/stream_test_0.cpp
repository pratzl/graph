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
#include "plain_range.hpp"

int main(int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio" << std::endl;
    return -1;
  }

  auto                           aos_a = read_mm<directed>(argv[1]);
  compressed_sparse<0, directed> A(aos_a);

  size_t N = A.end() - A.begin();

  for (auto y : plain_range(A)) {
    std::cout << std::get<0>(y) << std::endl;
  }

  return 0;
}
