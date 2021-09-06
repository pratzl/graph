#include <iostream>
#include "simple_graph.hpp"
#include <catch2/catch.hpp>
#include "graph/graph_utility.hpp"
#include "using_graph.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::list;


// A new namespace and graph type is defined for each set of member/adl tests
// to assure there is no carry-over definitions from other tests.
namespace adl {

//
// The Graph
//
struct simple_edge : public simple_ns::simple_edge_base {
  using base_t = simple_ns::simple_edge_base;

  simple_edge(const edge_key_type& k, edge_value_type const& val = edge_value_type{}) : base_t(k, val) {}
  simple_edge()                   = default;
  simple_edge(simple_edge const&) = default;
  ~simple_edge()                  = default;
  simple_edge& operator=(simple_edge const&) = default;
};

struct simple_vertex : public simple_ns::simple_vertex_base<simple_edge> {
  using base_t = simple_ns::simple_vertex_base<simple_edge>;

  simple_vertex(vertex_key_type key, vertex_value_type val, const edge_range& init) : base_t(key, val, init) {}
  simple_vertex()                     = default;
  simple_vertex(simple_vertex const&) = default;
  ~simple_vertex()                    = default;
  simple_vertex& operator=(simple_vertex const&) = default;
};

struct simple_graph : public simple_ns::simple_graph_base<simple_vertex, simple_edge> {
  using base_t = simple_ns::simple_graph_base<simple_vertex, simple_edge>;

  simple_graph()                    = default;
  simple_graph(simple_graph const&) = default;
  ~simple_graph()                   = default;
  simple_graph& operator=(simple_graph const&) = default;
};

//
// free functions to test ADL (argument-dependent lookup)
//
auto& vertices(simple_graph& g) { return g.the_vertices; }
auto& vertices(const simple_graph& g) { return g.the_vertices; }

auto& edges(simple_graph& g, vertex_iterator_t<simple_graph> u) { return u->the_edges; }
auto& edges(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u->the_edges; }

auto& graph_value(simple_graph& g) { return g.the_value; }
auto& graph_value(const simple_graph& g) { return g.the_value; }

auto vertex_key(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u - g.the_vertices.begin(); }

auto& vertex_value(simple_graph& g, vertex_iterator_t<simple_graph> u) { return u->the_value; }
auto& vertex_value(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u->the_value; }

auto edge_key(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_key; }

auto&       edge_value(simple_graph& g, vertex_edge_iterator_t<simple_graph> uv) { return uv->the_value; }
const auto& edge_value(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_value; }

// TEST_CASE("cpo accessor", "[cpo][trace]") {
//   simple_graph g;
//   auto         it    = std::ranges::begin(g);
//   auto         value = std::graph::graph_value(g);
//   static_assert(std::graph::_graph_value_::_gph_has_ADL<simple_graph>);
//
//   int x = 0;
// }

TEMPLATE_TEST_CASE("simple graph adl", "[simple][accessors][adl]", (simple_graph), (const simple_graph)) {
  static_assert(std::is_same_v<TestType, simple_graph> || std::is_same_v<TestType, const simple_graph>);
  using G = TestType;
  G g;

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
    REQUIRE(std::ranges::random_access_range<decltype(vv)>);
    REQUIRE(size(vertices(g)) == 3);

    auto u = ++begin(vv);
    REQUIRE(vertex_key(g, u) == 1);
    REQUIRE(vertex_value(g, u) == 11);

    auto& ee = edges(g, u);
    REQUIRE(size(ee) == 1);
    auto uv = begin(ee);
    REQUIRE(edge_key(g, uv).first == 1);
    REQUIRE(edge_key(g, uv).second == 2);
    REQUIRE(edge_value(g, uv) == 2.2);
  }

  //
  // value types
  //

  // graph values
  SECTION("graph_value(g)") { REQUIRE(graph_value(g) == 7); }

  // vertex values
  SECTION("vertex_key(g,u)") {
    REQUIRE(vertex_key(g, ++begin(vertices(g))) == 1); // no fmt
    //REQUIRE(vertex_value(g, begin(vertices(g))) == 8); // no fmt
  }
}

} // namespace adl
