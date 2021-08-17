//#include "pch.h"
#include "graph/graph2.hpp"
#include <iostream>
#include <catch2/catch.hpp>

#define EXPECT_EQ(a, b) REQUIRE((a) == (b))
#define EXPECT_NE(a, b) REQUIRE((a) != (b))
#define EXPECT_FALSE(a) REQUIRE(!(a))
#define EXPECT_TRUE(a) REQUIRE(a);

// Google Test compatibility
#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST

using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;

using std::ranges::size;

template <std::semiregular T>
auto test_semiregular(T* x) {
  if constexpr (!std::is_same_v<T, void>) {
    return *x;
  }
}

TEST_CASE("graph2 hpp", "[graph2][hpp]") {
  int   x = 1;
  void* y = nullptr;
  //test_semiregular(y); // void is not semi-regular

#if TEST_OPTION == TEST_OPTION_OUTPUT
#elif TEST_OPTION == TEST_OPTION_GEN
}
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST_CASE("graph2b hpp", "[graph2][hpp]") {

#if TEST_OPTION == TEST_OPTION_OUTPUT
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}
