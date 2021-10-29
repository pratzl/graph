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
  auto size() const { return the_edges.size(); }
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
  //auto size() const { return the_vertices.size(); } // ranges::size will calc size of random_access_range
};

TEMPLATE_TEST_CASE("simple graph rng", "[simple][accessors][rng]", (simple_graph), (const simple_graph)) {
  static_assert(std::is_same_v<TestType, simple_graph> || std::is_same_v<TestType, const simple_graph>);
  using G = TestType;
  G g;

  using namespace std::graph;
  using namespace std::ranges;

  // graph values
  SECTION("graph_value(g)") {
    //REQUIRE(graph_value(g) == 7); // graph_value(g) isn't defined for this graph
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
    //REQUIRE(vertex_value(g, u) == 11); // vertex_value(g,u) isn't defined for this graph
    REQUIRE(find_vertex(g, 1) == u);
  }

  //
  // vertex-edge range & edge values
  //
  SECTION("edges(g,u)") {
    auto u = ++begin(vertices(g));

    auto& ee = edges(g, u);
    REQUIRE(size(ee) == 1);
    auto uv = begin(ee);
    //REQUIRE(edge_key(g, uv).first == 1);         // n/a because ource_key(g,uv) not defined
    //REQUIRE(edge_key(g, uv).second == 2);        // n/a because ource_key(g,uv) not defined
    //REQUIRE(edge_value(g, uv) == 2.2);           // edge_value(g,uv) isn't defined for this graph
    //REQUIRE(target(g, uv) == find_vertex(g, 2)); // target(g,uv) isn't defined
    //REQUIRE(target_key(g, uv) == 2);             // target_key(g,uv) isn't defined
  }
}

} // namespace rng
