#include <iostream>
#include "simple_graph.hpp"
#include <catch2/catch.hpp>
#include "graph/graph_utility.hpp"
#include "using_graph.hpp"

using std::cout;
using std::endl;
using std::pair;


// A new namespace and graph type is defined for each set of member/adl tests
// to assure there is no carry-over definitions from other tests.
namespace member {

//
// The Graph
//
struct simple_edge;
struct simple_vertex;
struct simple_graph;

struct simple_edge : public simple_ns::simple_edge_base {
  using base_t = simple_ns::simple_edge_base;

  simple_edge(const edge_key_type& k, edge_value_type const& val = edge_value_type{}) : base_t(k, val) {}
  simple_edge()                   = default;
  simple_edge(simple_edge const&) = default;
  ~simple_edge()                  = default;
  simple_edge& operator=(simple_edge const&) = default;

  auto edge_key(const simple_graph&) const { return the_key; }

  auto& edge_value(simple_graph&) { return the_value; }
  auto& edge_value(const simple_graph&) const { return the_value; }
};

struct simple_vertex : public simple_ns::simple_vertex_base<simple_edge> {
  using base_t = simple_ns::simple_vertex_base<simple_edge>;

  simple_vertex(vertex_key_type key, vertex_value_type val, const edge_range& init) : base_t(key, val, init) {}
  simple_vertex()                     = default;
  simple_vertex(simple_vertex const&) = default;
  ~simple_vertex()                    = default;
  simple_vertex& operator=(simple_vertex const&) = default;

  auto vertex_key(const simple_graph&) const { return the_key; }

  auto& vertex_value(simple_graph& g) { return the_value; }
  auto& vertex_value(const simple_graph& g) const { return the_value; }

  auto& edges(simple_graph&) { return the_edges; }
  auto& edges(const simple_graph&) const { return the_edges; }
};

struct simple_graph : public simple_ns::simple_graph_base<simple_vertex, simple_edge> {
  using base_t = simple_ns::simple_graph_base<simple_vertex, simple_edge>;

  simple_graph()                    = default;
  simple_graph(simple_graph const&) = default;
  ~simple_graph()                   = default;
  simple_graph& operator=(simple_graph const&) = default;

  auto& vertices() { return the_vertices; }
  auto& vertices() const { return the_vertices; }

  auto& graph_value() { return the_value; }
  auto& graph_value() const { return the_value; }
};

TEMPLATE_TEST_CASE("simple graph member", "[simple][accessors][member]", (simple_graph), (const simple_graph)) {
  static_assert(std::is_same_v<TestType, simple_graph> || std::is_same_v<TestType, const simple_graph>);
  using G = TestType;
  G g;

  using namespace std::graph;

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

} // namespace member
