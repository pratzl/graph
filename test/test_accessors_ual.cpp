#include "graph/undirected_adjacency_list.hpp"
#include "graph/graph_utility.hpp"
//#include "data_routes.hpp"
#include <tuple>
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
using std::tuple;
using std::cout;
using std::endl;
using std::is_same;


using namespace std::graph;
using namespace std::ranges;

using Graph = std::graph::containers::undirected_adjacency_list<double, double, double, uint32_t>;

// do both parameters have non-const, or both const, values?
template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

TEMPLATE_TEST_CASE("ual accessors", "[ual][accessors]", (Graph), (const Graph)) {
  static_assert(std::is_same_v<TestType, Graph> || std::is_same_v<TestType, const Graph>);
  using G = TestType;

  vector<double> the_vtx_vals = {10.0, 11.0, 12.0};

  using edge_data_type                = tuple<uint32_t, uint32_t, double>;
  using edge_key_type                 = edge_key_t<G, vertex_edge_iterator_t<G>>;
  vector<edge_data_type> the_edg_vals = {{0, 1, 1.1}, {0, 2, 2.1}, {1, 2, 2.2}, {2, 0, 0.1}}; // {ukey, vkey, val}

  auto ekey_fnc = [](const edge_data_type& data) -> edge_key_type { return edge_key_type(get<0>(data), get<1>(data)); };
  auto eval_fnc = [](const edge_data_type& data) -> double { return get<2>(data); };
  auto vval_fnc = [](const double& vtxval) -> double { return vtxval; };
  G    g(the_edg_vals, the_vtx_vals, ekey_fnc, eval_fnc, vval_fnc, 7.0);

  //
  // graph values
  //
  SECTION("graph_value(g)") {
    REQUIRE(graph_value(g) == 7); // no fmt
  }

  //
  // vertex range & vertex values
  //
  SECTION("vertices(g)") {
    auto& vv = vertices(g);
    REQUIRE(is_same_const(g, vv));
    REQUIRE(size(vertices(g)) == 3);
    REQUIRE(std::ranges::random_access_range<decltype(vv)>);

    auto u = ++begin(vv);
    REQUIRE(vertex_key(g, u) == 1);
    REQUIRE(vertex_value(g, u) == 11);
    REQUIRE(find_vertex(g, 1) == u);
  }

  //
  // vertex-edge range & edge values
  //
  SECTION("edges(g,u)") {
    auto  u  = ++begin(vertices(g));
    auto& ee = edges(g, u);
    REQUIRE(size(ee) == 2);

    auto uv = begin(ee);
    REQUIRE(edge_key(g, uv).first == 0);
    REQUIRE(edge_key(g, uv).second == 1);
    REQUIRE(edge_value(g, uv) == 1.1);
    REQUIRE(target(g, uv) == find_vertex(g, 1));
    REQUIRE(target_key(g, uv) == 1);
    REQUIRE(target(g, uv) == u); // b/c vertices are unordered on the edge
  }

  SECTION("sourced_edges(g,u)") {
    auto  x    = ++begin(vertices(g));
    auto  xkey = vertex_key(g, x);
    auto& ee   = edges(g, x); // eval'd by CPO for vertex_iterator that points to a forward_range object

    auto uv   = begin(ee);
    auto u    = source(g, uv);
    auto ukey = source_key(g, uv);
    auto v    = target(g, uv);
    auto vkey = target_key(g, uv);

    REQUIRE(target(g, uv) == x);
    REQUIRE(source_key(g, uv) == 0);
    REQUIRE(source_key(g, uv) == vertex_key(g, u));
#if 0
    REQUIRE(other_vertex(g, uv, u) == v);
    REQUIRE(other_vertex(g, uv, v) == u);
    REQUIRE(other_vertex_key(g, uv, ukey) == vkey);
    REQUIRE(other_vertex_key(g, uv, vkey) == ukey);
    REQUIRE(edge_key(g, uv).first == 0);  // n/a because edge only has source key on it
    REQUIRE(edge_key(g, uv).second == 1); // n/a because edge only has source key on it
#endif
  }
} //TEMPLATE_TEST_CASE("ual accessors"
