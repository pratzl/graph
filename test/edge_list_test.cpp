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

#include "common/test_header.hpp"

using namespace bgl17;

TEST(edge_list, edge_list) {

  size_t N = 1024;
  /*SECTION("push_back")*/ {
    edge_list<directed, double> A(N);
    A.push_back(8, 6, 7);
    A.push_back(5, 3, 0);

    EXPECT_TRUE(std::get<0>(A.storage_[0]) == 8);
    EXPECT_TRUE(std::get<1>(A.storage_[0]) == 6);
    EXPECT_TRUE(std::get<2>(A.storage_[0]) == 7);

    EXPECT_TRUE(std::get<0>(A.storage_[1]) == 5);
    EXPECT_TRUE(std::get<1>(A.storage_[1]) == 3);
    EXPECT_TRUE(std::get<2>(A.storage_[1]) == 0);

    std::cout << A.size() << std::endl;
    EXPECT_TRUE(A.storage_.size() == 2);
  }

  /*SECTION("push_back - three tuples")*/ {
    edge_list<directed, double, float> B(N);
    B.push_back(8, 6, 7, 5);
    B.push_back(3, 0, 9, 9);

    EXPECT_TRUE(std::get<0>(B.storage_[0]) == 8);
    EXPECT_TRUE(std::get<1>(B.storage_[0]) == 6);
    EXPECT_TRUE(std::get<2>(B.storage_[0]) == 7);
    EXPECT_TRUE(std::get<3>(B.storage_[0]) == 5);

    EXPECT_TRUE(std::get<0>(B.storage_[1]) == 3);
    EXPECT_TRUE(std::get<1>(B.storage_[1]) == 0);
    EXPECT_TRUE(std::get<2>(B.storage_[1]) == 9);
    EXPECT_TRUE(std::get<3>(B.storage_[1]) == 9);

    EXPECT_TRUE(B.storage_.size() == 2);
  }
}

TEST(edge_list_sort, edge_list_sort) {

  size_t N = 1024;
  edge_list<directed, double, size_t> C(N);

  C.push_back(1, 2, 3.14, 159);
  C.push_back(3, 3, 3.141, 59);
  C.push_back(2, 1, 3.1415, 9);

  /*SECTION("sort 0")*/ {
    C.sort_by<0>();
    EXPECT_TRUE(std::get<0>(C.storage_[0]) == 1);
    EXPECT_TRUE(std::get<0>(C.storage_[1]) == 2);
    EXPECT_TRUE(std::get<0>(C.storage_[2]) == 3);
  }

  /*SECTION("sort 1")*/ {
    C.sort_by<1>();
    EXPECT_TRUE(std::get<1>(C.storage_[0]) == 1);
    EXPECT_TRUE(std::get<1>(C.storage_[1]) == 2);
    EXPECT_TRUE(std::get<1>(C.storage_[2]) == 3);
  }

}


#if 0
int main() {

  size_t N = 1024;

  edge_list<directed, double> A(N);
  A.push_back(8, 6, 7);
  A.push_back(5, 3, 0);

  edge_list<directed, double, float> B(N);
  B.push_back(8, 6, 7, 5);
  B.push_back(3, 0, 9, 9);

  edge_list<directed, double, size_t> C(N);

  C.push_back(1, 2, 3.14, 159);
  C.push_back(3, 3, 3.141, 59);
  C.push_back(2, 1, 3.1415, 9);

  std::cout << std::get<0>(C.storage_[0]) << " " << std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " "
            << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " " << std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " "
            << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " " << std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " "
            << std::get<3>(C.storage_[2]) << std::endl;

  C.sort_by<0>();

  std::cout << std::endl;
  std::cout << std::get<0>(C.storage_[0]) << " " << std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " "
            << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " " << std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " "
            << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " " << std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " "
            << std::get<3>(C.storage_[2]) << std::endl;

  C.sort_by<1>();

  std::cout << std::endl;
  std::cout << std::get<0>(C.storage_[0]) << " " << std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " "
            << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " " << std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " "
            << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " " << std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " "
            << std::get<3>(C.storage_[2]) << std::endl;

#if 0

  array_of_structs<size_t> A;
  array_of_structs<size_t, size_t> B;
  array_of_structs<size_t, size_t, double> C;
  array_of_structs<double, size_t, size_t, double> D;

  A.push_back(8675309);
  B.push_back(867, 5309);
  C.push_back(867, 5309, 3.14159);
  D.push_back(95141.3, 867, 5309, 3.14159);


  struct_of_arrays<size_t> E;
  struct_of_arrays<size_t, size_t> F;
  struct_of_arrays<size_t, size_t, double> G;
  struct_of_arrays<double, size_t, size_t, double> H;

  E.push_back(8675309);
  F.push_back(867, 5309);
  G.push_back(867, 5309, 3.14159);
  H.push_back(95141.3, 867, 5309, 3.14159);

  H.clear();
  D.clear();



  sparse_aos<undirected> A(N, N);
  A.push_back(1, 2);

  sparse_aos<undirected, double> B(N, N);
  B.push_back(1, 2, 3.14);

  sparse_aos<undirected, double, size_t> C(N, N);
  C.push_back(1, 2, 3.14, 159);
  C.push_back(3, 3, 3.141, 59);
  C.push_back(2, 1, 3.1415, 9);

  std::cout << std::get<0>(C.storage_[0]) << " "<< std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " " << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " "<< std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " " << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " "<< std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " " << std::get<3>(C.storage_[2]) << std::endl;

  C.sort_by<0>();

  std::cout << std::endl;
  std::cout << std::get<0>(C.storage_[0]) << " "<< std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " " << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " "<< std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " " << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " "<< std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " " << std::get<3>(C.storage_[2]) << std::endl;

  C.sort_by<1>();

  std::cout << std::endl;
  std::cout << std::get<0>(C.storage_[0]) << " "<< std::get<1>(C.storage_[0]) << " " << std::get<2>(C.storage_[0]) << " " << std::get<3>(C.storage_[0]) << std::endl;
  std::cout << std::get<0>(C.storage_[1]) << " "<< std::get<1>(C.storage_[1]) << " " << std::get<2>(C.storage_[1]) << " " << std::get<3>(C.storage_[1]) << std::endl;
  std::cout << std::get<0>(C.storage_[2]) << " "<< std::get<1>(C.storage_[2]) << " " << std::get<2>(C.storage_[2]) << " " << std::get<3>(C.storage_[2]) << std::endl;


  compressed_sparse<0, undirected, double, size_t> D(C);
#endif
  return 0;
}

#endif
