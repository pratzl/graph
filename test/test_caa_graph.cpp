#include "pch.h"
#include "graph/compressed_adj_array.hpp"
#include "graph/range/dfs.hpp"
#include "graph/range/bfs.hpp"
#include "graph/algorithm/shortest_paths.hpp"
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find.hpp>
#include <string>

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

struct route;
struct route2;
using Routes  = vector<route>;
using Route2s = vector<route2>;

using std::graph::empty_value;
using std::graph::name_value;
using std::graph::weight_value;

using Graph      = std::graph::compressed_adjacency_array<name_value, weight_value>;
using vtx_iter_t = std::graph::vertex_iterator_t<Graph>;
using vtx_key_t  = std::graph::vertex_key_t<Graph>;
//using vector_index = size_t;

// Bring functions into global namespace
using std::graph::vertex_key;
using std::graph::vertex;
using std::graph::value;
using std::graph::begin;
using std::graph::end;
using std::graph::vertex_t;
using std::graph::vertex_key_t;
using std::graph::vertex_iterator_t;
using std::graph::const_vertex_iterator_t;
using std::graph::edge_t;
using std::graph::edge_value_t;

using std::graph::shortest_distance;
using std::graph::shortest_path;

struct route;
vector<string>                 unique_cities(vector<route>& routes);
vector<Graph::edge_value_type> to_edge_values(vector<route> const& routes, vector<string> const& cities);
void                           imbue_utf8();

struct route {
  string from;
  string to;
  int    km = 0;

  route(string const& from_city, string const& to_city, int kilometers)
        : from(from_city), to(to_city), km(kilometers) {}
};

static vector<route> routes{
      {"Frankfürt", "Mannheim", 85}, {"Frankfürt", "Würzburg", 217}, {"Frankfürt", "Kassel", 173},
      {"Mannheim", "Karlsruhe", 80}, {"Karlsruhe", "Augsburg", 250}, {"Augsburg", "München", 84},
      {"Würzburg", "Erfurt", 186},   {"Würzburg", "Nürnberg", 103},  {"Nürnberg", "Stuttgart", 183},
      {"Nürnberg", "München", 167},  {"Kassel", "München", 502}};


static vector<string>                 cities      = unique_cities(routes); // cities is ordered
static vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);


Graph create_germany_routes_graph() {
  return Graph(
        edge_routes, cities, [](Graph::edge_value_type const& er) { return er.first; },
        [](Graph::edge_value_type const& er) { return er.second; },
        [](string const& city) -> string const& { return city; });
}

vector<string> unique_cities(vector<route>& routes) {
  imbue_utf8();
  vector<string> cities;
  cities.reserve(routes.size() * 2);
  for (route const& r : routes) {
    cities.push_back(r.from);
    cities.push_back(r.to);
  }
  return move(cities) | ranges::actions::sort | ranges::actions::unique;
};

void imbue_utf8() { cout.imbue(std::locale("en_US.utf8")); }

