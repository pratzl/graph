#include "graph/ordered_pair.hpp"
#include <tuple>
#include <catch2/catch.hpp>

using std::pair;
using std::ordered_pair;
using std::make_ordered_pair;
using std::make_unordered_pair;

TEST_CASE("ordered_pair", "[pair][ordered]") {
  using PairT  = ::std::pair<int, int>;
  using OPairT = ::std::ordered_pair<int, int>;
  //using first_type  = typename OPairT::first_type;
  //using second_type = typename OPairT::second_type;

  OPairT p1; // default construction
  REQUIRE(p1.first == 0);
  REQUIRE(p1.second == 0);

  OPairT p2(1, 2);
  REQUIRE(p2.first == 1);
  REQUIRE(p2.second == 2);

  OPairT p3(p2);
  REQUIRE(p3.first == 1);
  REQUIRE(p3.second == 2);
  REQUIRE(p3 == p2);

  OPairT p4 = p2;
  REQUIRE(p4.first == 1);
  REQUIRE(p4.second == 2);
  REQUIRE(p4 == p2);

  PairT  p5(3, 4);
  OPairT p6(p5);
  REQUIRE(p6.first == 3);
  REQUIRE(p6.second == 4);
  REQUIRE(p5 == p6);
  REQUIRE(p6 == p5);
  REQUIRE(p5 > p2);
  REQUIRE(p2 < p5);
  REQUIRE(p6 > p2);
  REQUIRE(p2 < p6);

  OPairT p7(move(p5));
  REQUIRE(p7.first == 3);
  REQUIRE(p7.second == 4);
  REQUIRE(p5 == p7);
  REQUIRE(p7 == p5);

  swap(p6, p3);
  REQUIRE(p3 == OPairT(3, 4));
  REQUIRE(p6 == OPairT(1, 2));
  swap(p6, p3);

  auto p8 = make_ordered_pair(5, 6);
  REQUIRE(p8.first == 5);
  REQUIRE(p8.second == 6);

  // The following tests are valid and can be used, but they generate warnings which cause extra noise
  //OPairT p10(1.1, 2.2);
  //REQUIRE(p2.first == 1);
  //REQUIRE(p2.second == 2);

  //todo
  //size_t s1 = std::tuple_size<PairT>::value;
  //size_t s2 = std::tuple_size<OPairT>::value;
}

TEST_CASE("unordered_pair", "[pair][ordered]") {
  using PairT  = std::pair<int, int>;
  using OPairT = std::unordered_pair<int, int>;
  //using first_type  = typename OPairT::first_type;
  //using second_type = typename OPairT::second_type;

  OPairT p1; // default construction
  REQUIRE(p1.first == 0);
  REQUIRE(p1.second == 0);

  OPairT p2(1, 2);
  REQUIRE(p2.first == 1);
  REQUIRE(p2.second == 2);

  OPairT p3(p2);
  REQUIRE(p3.first == 1);
  REQUIRE(p3.second == 2);
  REQUIRE(p3 == p2);

  OPairT p4 = p2;
  REQUIRE(p4.first == 1);
  REQUIRE(p4.second == 2);
  REQUIRE(p4 == p2);

  PairT  p5(3, 4);
  OPairT p6(p5);
  REQUIRE(p6.first == 3);
  REQUIRE(p6.second == 4);
  REQUIRE(p5 == p6);
  REQUIRE(p6 == p5);
  REQUIRE(p5 > p2);
  REQUIRE(p2 < p5);
  REQUIRE(p6 > p2);
  REQUIRE(p2 < p6);

  OPairT p7(move(p5));
  REQUIRE(p7.first == 3);
  REQUIRE(p7.second == 4);
  REQUIRE(p5 == p7);
  REQUIRE(p7 == p5);

  swap(p6, p3);
  REQUIRE(p3 == OPairT(3, 4));
  REQUIRE(p6 == OPairT(1, 2));
  swap(p6, p3);

  auto p8 = make_unordered_pair(5, 6);
  REQUIRE(p8.first == 5);
  REQUIRE(p8.second == 6);

  // The following tests are valid and can be used, but they generate warnings which cause extra noise
  //OPairT p10(1.1, 2.2);
  //REQUIRE(p2.first == 1);
  //REQUIRE(p2.second == 2);

  //todo
  //size_t s1 = std::tuple_size<PairT>::value;
  //size_t s2 = std::tuple_size<OPairT>::value;
}
