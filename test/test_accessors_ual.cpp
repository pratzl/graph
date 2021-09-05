#include "graph/undirected_adjacency_list.hpp"
#include "graph/graph_utility.hpp"
//#include "data_routes.hpp"
#include <iostream>
#include <catch2/catch.hpp>
//#include <range/v3/action/sort.hpp>
#include "using_graph.hpp"

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST

//using namespace std::graph;
//using namespace std::ranges;
using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;


using namespace std::graph;

using Graph = std::graph::containers::undirected_adjacency_list<name_value, weight_value>;

// do both parameters have non-const, or both const, values?
template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

TEMPLATE_TEST_CASE("ual accessors", "[ual][accessors]", (Graph), (const Graph)) {
  static_assert(std::is_same_v<TestType, Graph> || std::is_same_v<TestType, const Graph>);
  using G = TestType;
  G g;

  //
  // vertex range
  //
  SECTION("vertices(g)") {
    //static_assert(_vertices_::_gph_has_ADL<G>);
    auto& vv = vertices(g);
    REQUIRE(is_same_const(g, vv));
    REQUIRE(std::ranges::random_access_range<decltype(vv)>);
    //REQUIRE(size(vertices(g)) == 3);
    //auto u = begin(vv);
    //REQUIRE(u == g.begin());
    //REQUIRE(u->size() == 2);
    //REQUIRE(size(edges(g, u)) == 2);
  }

  //
  // value types
  //

  // graph values
  SECTION("graph_value(g)") {
    //REQUIRE(graph_value(g) == 7);
  }

  // vertex values
  SECTION("vertex_key(g,u)") {
    REQUIRE(vertex_key(g, ++begin(vertices(g))) == 1); // no fmt
    //REQUIRE(vertex_value(g, begin(vertices(g))) == 8); // no fmt
  }
}
