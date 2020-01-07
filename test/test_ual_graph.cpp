#include "pch.h"
#include "graph/undirected_adj_list.hpp"
//#include "graph/range/dfs.hpp"
//#include "graph/range/bfs.hpp"
//#include "graph/algorithm/shortest_paths.hpp"
#include "data_routes.hpp"

/* ToDo
    1.  Get graph output to match expected
    2.  Replace vector(g,uv) --> vector(g,uv,source)
    3.  Test dfs
    4.  Test bfs
    5.  Test shortest_paths
 */

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

using namespace std::graph; // Bring graph types & functions into global namespace
using Graph      = std::graph::undirected_adjacency_list<name_value, weight_value>;
using vtx_iter_t = std::graph::vertex_iterator_t<Graph>;
using vtx_key_t  = std::graph::vertex_key_t<Graph>;

struct route;
using Routes = routes_t;

template <class OStream>
OStream& operator<<(OStream& os, Graph const& g);

vector<Graph::edge_value_type> ual_germany_edge_routes = to_edge_values<Graph>(germany_routes, germany_cities);

vertex_iterator_t<Graph> find_city(Graph& g, string_view const city_name) {
  return ::ranges::find_if(g, [&city_name](vertex_t<Graph>& u) { return u.name == city_name; });
}

Graph create_germany_routes_graph() {
#if 1
  return Graph(
        ual_germany_edge_routes, germany_cities, [](Graph::edge_value_type const& er) { return er.first; },
        [](Graph::edge_value_type const& er) { return er.second; },
        [](string const& city) -> string const& { return city; });
#else
  auto  ekey_fnc  = [](Graph::edge_value_type const& er) { return er.first; };
  auto  eprop_fnc = [](Graph::edge_value_type const& er) { return er.second; };
  Graph g(vector<Graph::edge_value_type>(), germany_cities, ekey_fnc, eprop_fnc,
          [](string const& city) -> string const& { return city; });

  int hit = 0;
  for (auto& edge_data : ual_germany_edge_routes) {
    Graph::edge_key_type uv_key = ekey_fnc(edge_data);
    cout << "\n\nAdd: (" << ++hit << ") [" << uv_key.first << "," << uv_key.second << "] " << find_vertex(g, uv_key.first)->name << " <--> "
         << find_vertex(g, uv_key.second)->name << "\n";

    g.create_edge(uv_key.first, uv_key.second, eprop_fnc(edge_data));
    cout << g;
  }

  hit = 0;
  for (vertex_t<Graph>& u : vertices(g)) {
    while (edges_size(g, u) > 0) {
      edge_t<Graph>* uv = &*begin(g, u);
      vertex_key_t<Graph> ukey = in_vertex_key(g, *uv);
      vertex_key_t<Graph> vkey = out_vertex_key(g, *uv);
      cout << "\n\nRemove: (" << ++hit << ") [" << ukey << "," << vkey << "] "
           << find_vertex(g, ukey)->name << " <--> " << find_vertex(g, vkey)->name << "\n";

      erase_edge(g, begin(g, u));
      cout << g;
    }
  }
  return g;
#endif
}

