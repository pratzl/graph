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

  size_t num_points = 8;
  size_t start      = 0;
  size_t seed       = 1980;

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio [ num_points ] [ seed ]" << std::endl;
    return -1;
  }

  if (argc >= 3) num_points = std::stol(argv[2]);
  if (argc >= 4) start = std::stol(argv[3]);
  if (argc >= 5) seed = std::stol(argv[4]);

  auto                           aos_a = read_mm<directed>(argv[1]);
  compressed_sparse<0, directed> A(aos_a);

  size_t N = A.end() - A.begin();

  for (auto&& [y] : plain_range(A)) {
    std::cout << y << std::endl;
  }

  for (auto&& [x, y] : plain_edge_range(A)) {
    std::cout << x << " " << y << std::endl;
  }

  return 0;
}
