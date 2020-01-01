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
#include "common/test_header.hpp"
#include "compressed.hpp"

using namespace bgl17;

// TODO: Need to add proper catch asserts to verify results

TEST(constructing_graphs_using_mmio, mmio) {

  /*SECTION("reading an undirected graph")*/ {
    edge_list<undirected> A = read_mm<undirected>(DATA_FILE);
    A.stream();
  }
  /*SECTION("reading a directed graph")*/ {
    edge_list<directed> B = read_mm<directed>(DATA_FILE);
    B.stream();
  }
  /*SECTION("undirected with attributes")*/ {
    edge_list<undirected, double> C = read_mm<undirected, double>(DATA_FILE);
    C.stream();
  }
  /*SECTION("directed with attributes")*/ {
    edge_list<directed, double> D = read_mm<directed, double>(DATA_FILE);
    D.stream();
  }
}

TEST(constructing_graphs_using_mmio_testing_with_auto, mmio) {

  /*SECTION("reading an undirected graph")*/ {
    auto A = read_mm<undirected>(DATA_FILE);
    A.stream();
  }
  /*SECTION("reading a directed graph")*/ {
    auto B = read_mm<directed>(DATA_FILE);
    B.stream();
  }
  /*SECTION("undirected with attributes")*/ {
    auto C = read_mm<undirected, double>(DATA_FILE);
    C.stream();
  }
  /*SECTION("directed with attributes")*/ {
    auto D = read_mm<directed, double>(DATA_FILE);
    D.stream();
  }
}

TEST(writing_graphsusing_mmio, mmio) {
  edge_list<directed, double> A(5);
  A.push_back(0, 1, 1);
  A.push_back(1, 2, 2);
  A.push_back(2, 3, 3);
  A.push_back(3, 4, 4);
  write_mm<2, double>("data/mmout_test.mtx", A);

  edge_list<undirected, double> B(5);
  B.push_back(0, 1, 1);
  B.push_back(1, 2, 2);
  B.push_back(2, 3, 3);
  B.push_back(3, 4, 4);
  write_mm<2, double>("data/mmout_test2.mtx", B);
  write_mm<2, double>("data/mmout_test3.mtx", B, "symmetric");

  edge_list<undirected, int, double> C(5);
  C.push_back(0, 1, 1, 5);
  C.push_back(1, 2, 2, 6);
  C.push_back(2, 3, 3, 7);
  C.push_back(3, 4, 4, 8);
  write_mm<2, int>("data/mmout_test4.mtx", C);
  write_mm<3, double>("data/mmout_test5.mtx", C);
  write_mm("data/mmout_test5.mtx", C);

  edge_list<undirected> D(5);
  D.push_back(0, 1);
  D.push_back(1, 2);
  D.push_back(2, 3);
  D.push_back(3, 4);
  write_mm<undirected>("data/mmout_test6.mtx", D);

  adjacency<0, double>      A_A(A);
  adjacency<0, double>      A_B(B);
  adjacency<0, int, double> A_C(C);
  write_mm<1, double>("data/mmout_test7.mtx", A_A);
  write_mm<1, double>("data/mmout_test8.mtx", A_B);
}