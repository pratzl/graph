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
namespace rng {

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
};

struct simple_vertex : public simple_ns::simple_vertex_base<simple_edge> {
  using base_t = simple_ns::simple_vertex_base<simple_edge>;

  simple_vertex(vertex_key_type key, vertex_value_type val, const edge_range& init) : base_t(key, val, init) {}
  simple_vertex()                     = default;
  simple_vertex(simple_vertex const&) = default;
  ~simple_vertex()                    = default;
  simple_vertex& operator=(simple_vertex const&) = default;

  auto begin() { return the_edges.begin(); }
  auto begin() const { return the_edges.begin(); }
  auto end() { return the_edges.end(); }
  auto end() const { return the_edges.end(); }
};

struct simple_graph : public simple_ns::simple_graph_base<simple_vertex, simple_edge> {
  using base_t          = simple_ns::simple_graph_base<simple_vertex, simple_edge>;
  using value_type      = typename vertex_range::value_type;
  using pointer         = typename vertex_range::pointer;
  using const_pointer   = typename vertex_range::const_pointer;
  using reference       = typename vertex_range::reference;
  using const_reference = typename vertex_range::const_reference;
  using const_iterator  = typename vertex_range::const_iterator;
  using iterator        = typename vertex_range::iterator;
  using size_type       = typename vertex_range::size_type;
  using difference_type = typename vertex_range::difference_type;

  simple_graph()                    = default;
  simple_graph(simple_graph const&) = default;
  ~simple_graph()                   = default;
  simple_graph& operator=(simple_graph const&) = default;

  auto begin() { return the_vertices.begin(); }
  auto begin() const { return the_vertices.begin(); }
  auto end() { return the_vertices.end(); }
  auto end() const { return the_vertices.end(); }
  //auto size() const { return the_vertices.size(); }
};

// TEST_CASE("cpo accessor", "[cpo][trace]") {
//   simple_graph g;
//   auto         it    = std::ranges::begin(g);
//   auto         value = std::graph::graph_value(g);
//   static_assert(std::graph::_graph_value_::_gph_has_ADL<simple_graph>);
//
//   int x = 0;
// }

TEMPLATE_TEST_CASE("simple graph rng", "[simple][accessors][rng]", (simple_graph), (const simple_graph)) {
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
    REQUIRE(std::ranges::size(vertices(g)) == 3);
    //auto u = begin(vv);
    //REQUIRE(u == g.begin());
    //REQUIRE(u->size() == 2);
    //REQUIRE(size(edges(g, u)) == 2);
  }

  //
  // value types
  //

  // graph values
  //SECTION("graph_value(g)") { REQUIRE(graph_value(g) == 7); }

  // vertex values
  SECTION("vertex_key(g,u)") {
    //REQUIRE(vertex_key(g, ++begin(vertices(g))) == 1); // no fmt
    //REQUIRE(vertex_value(g, begin(vertices(g))) == 8); // no fmt
  }
}

} // namespace rng
