//#include "pch.h"
#include "graph/undirected_adjacency_list.hpp"
#include "graph/range/depth_first_search.hpp"
#include "graph/range/breadth_first_search.hpp"
#include "graph/algorithm/shortest_paths.hpp"
#include "data_routes.hpp"
#include "using_graph.hpp"
#include <iostream>
#include <catch2/catch.hpp>


/* ToDo
    1x  Get graph output to match expected
    2.  Replace vector(g,uv) --> vector(g,uv,source)

    Tests
    0.  pass: Basic creation, destruction, traversal
    1.  DFS
        a.  pass: depth_first_search_vertex_range
        b.  pass: depth_first_search_edge_range
    4.  BFS
        a.  pass: breadth_first_search_vertex_range
        b.  pass: breadth_first_search_edge_range
    5.  shortest_paths
        a. pass: dijkstra_shortest_distances
        b. pass: dijkstra_shortest_paths
        c. pass: bellman_ford_shortest_distances
        d. pass: bellman_ford_shortest_paths
 */

// Google Test compatibility
#define EXPECT_EQ(a, b) REQUIRE((a) == (b))
#define EXPECT_NE(a, b) REQUIRE((a) != (b))
#define EXPECT_FALSE(a) REQUIRE(!(a))
#define EXPECT_TRUE(a) REQUIRE(a);

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

using std::ranges::size;

using std::breadth_first_search_vertex_range;
using std::breadth_first_search_edge_range;
using std::depth_first_search_vertex_range;
using std::depth_first_search_edge_range;
using std::shortest_distance;

using Graph      = std::undirected_adjacency_list<name_value, weight_value>;
using vtx_iter_t = std::vertex_iterator_t<Graph>;
using vtx_key_t  = std::vertex_key_t<Graph>;

struct route;
using Routes = routes_t;

template <class OStream>
OStream& operator<<(OStream& os, const Graph& g);

using edge_kv = std::pair<Graph::edge_key_type, Graph::edge_value_type>;

const vector<edge_kv>& ual_germany_edge_routes = germany_routes_undirected_graph.edge_values();

vertex_iterator_t<Graph> find_city(Graph& g, string_view const city_name) {
  return std::ranges::find_if(g, [&city_name](vertex_t<Graph>& u) { return u.name == city_name; });
}

static Graph create_germany_routes_graph() { return germany_routes_undirected_graph.create_graph(); }
static const vector<std::string>& germany_cities() { return germany_routes_undirected_graph.vertex_values(); }

template <class OStream>
OStream& operator<<(OStream& os, const Graph& g) {
  for (const_vertex_iterator_t<Graph> u = begin(g); u != end(g); ++u) {
    vertex_key_t<Graph> ukey = vertex_key(g, u);
    os << "\n[" << ukey << "] " << u->name;
    const_vertex_edge_range_t<Graph> ve = edges(g, u);
    for (const_vertex_edge_iterator_t<Graph> uv = std::ranges::begin(ve); uv != std::ranges::end(ve); ++uv) {
      const_vertex_iterator_t<Graph> v    = vertex(g, uv, u);
      vertex_key_t<Graph>            vkey = vertex_key(g, v);
      os << "\n  <--> [" << vkey << " " << v->name << "] " << uv->weight << "km";
    }
  }
  os << "\n";
  return os;
}


