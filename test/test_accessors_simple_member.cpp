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

  auto source(simple_graph& g);
  auto source(simple_graph const& g) const;

  auto target(simple_graph& g);
  auto target(simple_graph const& g) const;

  auto source_key(simple_graph const& g) const noexcept { return the_key.first; }
  auto target_key(simple_graph const& g) const noexcept { return the_key.second; }

  auto other_vertex(simple_graph& g, std::vector<simple_vertex>::const_iterator src) const;
  auto other_vertex(const simple_graph& g, std::vector<simple_vertex>::const_iterator src) const;
  auto other_vertex_key(const simple_graph& g, size_t src_key) const;
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

  auto& graph_value() { return the_value; }
  auto& graph_value() const { return the_value; }

  auto& vertices() { return the_vertices; }
  auto& vertices() const { return the_vertices; }

  auto find_vertex(vertex_key_type ukey) { return the_vertices.begin() + ukey; }
  auto find_vertex(vertex_key_type ukey) const { return the_vertices.begin() + ukey; }
};


auto simple_edge::source(simple_graph& g) { return g.the_vertices.begin() + the_key.first; }
auto simple_edge::source(simple_graph const& g) const { return g.the_vertices.begin() + the_key.first; }

auto simple_edge::target(simple_graph& g) { return g.the_vertices.begin() + the_key.second; }
auto simple_edge::target(simple_graph const& g) const { return g.the_vertices.begin() + the_key.second; }

auto simple_edge::other_vertex(simple_graph& g, std::vector<simple_vertex>::const_iterator src) const {
  return src == source(g) ? target(g) : source(g);
}
auto simple_edge::other_vertex(const simple_graph& g, std::vector<simple_vertex>::const_iterator src) const {
  return src == source(g) ? target(g) : source(g);
}
auto simple_edge::other_vertex_key(const simple_graph& g, size_t src_key) const {
  return src_key == the_key.first ? the_key.second : the_key.first;
}

TEMPLATE_TEST_CASE("simple graph member", "[simple][accessors][member]", (simple_graph), (const simple_graph)) {
  static_assert(std::is_same_v<TestType, simple_graph> || std::is_same_v<TestType, const simple_graph>);
  using G = TestType;
  G g;

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
    REQUIRE(size(ee) == 1);

    auto uv = begin(ee);
    auto v  = target(g, uv);
    REQUIRE(edge_key(g, uv).first == 1);
    REQUIRE(edge_key(g, uv).second == 2);
    REQUIRE(edge_value(g, uv) == 2.2);
    REQUIRE(v == find_vertex(g, 2));
    REQUIRE(target_key(g, uv) == 2);
    //REQUIRE(vertex(g, uv, u) == target(g, uv));
    //REQUIRE(vertex_key(g, u, vertex_key(u)) == target_key(g, uv));
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
#if 0
    REQUIRE(other_vertex(g, uv, u) == v);
    REQUIRE(other_vertex(g, uv, v) == u);
    REQUIRE(other_vertex_key(g, uv, ukey) == vkey);
    REQUIRE(other_vertex_key(g, uv, vkey) == ukey);
    REQUIRE(edge_key(g, uv).first == 1);  // n/a because edge only has source key on it
    REQUIRE(edge_key(g, uv).second == 2); // n/a because edge only has source key on it
#endif
  }
}

} // namespace member
