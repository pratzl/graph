//#include "pch.h"
#include "graph/directed_adjacency_array.hpp"
#include "graph/range/dfs.hpp"
#include "graph/range/bfs.hpp"
#include "graph/algorithm/shortest_paths.hpp"
#include "graph/algorithm/transitive_closure.hpp"
#include "data_routes.hpp"
#include <iostream>
#include <catch2/catch.hpp>
#include <range/v3/action/sort.hpp>

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST


// Google Test compatibility
#define EXPECT_EQ(a, b) REQUIRE((a) == (b))
#define EXPECT_NE(a, b) REQUIRE((a) != (b))
#define EXPECT_FALSE(a) REQUIRE(!(a))
#define EXPECT_TRUE(a) REQUIRE(a);

using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;

using namespace std::graph; // Bring graph functions into global namespace
using Graph      = std::graph::directed_adjacency_array<name_value, weight_value>;
using vtx_iter_t = std::graph::vertex_iterator_t<Graph>;
using vtx_key_t  = std::graph::vertex_key_t<Graph>;

struct route;
using Routes = routes_t;

vector<Graph::edge_value_type> const& daa_germany_edge_routes = germany_routes_directed_graph.edge_values();

vertex_iterator_t<Graph> find_city(Graph& g, string_view const city_name) {
  return ::ranges::find_if(g, [&city_name](vertex_t<Graph>& u) { return u.name == city_name; });
}

static Graph                      create_germany_routes_graph() { return germany_routes_directed_graph.create_graph(); }
static vector<std::string> const& germany_cities() { return germany_routes_directed_graph.vertex_values(); }

template <class OStream>
OStream& operator<<(OStream& os, Graph const& g) {
  for (vertex_t<Graph> const& u : vertices(g)) {
    vertex_key_t<Graph> ukey = vertex_key(g, u);
    os << "\n[" << ukey << "] " << u.name;
    for (edge_t<Graph> const& uv : edges(g, u)) {
      const_vertex_iterator_t<Graph> v    = out_vertex(g, uv);
      vertex_key_t<Graph>            vkey = vertex_key(g, *v);
      os << "\n  --> [" << vkey << " " << v->name << "] " << uv.weight << "km";
    }
  }
  os << "\n";
  return os;
}


