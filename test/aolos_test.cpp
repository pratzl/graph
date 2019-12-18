//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "pch.h"
#include "aolos.hpp"
#include "common/test_header.hpp"
#include <complex>

TEST(array_of_list_of_structures, array_of_list_of_structs) {
  /*SECTION("construct")*/ { bgl17::array_of_list_of_structs A(5); }
}

TEST(adjacency_list, adj_list) {
  /*SECTION("construct")*/ {
    bgl17::adj_list A(5);
    A.push_back(3, 4);
    bgl17::adj_list<double> B(5);
    B.push_back(3, 1, 4.159);
    bgl17::adj_list<double, std::complex<float>> C(5);
    C.push_back(3, 1, 4.159, {86.7f, 5.309f});
  }
}