vector<Graph::edge_value_type> to_edge_values(vector<route> const& routes, vector<string> const& cities) {
  vector<Graph::edge_value_type> edge_values;
  edge_values.reserve(routes.size());
  for (route const& r : routes) {
    vtx_key_t from = static_cast<vtx_key_t>(::ranges::find(cities, r.from) - cities.begin());
    vtx_key_t to   = static_cast<vtx_key_t>(::ranges::find(cities, r.to) - cities.begin());
    assert(from < cities.size());
    assert(to < cities.size());
    edge_values.push_back({{from, to}, r.km});
  }
  auto cmp = [](Graph::edge_value_type const& lhs, Graph::edge_value_type const& rhs) {
    return lhs.first.first < rhs.first.first;
  };
  ::ranges::sort(edge_values, cmp);
  return edge_values;
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


TEST(TestCAAGraph, TestMinObjSize) {
  using G = std::graph::compressed_adjacency_array<>;
  EXPECT_EQ(4, sizeof(G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(8, sizeof(G::edge_type));   // edge size = 8 bytes
}

TEST(TestCAAGraph, TestEmptyGraph) {
  Graph g;
  EXPECT_EQ(0, vertices_size(g));
  EXPECT_EQ(0, edges_size(g));
  //EXPECT_EQ(sizeof(Graph::edge_size_type) + sizeof(name_value), sizeof(Graph::vertex_type));
  //EXPECT_EQ(sizeof(Graph::vertex_size_type) + sizeof(weight_value), sizeof(Graph::edge_type));
}

TEST(TestCAAGraph, TestGraphInit) {
#if 0
  vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);
  Graph                          g(cities, edge_routes);
#endif
  Graph g = create_germany_routes_graph();
  EXPECT_EQ(cities.size(), vertices_size(g));
  EXPECT_EQ(edge_routes.size(), edges_size(g));

#if 0
  cout << endl << "Cities:" << endl;
  for (auto& city : cities)
    cout << "  " << (&city - cities.data()) << ". " << city << endl;

  cout << endl << "Routes:" << endl;
  for (auto& r : edge_routes)
    cout << "  " << cities[r.first.first] << " --> " << cities[r.first.second] << " " << r.second.weight << "km" << endl;
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
  EXPECT_EQ(cities.size(), nVertices);
  EXPECT_EQ(edge_routes.size(), nEdges);

  // iterate thru edges range
  size_t n = 0;
  for (auto& uv : edges(g))
    ++n;
  EXPECT_EQ(edge_routes.size(), n);

#if TEST_OPTION == TEST_OPTION_OUTPUT
  cout << "\nGermany Routes"
       << "\n-------------------------------" << g << endl;

/* Output
    Germany Routes
    -------------------------------
    [0] Augsburg
      --> [6 Mnnchen] 84km
    [1] Erfurt
    [2] Frankfnrt
      --> [5 Mannheim] 85km
      --> [9 Wnrzburg] 217km
      --> [4 Kassel] 173km
    [3] Karlsruhe
      --> [0 Augsburg] 250km
    [4] Kassel
      --> [6 Mnnchen] 502km
    [5] Mannheim
      --> [3 Karlsruhe] 80km
    [6] Mnnchen
    [7] Nnrnberg
      --> [8 Stuttgart] 183km
      --> [6 Mnnchen] 167km
    [8] Stuttgart
    [9] Wnrzburg
      --> [1 Erfurt] 186km
      --> [7 Nnrnberg] 103km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
  for (vertex_t<Graph> const& u : vertices(g)) {
    vertex_key_t<Graph> ukey = vertex_key(g, u);
    os << "\n[" << ukey << "] " << u.name;
    for (edge_t<Graph> const& uv : edges(g, u)) {
      const_vertex_iterator_t<Graph> v    = out_vertex(g, uv);
      vertex_key_t<Graph>            vkey = vertex_key(g, *v);
      os << "\n  --> [" << vkey << " " << v->name << "] " << uv.weight << "km";
    }
  }
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST(TestCAAGraph, AllGraphFunctions) {
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

TEST(TestCAAGraph, AllVertexFunctions) {
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

TEST(TestCAAGraph, AllEdgeFunctions) {
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

  vertex_out_edge_iterator_t<Graph> uv3;
  uv2 = find_out_edge(g, *u, *v);
  uv3 = find_out_edge(g, vertex_key(g, *u), vertex_key(g, *v));
  EXPECT_EQ(uv, uv2);
  EXPECT_EQ(uv, uv3);
}

TEST(TestCAAGraph, DFSVertex) {
  Graph g = create_germany_routes_graph();

#if TEST_OPTION == TEST_OPTION_OUTPUT
  using vrange = std::graph::dfs_vertex_range;
  vrange dfs_vtx_rng(g, begin(g) + 2); // Frankfürt
  for (auto u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << u->name << endl;

  /* Output: seed = Frankfnrt
  Frankfnrt
    Mannheim
      Karlsruhe
        Augsburg
          Mnnchen
    Wnrzburg
      Erfurt
      Nnrnberg
        Stuttgart
    Kassel
  */

  // a flat list when using range syntax (depth n/a on vertex)
  cout << endl;
  for (auto& u : vrange(g, begin(g) + 2)) // Frankfürt
    cout << u.name << endl;
    /* Output: seed = Frankfnrt
  Frankfnrt
  Mannheim
  Karlsruhe
  Augsburg
  Mnnchen
  Wnrzburg
  Erfurt
  Nnrnberg
  Stuttgart
  Kassel
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}


TEST(TestCAAGraph, DFSEdge) {
  Graph g = create_germany_routes_graph();

#if TEST_OPTION == TEST_OPTION_OUTPUT
  using erange = std::graph::dfs_edge_range;
  erange dfs_edge_rng(g, begin(g) + 2); // Frankfürt
  for (auto uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end(); ++uv) {
    vertex_iterator_t<Graph> u     = out_vertex(g, *uv);
    vertex_key_t<Graph>      u_key = vertex_key(g, *u);
    if (uv.is_back_edge()) {
      cout << string(uv.depth() * 2, ' ') << "view " << u->name << endl;
    } else {
      vtx_iter_t v = out_vertex(g, *uv); // or vertex(g, *uv)
      cout << string(uv.depth() * 2, ' ') << "travel " << u->name << " --> " << v->name << " " << uv->weight << "km"
           << endl;
    }
  }

  /* Output: seed = Frankfnrt
      travel Frankfnrt --> Mannheim 85km
        travel Mannheim --> Karlsruhe 80km
        travel Karlsruhe --> Augsburg 250km
        travel Augsburg --> Mnnchen 84km
        view Mnnchen
      travel Frankfnrt --> Wnrzburg 217km
        travel Wnrzburg --> Erfurt 186km
          view Erfurt
        travel Wnrzburg --> Nnrnberg 103km
        travel Nnrnberg --> Stuttgart 183km
          view Stuttgart
        travel Nnrnberg --> Mnnchen 167km
      travel Frankfnrt --> Kassel 173km
      travel Kassel --> Mnnchen 502km
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST(TestCAAGraph, BFSVertex) {
  Graph g = create_germany_routes_graph();

#if TEST_OPTION == TEST_OPTION_OUTPUT
  using vrange = std::graph::bfs_vertex_range;
  vrange bfs_vtx_rng(g, begin(g) + 2); // Frankfürt
  for (auto u = bfs_vtx_rng.begin(); u != bfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << u->name << endl;

    /* Output: seed = Frankfnrt
    Frankfnrt
      Mannheim
      Wnrzburg
      Kassel
        Karlsruhe
        Erfurt
        Nnrnberg
        Mnnchen
          Augsburg
          Stuttgart
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST(TestCAAGraph, BFSEdge) {
  Graph g = create_germany_routes_graph();

#if TEST_OPTION == TEST_OPTION_OUTPUT
  using erange = std::graph::bfs_edge_range;
  erange bfs_edge_rng(g, begin(g) + 2); // Frankfürt
  for (auto uv = bfs_edge_rng.begin(); uv != bfs_edge_rng.end(); ++uv) {
    vertex_iterator_t<Graph> u     = in_vertex(g, *uv);
    vertex_key_t<Graph>      u_key = vertex_key(g, *u);
    if (uv.is_back_edge()) {
      cout << string(uv.depth() * 2, ' ') << "view " << u->name << endl;
    } else {
      vtx_iter_t v = vertex(g, *uv);
      cout << string(uv.depth() * 2, ' ') << "travel " << u->name << " --> " << v->name << " " << uv->weight << "km"
           << endl;
    }
  }

  /* Output: seed = Frankfnrt
      travel Frankfnrt --> Mannheim 85km
      travel Frankfnrt --> Wnrzburg 217km
      travel Frankfnrt --> Kassel 173km
        travel Mannheim --> Karlsruhe 80km
        travel Wnrzburg --> Erfurt 186km
        travel Wnrzburg --> Nnrnberg 103km
        travel Kassel --> Mnnchen 502km
          travel Karlsruhe --> Augsburg 250km
          view Erfurt
          travel Nnrnberg --> Stuttgart 183km
          travel Nnrnberg --> Mnnchen 167km
          view Mnnchen
            travel Augsburg --> Mnnchen 84km
            view Stuttgart
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
#endif
}

TEST(TestCAAGraph, DijkstraShortestDistances) {
  using std::graph::dijkstra_shortest_distances;
  using std::graph::dijkstra_shortest_paths;
  using std::graph::shortest_distance;
  using std::graph::shortest_path;

  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

  dijkstra_shortest_distances<int>(g, u, back_inserter(short_dists), false, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfurt
    Frankfnrt --> Augsburg  415km
    Frankfnrt --> Erfurt  403km
    Frankfnrt --> Frankfnrt  0km
    Frankfnrt --> Karlsruhe  165km
    Frankfnrt --> Kassel  173km
    Frankfnrt --> Mannheim  85km
    Frankfnrt --> Mnnchen  487km
    Frankfnrt --> Nnrnberg  320km
    Frankfnrt --> Stuttgart  503km
    Frankfnrt --> Wnrzburg  217km
  */

  cout << "\n";
  short_dists.clear();
  dijkstra_shortest_distances<int>(g, u, back_inserter(short_dists), true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfurt
    Frankfnrt --> Erfurt  403km
    Frankfnrt --> Mnnchen  487km
    Frankfnrt --> Stuttgart  503km
  */
}

TEST(TestCAAGraph, BellmanFordShortestDistances) {
  using std::graph::bellman_ford_shortest_distances;
  using std::graph::shortest_distance;

  using short_dist_t  = shortest_distance<vertex_iterator_t<Graph>, int>;
  using short_dists_t = vector<short_dist_t>;
  short_dists_t short_dists;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

  bellman_ford_shortest_distances<int>(g, u, back_inserter(short_dists), false, true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfurt
    Frankfnrt --> Augsburg  415km
    Frankfnrt --> Erfurt  403km
    Frankfnrt --> Frankfnrt  0km
    Frankfnrt --> Karlsruhe  165km
    Frankfnrt --> Kassel  173km
    Frankfnrt --> Mannheim  85km
    Frankfnrt --> Mnnchen  487km
    Frankfnrt --> Nnrnberg  320km
    Frankfnrt --> Stuttgart  503km
    Frankfnrt --> Wnrzburg  217km
  */

  cout << "\n";
  short_dists.clear();
  bellman_ford_shortest_distances<int>(g, u, back_inserter(short_dists), true, true, weight_fnc);
  for (short_dist_t& sd : short_dists)
    cout << sd.first->name << " --> " << sd.last->name << "  " << sd.distance << "km\n";
  /* Output: source = Frankfurt
    Frankfnrt --> Erfurt  403km
    Frankfnrt --> Mnnchen  487km
    Frankfnrt --> Stuttgart  503km
  */
}

TEST(TestCAAGraph, DijkstraShortestPaths) {
  using std::graph::dijkstra_shortest_paths;
  using std::graph::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

  dijkstra_shortest_paths<int>(g, u, back_inserter(short_paths), false, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfurt
    Frankfnrt --> Mannheim --> Karlsruhe --> Augsburg  415km
    Frankfnrt --> Wnrzburg --> Erfurt  403km
    Frankfnrt  0km
    Frankfnrt --> Mannheim --> Karlsruhe  165km
    Frankfnrt --> Kassel  173km
    Frankfnrt --> Mannheim  85km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Mnnchen  487km
    Frankfnrt --> Wnrzburg --> Nnrnberg  320km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Stuttgart  503km
    Frankfnrt --> Wnrzburg  217km
  */

  cout << "\n";
  short_paths.clear();
  dijkstra_shortest_paths<int>(g, u, back_inserter(short_paths), true, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfurt
    Frankfnrt --> Wnrzburg --> Erfurt  403km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Mnnchen  487km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Stuttgart  503km
  */
}

TEST(TestCAAGraph, BellmanFordShortestPaths) {
  using std::graph::bellman_ford_shortest_paths;
  using std::graph::shortest_path;

  using short_path_t  = shortest_path<vertex_iterator_t<Graph>, int>;
  using short_paths_t = vector<short_path_t>;
  short_paths_t short_paths;

  Graph                    g = create_germany_routes_graph();
  vertex_iterator_t<Graph> u = ::ranges::find_if(
        g, [](vertex_t<Graph>& u) { return u.name == "Frankfürt"; });

  auto weight_fnc = [](edge_value_t<Graph>& uv) -> int { return uv.weight; };

  bool neg_edge_cycle_exists = bellman_ford_shortest_paths<int>(
        g, u, back_inserter(short_paths), false, true,
                                   weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfurt
    Frankfnrt --> Mannheim --> Karlsruhe --> Augsburg  415km
    Frankfnrt --> Wnrzburg --> Erfurt  403km
    Frankfnrt  0km
    Frankfnrt --> Mannheim --> Karlsruhe  165km
    Frankfnrt --> Kassel  173km
    Frankfnrt --> Mannheim  85km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Mnnchen  487km
    Frankfnrt --> Wnrzburg --> Nnrnberg  320km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Stuttgart  503km
    Frankfnrt --> Wnrzburg  217km
  */

  cout << "\n";
  short_paths.clear();
  neg_edge_cycle_exists = bellman_ford_shortest_paths<int>(
        g, u, back_inserter(short_paths), true, true, weight_fnc);
  for (short_path_t& sp : short_paths) {
    for (size_t i = 0; i < sp.path.size(); ++i) {
      if (i > 0)
        cout << " --> ";
      cout << sp.path[i]->name;
    }
    cout << "  " << sp.distance << "km\n";
  }
  /* Output: source = Frankfurt
    Frankfnrt --> Wnrzburg --> Erfurt  403km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Mnnchen  487km
    Frankfnrt --> Wnrzburg --> Nnrnberg --> Stuttgart  503km
  */
}
