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
                                                adl mem rng
                                            vol smp smp smp dav ual
  graph_value(g)                            x   x   x   na  x   x

  vertices(g)                               x   x   x   x   x   x
    vertex_key(g,u)                         x   x   x   x   x   x
    vertex_value(g,u)                       x   x   x   na  x   x
    find_vertex(g,ukey)                     x   x   x   x   x   x
    degree(g,u)

  edges(g,u)                                x   x   x   x   x   x
    edge_key(g,uv)                          na  x   x   x   x   x
    target(g,uv)                            x   x   x   na  x   x
    target_key(g,uv)                        x   x   x   na  x   x
    edge_value(g,uv)                        x   x   x   na  x   x

    source(g,uv);                           na  x   x   na  x   x
    source_key(g,uv)                        na  x   x   na  x   x
    vertex(g,uv,src)
    vertex_key(g,uv,src_key)

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

auto&       edge_value(vol_adl::vol_graph&, vol_adl::vol_edge_range::iterator uv) { return uv->second; }
const auto& edge_value(const vol_adl::vol_graph&, vol_adl::vol_edge_range::const_iterator uv) { return uv->second; }

auto target_key(vol_adl::vol_graph& g, vol_adl::vol_edge_range::iterator uv) { return uv->first; }
auto target_key(const vol_adl::vol_graph& g, vol_adl::vol_edge_range::const_iterator uv) { return uv->first; }

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
    REQUIRE(find_vertex(g, 1) == u);
    //static_assert(std::is_signed_v<decltype(vertex_key(g, u))>);
  }

  //
  // vertex-edge range & edge values
  //
  SECTION("edges(g,u)") {
    auto  u  = ++begin(vertices(g));
    auto& ee = edges(g, u); // eval'd by CPO for vertex_iterator that points to a forward_range object
    REQUIRE(size(ee) == 1);

    auto uv = begin(ee);
    REQUIRE(edge_value(g, uv) == 2.2);
    REQUIRE(target(g, uv) == find_vertex(g, 2));
    REQUIRE(target_key(g, uv) == 2);
    static_assert(std::is_unsigned_v<decltype(target_key(g, uv))>);
    static_assert(std::is_unsigned_v<decltype(vertex_key(g, target(g, uv)))>);
    REQUIRE(target_key(g, uv) == vertex_key(g, target(g, uv)));
  }

  SECTION("sourced_edges(g,u)") {
    auto  u  = ++begin(vertices(g));
    auto& ee = edges(g, u); // eval'd by CPO for vertex_iterator that points to a forward_range object

    auto uv = begin(ee);
    //REQUIRE(source(g, uv) == find_vertex(g, 1));
    //REQUIRE(source_key(g, uv) == 0);
    //static_assert(std::is_unsigned_v<decltype(source_key(g, uv))>);
    //REQUIRE(source_key(g, uv) == vertex_key(g, source(g, uv)));
    //REQUIRE(edge_key(g, uv).first == 1);  // n/a because edge only has source key on it
    //REQUIRE(edge_key(g, uv).second == 2); // n/a because edge only has source key on it
  }
} // TEMPLATE_TEST_CASE
} // namespace vol_adl
