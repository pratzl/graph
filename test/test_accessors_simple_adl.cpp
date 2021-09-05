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

template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}


namespace simple1 {
//
// simple_graph
//

struct simple_edge {
  using edge_key_type   = pair<size_t, size_t>;
  using edge_value_type = double;

  edge_key_type   the_key   = {};
  edge_value_type the_value = {};

  simple_edge(const edge_key_type& k, edge_value_type const& val = edge_value_type{}) : the_key(k), the_value(val) {}
  simple_edge()                   = default;
  simple_edge(simple_edge const&) = default;
  ~simple_edge()                  = default;
  simple_edge& operator=(simple_edge const&) = default;
};

struct simple_vertex {
  using vertex_key_type   = size_t;
  using vertex_value_type = double;
  using edge_range        = list<simple_edge>;

  vertex_key_type   the_key   = {};
  vertex_value_type the_value = {};
  edge_range        the_edges;

  simple_vertex(vertex_key_type key, vertex_value_type val, const edge_range& init)
        : the_key(key), the_value(val), the_edges(init) {}
  simple_vertex()                     = default;
  simple_vertex(simple_vertex const&) = default;
  ~simple_vertex()                    = default;
  simple_vertex& operator=(simple_vertex const&) = default;
};

struct simple_graph {
  using edge_range       = typename simple_vertex::edge_range;
  using vertex_range     = vector<simple_vertex>;
  using graph_value_type = int;

  graph_value_type the_value = {};
  vertex_range     the_vertices;

  simple_graph() { init(); };
  simple_graph(simple_graph const&) = default;
  ~simple_graph()                   = default;
  simple_graph& operator=(simple_graph const&) = default;

  void init() {
    the_value = 7;
    the_vertices.push_back(simple_vertex({0, 10, edge_range{{{0, 1}, 1.1}, {{0, 2}, 2.1}}}));
    the_vertices.push_back(simple_vertex({1, 11, edge_range{{{1, 2}, 2.2}}}));
    the_vertices.push_back(simple_vertex({2, 12, edge_range{{{2, 0}, 0.1}}}));
  }
};

auto& vertices(simple_graph& g) { return g.the_vertices; }
auto& vertices(const simple_graph& g) { return g.the_vertices; }

auto graph_value(simple_graph& g) { return g.the_value; }
auto graph_value(const simple_graph& g) { return g.the_value; }

auto vertex_key(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u - g.the_vertices.begin(); }

auto& vertex_value(simple_graph& g, vertex_iterator_t<simple_graph> u) { return u->the_value; }
auto& vertex_value(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u->the_value; }

// TEST_CASE("cpo accessor", "[cpo][trace]") {
//   simple_graph g;
//   auto         it    = std::ranges::begin(g);
//   auto         value = std::graph::graph_value(g);
//   static_assert(std::graph::_graph_value_::_gph_has_ADL<simple_graph>);
//
//   int x = 0;
// }

TEMPLATE_TEST_CASE("smple graph adl", "[simple][accessors][adl]", (simple_graph), (const simple_graph)) {
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
    //auto u = begin(vv);
    //REQUIRE(u == g.begin());
    //REQUIRE(u->size() == 2);
    //REQUIRE(size(edges(g, u)) == 2);
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

} // namespace simple1