TEST_CASE("daa minsize", "[daa][minsize]") {
  using G = std::graph::directed_adjacency_array<>;
  EXPECT_EQ(4, sizeof(G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(8, sizeof(G::edge_type));   // edge size = 8 bytes
}

TEST_CASE("daa empty", "[daa][empty]") {
  Graph g;
  EXPECT_EQ(0, vertices_size(g));
  EXPECT_EQ(0, edges_size(g));
  //EXPECT_EQ(sizeof(Graph::edge_size_type) + sizeof(name_value), sizeof(Graph::vertex_type));
  //EXPECT_EQ(sizeof(Graph::vertex_size_type) + sizeof(weight_value), sizeof(Graph::edge_type));
}

TEST_CASE("daa initializer list", "[daa][init][initializer list]") {
  //using edge_key_val = std::tuple<vertex_key_t<Graph>, vertex_key_t<Graph>, edge_value_t<Graph>>;
  Graph g0{};                     // empty graph
  Graph g1{{1, 2, 3}};            // one edge
  Graph g2{{1, 2, 3}, {4, 5, 6}}; // two edges
}

TEST_CASE("daa example 1", "[daa][example][1]") {
  using std::graph::directed_adjacency_array;
  using ::ranges::sort;
  using ::ranges::find;
  using ::ranges::end;
  struct route {
    string from;
    string to;
    int    miles;
  };
  vector<string> cities = {"Apex", "Cary", "Raleigh"};
  vector<route>  routes = {{"Apex", "Cary", 5}, {"Apex", "Raleigh", 10}};
  sort(cities);

  using G          = directed_adjacency_array<name_value, weight_value, name_value>;
  using edge_key_t = typename G::edge_key_type;

  auto find_city    = [&cities](string const& city) { return find(cities, city) - begin(cities); };
  auto vertex_value = [](string const& name) { return name; };
  auto edge_key     = [&cities, &find_city](route const& r) { return edge_key_t(find_city(r.from), find_city(r.to)); };
  auto edge_value   = [&cities](route const& r) { return r.miles; };

  G g(routes, cities, edge_key, edge_value, vertex_value, name_value("NC Routes"));
  REQUIRE(vertices_size(g) == 3);
  REQUIRE(edges_size(g) == 2);
}

TEST_CASE("daa example 2", "[daa][example][2]") {
  using std::graph::directed_adjacency_array;
  using G = directed_adjacency_array<empty_value, weight_value>;

  G g{{0, 1, 5}, {0, 2, 10}};
  REQUIRE(vertices_size(g) == 3);
  REQUIRE(edges_size(g) == 2);
}

TEST_CASE("daa init", "[daa][init]") {
#if 0
  vector<Graph::edge_value_type> daa_germany_edge_routes = to_edge_values(routes, germany_cities);
  Graph                          g(germany_cities, daa_germany_edge_routes);
#endif
  Graph g = create_germany_routes_graph();
  EXPECT_EQ(germany_cities().size(), vertices_size(g));
  EXPECT_EQ(daa_germany_edge_routes.size(), edges_size(g));

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
  for (auto& u : vertices(g)) {
    ++nVertices;

    size_t n1 = 0;
    for (auto& uv : out_edges(g, u))
      ++n1;

    size_t n2 = 0;
    for (auto& uv : edges(g, u))
      ++n2;
    EXPECT_EQ(n1, n2); // same as before?
    nEdges += n1;
  }
  EXPECT_EQ(germany_cities().size(), nVertices);
  EXPECT_EQ(daa_germany_edge_routes.size(), nEdges);

  // iterate thru edges range
  size_t n = 0;
  for (auto& uv : edges(g))
    ++n;
  EXPECT_EQ(daa_germany_edge_routes.size(), n);

#if TEST_OPTION == TEST_OPTION_OUTPUT
  cout << "\nGermany Routes"
       << "\n-------------------------------" << g << endl;

  /* Output
    Germany Routes
    -------------------------------
    [0] Augsburg
      --> [6 München] 84km
    [1] Erfurt
    [2] Frankfürt
      --> [5 Mannheim] 85km
      --> [9 Würzburg] 217km
      --> [4 Kassel] 173km
    [3] Karlsruhe
      --> [0 Augsburg] 250km
    [4] Kassel
      --> [6 München] 502km
    [5] Mannheim
      --> [3 Karlsruhe] 80km
    [6] München
    [7] Nürnberg
      --> [8 Stuttgart] 183km
      --> [6 München] 167km
    [8] Stuttgart
    [9] Würzburg
      --> [1 Erfurt] 186km
      --> [7 Nürnberg] 103km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  vertex_iterator_t<Graph>      u;
  vertex_edge_iterator_t<Graph> uv;

  cout << "vertex_iterator_t<Graph>      u; \n";
  cout << "vertex_edge_iterator_t<Graph> uv;\n";

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
      cout << "EXPECT_EQ(" << out_vertex_key(g, *uv) << ", out_vertex_key(g, *uv));\n";
      cout << "EXPECT_EQ(\"" << out_vertex(g, *uv)->name << "\", out_vertex(g, *uv)->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  vertex_iterator_t<Graph>      u;
  vertex_edge_iterator_t<Graph> uv;

  u = begin(g) + 0;
  EXPECT_EQ("Augsburg", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(84, uv->weight);

  u = begin(g) + 1;
  EXPECT_EQ("Erfurt", u->name);
  EXPECT_EQ(0, edges_size(g, *u));
  uv = begin(g, *u);

  u = begin(g) + 2;
  EXPECT_EQ("Frankfürt", u->name);
  EXPECT_EQ(3, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(5, out_vertex_key(g, *uv));
  EXPECT_EQ("Mannheim", out_vertex(g, *uv)->name);
  EXPECT_EQ(85, uv->weight);
  ++uv;
  EXPECT_EQ(9, out_vertex_key(g, *uv));
  EXPECT_EQ("Würzburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(217, uv->weight);
  ++uv;
  EXPECT_EQ(4, out_vertex_key(g, *uv));
  EXPECT_EQ("Kassel", out_vertex(g, *uv)->name);
  EXPECT_EQ(173, uv->weight);

  u = begin(g) + 3;
  EXPECT_EQ("Karlsruhe", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(0, out_vertex_key(g, *uv));
  EXPECT_EQ("Augsburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(250, uv->weight);

  u = begin(g) + 4;
  EXPECT_EQ("Kassel", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(502, uv->weight);

  u = begin(g) + 5;
  EXPECT_EQ("Mannheim", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(3, out_vertex_key(g, *uv));
  EXPECT_EQ("Karlsruhe", out_vertex(g, *uv)->name);
  EXPECT_EQ(80, uv->weight);

  u = begin(g) + 6;
  EXPECT_EQ("München", u->name);
  EXPECT_EQ(0, edges_size(g, *u));
  uv = begin(g, *u);

  u = begin(g) + 7;
  EXPECT_EQ("Nürnberg", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(8, out_vertex_key(g, *uv));
  EXPECT_EQ("Stuttgart", out_vertex(g, *uv)->name);
  EXPECT_EQ(183, uv->weight);
  ++uv;
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(167, uv->weight);

  u = begin(g) + 8;
  EXPECT_EQ("Stuttgart", u->name);
  EXPECT_EQ(0, edges_size(g, *u));
  uv = begin(g, *u);

  u = begin(g) + 9;
  EXPECT_EQ("Würzburg", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(1, out_vertex_key(g, *uv));
  EXPECT_EQ("Erfurt", out_vertex(g, *uv)->name);
  EXPECT_EQ(186, uv->weight);
  ++uv;
  EXPECT_EQ(7, out_vertex_key(g, *uv));
  EXPECT_EQ("Nürnberg", out_vertex(g, *uv)->name);
  EXPECT_EQ(103, uv->weight);
#endif
}

TEST_CASE("daa graph functions", "[daa][graph][functions]") {
  Graph        g  = create_germany_routes_graph();
  Graph const& gc = create_germany_routes_graph();

  //EXPECT_EQ(true, (is_same<empty_value, decltype(value(g))>::value));
  std::graph::vertex_range_t<Graph>       vr  = std::graph::vertices(g);
  std::graph::const_vertex_range_t<Graph> vrc = std::graph::vertices(gc);
  EXPECT_EQ(vr.size(), vrc.size());
  EXPECT_EQ(vr.size(), std::graph::vertices_size(gc));

  size_t cnt = 0;
  for (std::graph::vertex_iterator_t<Graph> u = std::graph::begin(g); u != std::graph::end(g); ++u, ++cnt)
    ;
  EXPECT_EQ(std::graph::vertices_size(gc), cnt);

  cnt = 0;
  for (std::graph::const_vertex_iterator_t<Graph> u = std::graph::begin(gc); u != std::graph::end(gc); ++u, ++cnt)
    ;
  EXPECT_EQ(std::graph::vertices_size(gc), cnt);

  cnt = 0;
  for (std::graph::const_vertex_iterator_t<Graph> u = std::graph::cbegin(gc); u != std::graph::cend(gc); ++u, ++cnt)
    ;
  EXPECT_EQ(std::graph::vertices_size(gc), cnt);

  //std::graph::reserve_vertices(g, 100); //undefined for semi-mutable graph
  //std::graph::resisze_vertices(g, 100); //undefined for semi-mutable graph

  std::graph::edge_range_t<Graph>       er       = std::graph::edges(g);
  std::graph::const_edge_range_t<Graph> erc      = std::graph::edges(gc);
  std::graph::edge_size_t<Graph>        edg_size = std::graph::edges_size(gc);
  EXPECT_EQ(std::graph::edges_size(gc), er.size());
  EXPECT_EQ(std::graph::edges_size(gc), erc.size());
  // std::graph::reserve_edges(g,100); // undefined for semi-mutable graph
  // std::graph::clear(g);             // undefined for semi-mutable graph

#if TEST_OPTION == TEST_OPTION_OUTPUT
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST_CASE("daa vertex functions", "[daa][vertex][functions]") {
  Graph        g  = create_germany_routes_graph();
  Graph const& gc = g;

  std::graph::vertex_iterator_t<Graph>       ui  = std::graph::begin(g);
  std::graph::const_vertex_iterator_t<Graph> uic = std::graph::cbegin(g);
  std::graph::vertex_t<Graph>&               u   = *ui;
  std::graph::vertex_t<Graph> const&         uc  = *uic;

  std::graph::vertex_key_t<Graph> vkey  = std::graph::vertex_key(g, u);
  std::graph::vertex_key_t<Graph> vkeyc = std::graph::vertex_key(g, uc);
  auto                            val   = std::graph::value(u);

  std::graph::vertex_iterator_t<Graph>       f1 = std::graph::find_vertex(g, 1);
  std::graph::const_vertex_iterator_t<Graph> f2 = std::graph::find_vertex(gc, 1);
  EXPECT_EQ(f1, f2);

  vertex_iterator_t<Graph> f3 = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });
  EXPECT_NE(f3, g.vertices().end());
  EXPECT_EQ(2, vertex_key(g, *f3));

  {
    std::graph::vertex_edge_range_t<Graph>          uvr      = std::graph::edges(g, u);
    std::graph::const_vertex_edge_range_t<Graph>    uvrc     = std::graph::edges(g, uc);
    std::graph::vertex_edge_iterator_t<Graph>       uvi_beg1 = std::graph::begin(g, u);
    std::graph::const_vertex_edge_iterator_t<Graph> uvi_beg2 = std::graph::begin(g, uc);
    std::graph::const_vertex_edge_iterator_t<Graph> uvi_beg3 = std::graph::cbegin(g, u);
    std::graph::vertex_edge_iterator_t<Graph>       uvi_end1 = std::graph::end(g, u);
    std::graph::const_vertex_edge_iterator_t<Graph> uvi_end2 = std::graph::end(g, uc);
    std::graph::const_vertex_edge_iterator_t<Graph> uvi_end3 = std::graph::cend(g, u);
    EXPECT_EQ(std::graph::edges_size(g, u), std::graph::edges_degree(g, u));
    EXPECT_EQ(std::graph::edges_size(g, u), uvr.size());
  }

  {
    std::graph::vertex_out_edge_range_t<Graph>          uvr      = std::graph::out_edges(g, u);
    std::graph::const_vertex_out_edge_range_t<Graph>    uvrc     = std::graph::out_edges(g, uc);
    std::graph::vertex_out_edge_iterator_t<Graph>       uvi_beg1 = std::graph::out_begin(g, u);
    std::graph::const_vertex_out_edge_iterator_t<Graph> uvi_beg2 = std::graph::out_begin(g, uc);
    std::graph::const_vertex_out_edge_iterator_t<Graph> uvi_beg3 = std::graph::out_cbegin(g, u);
    std::graph::vertex_out_edge_iterator_t<Graph>       uvi_end1 = std::graph::out_end(g, u);
    std::graph::const_vertex_out_edge_iterator_t<Graph> uvi_end2 = std::graph::out_end(g, uc);
    std::graph::const_vertex_out_edge_iterator_t<Graph> uvi_end3 = std::graph::out_cend(g, u);
    EXPECT_EQ(std::graph::out_size(g, u), std::graph::out_degree(g, u));
    EXPECT_EQ(std::graph::out_size(g, u), uvr.size());
  }
}

TEST_CASE("daa edge functions", "[daa][edge][functions]") {
  using namespace std::graph;
  Graph        g  = create_germany_routes_graph();
  Graph const& gc = g;

  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });
  vertex_iterator_t<Graph> v = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Mannheim"; });
  EXPECT_NE(end(g), u);
  EXPECT_NE(end(g), v);

  edge_iterator_t<Graph> uv = find_edge(g, *u, *v); // find edge Frankfurt --> Mannheim
  EXPECT_NE(end(edges(g)), uv);
  EXPECT_EQ(v, vertex(g, *uv, *u));
  EXPECT_EQ(v, out_vertex(g, *uv));
  EXPECT_EQ(u, in_vertex(g, *uv));
  edge_iterator_t<Graph> uv2 = find_edge(g, vertex_key(g, *u), vertex_key(g, *v));
  EXPECT_EQ(uv, uv2);

  vertex_out_edge_iterator_t<Graph> uv3;
  uv2 = find_out_edge(g, *u, *v);
  uv3 = find_out_edge(g, vertex_key(g, *u), vertex_key(g, *v));
  EXPECT_EQ(uv, uv2);
  EXPECT_EQ(uv, uv3);
}

TEST_CASE("daa dfs vertex", "[daa][dfs][vertex]") {
  Graph g = create_germany_routes_graph();

#if TEST_OPTION == TEST_OPTION_OUTPUT
  dfs_vertex_range dfs_vtx_rng(g, find_city(g, "Frankfürt"));
  for (dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << u->name << endl;

  /* Output: seed = Frankfürt
  Frankfürt
    Mannheim
      Karlsruhe
        Augsburg
          München
    Würzburg
      Erfurt
      Nürnberg
        Stuttgart
    Kassel
  */

  // a flat list when using range syntax (depth n/a on vertex)
  cout << endl;
  for (auto& u : dfs_vertex_range(g, begin(g) + 2)) // Frankfürt
    cout << u.name << endl;
    /* Output: seed = Frankfürt
    Frankfürt
    Mannheim
    Karlsruhe
    Augsburg
    München
    Würzburg
    Erfurt
    Nürnberg
    Stuttgart
    Kassel
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  dfs_vertex_range                  dfs_vtx_rng(g, find_city(g, "Frankfürt"));
  dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin();
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
  EXPECT_EQ("Würzburg", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Erfurt", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Nürnberg", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Stuttgart", (++u)->name);
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ("Kassel", (++u)->name);
  EXPECT_EQ(2, u.depth());
#endif
}

TEST_CASE("daa dfs edge", "[daa][dfs][edge]") {
  Graph          g = create_germany_routes_graph();
  dfs_edge_range dfs_edge_rng(g, find_city(g, "Frankfürt"));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (auto uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end(); ++uv) {
    vertex_iterator_t<Graph> u     = uv.in_vertex();
    vertex_key_t<Graph>      u_key = vertex_key(g, *u);
    if (uv.is_path_end()) {
      cout << string(uv.depth() * 2, ' ') << "view " << u->name << endl;
    } else {
      vtx_iter_t v         = uv.out_vertex(); // or vertex(g, *uv)
      bool       back_edge = uv.is_back_edge();
      cout << string(uv.depth() * 2, ' ') << "travel " << u->name << " --> " << v->name << " " << uv->weight << "km"
           << (uv.is_back_edge() ? " [back edge]" : "") << endl;
    }
  }

  /* Output: seed = Frankfürt
      travel Frankfürt --> Mannheim 85km
        travel Mannheim --> Karlsruhe 80km
        travel Karlsruhe --> Augsburg 250km
        travel Augsburg --> München 84km
        view München
      travel Frankfürt --> Würzburg 217km
        travel Würzburg --> Erfurt 186km
          view Erfurt
        travel Würzburg --> Nürnberg 103km
        travel Nürnberg --> Stuttgart 183km
          view Stuttgart
        travel Nürnberg --> München 167km
      travel Frankfürt --> Kassel 173km
      travel Kassel --> München 502km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  cout << "dfs_edge_range<Graph>::iterator uv = dfs_edge_rng.begin();\n"
       << "\n";
  size_t uvi = 0;
  for (dfs_edge_range<Graph>::iterator uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end(); ++uv, ++uvi) {
    if (uvi > 0)
      cout << "\n"
           << "++uv;\n";

    if (uv.is_path_end()) {
      cout << "EXPECT_TRUE(uv.is_path_end());\n";
      cout << "EXPECT_EQ(\"" << uv.back_vertex()->name << "\", uv.back_vertex()->name);\n";
      cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
    } else {
      cout << "EXPECT_FALSE(uv.is_path_end());\n";
      if (uv.is_back_edge())
        cout << "EXPECT_TRUE(uv.is_back_edge());\n";
      else
        cout << "EXPECT_FALSE(uv.is_back_edge());\n";
      cout << "EXPECT_EQ(\"" << in_vertex(g, *uv)->name << "\", in_vertex(g, *uv)->name);\n";
      cout << "EXPECT_EQ(\"" << out_vertex(g, *uv)->name << "\", out_vertex(g, *uv)->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
      cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  dfs_edge_range<Graph>::iterator uv = dfs_edge_rng.begin();

  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Frankfürt", in_vertex(g, *uv)->name);
  EXPECT_EQ("Mannheim", out_vertex(g, *uv)->name);
  EXPECT_EQ(85, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Mannheim", in_vertex(g, *uv)->name);
  EXPECT_EQ("Karlsruhe", out_vertex(g, *uv)->name);
  EXPECT_EQ(80, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Karlsruhe", in_vertex(g, *uv)->name);
  EXPECT_EQ("Augsburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(250, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Augsburg", in_vertex(g, *uv)->name);
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(84, uv->weight);
  EXPECT_EQ(4, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Stuttgart", uv.back_vertex()->name);
  EXPECT_EQ(5, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Frankfürt", in_vertex(g, *uv)->name);
  EXPECT_EQ("Würzburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(217, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", in_vertex(g, *uv)->name);
  EXPECT_EQ("Erfurt", out_vertex(g, *uv)->name);
  EXPECT_EQ(186, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Mannheim", uv.back_vertex()->name);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", in_vertex(g, *uv)->name);
  EXPECT_EQ("Nürnberg", out_vertex(g, *uv)->name);
  EXPECT_EQ(103, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", in_vertex(g, *uv)->name);
  EXPECT_EQ("Stuttgart", out_vertex(g, *uv)->name);
  EXPECT_EQ(183, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Erfurt", uv.back_vertex()->name);
  EXPECT_EQ(4, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", in_vertex(g, *uv)->name);
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(167, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Frankfürt", in_vertex(g, *uv)->name);
  EXPECT_EQ("Kassel", out_vertex(g, *uv)->name);
  EXPECT_EQ(173, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("Kassel", in_vertex(g, *uv)->name);
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(502, uv->weight);
  EXPECT_EQ(2, uv.depth());
#endif
}

TEST_CASE("daa bfs vertex", "[daa][bfs][vertex]") {
  Graph            g = create_germany_routes_graph();
  bfs_vertex_range bfs_vtx_rng(g, find_city(g, "Frankfürt"));

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
  bfs_vertex_range<Graph>::iterator u = bfs_vtx_rng.begin();
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

TEST_CASE("cass bfs edge", "[daa][bfs][edge]") {
  Graph          g = create_germany_routes_graph();
  bfs_edge_range bfs_edge_rng(g, find_city(g, "Frankfürt"));

#if TEST_OPTION == TEST_OPTION_OUTPUT
  for (auto uv = bfs_edge_rng.begin(); uv != bfs_edge_rng.end(); ++uv) {
    vertex_iterator_t<Graph> u     = in_vertex(g, *uv);
    vertex_key_t<Graph>      u_key = vertex_key(g, *u);
    if (uv.is_path_end()) {
      cout << string(uv.depth() * 2, ' ') << "view " << uv.back_vertex()->name << endl;
    } else {
      vtx_iter_t          v         = vertex(g, *uv, *u);
      vertex_key_t<Graph> v_key     = vertex_key(g, *v);
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
          view München
            travel Augsburg --> München 84km [back edge]
            view Stuttgart
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  cout << "bfs_edge_range<Graph>::iterator uv = bfs_edge_rng.begin();\n"
       << "\n";
  size_t uvi = 0;
  for (bfs_edge_range<Graph>::iterator uv = bfs_edge_rng.begin(); uv != bfs_edge_rng.end(); ++uv, ++uvi) {
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
      cout << "EXPECT_EQ(\"" << vertex(g, *uv, *uv.back_vertex())->name
           << "\", vertex(g, *uv, *uv.back_vertex())->name);\n";
      cout << "EXPECT_EQ(" << uv->weight << ", uv->weight);\n";
    }
    cout << "EXPECT_EQ(" << uv.depth() << ", uv.depth());\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  bfs_edge_range<Graph>::iterator uv = bfs_edge_rng.begin();

  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Mannheim", uv.back_vertex()->name);
  EXPECT_EQ("Mannheim", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(85, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Würzburg", uv.back_vertex()->name);
  EXPECT_EQ("Würzburg", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(217, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Kassel", uv.back_vertex()->name);
  EXPECT_EQ("Kassel", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(173, uv->weight);
  EXPECT_EQ(1, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Karlsruhe", uv.back_vertex()->name);
  EXPECT_EQ("Karlsruhe", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(80, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Erfurt", uv.back_vertex()->name);
  EXPECT_EQ("Erfurt", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(186, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Nürnberg", uv.back_vertex()->name);
  EXPECT_EQ("Nürnberg", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(103, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("München", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(502, uv->weight);
  EXPECT_EQ(2, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Augsburg", uv.back_vertex()->name);
  EXPECT_EQ("Augsburg", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(250, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Erfurt", uv.back_vertex()->name);
  EXPECT_EQ(85, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_FALSE(uv.is_back_edge());
  EXPECT_EQ("Stuttgart", uv.back_vertex()->name);
  EXPECT_EQ("Stuttgart", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(183, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("München", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(167, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ(183, uv->weight);
  EXPECT_EQ(3, uv.depth());

  ++uv;
  EXPECT_FALSE(uv.is_path_end());
  EXPECT_TRUE(uv.is_back_edge());
  EXPECT_EQ("München", uv.back_vertex()->name);
  EXPECT_EQ("München", vertex(g, *uv, *uv.back_vertex())->name);
  EXPECT_EQ(84, uv->weight);
  EXPECT_EQ(4, uv.depth());

  ++uv;
  EXPECT_TRUE(uv.is_path_end());
  EXPECT_EQ("Stuttgart", uv.back_vertex()->name);
  EXPECT_EQ(186, uv->weight);
  EXPECT_EQ(4, uv.depth());
#endif
}

TEST_CASE("daa dijkstra distance", "[daa][dikjstra][distance]") {
  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

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

TEST_CASE("daa bellman-ford distance", "[daa][bellman-ford][distance]") {
  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

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

TEST_CASE("daa dijkstra shortest path", "[daa][dikjstra][path]") {
  using std::graph::dijkstra_shortest_paths;
  using std::graph::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

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

TEST_CASE("daa bellman-fort shortest path", "[daa][bellman-ford][path]") {
  using std::graph::bellman_ford_shortest_paths;
  using std::graph::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

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


TEST_CASE("daa dfs transitive closure", "[daa][dfs][transitive closure]") {
  using std::graph::dfs_transitive_closure;
  using std::graph::reaches;

  using reachs_t      = reaches<Graph>;
  using reaches_vec_t = vector<reachs_t>;
  reaches_vec_t reaches_vec;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

#if TEST_OPTION == TEST_OPTION_OUTPUT
  dfs_transitive_closure(g, back_inserter(reaches_vec));
  for (reachs_t& r : reaches_vec) {
    cout << r.from->name << " --> " << r.to->name << "\n";
  }

  /* Output:
    Augsburg --> München
    Frankfürt --> Mannheim
    Frankfürt --> Karlsruhe
    Frankfürt --> Augsburg
    Frankfürt --> München
    Frankfürt --> Würzburg
    Frankfürt --> Erfurt
    Frankfürt --> Nürnberg
    Frankfürt --> Stuttgart
    Frankfürt --> Kassel
    Karlsruhe --> Augsburg
    Karlsruhe --> München
    Kassel --> München
    Mannheim --> Karlsruhe
    Mannheim --> Augsburg
    Mannheim --> München
    Nürnberg --> Stuttgart
    Nürnberg --> München
    Würzburg --> Erfurt
    Würzburg --> Nürnberg
    Würzburg --> Stuttgart
    Würzburg --> München
  */

#elif TEST_OPTION == TEST_OPTION_GEN
  {
    cout << "{\n";
    dfs_transitive_closure(g, back_inserter(reaches_vec));

    cout << "  dfs_transitive_closure(g, back_inserter(reaches_vec));\n";
    cout << "  EXPECT_EQ(" << reaches_vec.size() << ", reaches_vec.size());\n";
    for (size_t i = 0; i < reaches_vec.size(); ++i) {
      cout << "\n";
      cout << "  EXPECT_EQ(\"" << reaches_vec[i].from->name << "\", reaches_vec[" << i << "].from->name);\n";
      cout << "  EXPECT_EQ(\"" << reaches_vec[i].to->name << "\", reaches_vec[" << i << "].to->name);\n";
    }
    cout << "}\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  {
    dfs_transitive_closure(g, back_inserter(reaches_vec));
    EXPECT_EQ(22, reaches_vec.size());

    EXPECT_EQ("Augsburg", reaches_vec[0].from->name);
    EXPECT_EQ("München", reaches_vec[0].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[1].from->name);
    EXPECT_EQ("Mannheim", reaches_vec[1].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[2].from->name);
    EXPECT_EQ("Karlsruhe", reaches_vec[2].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[3].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[3].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[4].from->name);
    EXPECT_EQ("München", reaches_vec[4].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[5].from->name);
    EXPECT_EQ("Würzburg", reaches_vec[5].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[6].from->name);
    EXPECT_EQ("Erfurt", reaches_vec[6].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[7].from->name);
    EXPECT_EQ("Nürnberg", reaches_vec[7].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[8].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[8].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[9].from->name);
    EXPECT_EQ("Kassel", reaches_vec[9].to->name);

    EXPECT_EQ("Karlsruhe", reaches_vec[10].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[10].to->name);

    EXPECT_EQ("Karlsruhe", reaches_vec[11].from->name);
    EXPECT_EQ("München", reaches_vec[11].to->name);

    EXPECT_EQ("Kassel", reaches_vec[12].from->name);
    EXPECT_EQ("München", reaches_vec[12].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[13].from->name);
    EXPECT_EQ("Karlsruhe", reaches_vec[13].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[14].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[14].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[15].from->name);
    EXPECT_EQ("München", reaches_vec[15].to->name);

    EXPECT_EQ("Nürnberg", reaches_vec[16].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[16].to->name);

    EXPECT_EQ("Nürnberg", reaches_vec[17].from->name);
    EXPECT_EQ("München", reaches_vec[17].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[18].from->name);
    EXPECT_EQ("Erfurt", reaches_vec[18].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[19].from->name);
    EXPECT_EQ("Nürnberg", reaches_vec[19].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[20].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[20].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[21].from->name);
    EXPECT_EQ("München", reaches_vec[21].to->name);
  }
#endif
}


TEST_CASE("daa warshall transitive closure", "[daa][warshall][transitive closure]") {
  using std::graph::warshall_transitive_closure;
  using std::graph::reaches;

  using reachs_t      = reaches<Graph>;
  using reaches_vec_t = vector<reachs_t>;
  reaches_vec_t reaches_vec;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u2) { return u2.name == "Frankfürt"; });

#if TEST_OPTION == TEST_OPTION_OUTPUT
  warshall_transitive_closure(g, back_inserter(reaches_vec));
  for (reachs_t& r : reaches_vec) {
    cout << r.from->name << " --> " << r.to->name << "\n";
  }

  /* Output:
    Augsburg --> München
    Frankfürt --> Augsburg
    Frankfürt --> Erfurt
    Frankfürt --> Karlsruhe
    Frankfürt --> Kassel
    Frankfürt --> Mannheim
    Frankfürt --> München
    Frankfürt --> Nürnberg
    Frankfürt --> Stuttgart
    Frankfürt --> Würzburg
    Karlsruhe --> Augsburg
    Karlsruhe --> München
    Kassel --> München
    Mannheim --> Augsburg
    Mannheim --> Karlsruhe
    Mannheim --> München
    Nürnberg --> München
    Nürnberg --> Stuttgart
    Würzburg --> Erfurt
    Würzburg --> München
    Würzburg --> Nürnberg
    Würzburg --> Stuttgart
  */

#elif TEST_OPTION == TEST_OPTION_GEN
  {
    cout << "{\n";
    warshall_transitive_closure(g, back_inserter(reaches_vec));

    cout << "  warshall_transitive_closure(g, back_inserter(reaches_vec));\n";
    cout << "  EXPECT_EQ(" << reaches_vec.size() << ", reaches_vec.size());\n";
    for (size_t i = 0; i < reaches_vec.size(); ++i) {
      cout << "\n";
      cout << "  EXPECT_EQ(\"" << reaches_vec[i].from->name << "\", reaches_vec[" << i << "].from->name);\n";
      cout << "  EXPECT_EQ(\"" << reaches_vec[i].to->name << "\", reaches_vec[" << i << "].to->name);\n";
    }
    cout << "}\n";
  }
#elif TEST_OPTION == TEST_OPTION_TEST
  {
    warshall_transitive_closure(g, back_inserter(reaches_vec));
    EXPECT_EQ(22, reaches_vec.size());

    EXPECT_EQ("Augsburg", reaches_vec[0].from->name);
    EXPECT_EQ("München", reaches_vec[0].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[1].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[1].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[2].from->name);
    EXPECT_EQ("Erfurt", reaches_vec[2].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[3].from->name);
    EXPECT_EQ("Karlsruhe", reaches_vec[3].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[4].from->name);
    EXPECT_EQ("Kassel", reaches_vec[4].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[5].from->name);
    EXPECT_EQ("Mannheim", reaches_vec[5].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[6].from->name);
    EXPECT_EQ("München", reaches_vec[6].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[7].from->name);
    EXPECT_EQ("Nürnberg", reaches_vec[7].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[8].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[8].to->name);

    EXPECT_EQ("Frankfürt", reaches_vec[9].from->name);
    EXPECT_EQ("Würzburg", reaches_vec[9].to->name);

    EXPECT_EQ("Karlsruhe", reaches_vec[10].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[10].to->name);

    EXPECT_EQ("Karlsruhe", reaches_vec[11].from->name);
    EXPECT_EQ("München", reaches_vec[11].to->name);

    EXPECT_EQ("Kassel", reaches_vec[12].from->name);
    EXPECT_EQ("München", reaches_vec[12].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[13].from->name);
    EXPECT_EQ("Augsburg", reaches_vec[13].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[14].from->name);
    EXPECT_EQ("Karlsruhe", reaches_vec[14].to->name);

    EXPECT_EQ("Mannheim", reaches_vec[15].from->name);
    EXPECT_EQ("München", reaches_vec[15].to->name);

    EXPECT_EQ("Nürnberg", reaches_vec[16].from->name);
    EXPECT_EQ("München", reaches_vec[16].to->name);

    EXPECT_EQ("Nürnberg", reaches_vec[17].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[17].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[18].from->name);
    EXPECT_EQ("Erfurt", reaches_vec[18].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[19].from->name);
    EXPECT_EQ("München", reaches_vec[19].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[20].from->name);
    EXPECT_EQ("Nürnberg", reaches_vec[20].to->name);

    EXPECT_EQ("Würzburg", reaches_vec[21].from->name);
    EXPECT_EQ("Stuttgart", reaches_vec[21].to->name);
  }

#endif
}