TEST_CASE("ual minsize", "[ual][minsize]") {
  using G      = std::undirected_adjacency_list<>;
  size_t vsize = sizeof(typename G::vertex_type);
  size_t esize = sizeof(typename G::edge_type);
#ifdef _MSC_VER
  EXPECT_EQ(12, sizeof(typename G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(24, sizeof(typename G::edge_type));   // edge size = 8 bytes
#else
  EXPECT_EQ(24, sizeof(typename G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(48, sizeof(typename G::edge_type));   // edge size = 8 bytes
#endif
}

TEST_CASE("ual empty", "[ual][empty]") {
  Graph g;
  auto  ans = size(edges(g));
  EXPECT_EQ(0, size(g));
  EXPECT_EQ(0, size(edges(g)));
}

TEST_CASE("ual initializer list", "[ual][init][initializer list]") {
  Graph g0{};                     // empty graph
  Graph g1{{1, 2, 3}};            // one edge
  Graph g2{{1, 2, 3}, {4, 5, 6}}; // two edges
}

TEST_CASE("ual init", "[ual][init]") {
#if 0
  vector<Graph::edge_kv> daa_germany_edge_routes = to_edge_values(routes, germany_cities);
  Graph                          g(germany_cities, daa_germany_edge_routes);
#endif
  Graph g = create_germany_routes_graph();
  EXPECT_EQ(germany_cities().size(), size(g));
  EXPECT_EQ(ual_germany_edge_routes.size(), size(edges(g)));

#if 0
  cout << endl << "Cities:" << endl;
  for (auto& city : germany_cities)
    cout << "  " << (&city - germany_cities.data()) << ". " << city << endl;

  cout << endl << "Routes:" << endl;
  for (auto& r : daa_germany_edge_routes)
    cout << "  " << germany_cities[r.first.first] << " --> " << germany_cities[r.first.second] << " " << r.second.weight << "km" << endl;
#endif

  // iterate thru vertices range
  size_t nVertices = 0;
  size_t nEdges    = 0;
  for (vertex_iterator_t<Graph> u = begin(g); u != end(g); ++u) {
    ++nVertices;

    size_t n1 = 0;
    for (auto& uv : edges(g, u))
      ++n1;

    size_t n2 = 0;
    for (auto& uv : edges(g, u))
      ++n2;
    EXPECT_EQ(n1, n2); // same as before?
    nEdges += n1;
  }
  EXPECT_EQ(germany_cities().size(), nVertices);
  EXPECT_EQ(ual_germany_edge_routes.size() * 2, nEdges);

  // iterate thru edges range
  size_t n = 0;
  for (auto& uv : edges(g))
    ++n;
  EXPECT_EQ(ual_germany_edge_routes.size() * 2, n);

#if TEST_OPTION == TEST_OPTION_OUTPUT
  cout << "\nGermany Routes"
       << "\n-------------------------------" << g << endl;

  /* Output
    Germany Routes
    -------------------------------
    [0] Augsburg
      <--> [6 München] 84km
      <--> [3 Karlsruhe] 250km
    [1] Erfurt
      <--> [9 Würzburg] 186km
    [2] Frankfürt
      <--> [5 Mannheim] 85km
      <--> [9 Würzburg] 217km
      <--> [4 Kassel] 173km
    [3] Karlsruhe
      <--> [0 Augsburg] 250km
      <--> [5 Mannheim] 80km
    [4] Kassel
      <--> [2 Frankfürt] 173km
      <--> [6 München] 502km
    [5] Mannheim
      <--> [2 Frankfürt] 85km
      <--> [3 Karlsruhe] 80km
    [6] München
      <--> [0 Augsburg] 84km
      <--> [4 Kassel] 502km
      <--> [7 Nürnberg] 167km
    [7] Nürnberg
      <--> [8 Stuttgart] 183km
      <--> [6 München] 167km
      <--> [9 Würzburg] 103km
    [8] Stuttgart
      <--> [7 Nürnberg] 183km
    [9] Würzburg
      <--> [2 Frankfürt] 217km
      <--> [1 Erfurt] 186km
      <--> [7 Nürnberg] 103km
  */

#elif TEST_OPTION == TEST_OPTION_GEN
  vertex_iterator_t<Graph>      u;
  vertex_edge_iterator_t<Graph> uv;

  cout << "vertex_iterator_t<Graph>      u; \n";
  cout << "vertex_edge_iterator_t<Graph> uv;\n";

  for (size_t ui = 0; ui < size(g); ++ui) {
    cout << "\n";
    u = begin(g) + ui;
    cout << "u = begin(g) + " << ui << ";\n";
    cout << "EXPECT_EQ(\"" << u->name << "\", u->name);\n";
    cout << "EXPECT_EQ(" << size(edges(g, u)) << ", size(edges(g, u)));\n";
    cout << "uv = begin(g, *u);\n";
    size_t uvi = 0;
    for (uv = begin(g, *u); uv != end(g, *u); ++uv, ++uvi) {
      if (uvi > 0) {
        cout << "++uv;\n";
      }
      cout << "EXPECT_EQ(" << outward_vertex_key(g, uv) << ", outward_vertex_key(g, uv));\n";
      cout << "EXPECT_EQ(\"" << outward_vertex(g, uv)->name << "\", outward_vertex(g, *uv)->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  vertex_iterator_t<Graph>      u;
  vertex_edge_iterator_t<Graph> uv;

  u = begin(g) + 0;
  EXPECT_EQ("Augsburg", u->name);
  EXPECT_EQ(2, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(84, uv->weight);
  ++uv;
  EXPECT_EQ(0, outward_vertex_key(g, uv));
  EXPECT_EQ("Augsburg", outward_vertex(g, uv)->name);
  EXPECT_EQ(250, uv->weight);

  u = begin(g) + 1;
  EXPECT_EQ("Erfurt", u->name);
  EXPECT_EQ(1, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(1, outward_vertex_key(g, uv));
  EXPECT_EQ("Erfurt", outward_vertex(g, uv)->name);
  EXPECT_EQ(186, uv->weight);

  u = begin(g) + 2;
  EXPECT_EQ("Frankfürt", u->name);
  EXPECT_EQ(3, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(5, outward_vertex_key(g, uv));
  EXPECT_EQ("Mannheim", outward_vertex(g, uv)->name);
  EXPECT_EQ(85, uv->weight);
  ++uv;
  EXPECT_EQ(9, outward_vertex_key(g, uv));
  EXPECT_EQ("Würzburg", outward_vertex(g, uv)->name);
  EXPECT_EQ(217, uv->weight);
  ++uv;
  EXPECT_EQ(4, outward_vertex_key(g, uv));
  EXPECT_EQ("Kassel", outward_vertex(g, uv)->name);
  EXPECT_EQ(173, uv->weight);

  u = begin(g) + 3;
  EXPECT_EQ("Karlsruhe", u->name);
  EXPECT_EQ(2, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(0, outward_vertex_key(g, uv));
  EXPECT_EQ("Augsburg", outward_vertex(g, uv)->name);
  EXPECT_EQ(250, uv->weight);
  ++uv;
  EXPECT_EQ(3, outward_vertex_key(g, uv));
  EXPECT_EQ("Karlsruhe", outward_vertex(g, uv)->name);
  EXPECT_EQ(80, uv->weight);

  u = begin(g) + 4;
  EXPECT_EQ("Kassel", u->name);
  EXPECT_EQ(2, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(4, outward_vertex_key(g, uv));
  EXPECT_EQ("Kassel", outward_vertex(g, uv)->name);
  EXPECT_EQ(173, uv->weight);
  ++uv;
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(502, uv->weight);

  u = begin(g) + 5;
  EXPECT_EQ("Mannheim", u->name);
  EXPECT_EQ(2, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(5, outward_vertex_key(g, uv));
  EXPECT_EQ("Mannheim", outward_vertex(g, uv)->name);
  EXPECT_EQ(85, uv->weight);
  ++uv;
  EXPECT_EQ(3, outward_vertex_key(g, uv));
  EXPECT_EQ("Karlsruhe", outward_vertex(g, uv)->name);
  EXPECT_EQ(80, uv->weight);

  u = begin(g) + 6;
  EXPECT_EQ("München", u->name);
  EXPECT_EQ(3, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(84, uv->weight);
  ++uv;
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(502, uv->weight);
  ++uv;
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(167, uv->weight);

  u = begin(g) + 7;
  EXPECT_EQ("Nürnberg", u->name);
  EXPECT_EQ(3, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(8, outward_vertex_key(g, uv));
  EXPECT_EQ("Stuttgart", outward_vertex(g, uv)->name);
  EXPECT_EQ(183, uv->weight);
  ++uv;
  EXPECT_EQ(6, outward_vertex_key(g, uv));
  EXPECT_EQ("München", outward_vertex(g, uv)->name);
  EXPECT_EQ(167, uv->weight);
  ++uv;
  EXPECT_EQ(7, outward_vertex_key(g, uv));
  EXPECT_EQ("Nürnberg", outward_vertex(g, uv)->name);
  EXPECT_EQ(103, uv->weight);

  u = begin(g) + 8;
  EXPECT_EQ("Stuttgart", u->name);
  EXPECT_EQ(1, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(8, outward_vertex_key(g, uv));
  EXPECT_EQ("Stuttgart", outward_vertex(g, uv)->name);
  EXPECT_EQ(183, uv->weight);

  u = begin(g) + 9;
  EXPECT_EQ("Würzburg", u->name);
  EXPECT_EQ(3, size(edges(g, u)));
  uv = begin(edges(g, u));
  EXPECT_EQ(9, outward_vertex_key(g, uv));
  EXPECT_EQ("Würzburg", outward_vertex(g, uv)->name);
  EXPECT_EQ(217, uv->weight);
  ++uv;
  EXPECT_EQ(1, outward_vertex_key(g, uv));
  EXPECT_EQ("Erfurt", outward_vertex(g, uv)->name);
  EXPECT_EQ(186, uv->weight);
  ++uv;
  EXPECT_EQ(7, outward_vertex_key(g, uv));
  EXPECT_EQ("Nürnberg", outward_vertex(g, uv)->name);
  EXPECT_EQ(103, uv->weight);
#endif
}


TEST_CASE("ual graph functions", "[ual][graph][functions]") {
  Graph        g  = create_germany_routes_graph();
  const Graph& gc = create_germany_routes_graph();

  //EXPECT_EQ(true, (is_same<empty_value, decltype(value(g))>::value));
  std::vertex_range_t<Graph>       vr  = std::vertices(g);
  std::const_vertex_range_t<Graph> vrc = std::vertices(gc);
  EXPECT_EQ(vr.size(), vrc.size());
  EXPECT_EQ(vr.size(), std::ranges::size(gc));

  size_t cnt = 0;
  for (std::vertex_iterator_t<Graph> u = begin(std::vertices(g)); u != end(std::vertices(g)); ++u, ++cnt)
    ;
  EXPECT_EQ(std::ranges::size(gc), cnt);

  cnt = 0;
  for (std::const_vertex_iterator_t<Graph> u = begin(std::vertices(gc)); u != end(std::vertices(gc)); ++u, ++cnt)
    ;
  EXPECT_EQ(std::ranges::size(gc), cnt);

  cnt = 0;
  for (std::const_vertex_iterator_t<Graph> u = cbegin(std::vertices(gc)); u != cend(std::vertices(gc)); ++u, ++cnt)
    ;
  EXPECT_EQ(std::ranges::size(gc), cnt);

  //std::reserve_vertices(g, 100); //undefined for semi-mutable graph
  //std::resisze_vertices(g, 100); //undefined for semi-mutable graph

  std::edge_range_t<Graph>       er       = std::edges(g);
  std::const_edge_range_t<Graph> erc      = std::edges(gc);
  std::edge_size_t<Graph>        edg_size = size(std::edges(gc));
  //EXPECT_EQ(size(std::edges(gc)), er.size());  // forward-only range; size n/a
  //EXPECT_EQ(size(std::edges(gc)), erc.size()); // forward-only range; size n/a
  // std::reserve_edges(g,100); // undefined for semi-mutable graph
  // std::clear(g);             // undefined for semi-mutable graph

#if TEST_OPTION == TEST_OPTION_OUTPUT
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST_CASE("ual vertex functions", "[ual][vertex][functions]") {
  Graph        g  = create_germany_routes_graph();
  const Graph& gc = g;

  std::vertex_iterator_t<Graph>       ui  = begin(std::vertices(g));
  std::const_vertex_iterator_t<Graph> uic = cbegin(std::vertices(g));
  std::vertex_t<Graph>&               u   = *ui;
  const std::vertex_t<Graph>&         uc  = *uic;

  std::vertex_key_t<Graph> vkey  = std::vertex_key(g, ui);
  std::vertex_key_t<Graph> vkeyc = std::vertex_key(g, uic);
  auto                     val   = std::vertex_value(g, ui);

  std::vertex_iterator_t<Graph>       f1 = std::find_vertex(g, 1);
  std::const_vertex_iterator_t<Graph> f2 = std::find_vertex(gc, 1);
  EXPECT_EQ(f1, f2);

  vertex_iterator_t<Graph> f3 = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });
  EXPECT_NE(f3, g.vertices().end());
  EXPECT_EQ(2, vertex_key(g, f3));

  {
    std::vertex_edge_range_t<Graph>          uvr      = std::edges(g, ui);
    std::const_vertex_edge_range_t<Graph>    uvrc     = std::edges(g, uic);
    std::vertex_edge_iterator_t<Graph>       uvi_beg1 = begin(std::edges(g, ui));
    std::const_vertex_edge_iterator_t<Graph> uvi_beg2 = begin(std::edges(g, uic));
    std::const_vertex_edge_iterator_t<Graph> uvi_beg3 = cbegin(std::edges(g, ui));
    std::vertex_edge_iterator_t<Graph>       uvi_end1 = end(std::edges(g, ui));
    std::const_vertex_edge_iterator_t<Graph> uvi_end2 = end(std::edges(g, uic));
    std::const_vertex_edge_iterator_t<Graph> uvi_end3 = end(std::edges(g, ui));
    //EXPECT_EQ(std::size(edges(g, u)), uvr.size()); // forward-only range; size n/a
  }
}

TEST_CASE("ual edge functions", "[ual][edge][functions]") {
  Graph        g  = create_germany_routes_graph();
  const Graph& gc = g;

  vertex_iterator_t<Graph> u = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });
  vertex_iterator_t<Graph> v = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Mannheim"; });
  EXPECT_NE(end(g), u);
  EXPECT_NE(end(g), v);

  edge_iterator_t<Graph> uv = find_edge(g, u, v); // find edge Frankfurt --> Mannheim
  EXPECT_NE(std::ranges::end(edges(g)), uv);
  EXPECT_EQ(v, vertex(g, uv, u));
  EXPECT_EQ(v, outward_vertex(g, uv));
  EXPECT_EQ(u, inward_vertex(g, uv));
  edge_iterator_t<Graph> uv2 = find_edge(g, vertex_key(g, u), vertex_key(g, v));
  EXPECT_EQ(uv, uv2);

  edge_iterator_t<Graph> uv3;
  uv2 = find_edge(g, u, v);
  uv3 = find_edge(g, vertex_key(g, u), vertex_key(g, v));
  EXPECT_EQ(uv, uv2);
  EXPECT_EQ(uv, uv3);
}

TEST_CASE("ual vertex-vertex range", "[ual][vertex_vertex][range]") {
  Graph        g  = create_germany_routes_graph();
  const Graph& gc = g;

  vertex_iterator_t<Graph> u    = std::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });
  vertex_key_t<Graph>      ukey = vertex_key(g, u);
  REQUIRE(size(edges(g, u)) == size(vertices(g, u)));
  REQUIRE(ssize(edges(g, u)) == ssize(vertices(g, u)));

  SECTION("const_iterator") {
    size_t                                cnt   = 0;
    const_vertex_edge_iterator_t<Graph>   uv_it = cbegin(edges(g, u));
    const_vertex_vertex_iterator_t<Graph> u_it  = begin(vertices(g, u));

    for (vertex_iterator_t<Graph> other = u; uv_it != cend(edges(g, u)) && u_it != end(vertices(g, u));
         ++uv_it, ++u_it, ++cnt) {
      REQUIRE(outward_vertex_key(g, uv_it) == vertex_key(g, u_it));
    }

    REQUIRE(size(edges(g, u)) == cnt);    // all edges/vertices visited?
    REQUIRE(uv_it == cend(edges(g, u)));  // reached the end?
    REQUIRE(u_it == end(vertices(g, u))); // reached the end?

    REQUIRE(size(edges(g, u)) >= 2);
    u_it = begin(vertices(g, u));
    ++u_it;
    --u_it;
    REQUIRE(u_it == begin(vertices(g, u)));
  }
  SECTION("iterator") {
    size_t                          cnt   = 0;
    vertex_edge_iterator_t<Graph>   uv_it = begin(edges(g, u));
    vertex_vertex_iterator_t<Graph> u_it  = begin(vertices(g, u));

    for (vertex_iterator_t<Graph> other = u; uv_it != end(edges(g, u)) && u_it != end(vertices(g, u));
         ++uv_it, ++u_it, ++cnt) {
      REQUIRE(outward_vertex_key(g, uv_it) == vertex_key(g, u_it));
    }

    REQUIRE(size(edges(g, u)) == cnt);    // all edges/vertices visited?
    REQUIRE(uv_it == end(edges(g, u)));   // reached the end?
    REQUIRE(u_it == end(vertices(g, u))); // reached the end?

    REQUIRE(size(edges(g, u)) >= 2);
    u_it = begin(vertices(g, u));
    ++u_it;
    --u_it;
    REQUIRE(u_it == begin(vertices(g, u)));
  }
}

TEST_CASE("ual dfs vertex", "[ual][dfs][vertex]") {
  Graph g = create_germany_routes_graph();

#if 0
#elif TEST_OPTION == TEST_OPTION_OUTPUT
  depth_first_search_vertex_range dfs_vtx_rng(g, find_city(g, "Frankfürt"));
  for (depth_first_search_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << u->name << endl;

  /* Output: seed = Frankfürt
    Frankfürt
      Mannheim
        Karlsruhe
          Augsburg
            München
              Kassel
              Nürnberg
                Stuttgart
                Würzburg
                  Erfurt
  */

  // a flat list when using range syntax (depth n/a on vertex)
  cout << endl;
  for (auto& u : depth_first_search_vertex_range(g, begin(g) + 2)) // Frankfürt
    cout << u.name << endl;
    /* Output: seed = Frankfürt
    Frankfürt
    Mannheim
    Karlsruhe
    Augsburg
    München
    Kassel
    Nürnberg
    Stuttgart
    Würzburg
    Erfurt
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  depth_first_search_vertex_range                  dfs_vtx_rng(g, find_city(g, "Frankfürt"));
  depth_first_search_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin();
  EXPECT_EQ("Frankfürt", u->name);
  EXPECT_EQ(1, u.depth());
  EXPECT_EQ("Mannheim", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Karlsruhe", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Augsburg", (++u)->name);
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ("München", (++u)->name);
  EXPECT_EQ(5, u.depth());
  EXPECT_EQ("Kassel", (++u)->name);
  EXPECT_EQ(6, u.depth());
  EXPECT_EQ("Nürnberg", (++u)->name);
  EXPECT_EQ(6, u.depth());
  EXPECT_EQ("Stuttgart", (++u)->name);
  EXPECT_EQ(7, u.depth());
  EXPECT_EQ("Würzburg", (++u)->name);
  EXPECT_EQ(7, u.depth());
  EXPECT_EQ("Erfurt", (++u)->name);
  EXPECT_EQ(8, u.depth());
#endif
}

TEST_CASE("ual dfs edge", "[ual][dfs][edge]") {
  Graph                         g = create_germany_routes_graph();
  depth_first_search_edge_range dfs_edge_rng(g, find_city(g, "Frankfürt"));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  int ln = 0;
  for (auto uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end(); ++uv, ++ln) {
    vertex_iterator_t<Graph> u     = uv.inward_vertex();
    vertex_key_t<Graph>      u_key = vertex_key(g, *u);
    if (uv.is_path_end()) {
      cout << string(uv.depth() * 2, ' ') << "view " << u->name << endl;
    } else {
      vtx_iter_t          v     = uv.back_vertex();
      vertex_key_t<Graph> v_key = vertex_key(g, uv, u);
      cout << string(uv.depth() * 2, ' ') << "travel " << u->name << " --> " << v->name << " " << uv->weight << "km"
           << (uv.is_back_edge() ? " [back edge]" : "") << endl;
    }
  }

  /* Output: seed = Frankfürt
  travel Frankfürt --> Mannheim 85km
    travel Mannheim --> Karlsruhe 80km
      travel Karlsruhe --> Augsburg 250km
        travel Augsburg --> München 84km
          travel München --> Kassel 502km
            travel Kassel --> Frankfürt 173km [back edge]
          travel München --> Nürnberg 167km
            travel Nürnberg --> Stuttgart 183km
              view Stuttgart
            travel Nürnberg --> Würzburg 103km
              travel Würzburg --> Frankfürt 217km [back edge]
              travel Würzburg --> Erfurt 186km
                view Erfurt
  travel Frankfürt --> Würzburg 217km [back edge]
  travel Frankfürt --> Kassel 173km [back edge]
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  cout << "depth_first_search_edge_range<Graph>::iterator uv = dfs_edge_rng.begin();\n"
       << "\n";
  size_t uvi = 0;
  for (depth_first_search_edge_range<Graph>::iterator uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end();
       ++uv, ++uvi) {
    if (uvi > 0)
      cout << "\n"
           << "++uv;\n";

    if (uv.is_back_edge()) {
      cout << "EXPECT_TRUE(uv.is_back_edge());\n";
      if (uv.is_path_end()) {
        cout << "EXPECT_TRUE(uv.is_path_end());\n";
      } else {
        cout << "EXPECT_FALSE(uv.is_path_end());\n";
        cout << "EXPECT_EQ(\"" << uv.back_vertex()->name << "\", uv.back_vertex()->name);\n";
      }
      cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
    } else {
      cout << "EXPECT_FALSE(uv.is_back_edge());\n";
      cout << "EXPECT_EQ(\"" << inward_vertex(g, uv.operator->())->name
           << "\", inward_vertex(g, uv.operator->())->name);\n";
      cout << "EXPECT_EQ(\"" << outward_vertex(g, uv.operator->())->name
           << "\", outward_vertex(g, uv.operator->())->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
      cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  depth_first_search_edge_range<Graph>::iterator uv = dfs_edge_rng.begin();

  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Frankfürt", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Mannheim", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(85, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Mannheim", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Karlsruhe", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(80, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Karlsruhe", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Augsburg", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(250, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Augsburg", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("München", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(84, uv->weight);
  EXPECT_EQ(4, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Kassel", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("München", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(502, uv->weight);
  EXPECT_EQ(5, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_EQ("Frankfürt", uv.back_vertex()->name);
  EXPECT_EQ(6, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("München", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(167, uv->weight);
  EXPECT_EQ(5, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Stuttgart", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(183, uv->weight);
  EXPECT_EQ(6, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ(7, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Nürnberg", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(103, uv->weight);
  EXPECT_EQ(6, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_EQ("Frankfürt", uv.back_vertex()->name);
  EXPECT_EQ(7, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", inward_vertex(g, uv.operator->())->name);
  EXPECT_EQ("Erfurt", outward_vertex(g, uv.operator->())->name);
  EXPECT_EQ(186, uv->weight);
  EXPECT_EQ(7, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ(8, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_EQ("Würzburg", uv.back_vertex()->name);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_EQ("Kassel", uv.back_vertex()->name);
  EXPECT_EQ(1, uv.depth());
#endif
}

TEST_CASE("ual bfs vertex", "[ual][bfs][vertex]") {
  Graph                             g = create_germany_routes_graph();
  breadth_first_search_vertex_range bfs_vtx_rng(g, find_city(g, "Frankfürt"));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (auto u = bfs_vtx_rng.begin(); u != bfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << u->name << endl;

    /* Output: seed = Frankfürt
      Frankfürt
        Mannheim
        Würzburg
        Kassel
          Karlsruhe
          Erfurt
          Nürnberg
          München
            Augsburg
            Stuttgart
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  breadth_first_search_vertex_range<Graph>::iterator u = bfs_vtx_rng.begin();
  EXPECT_EQ("Frankfürt", u->name);
  EXPECT_EQ(1, u.depth());
  EXPECT_EQ("Mannheim", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Würzburg", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Kassel", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Karlsruhe", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Erfurt", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Nürnberg", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("München", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Augsburg", (++u)->name);
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ("Stuttgart", (++u)->name);
  EXPECT_EQ(4, u.depth());
#endif
}

TEST_CASE("ual bfs edge", "[ual][bfs][edge]") {
  Graph                           g = create_germany_routes_graph();
  breadth_first_search_edge_range bfs_edge_rng(g, find_city(g, "Frankfürt"));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (auto uv = bfs_edge_rng.begin(); uv != bfs_edge_rng.end(); ++uv) {
    vertex_iterator_t<Graph> v     = uv.back_vertex();
    vertex_key_t<Graph>      v_key = vertex_key(g, v);
    if (uv.is_path_end()) {
      cout << string(uv.depth() * 2, ' ') << "view " << v->name << endl;
    } else {
      vtx_iter_t          u         = vertex(g, *uv, *v);
      vertex_key_t<Graph> u_key     = vertex_key(g, u);
      bool                back_edge = uv.is_back_edge();
      cout << string(uv.depth() * 2, ' ') << "travel " << u->name << " --> " << v->name << " " << uv->weight << "km"
           << (back_edge ? " [back edge]" : "") << endl;
    }
  }

  /* Output: seed = Frankfürt
      travel Frankfürt --> Mannheim 85km
      travel Frankfürt --> Würzburg 217km
      travel Frankfürt --> Kassel 173km
        travel Mannheim --> Karlsruhe 80km
        travel Würzburg --> Erfurt 186km
        travel Würzburg --> Nürnberg 103km
        travel Kassel --> München 502km
          travel Karlsruhe --> Augsburg 250km
          view Erfurt
          travel Nürnberg --> Stuttgart 183km
          travel Nürnberg --> München 167km [back edge]
          travel München --> Augsburg 84km [back edge]
          travel München --> Nürnberg 167km [back edge]
            travel Augsburg --> München 84km [back edge]
            view Stuttgart
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  cout << "breadth_first_search_edge_range<Graph>::iterator uv = bfs_edge_rng.begin();\n"
       << "\n";
  size_t uvi = 0;
  for (breadth_first_search_edge_range<Graph>::iterator uv = bfs_edge_rng.begin(); uv != bfs_edge_rng.end();
       ++uv, ++uvi) {
    if (uvi > 0)
      cout << "\n"
           << "++uv;\n";

    if (uv.is_path_end()) {
      cout << "EXPECT_TRUE(uv.is_path_end());\n";
      cout << "EXPECT_EQ(\"" << uv.back_vertex()->name << "\", uv.back_vertex()->name);\n";
    } else {
      cout << "EXPECT_FALSE(uv.is_path_end());\n";
      if (uv.is_back_edge())
        cout << "EXPECT_TRUE(uv.is_back_edge());\n";
      else
        cout << "EXPECT_FALSE(uv.is_back_edge());\n";
      cout << "EXPECT_EQ(\"" << uv.back_vertex()->name << "\", uv.back_vertex()->name);\n";
      cout << "EXPECT_EQ(\"" << vertex(g, uv.operator->(), uv.back_vertex())->name
           << "\", vertex(g, uv.operator->(), uv.back_vertex())->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
    }
    cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  breadth_first_search_edge_range<Graph>::iterator uv = bfs_edge_rng.begin();

  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Mannheim", uv.back_vertex()->name);
  EXPECT_EQ("Frankfürt", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(85, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", uv.back_vertex()->name);
  EXPECT_EQ("Frankfürt", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(217, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Kassel", uv.back_vertex()->name);
  EXPECT_EQ("Frankfürt", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(173, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Karlsruhe", uv.back_vertex()->name);
  EXPECT_EQ("Mannheim", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(80, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Erfurt", uv.back_vertex()->name);
  EXPECT_EQ("Würzburg", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(186, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", uv.back_vertex()->name);
  EXPECT_EQ("Würzburg", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(103, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("Kassel", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(502, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Augsburg", uv.back_vertex()->name);
  EXPECT_EQ("Karlsruhe", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(250, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Erfurt", uv.back_vertex()->name);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Stuttgart", uv.back_vertex()->name);
  EXPECT_EQ("Nürnberg", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(183, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("Nürnberg", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(167, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("Augsburg", uv.back_vertex()->name);
  EXPECT_EQ("München", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(84, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", uv.back_vertex()->name);
  EXPECT_EQ("München", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(167, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("Augsburg", vertex(g, uv.operator->(), uv.back_vertex())->name);
  EXPECT_EQ(84, uv->weight);
  EXPECT_EQ(4, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Stuttgart", uv.back_vertex()->name);
  EXPECT_EQ(4, uv.depth());
#endif
}

TEST_CASE("ual dikjstra distance", "[ual][dikjstra][distance]") {
  using std::dijkstra_shortest_distances;
  using std::dijkstra_shortest_paths;
  using std::shortest_distance;
  using std::shortest_path;

  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

#if TEST_OPTION == TEST_OPTION_OUTPUT
  dijkstra_shortest_distances(g, u, back_inserter(short_dists), false, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfurt
    Frankfürt --> Augsburg  415km
    Frankfürt --> Erfurt  403km
    Frankfürt --> Frankfürt  0km
    Frankfürt --> Karlsruhe  165km
    Frankfürt --> Kassel  173km
    Frankfürt --> Mannheim  85km
    Frankfürt --> München  487km
    Frankfürt --> Nürnberg  320km
    Frankfürt --> Stuttgart  503km
    Frankfürt --> Würzburg  217km
  */

  cout << "\n";
  short_dists.clear();
  dijkstra_shortest_distances(g, u, back_inserter(short_dists), true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
    /* Output: source = Frankfurt
    Frankfürt --> Erfurt  403km
    Frankfürt --> München  487km
    Frankfürt --> Stuttgart  503km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  {
    dijkstra_shortest_distances(g, u, back_inserter(short_dists), false, weight_fnc);
    EXPECT_EQ("Frankfürt", short_dists[0].first->name);
    EXPECT_EQ("Augsburg", short_dists[0].last->name);
    EXPECT_EQ(415, short_dists[0].distance);

    EXPECT_EQ("Frankfürt", short_dists[1].first->name);
    EXPECT_EQ("Erfurt", short_dists[1].last->name);
    EXPECT_EQ(403, short_dists[1].distance);

    EXPECT_EQ("Frankfürt", short_dists[2].first->name);
    EXPECT_EQ("Frankfürt", short_dists[2].last->name);
    EXPECT_EQ(0, short_dists[2].distance);

    EXPECT_EQ("Frankfürt", short_dists[3].first->name);
    EXPECT_EQ("Karlsruhe", short_dists[3].last->name);
    EXPECT_EQ(165, short_dists[3].distance);

    EXPECT_EQ("Frankfürt", short_dists[4].first->name);
    EXPECT_EQ("Kassel", short_dists[4].last->name);
    EXPECT_EQ(173, short_dists[4].distance);

    EXPECT_EQ("Frankfürt", short_dists[5].first->name);
    EXPECT_EQ("Mannheim", short_dists[5].last->name);
    EXPECT_EQ(85, short_dists[5].distance);

    EXPECT_EQ("Frankfürt", short_dists[6].first->name);
    EXPECT_EQ("München", short_dists[6].last->name);
    EXPECT_EQ(487, short_dists[6].distance);

    EXPECT_EQ("Frankfürt", short_dists[7].first->name);
    EXPECT_EQ("Nürnberg", short_dists[7].last->name);
    EXPECT_EQ(320, short_dists[7].distance);

    EXPECT_EQ("Frankfürt", short_dists[8].first->name);
    EXPECT_EQ("Stuttgart", short_dists[8].last->name);
    EXPECT_EQ(503, short_dists[8].distance);

    EXPECT_EQ("Frankfürt", short_dists[9].first->name);
    EXPECT_EQ("Würzburg", short_dists[9].last->name);
    EXPECT_EQ(217, short_dists[9].distance);
  }
  {
    short_dists.clear();
    dijkstra_shortest_distances(g, u, back_inserter(short_dists), true, weight_fnc);

    EXPECT_EQ("Frankfürt", short_dists[0].first->name);
    EXPECT_EQ("Erfurt", short_dists[0].last->name);
    EXPECT_EQ(403, short_dists[0].distance);

    EXPECT_EQ("Frankfürt", short_dists[1].first->name);
    EXPECT_EQ("München", short_dists[1].last->name);
    EXPECT_EQ(487, short_dists[1].distance);

    EXPECT_EQ("Frankfürt", short_dists[2].first->name);
    EXPECT_EQ("Stuttgart", short_dists[2].last->name);
    EXPECT_EQ(503, short_dists[2].distance);
  }
#endif
}

TEST_CASE("ual bellman-ford distance", "[ual][bellman-ford][distance]") {
  using std::bellman_ford_shortest_distances;
  using std::shortest_distance;

  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

#if TEST_OPTION == TEST_OPTION_OUTPUT
  bellman_ford_shortest_distances(g, u, back_inserter(short_dists), false, true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfürt
    Frankfürt --> Augsburg  415km
    Frankfürt --> Erfurt  403km
    Frankfürt --> Frankfürt  0km
    Frankfürt --> Karlsruhe  165km
    Frankfürt --> Kassel  173km
    Frankfürt --> Mannheim  85km
    Frankfürt --> München  487km
    Frankfürt --> Nürnberg  320km
    Frankfürt --> Stuttgart  503km
    Frankfürt --> Würzburg  217km
  */

  cout << "\n";
  short_dists.clear();
  bellman_ford_shortest_distances(g, u, back_inserter(short_dists), true, true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
    /* Output: source = Frankfürt
    Frankfürt --> Erfurt  403km
    Frankfürt --> München  487km
    Frankfürt --> Stuttgart  503km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  {
    bellman_ford_shortest_distances(g, u, back_inserter(short_dists), false, true, weight_fnc);

    EXPECT_EQ("Frankfürt", short_dists[0].first->name);
    EXPECT_EQ("Augsburg", short_dists[0].last->name);
    EXPECT_EQ(415, short_dists[0].distance);

    EXPECT_EQ("Frankfürt", short_dists[1].first->name);
    EXPECT_EQ("Erfurt", short_dists[1].last->name);
    EXPECT_EQ(403, short_dists[1].distance);

    EXPECT_EQ("Frankfürt", short_dists[2].first->name);
    EXPECT_EQ("Frankfürt", short_dists[2].last->name);
    EXPECT_EQ(0, short_dists[2].distance);

    EXPECT_EQ("Frankfürt", short_dists[3].first->name);
    EXPECT_EQ("Karlsruhe", short_dists[3].last->name);
    EXPECT_EQ(165, short_dists[3].distance);

    EXPECT_EQ("Frankfürt", short_dists[4].first->name);
    EXPECT_EQ("Kassel", short_dists[4].last->name);
    EXPECT_EQ(173, short_dists[4].distance);

    EXPECT_EQ("Frankfürt", short_dists[5].first->name);
    EXPECT_EQ("Mannheim", short_dists[5].last->name);
    EXPECT_EQ(85, short_dists[5].distance);

    EXPECT_EQ("Frankfürt", short_dists[6].first->name);
    EXPECT_EQ("München", short_dists[6].last->name);
    EXPECT_EQ(487, short_dists[6].distance);

    EXPECT_EQ("Frankfürt", short_dists[7].first->name);
    EXPECT_EQ("Nürnberg", short_dists[7].last->name);
    EXPECT_EQ(320, short_dists[7].distance);

    EXPECT_EQ("Frankfürt", short_dists[8].first->name);
    EXPECT_EQ("Stuttgart", short_dists[8].last->name);
    EXPECT_EQ(503, short_dists[8].distance);

    EXPECT_EQ("Frankfürt", short_dists[9].first->name);
    EXPECT_EQ("Würzburg", short_dists[9].last->name);
    EXPECT_EQ(217, short_dists[9].distance);
  }
  {
    short_dists.clear();
    bellman_ford_shortest_distances(g, u, back_inserter(short_dists), true, true, weight_fnc);

    EXPECT_EQ("Frankfürt", short_dists[0].first->name);
    EXPECT_EQ("Erfurt", short_dists[0].last->name);
    EXPECT_EQ(403, short_dists[0].distance);

    EXPECT_EQ("Frankfürt", short_dists[1].first->name);
    EXPECT_EQ("München", short_dists[1].last->name);
    EXPECT_EQ(487, short_dists[1].distance);

    EXPECT_EQ("Frankfürt", short_dists[2].first->name);
    EXPECT_EQ("Stuttgart", short_dists[2].last->name);
    EXPECT_EQ(503, short_dists[2].distance);
  }
#endif
}


TEST_CASE("ual dikjstra path", "[ual][dikjstra][path]") {
  using std::dijkstra_shortest_paths;
  using std::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

#if TEST_OPTION == TEST_OPTION_OUTPUT
  dijkstra_shortest_paths(g, u, back_inserter(short_paths), false, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfürt
    Frankfürt --> Mannheim --> Karlsruhe --> Augsburg  415km
    Frankfürt --> Würzburg --> Erfurt  403km
    Frankfürt  0km
    Frankfürt --> Mannheim --> Karlsruhe  165km
    Frankfürt --> Kassel  173km
    Frankfürt --> Mannheim  85km
    Frankfürt --> Würzburg --> Nürnberg --> München  487km
    Frankfürt --> Würzburg --> Nürnberg  320km
    Frankfürt --> Würzburg --> Nürnberg --> Stuttgart  503km
    Frankfürt --> Würzburg  217km
  */

  cout << "\n";
  short_paths.clear();
  dijkstra_shortest_paths(g, u, back_inserter(short_paths), true, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfürt
    Frankfürt --> Würzburg --> Erfurt  403km
    Frankfürt --> Würzburg --> Nürnberg --> München  487km
    Frankfürt --> Würzburg --> Nürnberg --> Stuttgart  503km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  {
    cout << "{\n";
    dijkstra_shortest_paths(g, u, back_inserter(short_paths), false, weight_fnc);
    cout << "  dijkstra_shortest_paths(g, u, back_inserter(short_paths), false, "
            "weight_fnc);\n";
    for (size_t spi = 0; spi < short_paths.size(); ++spi) {
      cout << "\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].distance << ", short_paths[" << spi << "].distance);\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].path.size() << ", short_paths[" << spi << "].path.size());\n";
      for (size_t i = 0; i < short_paths[spi].path.size(); ++i) {
        cout << "  EXPECT_EQ(\"" << short_paths[spi].path[i]->name << "\", short_paths[" << spi << "].path[" << i
             << "]->name);\n";
      }
    }
    cout << "}\n";
  }

  {
    short_paths.clear();
    dijkstra_shortest_paths(g, u, back_inserter(short_paths), true, weight_fnc);
    cout << "{\n";
    cout << "  short_paths.clear();\n";
    cout << "  dijkstra_shortest_paths(g, u, back_inserter(short_paths), true, "
            "weight_fnc);\n";
    for (size_t spi = 0; spi < short_paths.size(); ++spi) {
      cout << "\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].distance << ", short_paths[" << spi << "].distance);\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].path.size() << ", short_paths[" << spi << "].path.size());\n";
      for (size_t i = 0; i < short_paths[spi].path.size(); ++i) {
        cout << "  EXPECT_EQ(\"" << short_paths[spi].path[i]->name << "\", short_paths[" << spi << "].path[" << i
             << "]->name);\n";
      }
    }
    cout << "}\n";
  }

#elif TEST_OPTION == TEST_OPTION_TEST
  {
    dijkstra_shortest_paths(g, u, back_inserter(short_paths), false, weight_fnc);

    EXPECT_EQ(415, short_paths[0].distance);
    EXPECT_EQ(4, short_paths[0].path.size());
    EXPECT_EQ("Frankfürt", short_paths[0].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[0].path[1]->name);
    EXPECT_EQ("Karlsruhe", short_paths[0].path[2]->name);
    EXPECT_EQ("Augsburg", short_paths[0].path[3]->name);

    EXPECT_EQ(403, short_paths[1].distance);
    EXPECT_EQ(3, short_paths[1].path.size());
    EXPECT_EQ("Frankfürt", short_paths[1].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[1].path[1]->name);
    EXPECT_EQ("Erfurt", short_paths[1].path[2]->name);

    EXPECT_EQ(0, short_paths[2].distance);
    EXPECT_EQ(1, short_paths[2].path.size());
    EXPECT_EQ("Frankfürt", short_paths[2].path[0]->name);

    EXPECT_EQ(165, short_paths[3].distance);
    EXPECT_EQ(3, short_paths[3].path.size());
    EXPECT_EQ("Frankfürt", short_paths[3].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[3].path[1]->name);
    EXPECT_EQ("Karlsruhe", short_paths[3].path[2]->name);

    EXPECT_EQ(173, short_paths[4].distance);
    EXPECT_EQ(2, short_paths[4].path.size());
    EXPECT_EQ("Frankfürt", short_paths[4].path[0]->name);
    EXPECT_EQ("Kassel", short_paths[4].path[1]->name);

    EXPECT_EQ(85, short_paths[5].distance);
    EXPECT_EQ(2, short_paths[5].path.size());
    EXPECT_EQ("Frankfürt", short_paths[5].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[5].path[1]->name);

    EXPECT_EQ(487, short_paths[6].distance);
    EXPECT_EQ(4, short_paths[6].path.size());
    EXPECT_EQ("Frankfürt", short_paths[6].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[6].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[6].path[2]->name);
    EXPECT_EQ("München", short_paths[6].path[3]->name);

    EXPECT_EQ(320, short_paths[7].distance);
    EXPECT_EQ(3, short_paths[7].path.size());
    EXPECT_EQ("Frankfürt", short_paths[7].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[7].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[7].path[2]->name);

    EXPECT_EQ(503, short_paths[8].distance);
    EXPECT_EQ(4, short_paths[8].path.size());
    EXPECT_EQ("Frankfürt", short_paths[8].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[8].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[8].path[2]->name);
    EXPECT_EQ("Stuttgart", short_paths[8].path[3]->name);

    EXPECT_EQ(217, short_paths[9].distance);
    EXPECT_EQ(2, short_paths[9].path.size());
    EXPECT_EQ("Frankfürt", short_paths[9].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[9].path[1]->name);
  }
  {
    short_paths.clear();
    dijkstra_shortest_paths(g, u, back_inserter(short_paths), true, weight_fnc);
    EXPECT_EQ(3, short_paths.size());

    EXPECT_EQ(403, short_paths[0].distance);
    EXPECT_EQ(3, short_paths[0].path.size());
    EXPECT_EQ("Frankfürt", short_paths[0].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[0].path[1]->name);
    EXPECT_EQ("Erfurt", short_paths[0].path[2]->name);

    EXPECT_EQ(487, short_paths[1].distance);
    EXPECT_EQ(4, short_paths[1].path.size());
    EXPECT_EQ("Frankfürt", short_paths[1].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[1].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[1].path[2]->name);
    EXPECT_EQ("München", short_paths[1].path[3]->name);

    EXPECT_EQ(503, short_paths[2].distance);
    EXPECT_EQ(4, short_paths[2].path.size());
    EXPECT_EQ("Frankfürt", short_paths[2].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[2].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[2].path[2]->name);
    EXPECT_EQ("Stuttgart", short_paths[2].path[3]->name);
  }
#endif
}

TEST_CASE("ual bellman-ford path", "[ual][bellman-ford][path]") {
  using std::bellman_ford_shortest_paths;
  using std::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = std::ranges::find_if(g, [](vertex_t<Graph>& uu) { return uu.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

#if TEST_OPTION == TEST_OPTION_OUTPUT
  bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), false, true, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfürt
    Frankfürt --> Mannheim --> Karlsruhe --> Augsburg  415km
    Frankfürt --> Würzburg --> Erfurt  403km
    Frankfürt  0km
    Frankfürt --> Mannheim --> Karlsruhe  165km
    Frankfürt --> Kassel  173km
    Frankfürt --> Mannheim  85km
    Frankfürt --> Würzburg --> Nürnberg --> München  487km
    Frankfürt --> Würzburg --> Nürnberg  320km
    Frankfürt --> Würzburg --> Nürnberg --> Stuttgart  503km
    Frankfürt --> Würzburg  217km
  */

  cout << "\n";
  short_paths.clear();
  neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), true, true, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfürt
    Frankfürt --> Würzburg --> Erfurt  403km
    Frankfürt --> Würzburg --> Nürnberg --> München  487km
    Frankfürt --> Würzburg --> Nürnberg --> Stuttgart  503km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  {
    cout << "{\n";
    bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), false, true, weight_fnc);
    cout << "  bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), false, "
            "true, weight_fnc);\n ";
    cout << "  EXPECT_FALSE(neg_edge_cycle_exists);\n";
    for (size_t spi = 0; spi < short_paths.size(); ++spi) {
      cout << "\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].distance << ", short_paths[" << spi << "].distance);\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].path.size() << ", short_paths[" << spi << "].path.size());\n";
      for (size_t i = 0; i < short_paths[spi].path.size(); ++i) {
        cout << "  EXPECT_EQ(\"" << short_paths[spi].path[i]->name << "\", short_paths[" << spi << "].path[" << i
             << "]->name);\n";
      }
    }
    cout << "}\n";
  }

  {
    short_paths.clear();
    bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), true, true, weight_fnc);
    cout << "{\n";
    cout << "  short_paths.clear();\n";
    cout << "  bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), true, "
            "true, weight_fnc);\n ";
    cout << "  EXPECT_FALSE(neg_edge_cycle_exists);\n";
    for (size_t spi = 0; spi < short_paths.size(); ++spi) {
      cout << "\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].distance << ", short_paths[" << spi << "].distance);\n";
      cout << "  EXPECT_EQ(" << short_paths[spi].path.size() << ", short_paths[" << spi << "].path.size());\n";
      for (size_t i = 0; i < short_paths[spi].path.size(); ++i) {
        cout << "  EXPECT_EQ(\"" << short_paths[spi].path[i]->name << "\", short_paths[" << spi << "].path[" << i
             << "]->name);\n";
      }
    }
    cout << "}\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  {
    bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), false, true, weight_fnc);
    EXPECT_FALSE(neg_edge_cycle_exists);

    EXPECT_EQ(415, short_paths[0].distance);
    EXPECT_EQ(4, short_paths[0].path.size());
    EXPECT_EQ("Frankfürt", short_paths[0].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[0].path[1]->name);
    EXPECT_EQ("Karlsruhe", short_paths[0].path[2]->name);
    EXPECT_EQ("Augsburg", short_paths[0].path[3]->name);

    EXPECT_EQ(403, short_paths[1].distance);
    EXPECT_EQ(3, short_paths[1].path.size());
    EXPECT_EQ("Frankfürt", short_paths[1].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[1].path[1]->name);
    EXPECT_EQ("Erfurt", short_paths[1].path[2]->name);

    EXPECT_EQ(0, short_paths[2].distance);
    EXPECT_EQ(1, short_paths[2].path.size());
    EXPECT_EQ("Frankfürt", short_paths[2].path[0]->name);

    EXPECT_EQ(165, short_paths[3].distance);
    EXPECT_EQ(3, short_paths[3].path.size());
    EXPECT_EQ("Frankfürt", short_paths[3].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[3].path[1]->name);
    EXPECT_EQ("Karlsruhe", short_paths[3].path[2]->name);

    EXPECT_EQ(173, short_paths[4].distance);
    EXPECT_EQ(2, short_paths[4].path.size());
    EXPECT_EQ("Frankfürt", short_paths[4].path[0]->name);
    EXPECT_EQ("Kassel", short_paths[4].path[1]->name);

    EXPECT_EQ(85, short_paths[5].distance);
    EXPECT_EQ(2, short_paths[5].path.size());
    EXPECT_EQ("Frankfürt", short_paths[5].path[0]->name);
    EXPECT_EQ("Mannheim", short_paths[5].path[1]->name);

    EXPECT_EQ(487, short_paths[6].distance);
    EXPECT_EQ(4, short_paths[6].path.size());
    EXPECT_EQ("Frankfürt", short_paths[6].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[6].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[6].path[2]->name);
    EXPECT_EQ("München", short_paths[6].path[3]->name);

    EXPECT_EQ(320, short_paths[7].distance);
    EXPECT_EQ(3, short_paths[7].path.size());
    EXPECT_EQ("Frankfürt", short_paths[7].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[7].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[7].path[2]->name);

    EXPECT_EQ(503, short_paths[8].distance);
    EXPECT_EQ(4, short_paths[8].path.size());
    EXPECT_EQ("Frankfürt", short_paths[8].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[8].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[8].path[2]->name);
    EXPECT_EQ("Stuttgart", short_paths[8].path[3]->name);

    EXPECT_EQ(217, short_paths[9].distance);
    EXPECT_EQ(2, short_paths[9].path.size());
    EXPECT_EQ("Frankfürt", short_paths[9].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[9].path[1]->name);
  }
  {
    short_paths.clear();
    bool neg_edge_cycle_exists = bellman_ford_shortest_paths(g, u, back_inserter(short_paths), true, true, weight_fnc);
    EXPECT_FALSE(neg_edge_cycle_exists);

    EXPECT_EQ(403, short_paths[0].distance);
    EXPECT_EQ(3, short_paths[0].path.size());
    EXPECT_EQ("Frankfürt", short_paths[0].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[0].path[1]->name);
    EXPECT_EQ("Erfurt", short_paths[0].path[2]->name);

    EXPECT_EQ(487, short_paths[1].distance);
    EXPECT_EQ(4, short_paths[1].path.size());
    EXPECT_EQ("Frankfürt", short_paths[1].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[1].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[1].path[2]->name);
    EXPECT_EQ("München", short_paths[1].path[3]->name);

    EXPECT_EQ(503, short_paths[2].distance);
    EXPECT_EQ(4, short_paths[2].path.size());
    EXPECT_EQ("Frankfürt", short_paths[2].path[0]->name);
    EXPECT_EQ("Würzburg", short_paths[2].path[1]->name);
    EXPECT_EQ("Nürnberg", short_paths[2].path[2]->name);
    EXPECT_EQ("Stuttgart", short_paths[2].path[3]->name);
  }
#endif
}
