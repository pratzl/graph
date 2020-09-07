//#include "pch.h"
#include "vov_adaptor.hpp"
#include "data_routes.hpp"
#include "graph/range/dfs.hpp"

#include <iostream>
#include <catch2/catch.hpp>

// Google Test compatibility
#define EXPECT_EQ(a, b) REQUIRE((a) == (b))
#define EXPECT_NE(a, b) REQUIRE((a) != (b))
#define EXPECT_FALSE(a) REQUIRE(!(a))
#define EXPECT_TRUE(a) REQUIRE(a);

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST

using std::string;
using std::cout;
using std::endl;


namespace graph::adaptor {} // namespace graph::adaptor

using vov_germany_t      = vov<double>;
using vov_germany_wrap_t = vov_germany_t;

vov_germany_t get_germany_routes() {
  using directed_graph_t = typename GraphXlate<daa_routes_edge_mapper>::target_graph_t;
  directed_graph_t daa_routes_graph(
        germany_routes_directed_graph.create_graph()); // translates string labels to indexes

  vov_germany_t routes(vertices_size(daa_routes_graph));
  for (std::graph::edge_t<directed_graph_t>& uv : edges(daa_routes_graph)) {
    std::graph::vertex_key_t<directed_graph_t> u_key = inward_vertex_key(daa_routes_graph, uv);
    std::graph::vertex_key_t<directed_graph_t> v_key = outward_vertex_key(daa_routes_graph, uv);
    routes.push_back(u_key, v_key, uv.weight);
  }

  //auto n = std::graph::vertices_size(routes);
  //std::graph::vertex_key_t<vov_germany_t> ukey = std::graph::vertex_key(routes, *std::graph::begin(routes));
  //cout << ukey;

  return routes;
}


TEST_CASE("vov graph", "[vov]") {
  //using namespace graph::adaptor;
  using namespace std::graph;

  using Graph = vov_germany_t;
  Graph g     = get_germany_routes();

  //auto n = vertices_size(g);

  REQUIRE(10 == vertices_size(g));
  REQUIRE(11 == edges_size(g));

  using lhs  = std::forward_list<std::tuple<size_t, double>>;
  using rhs  = typename vov_germany_t::inner_container;
  using et   = std::graph::edge_t<Graph>;
  using evt  = std::graph::edge_value_t<Graph>;
  using evt2 = typename vov_germany_t::inner_container::value_type;
  static_assert(std::is_same_v<lhs, rhs>, "not same");
  static_assert(std::is_same_v<et, std::tuple<size_t, double>>, "edge types not same");
  static_assert(std::is_same_v<evt, std::tuple<double>>, "edge value types not same");
  static_assert(std::is_same_v<evt2, std::tuple<size_t, double>>, "edge value types2 not same");

#if 0
  for (std::graph::vertex_t<Graph>& u : vertices(g)) {
    cout << "city[" << vertex_key(g, u) << "]\n";
    for (std::graph::edge_t<Graph>& uv : edges(g, u)) {
      //graph::adaptor::value(uv);    // compiler error in msvc (bug?)
      cout << "  --> city[" << outward_vertex_key(g, uv) << "] " << get<1>(uv) << "km\n";
    }
  }
#endif
  /* Output: seed = Frankfürt
    Germany Routes
    -------------------------------
    [0] Augsburg
      --> [6 München] 84km
    [1] Erfurt
    [2] Frankfürt
      --> [4 Kassel] 173km
      --> [9 Würzburg] 217km
      --> [5 Mannheim] 85km
    [3] Karlsruhe
      --> [0 Augsburg] 250km
    [4] Kassel
      --> [6 München] 502km
    [5] Mannheim
      --> [3 Karlsruhe] 80km
    [6] München
    [7] Nürnberg
      --> [6 München] 167km
      --> [8 Stuttgart] 183km
    [8] Stuttgart
    [9] Würzburg
      --> [7 Nürnberg] 103km
      --> [1 Erfurt] 186km
  */
}

TEST_CASE("vov dfs vertex", "[vov][dfs][vertex]") {
  using namespace std::graph;
  using namespace graph::adaptor;
  using std::graph::dfs_vertex_range;
  using Graph = vov_germany_t;

  vov_germany_t    g = get_germany_routes();
  dfs_vertex_range dfs_vtx_rng(g, begin(g) + 2); // "Frankfürt"

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u) {
    //std::graph::vertex_key_t<Graph> ukey = vertex_key(g, *u);
    // cout << string(u.depth() * 2, ' ') << vertex_key(g, *u) << endl;
  }

  /* Output: seed = Frankfürt
    2 
      4
        6
      9
        7
          8
        1
      5
        3
          0
  */

  // a flat list when using range syntax (depth n/a on vertex)
  cout << endl;
  for (std::graph::vertex_t<Graph>& u : dfs_vertex_range(g, begin(g) + 2)) // Frankfürt
    cout << vertex_key(g, u) << endl;
    /* Output: seed = Frankfürt
      2
      4
      6
      9
      7
      8
      1
      5
      3
      0
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  int i = 0;
  for (dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u, ++i) {
    if (i == 0)
      cout << "EXPECT_EQ(" << vertex_key(g, *u) << ", vertex_key(g, *u));\n";
    else
      cout << "EXPECT_EQ(" << vertex_key(g, *u) << ", vertex_key(g, *(++u)));\n";
    cout << "EXPECT_EQ(" << u.depth() << ", u.depth());\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin();
  EXPECT_EQ(2, vertex_key(g, *u));
  EXPECT_EQ(1, u.depth());
  EXPECT_EQ(4, vertex_key(g, *(++u)));
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ(6, vertex_key(g, *(++u)));
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ(9, vertex_key(g, *(++u)));
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ(7, vertex_key(g, *(++u)));
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ(8, vertex_key(g, *(++u)));
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ(1, vertex_key(g, *(++u)));
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ(5, vertex_key(g, *(++u)));
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ(3, vertex_key(g, *(++u)));
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ(0, vertex_key(g, *(++u)));
  EXPECT_EQ(4, u.depth());
#endif // TEST_OPTION==
}
