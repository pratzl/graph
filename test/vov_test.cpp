//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//
#include "plain_range.hpp"
#include "spmv_range.hpp"
#include "vovos.hpp"
#include "common/test_header.hpp"
#include <complex>

TEST(vector_of_vector_of_structures, vector_of_vector_of_structs) {
  /*SECTION("construct")*/ { vector_of_vector_of_structs A(5); }
}

TEST(vector_of_vectors, vov) {
  /*SECTION("construct")*/ {
    vov A(5);
    A.push_back(3, 4);
    vov<double> B(5);
    B.push_back(3, 1, 4.159);
    vov<double,std::complex<float>> C(5);
    C.push_back(3, 1, 4.159, {86.7f, 5.309f});
  }
  /*SECTION("iterate")*/ {
    vov<double> A(5);
    A.push_back(0, 0, 3);
    A.push_back(1, 4, 1.5);
    A.push_back(2, 2, 6.9);
    A.push_back(2, 3, 2.9);
    A.push_back(3, 2, 7.8);
    A.push_back(4, 0, 3.9);
    for (auto&& [y] : plain_range(A)) {
    std::cout << y << std::endl;
  }

  for (auto&& [x, y] : plain_edge_range(A)) {
    std::cout << x << " " << y << std::endl;
  }
  for (auto&& [x, y, z] : spmv_range(A)) {
    std::cout << x << " " << y << " " << z << std::endl;
  }
  }
}

