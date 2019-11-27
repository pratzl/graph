//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include "aos.hpp"
#include "common/test_header.hpp"
#include <iterator>

using namespace bgl17;

template<size_t pos, typename Iterator>
void test_assignment(Iterator iter, size_t index = 0) {
  typename std::iterator_traits<Iterator>::reference tuple = index ? *iter : iter[index];
  std::get<pos>(tuple) = 0;
  typename std::iterator_traits<Iterator>::reference tuple1 = index ? *iter : iter[index];
  EXPECT_TRUE(std::get<pos>(tuple1) == 0);
  std::get<pos>(tuple1) = 1;
  typename std::iterator_traits<Iterator>::reference tuple2 = index ? *iter : iter[index];
  EXPECT_TRUE(std::get<pos>(tuple2) == 1);
}

TEST( array_of_structures, aos ) {
  /*SECTION( "push back" )*/ {
    array_of_structs<size_t>                         A;
    array_of_structs<size_t, size_t>                 B;
    array_of_structs<size_t, size_t, double>         C;
    array_of_structs<double, size_t, size_t, double> D;

    A.push_back(8675309);
    B.push_back(867, 5309);
    C.push_back(867, 5309, 3.14159);
    D.push_back(95141.3, 867, 5309, 3.14159);
  }

  /*SECTION( "assignment through iterator")*/ {
    array_of_structs<size_t>                         A;
    array_of_structs<size_t, size_t>                 B;
    array_of_structs<size_t, size_t, double>         C;
    array_of_structs<double, size_t, size_t, double> D;

    A.push_back(8675309);
    B.push_back(867, 5309);
    C.push_back(867, 5309, 3.14159);
    D.push_back(95141.3, 867, 5309, 3.14159);

    auto iterA = A.begin();
    test_assignment<0>(iterA);
    auto iterB = B.begin();
    test_assignment<0>(iterB);
    test_assignment<1>(iterB);
    auto iterC = C.begin();
    test_assignment<0>(iterC);
    test_assignment<1>(iterC);
    test_assignment<2>(iterC);
    auto iterD = D.begin();
    test_assignment<0>(iterD);
    test_assignment<1>(iterD);
    test_assignment<2>(iterD);
    test_assignment<3>(iterD);
  }
}

TEST( struct_of_arrays, soa ) {
  /*SECTION( "push back" )*/ {
    struct_of_arrays<size_t>                         E;
    struct_of_arrays<size_t, size_t>                 F;
    struct_of_arrays<size_t, size_t, double>         G;
    struct_of_arrays<double, size_t, size_t, double> H;

    E.push_back(8675309);
    F.push_back(867, 5309);
    G.push_back(867, 5309, 3.14159);
    H.push_back(95141.3, 867, 5309, 3.14159);
    H.push_back(195141.3, 1867, 15309, 13.14159);
    H.push_back(295141.3, 2867, 25309, 23.14159);
    H.push_back(395141.3, 3867, 35309, 33.14159);

    auto e = E.begin();
    EXPECT_TRUE(std::get<0>(*e) == 8675309);
    auto f = F.begin();
    EXPECT_TRUE(std::get<0>(*f) == 867);
    EXPECT_TRUE(std::get<1>(*f) == 5309);
    auto h = H.begin();
    EXPECT_TRUE(std::get<0>(*h) == 95141.3);
    EXPECT_TRUE(std::get<1>(*h) == 867);
    EXPECT_TRUE(std::get<2>(*h) == 5309);
    EXPECT_TRUE(std::get<3>(*h) == 3.14159);
    EXPECT_TRUE(std::get<0>(h[3]) == 395141.3);
    EXPECT_TRUE(std::get<1>(h[3]) == 3867);
    EXPECT_TRUE(std::get<2>(h[3]) == 35309);
    EXPECT_TRUE(std::get<3>(h[3]) == 33.14159); 
  }

  /*SECTION( "assignment through iterator" )*/ {
    struct_of_arrays<size_t>                         E;
    struct_of_arrays<size_t, size_t>                 F;
    struct_of_arrays<size_t, size_t, double>         G;
    struct_of_arrays<double, size_t, size_t, double> H;

    E.push_back(8675309);
    F.push_back(867, 5309);
    G.push_back(867, 5309, 3.14159);
    H.push_back(95141.3, 867, 5309, 3.14159);
    H.push_back(195141.3, 1867, 15309, 13.14159);
    H.push_back(295141.3, 2867, 25309, 23.14159);
    H.push_back(395141.3, 3867, 35309, 33.14159);

    auto e = E.begin();
    test_assignment<0>(e);
    auto f = F.begin();
    test_assignment<0>(f);
    test_assignment<1>(f);
    auto h = H.begin();
    test_assignment<0>(h);
    test_assignment<1>(h);
    test_assignment<3>(h);
    test_assignment<0>(h, 3);
    test_assignment<1>(h, 3);
    test_assignment<2>(h, 3);
    test_assignment<3>(h, 3);
  }
}


