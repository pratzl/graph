#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <catch2/catch.hpp>
#include "graph/graph_utility.hpp"
#include "using_graph.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::list;
using std::forward_list;

//
// vol - vector of list - graph
//
using vol_key         = size_t;
using vol_edge_val    = double;
using vol_edge_type   = pair<vol_key, vol_edge_val>;
using vol_graph       = vector<list<vol_edge_type>>;
using vol_vertex_type = vol_graph::value_type;

template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

// create functions in std for ADL, to match location of vector
namespace std {
auto& graph_value(vol_graph& g) {
  static int val = 7; // a bogus value only for validation
  return val;
}
const auto& graph_value(const vol_graph& g) {
  static const int val = 7;
  return val; // a bogus value only for validation
}
auto& vertex_value(vol_graph& g, vertex_iterator_t<vol_graph> u) {
  static int val = 8; // a bogus value only for validation
  return val;
}
const auto& vertex_value(const vol_graph& g, vertex_iterator_t<const vol_graph> u) {
  static int val = 8; // a bogus value only for validation
  return val;
}

auto&       edge_value(vol_graph&, list<vol_edge_type>::iterator uv) { return uv->second; }
const auto& edge_value(vol_graph const&, list<vol_edge_type>::const_iterator uv) { return uv->second; }

} // namespace std

TEMPLATE_TEST_CASE("vol graph", "[vol][accessors]", (vol_graph), (const vol_graph)) {
  static_assert(std::is_same_v<TestType, vol_graph> || std::is_same_v<TestType, const vol_graph>);
  using G = TestType;
  G g{{{1, 1.1}, {2, 2.1}}, {{2, 2.2}}, {{0, 0.1}}};

  using std::graph::vertices;
  using std::graph::edges;

  using std::graph::graph_value;
  using std::graph::vertex_key;
  using std::graph::vertex_value;

  //
  // vertex range
  //
  SECTION("vertices(g)") {
    //static_assert(std::graph::_vertices_::_gph_has_ADL<G>);
    auto& vv = vertices(g);
    REQUIRE(is_same_const(g, vv));
    REQUIRE(size(vertices(g)) == 3);

    auto u = begin(vv);
    REQUIRE(u == g.begin());
    REQUIRE(u->size() == 2);
    //REQUIRE(size(edges(g, u)) == 2);
  }

  //
  // value types
  //

  // graph values
  SECTION("graph_value(g)") {
    //static_assert(std::graph::_graph_value_::_gph_has_ADL<G>);
    REQUIRE(graph_value(g) == 7);
    //static_assert(std::_vertex_value_::_vtx_has_ADL<G, decltype(u)>);
    //REQUIRE(vertex_value(g, u) == 8);
  }

  // vertex values
  SECTION("vertex_key(g,u)") {
    REQUIRE(vertex_key(g, ++begin(g)) == 1); // no fmt
    //REQUIRE(vertex_value(g, begin(g)) == 8); // no fmt
  }

  // edge_values
  //edge_key
  //edge_value
}
