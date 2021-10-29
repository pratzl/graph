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

using std::graph::vertices;
//
// free functions to test ADL (argument-dependent lookup)
//
auto& graph_value(simple_graph& g) { return g.the_value; }
auto& graph_value(const simple_graph& g) { return g.the_value; }

auto& tag_invoke(std::graph::vertices_fn_t, simple_graph& g) noexcept { return g.the_vertices; }
auto& tag_invoke(std::graph::vertices_fn_t, const simple_graph& g) noexcept { return g.the_vertices; }

auto& vertex_value(simple_graph& g, vertex_iterator_t<simple_graph> u) { return u->the_value; }
auto& vertex_value(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u->the_value; }

auto vertex_key(const simple_graph& g, vertex_iterator_t<const simple_graph> u) {
  return static_cast<simple_graph::vertex_key_type>(u - g.the_vertices.begin());
}

auto find_vertex(simple_graph& g, vertex_key_t<simple_graph> ukey) { return begin(vertices(g)) + ukey; }
auto find_vertex(const simple_graph& g, vertex_key_t<simple_graph> ukey) { return begin(vertices(g)) + ukey; }

auto& edges(simple_graph& g, vertex_iterator_t<simple_graph> u) { return u->the_edges; }
auto& edges(const simple_graph& g, vertex_iterator_t<const simple_graph> u) { return u->the_edges; }

auto edge_key(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_key; }

auto& edge_value(simple_graph& g, vertex_edge_iterator_t<simple_graph> uv) { return uv->the_value; }
auto& edge_value(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_value; }

auto source(simple_graph& g, vertex_edge_iterator_t<simple_graph> uv) { return begin(vertices(g)) + uv->the_key.first; }
auto source(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) {
  return begin(vertices(g)) + uv->the_key.first;
}
auto source_key(simple_graph const& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_key.first; }


auto target(simple_graph& g, vertex_edge_iterator_t<simple_graph> uv) {
  return begin(vertices(g)) + uv->the_key.second;
}
auto target(const simple_graph& g, vertex_edge_iterator_t<const simple_graph> uv) {
  return begin(vertices(g)) + uv->the_key.second;
}
auto target_key(simple_graph const& g, vertex_edge_iterator_t<const simple_graph> uv) { return uv->the_key.second; }

auto other_vertex(simple_graph& g, vertex_edge_iterator_t<simple_graph> uv, vertex_iterator_t<simple_graph> src) {
  return src == source(g, uv) ? target(g, uv) : source(g, uv);
}
auto other_vertex(simple_graph const&                        g,
                  vertex_edge_iterator_t<const simple_graph> uv,
                  vertex_iterator_t<const simple_graph>      src) {
  return src == source(g, uv) ? target(g, uv) : source(g, uv);
}
auto other_vertex_key(simple_graph const&                        g,
                      vertex_edge_iterator_t<const simple_graph> uv,
                      vertex_key_t<const simple_graph>           src_key) {
  return src_key == source_key(g, uv) ? target_key(g, uv) : source_key(g, uv);
}

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

  using namespace std::graph;

  // graph values
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
    //REQUIRE(vertex_key(g, u) == 1);
    REQUIRE(vertex_value(g, u) == 11);
    REQUIRE(find_vertex(g, 1) == u);
  }

  //
  // vertex-edge range & edge values
  //
  SECTION("edges(g,u)") {
    auto  u  = ++begin(vertices(g));
    auto& ee = edges(g, u);
    REQUIRE(size(ee) == 1);

    auto uv = begin(ee);
    REQUIRE(edge_key(g, uv).first == 1);
    REQUIRE(edge_key(g, uv).second == 2);
    REQUIRE(edge_value(g, uv) == 2.2);
    REQUIRE(target(g, uv) == find_vertex(g, 2));
    REQUIRE(target_key(g, uv) == 2);
  }

  SECTION("sourced_edges(g,u)") {
    auto  u    = ++begin(vertices(g));
    auto  ukey = vertex_key(g, u);
    auto& ee   = edges(g, u); // eval'd by CPO for vertex_iterator that points to a forward_range object

    auto uv   = begin(ee);
    auto v    = target(g, uv);
    auto vkey = vertex_key(g, v);

    REQUIRE(source(g, uv) == u);
    REQUIRE(source_key(g, uv) == 1);
    REQUIRE(source_key(g, uv) == vertex_key(g, u));
    REQUIRE(other_vertex(g, uv, u) == v);
    REQUIRE(other_vertex(g, uv, v) == u);
    REQUIRE(other_vertex_key(g, uv, ukey) == vkey);
    REQUIRE(other_vertex_key(g, uv, vkey) == ukey);
    //REQUIRE(edge_key(g, uv).first == 1);  // n/a because edge only has source key on it
    //REQUIRE(edge_key(g, uv).second == 2); // n/a because edge only has source key on it
  }
}

} // namespace adl
