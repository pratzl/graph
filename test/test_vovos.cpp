#include "pch.h"
#include "edge_list.hpp"
#include "plain_range.hpp"
#include "edge_range.hpp"
#include "vovos.hpp"

#include <complex>

#include "util/print_types.hpp"

TEST(vector_of_vector_of_structures, vector_of_vector_of_structs) {
  vector_of_vector_of_structs A(5);
}

TEST(vector_of_vectors, vov) {
  //SECTION("construct") 
  {
    vov A(5);
    A.push_back(3, 4);
    vov<double> B(5);
    B.push_back(3, 1, 4.159);
    vov<double, std::complex<float>> C(5);
    C.push_back(3, 1, 4.159, std::complex{86.7, 5.309});
  }
  //SECTION("edge_list") 
  {
    edge_list<directed, double> A{{0, 0, 8.0}, {0, 1, 6.7}, {1, 2, 5.3}, {3, 0, 0.9}};
    vov<double>                 B(A);
    std::cout << "edgelist ->vov" << std::endl;
    //    for (auto&& [i, j, v] : make_edge_range<0>(B)) {
    //std::cout << i << " " << j << " " << v << std::endl;
    //}

    std::for_each(B.begin(), B.end(), [&](auto&& j) {
      std::for_each(j.begin(), j.end(),
                    [&](auto&& k) { std::cout << std::get<0>(k) << " " << std::get<1>(k) << std::endl; });
    });
    std::cout << std::endl;
  }
  //SECTION("iterate") 
  {
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

    for (auto&& [x, y] : make_edge_range(A)) {
      std::cout << x << " " << y << std::endl;
    }
    for (auto&& [x, y, z] : make_edge_range<0>(A)) {
      std::cout << x << " " << y << " " << z << std::endl;
    }
  }
}
