#include "pch.h"
#include "graph/compressed_adj_array.hpp"
#include "graph/iterators/std_dfs_range.hpp"
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find.hpp>
#include <string>

using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;

using vector_index = size_t;

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

// Bring functions into global namespace
using std::graph::vertex_key;
using std::graph::vertex;
using std::graph::value;
using std::graph::begin;
using std::graph::end;

struct route {
  string from;
  string to;
  int    km = 0;

  route(string const& from_city, string const& to_city, int kilometers)
        : from(from_city), to(to_city), km(kilometers) {}
};
struct route2 {
  vector_index from = numeric_limits<vector_index>::max();
  vector_index to   = numeric_limits<vector_index>::max();
  int          km   = 0;
  route2(vector_index from_city, vector_index to_city, int kilometers) : from(from_city), to(to_city), km(kilometers) {}
  route2(route const& r, vector<string> const& cities) : km(r.km) {
    from = ranges::find(cities, r.from) - cities.begin();
    to   = ranges::find(cities, r.to) - cities.begin();
    assert(from < cities.size());
    assert(to < cities.size());
  }
};

vector<string> unique_cities(vector<route>& routes) {
  vector<string> cities;
  cities.reserve(routes.size() * 2);
  for (route const& r : routes) {
    cities.push_back(r.from);
    cities.push_back(r.to);
  }
  return move(cities) | ranges::actions::sort | ranges::actions::unique;
};

vector<route2> to_route2s(vector<route> const& routes, vector<string> const& cities) {
  vector<route2> routes2;
  for (route const& r : routes)
    routes2.push_back(route2(r, cities));
  return routes2;
}

vector<Graph::edge_key_type> to_edge_keys(vector<route> const& routes, vector<string> const& cities) {
  vector<Graph::edge_key_type> edge_keys;
  for (route const& r : routes) {
    route2 r2(r, cities);
    edge_keys.push_back({r2.from, r2.to});
  }
  sort(edge_keys.begin(), edge_keys.end(),
       [](Graph::edge_key_type const& lhs, Graph::edge_key_type const& rhs) { return lhs.first < rhs.first; });
  return edge_keys;
}

vector<Graph::edge_value_type> to_edge_values(vector<route> const& routes, vector<string> const& cities) {
  vector<Graph::edge_value_type> edge_values;
  for (route const& r : routes) {
    route2 r2(r, cities);
    edge_values.push_back({{r2.from, r2.to}, r.km});
  }
  sort(edge_values.begin(), edge_values.end(),
       [](Graph::edge_value_type const& lhs, Graph::edge_value_type const& rhs) {
         return lhs.first.first < rhs.first.first;
       });
  return edge_values;
}

template <class OStream>
OStream& operator<<(OStream& os, Graph const& g) {
  for (auto& u : vertices(g)) {
    auto ukey = vertex_key(g, u);
    os << "\n[" << ukey << "] " << u.name;
    for (auto& uv : edges(g, u)) {
      auto v    = out_vertex(g, uv);
      auto vkey = vertex_key(g, *v);
      os << "\n  --> [" << vkey << " " << v->name << "] " << uv.weight << "km";
    }
  }
  os << "\n";
  return os;
}

static vector<route> routes{
      {"Frankfürt", "Mannheim", 85}, {"Frankfürt", "Würzburg", 217}, {"Frankfürt", "Kassel", 173},
      {"Mannheim", "Karlsruhe", 80}, {"Karlsruhe", "Augsburg", 250}, {"Augsburg", "München", 84},
      {"Würzburg", "Erfurt", 186},   {"Würzburg", "Nürnberg", 103},  {"Nürnberg", "Stuttgart", 183},
      {"Nürnberg", "München", 167},  {"Kassel", "München", 502}};


static vector<string> cities = unique_cities(routes); // cities is ordered


TEST(TestCAAGraph, TestMinObjSize) {
  using G = std::graph::compressed_adjacency_array<>;
  EXPECT_EQ(sizeof(size_t), sizeof(G::vertex_type));   // edge size = 8 bytes
  EXPECT_EQ(sizeof(size_t) * 2, sizeof(G::edge_type)); // vertex size = 16 bytes
}

TEST(TestCAAGraph, TestEmptyGraph) {
  Graph g;
  EXPECT_EQ(0, vertices_size(g));
  EXPECT_EQ(0, edges_size(g));
  //EXPECT_EQ(sizeof(Graph::edge_size_type) + sizeof(name_value), sizeof(Graph::vertex_type));
  //EXPECT_EQ(sizeof(Graph::vertex_size_type) + sizeof(weight_value), sizeof(Graph::edge_type));
}

TEST(TestCAAGraph, TestGraphInit) {
  vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);
  Graph                          g(cities, edge_routes);
  EXPECT_EQ(cities.size(), vertices_size(g));
  EXPECT_EQ(edge_routes.size(), edges_size(g));

  /*cout << endl << "Cities:" << endl;
  for (auto& city : cities)
    cout << "  " << (&city - cities.data()) << ". " << city << endl;

  cout << endl << "Routes:" << endl;
  for (auto& r : edge_routes)
    cout << "  " << cities[r.first.first] << " --> " << cities[r.first.second] << " " << r.second.weight << "km" << endl;*/

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
}

TEST(TestCAAGraph, DFSVertex) {
  vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);
  Graph                          g(cities, edge_routes);

  using vrange = std::graph::dfs_vertex_range;
  vrange dfs_vtx_rng(g, begin(g) + 2); // Frankfürt
  for (auto u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << value(*u).name << endl;

  /* Output:
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
}


TEST(TestCAAGraph, DFSEdge) {
  vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);
  Graph                          g(cities, edge_routes);

  using erange = std::graph::dfs_edge_range;
  erange dfs_edge_rng(g, begin(g) + 2); // Frankfürt
  for (auto uv = dfs_edge_rng.begin(); uv != dfs_edge_rng.end(); ++uv) {
    vtx_iter_t u     = uv.in_vertex();
    vtx_key_t  u_key = vertex_key(g, *u);
    if (uv.is_back_edge()) {
      cout << string(uv.depth(), ' ') << "view " << value(*u).name << endl;
    } else {
      vtx_iter_t v = vertex(g, *uv);
      cout << string(uv.depth(), ' ') << "travel " << value(*u).name << " --> " << value(*v).name << " "
           << value(*uv).weight << "km" << endl;
    }
  }

  /* Output
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
}
