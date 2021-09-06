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

/*
                                                adl mem
                                            vol smp smp dav ual
  graph_value(g)                            x   x   x   x   x

  vertices(g)                               x   x   x   x   x
    vertex_key(g,u)                         x   x   x   x   x
    vertex_value(g,u)                       x   x   x
    find_vertex(g,ukey)

  edges(g,u)                                x   x   x   x   x
    edge_key(g,uv)                          na  x   x   x   x
    target(g,uv)
    target_key(g,uv)
    edge_value(g,uv)

    source(g,uv);
    source_key(g,uv)
    vertex(g,uv,u)
    vertex_key(g,uv,ukey)

    find_vertex_edge(g, u, v)
    find_vertex_edge(g, ukey, vkey)

  vertices(g,u)                             
    edge_key(g,uv)
    target(g,uv)
    target_key(g,uv)

    source(g,uv);
    source_key(g,uv)
    vertex(g,uv,u)
    vertex_key(g,uv,ukey)

    find_vertex_vertex(g, u, v)
    find_vertex_vertex(g, ukey, vkey)
  
  edges(g)                                  
    edge_key(g,uv)
    target(g,uv)
    target_key(g,uv)
    edge_value(g,uv)

    source(g,uv);
    source_key(g,uv)
    vertex(g,uv,u)
    vertex_key(g,uv,ukey)

    find_edge(g, u, v)
    find_edge(g, ukey, vkey)

  contains_edge(g, u, v)
  contains_edge(g, ukey, vkey)

  outward_edges(g,u)  
  outward_vertices(g,u)
  inward_edges(g,u)                         na  na  na  na  na
  inward_vertices(g,u)                      na  na  na  na  na

  find_vertex_outward_edge(g, u, v)         na  na  na  na  na
  find_vertex_outward_edge(g, ukey, vkey)   na  na  na  na  na
  find_vertex_inward_edge(g, u, v)          na  na  na  na  na
  find_vertex_inward_edge(g, ukey, vkey)    na  na  na  na  na

  Notes
  vol requires accessors in std::
  CPO
    1.  default impl: vertices(g), edges(g,u), vertex_key(g,u)
    2.  returning subrange values (views), not range reference
*/


template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

//
// vol - vector of list - graph
//
namespace vol_adl {
using vol_key          = size_t;
using vol_edge_val     = double;
using vol_edge_type    = pair<vol_key, vol_edge_val>;
using vol_edge_range   = list<vol_edge_type>;
using vol_vertex_type  = vol_edge_range; // no vertex value for this graph; vertex only has the edges
using vol_vertex_range = vector<vol_vertex_type>;
using vol_graph        = vol_vertex_range;
} // namespace vol_adl

// create free functions for ADL
// they're created in std:: to match namespace of vector, a requirement for ADL
namespace std {
// vertices(g) -> vol_vertex_range (uses graph as the range)
// edges(g,u) -> vold_edge_range (used vertex as the range)

//
// value accessors
// in the same namespace as their containers (std::)
//
auto& graph_value(vol_adl::vol_graph& g) {
  static int val = 7; // a bogus value only for validation
  return val;
}
const auto& graph_value(const vol_adl::vol_graph& g) {
  static const int val = 7;
  return val; // a bogus value only for validation
}

// vertex_key(g,u) is eval'd by CPO for random-access iterators

auto& vertex_value(vol_adl::vol_graph& g, vertex_iterator_t<vol_adl::vol_graph> u) {
  static int val = 8; // a bogus value only for validation
  return val;
}
auto& vertex_value(const vol_adl::vol_graph& g, vertex_iterator_t<const vol_adl::vol_graph> u) {
  static int val = 8; // a bogus value only for validation
  return val;
}

auto&       edge_value(vol_adl::vol_graph&, list<vol_adl::vol_edge_type>::iterator uv) { return uv->second; }
const auto& edge_value(const vol_adl::vol_graph&, list<vol_adl::vol_edge_type>::const_iterator uv) {
  return uv->second;
}
} // namespace std


namespace vol_adl {
TEMPLATE_TEST_CASE("vol graph", "[vol][accessors]", (vol_graph), (const vol_graph)) {
  static_assert(std::is_same_v<TestType, vol_graph> || std::is_same_v<TestType, const vol_graph>);
  using G = TestType;

  G g{{{1, 1.1}, {2, 2.1}}, {{2, 2.2}}, {{0, 0.1}}};

  using namespace std::graph;

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
    REQUIRE(vertex_key(g, u) == 1); // eval'd by CPO for random_access_range
    REQUIRE(vertex_value(g, u) == 8);
  }

  //
  // vertex-edge range & edge values
  //
  SECTION("edges(g,u)") {
    auto u = ++begin(vertices(g));

    auto& ee = edges(g, u); // eval'd by CPO for vertex_iterator that points to a forward_range object
    REQUIRE(size(ee) == 1);
    auto uv = begin(ee);
    //REQUIRE(edge_key(g, uv).first == 1);  // n/a because edge only has target key on it
    //REQUIRE(edge_key(g, uv).second == 2); // n/a because edge only has target key on it
    REQUIRE(edge_value(g, uv) == 2.2);
  }
} // TEMPLATE_TEST_CASE
} // namespace vol_adl
