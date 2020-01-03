#include "pch.h"
#include "graph/undirected_adj_list.hpp"
//#include "graph/range/dfs.hpp"
//#include "graph/range/bfs.hpp"
//#include "graph/algorithm/shortest_paths.hpp"
#include "data_routes.hpp"

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_OUTPUT


using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;

using namespace std::graph; // Bring graph types & functions into global namespace
using Graph      = std::graph::undirected_adjacency_list<name_value, weight_value>;
using vtx_iter_t = std::graph::vertex_iterator_t<Graph>;
using vtx_key_t  = std::graph::vertex_key_t<Graph>;

struct route;
using Routes = routes_t;

vector<Graph::edge_value_type> ual_germany_edge_routes = to_edge_values<Graph>(germany_routes, germany_cities);

vertex_iterator_t<Graph> find_city(Graph& g, string_view const city_name) {
  return ::ranges::find_if(g, [&city_name](vertex_t<Graph>& u) { return u.name == city_name; });
}

Graph create_germany_routes_graph() {
  return Graph(
        ual_germany_edge_routes, germany_cities, [](Graph::edge_value_type const& er) { return er.first; },
        [](Graph::edge_value_type const& er) { return er.second; },
        [](string const& city) -> string const& { return city; });
}

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


TEST(TestUALGraph, TestMinObjSize) {
  using G = std::graph::undirected_adjacency_list<>;
  EXPECT_EQ(24, sizeof(G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(48, sizeof(G::edge_type));   // edge size = 8 bytes
}

TEST(TestUALGraph, TestEmptyGraph) {
  Graph g;
  EXPECT_EQ(0, vertices_size(g));
  EXPECT_EQ(0, edges_size(g));
}

TEST(TestUALGraph, TestGraphInit) {
#if 0
  vector<Graph::edge_value_type> caa_germany_edge_routes = to_edge_values(routes, germany_cities);
  Graph                          g(germany_cities, caa_germany_edge_routes);
#endif
  Graph g = create_germany_routes_graph();
  EXPECT_EQ(germany_cities.size(), vertices_size(g));
  EXPECT_EQ(ual_germany_edge_routes.size(), edges_size(g));

#if 0
  cout << endl << "Cities:" << endl;
  for (auto& city : germany_cities)
    cout << "  " << (&city - germany_cities.data()) << ". " << city << endl;

  cout << endl << "Routes:" << endl;
  for (auto& r : caa_germany_edge_routes)
    cout << "  " << germany_cities[r.first.first] << " --> " << germany_cities[r.first.second] << " " << r.second.weight << "km" << endl;
#endif

  // iterate thru vertices range
  size_t nVertices = 0;
  size_t nEdges    = 0;
  for (auto& u : vertices(g)) {
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
  EXPECT_EQ(germany_cities.size(), nVertices);
  EXPECT_EQ(ual_germany_edge_routes.size(), nEdges);

  // iterate thru edges range
  size_t n = 0;
  for (auto& uv : edges(g))
    ++n;
  EXPECT_EQ(ual_germany_edge_routes.size(), n);

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
