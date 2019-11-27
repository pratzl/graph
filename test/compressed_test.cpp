//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "compressed.hpp"
#include "edge_list.hpp"

#include "mmio.hpp"

#include "common/test_header.hpp"

#include <iostream>

using namespace bgl17;


// data/karate.mtx:%%MatrixMarket matrix coordinate pattern symmetric
// data/karate.mtx:%%MatrixMarket matrix coordinate pattern symmetric

// data/tree.mmio:%%MatrixMarket matrix coordinate real symmetric
// data/USAir97.mtx:%%MatrixMarket matrix coordinate real symmetric


TEST(compressed_class_IO, compressed_io) {
  /*SECTION("I/O (read real symmetric to edge_list and convert to compressed graph)")*/ {
    auto A = read_mm<directed>(DATA_DIR"tree.mmio");
    auto B = read_mm<undirected>(DATA_DIR"tree.mmio");

    auto C = read_mm<directed>(DATA_DIR"USAir97.mtx");
    auto D = read_mm<undirected>(DATA_DIR"USAir97.mtx");
  }
  /*SECTION("I/O (read pattern symmetric to edge_list and convert to compressed graph)")*/ {
    auto A = read_mm<directed>(DATA_DIR"karate.mtx");
    auto B = read_mm<undirected>(DATA_DIR"karate.mtx");
  }
  /*SECTION("I/O (read real unsymmetric to edge_list and convert to compressed graph)")*/ {
    auto A = read_mm<directed>(DATA_DIR"tree.mmio");
    auto B = read_mm<undirected>(DATA_DIR"tree.mmio");
  }
  /*SECTION("I/O (read pattern unsymmetric to edge_list and convert to compressed graph)")*/ {

  }
  /*SECTION("I/O (read to edge_list and convert to compressed matrix)")*/ {

  }
}

#if 0
TEST("compressed class iteration", "[compressed]") {
  SECTION("push_back") {

  }
  SECTION("read") {

  }
  SECTION("modify value") {
  }
}


  auto C = read_mm<directed, double>(argv[1]);
  auto D = read_mm<undirected, double>(argv[1]);

  compressed_sparse<0, directed>   E(A);
  compressed_sparse<1, directed>   F(A);
  compressed_sparse<0, undirected> G(B);
  compressed_sparse<1, undirected> H(B);

  compressed_sparse<0, directed, double>   I(C);
  compressed_sparse<1, directed, double>   J(C);
  compressed_sparse<0, undirected, double> K(D);
  compressed_sparse<1, undirected, double> L(D);

}
}
#endif
