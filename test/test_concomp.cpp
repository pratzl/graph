//#include "pch.h"
#include "graph/directed_adjacency_vector.hpp"
#include "graph/range/depth_first_search.hpp"
#include "graph/range/breadth_first_search.hpp"
#include "graph/algorithm/components.hpp"
#include "data_routes.hpp"
#include <iostream>
#include <catch2/catch.hpp>

#define EXPECT_EQ(a, b) REQUIRE((a) == (b))
#define EXPECT_NE(a, b) REQUIRE((a) != (b))
#define EXPECT_FALSE(a) REQUIRE(!(a))
#define EXPECT_TRUE(a) REQUIRE(a);

// Google Test compatibility
#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST

using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;

using namespace std::graph; // Bring graph functions into global namespace

using DollarGraph = GraphXlate<daa_data_edge_mapper>::target_graph_t;


TEST_CASE("daa dollar structure", "[daa][dollar][structure]") {

  DollarGraph g = dollar_directed_graph.create_graph();
  EXPECT_EQ(dollar_directed_graph.vertex_values().size(), vertices_size(g));
  EXPECT_EQ(dollar_directed_graph.edge_values().size(), edges_size(g));

  EXPECT_EQ(6, vertices_size(g));
  EXPECT_EQ(6, edges_size(g));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (vertex_t<DollarGraph>& u : vertices(g)) {
    vertex_key_t<DollarGraph> u_key  = vertex_key(g, u);
    const string&             u_name = value(u).name;
    cout << "[" << u_key << "] " << u_name << "\n";
    for (edge_t<DollarGraph>& uv : edges(g, u)) {
      vertex_iterator_t<DollarGraph> vi     = vertex(g, uv, u);
      vertex_key_t<DollarGraph>      v_key  = vertex_key(g, *vi);
      const string&                  v_name = value(*vi).name;
      cout << "  --> [" << v_key << " " << v_name << "] weight=" << value(uv).weight << "\n";
    }
  }

  /* Output
    [0] a1
      --> [2 b1] weight=100
    [1] a2
      --> [3 b2] weight=100
    [2] b1
      --> [4 c1] weight=50
      --> [3 b2] weight=50
    [3] b2
      --> [5 c2] weight=90
      --> [2 b1] weight=10
    [4] c1
    [5] c2
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  vertex_iterator_t<DollarGraph>      u;
  vertex_edge_iterator_t<DollarGraph> uv;

  cout << "vertex_iterator_t<DollarGraph>      u; \n";
  cout << "vertex_edge_iterator_t<DollarGraph> uv;\n";

  for (size_t ui = 0; ui < vertices_size(g); ++ui) {
    cout << "\n";
    u = begin(g) + ui;
    cout << "u = begin(g) + " << ui << ";\n";
    cout << "EXPECT_EQ(\"" << u->name << "\", u->name);\n";
    cout << "EXPECT_EQ(" << edges_size(g, *u) << ", edges_size(g, *u));\n";
    cout << "uv = begin(g, *u);\n";
    size_t uvi = 0;
    for (uv = begin(g, *u); uv != end(g, *u); ++uv, ++uvi) {
      if (uvi > 0) {
        cout << "++uv;\n";
      }
      cout << "EXPECT_EQ(" << outward_vertex_key(g, *uv) << ", outward_vertex_key(g, *uv));\n";
      cout << "EXPECT_EQ(\"" << outward_vertex(g, *uv)->name << "\", outward_vertex(g, *uv)->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  vertex_iterator_t<DollarGraph>      u;
  vertex_edge_iterator_t<DollarGraph> uv;

  u = begin(g) + 0;
  EXPECT_EQ("a1", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(2, outward_vertex_key(g, *uv));
  EXPECT_EQ("b1", outward_vertex(g, *uv)->name);
  EXPECT_EQ(100, uv->weight);

  u = begin(g) + 1;
  EXPECT_EQ("a2", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(3, outward_vertex_key(g, *uv));
  EXPECT_EQ("b2", outward_vertex(g, *uv)->name);
  EXPECT_EQ(100, uv->weight);

  u = begin(g) + 2;
  EXPECT_EQ("b1", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(4, outward_vertex_key(g, *uv));
  EXPECT_EQ("c1", outward_vertex(g, *uv)->name);
  EXPECT_EQ(50, uv->weight);
  ++uv;
  EXPECT_EQ(3, outward_vertex_key(g, *uv));
  EXPECT_EQ("b2", outward_vertex(g, *uv)->name);
  EXPECT_EQ(50, uv->weight);

  u = begin(g) + 3;
  EXPECT_EQ("b2", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(5, outward_vertex_key(g, *uv));
  EXPECT_EQ("c2", outward_vertex(g, *uv)->name);
  EXPECT_EQ(90, uv->weight);
  ++uv;
  EXPECT_EQ(2, outward_vertex_key(g, *uv));
  EXPECT_EQ("b1", outward_vertex(g, *uv)->name);
  EXPECT_EQ(10, uv->weight);

  u = begin(g) + 4;
  EXPECT_EQ("c1", u->name);
  EXPECT_EQ(0, edges_size(g, *u));
  uv = begin(g, *u);

  u = begin(g) + 5;
  EXPECT_EQ("c2", u->name);
  EXPECT_EQ(0, edges_size(g, *u));
  uv = begin(g, *u);
#endif
}

TEST_CASE("daa dollar stongly connected components", "[daa][dollar][components][connected]") {

  DollarGraph g = dollar_directed_graph.create_graph();
  EXPECT_EQ(dollar_directed_graph.vertex_values().size(), vertices_size(g));
  EXPECT_EQ(dollar_directed_graph.edge_values().size(), edges_size(g));

  EXPECT_EQ(6, vertices_size(g));
  EXPECT_EQ(6, edges_size(g));

  using comp_t  = std::graph::component<DollarGraph>;
  using comps_t = std::vector<comp_t>;
  comps_t comps;

  //strongly_connected_components(g, vertices(g), std::back_inserter(comps));
  strongly_connected_components(g, begin(vertices(g)) + 2, std::back_inserter(comps));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  cout << "Dollar strongly connected components\n";
  for (const comp_t& comp : comps) {
    cout << "[" << comp.component_number << "] " << comp.vertex->name << "\n";
  }

  /* Output
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}