template <class OStream>
OStream& operator<<(OStream& os, Graph const& g) {
  for (vertex_t<Graph> const& u : vertices(g)) {
    vertex_key_t<Graph> ukey = vertex_key(g, u);
    os << "\n[" << ukey << "] " << u.name;
    for (edge_t<Graph> const& uv : edges(g, u)) {
      const_vertex_iterator_t<Graph> v    = vertex(g, uv, u);
      vertex_key_t<Graph>            vkey = vertex_key(g, *v);
      os << "\n  <--> [" << vkey << " " << v->name << "] " << uv.weight << "km";
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
  EXPECT_EQ(ual_germany_edge_routes.size()*2, nEdges);

  // iterate thru edges range
  size_t n = 0;
  for (auto& uv : edges(g))
    ++n;
  EXPECT_EQ(ual_germany_edge_routes.size()*2, n);

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
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(84, uv->weight);
  ++uv;
  EXPECT_EQ(0, out_vertex_key(g, *uv));
  EXPECT_EQ("Augsburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(250, uv->weight);

  u = begin(g) + 1;
  EXPECT_EQ("Erfurt", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(1, out_vertex_key(g, *uv));
  EXPECT_EQ("Erfurt", out_vertex(g, *uv)->name);
  EXPECT_EQ(186, uv->weight);

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
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(0, out_vertex_key(g, *uv));
  EXPECT_EQ("Augsburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(250, uv->weight);
  ++uv;
  EXPECT_EQ(3, out_vertex_key(g, *uv));
  EXPECT_EQ("Karlsruhe", out_vertex(g, *uv)->name);
  EXPECT_EQ(80, uv->weight);

  u = begin(g) + 4;
  EXPECT_EQ("Kassel", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(4, out_vertex_key(g, *uv));
  EXPECT_EQ("Kassel", out_vertex(g, *uv)->name);
  EXPECT_EQ(173, uv->weight);
  ++uv;
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(502, uv->weight);

  u = begin(g) + 5;
  EXPECT_EQ("Mannheim", u->name);
  EXPECT_EQ(2, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(5, out_vertex_key(g, *uv));
  EXPECT_EQ("Mannheim", out_vertex(g, *uv)->name);
  EXPECT_EQ(85, uv->weight);
  ++uv;
  EXPECT_EQ(3, out_vertex_key(g, *uv));
  EXPECT_EQ("Karlsruhe", out_vertex(g, *uv)->name);
  EXPECT_EQ(80, uv->weight);

  u = begin(g) + 6;
  EXPECT_EQ("München", u->name);
  EXPECT_EQ(3, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(84, uv->weight);
  ++uv;
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(502, uv->weight);
  ++uv;
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(167, uv->weight);

  u = begin(g) + 7;
  EXPECT_EQ("Nürnberg", u->name);
  EXPECT_EQ(3, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(8, out_vertex_key(g, *uv));
  EXPECT_EQ("Stuttgart", out_vertex(g, *uv)->name);
  EXPECT_EQ(183, uv->weight);
  ++uv;
  EXPECT_EQ(6, out_vertex_key(g, *uv));
  EXPECT_EQ("München", out_vertex(g, *uv)->name);
  EXPECT_EQ(167, uv->weight);
  ++uv;
  EXPECT_EQ(7, out_vertex_key(g, *uv));
  EXPECT_EQ("Nürnberg", out_vertex(g, *uv)->name);
  EXPECT_EQ(103, uv->weight);

  u = begin(g) + 8;
  EXPECT_EQ("Stuttgart", u->name);
  EXPECT_EQ(1, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(8, out_vertex_key(g, *uv));
  EXPECT_EQ("Stuttgart", out_vertex(g, *uv)->name);
  EXPECT_EQ(183, uv->weight);

  u = begin(g) + 9;
  EXPECT_EQ("Würzburg", u->name);
  EXPECT_EQ(3, edges_size(g, *u));
  uv = begin(g, *u);
  EXPECT_EQ(9, out_vertex_key(g, *uv));
  EXPECT_EQ("Würzburg", out_vertex(g, *uv)->name);
  EXPECT_EQ(217, uv->weight);
  ++uv;
  EXPECT_EQ(1, out_vertex_key(g, *uv));
  EXPECT_EQ("Erfurt", out_vertex(g, *uv)->name);
  EXPECT_EQ(186, uv->weight);
  ++uv;
  EXPECT_EQ(7, out_vertex_key(g, *uv));
  EXPECT_EQ("Nürnberg", out_vertex(g, *uv)->name);
  EXPECT_EQ(103, uv->weight);
#endif
}


TEST(TestUALGraph, AllGraphFunctions) {
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
  //EXPECT_EQ(std::graph::edges_size(gc), er.size());  // forward-only range; size n/a
  //EXPECT_EQ(std::graph::edges_size(gc), erc.size()); // forward-only range; size n/a
  // std::graph::reserve_edges(g,100); // undefined for semi-mutable graph
  // std::graph::clear(g);             // undefined for semi-mutable graph

#if TEST_OPTION == TEST_OPTION_OUTPUT
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST(TestUALGraph, AllVertexFunctions) {
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

  vertex_iterator_t<Graph> f3 = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });
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
    //EXPECT_EQ(std::graph::edges_size(g, u), uvr.size()); // forward-only range; size n/a
  }

  /*{
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
  }*/
}

TEST(TestUALGraph, AllEdgeFunctions) {
  using namespace std::graph;
  Graph        g  = create_germany_routes_graph();
  Graph const& gc = g;

  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });
  vertex_iterator_t<Graph> v = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Mannheim"; });
  EXPECT_NE(end(g), u);
  EXPECT_NE(end(g), v);

  edge_iterator_t<Graph> uv = find_edge(g, *u, *v); // find edge Frankfurt --> Mannheim
  EXPECT_NE(end(g.edges()), uv);
  EXPECT_EQ(v, vertex(g, *uv));
  EXPECT_EQ(v, out_vertex(g, *uv));
  EXPECT_EQ(u, in_vertex(g, *uv));
  edge_iterator_t<Graph> uv2 = find_edge(g, vertex_key(g, *u), vertex_key(g, *v));
  EXPECT_EQ(uv, uv2);

  edge_iterator_t<Graph> uv3;
  uv2 = find_edge(g, *u, *v);
  uv3 = find_edge(g, vertex_key(g, *u), vertex_key(g, *v));
  EXPECT_EQ(uv, uv2);
  EXPECT_EQ(uv, uv3);
}
