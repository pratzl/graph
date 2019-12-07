#include "pch.h"
#include "graph/compressed_adj_array.hpp"
#include "graph/iterators/std_dfs_range.hpp"
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/algorithm/find.hpp>

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
using Graph = std::graph::compressed_adjacency_array<name_value, weight_value>;

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
      auto& v    = out_vertex(g, uv);
      auto  vkey = vertex_key(g, v);
      os << "\n  --> [" << vkey << " " << v.name << "] " << uv.weight << "km";
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

  //cout << "\nGermany Routes"
  //     << "\n-------------------------------" << g << endl;
}

#define ENABLE_CAA_DFS
#ifdef ENABLE_CAA_DFS

template<typename OStream>
OStream& operator<<(OStream& os, Graph::edge_type const& uv) {
  //os << value(uv);
  return os;
}

TEST(TestCAAGraph, DFS) {
  vector<Graph::edge_value_type> edge_routes = to_edge_values(routes, cities);
  Graph                          g(cities, edge_routes);

  std::graph::dfs_edge_range ranges(g, 7);
  auto                       ite = ranges.begin();
  for (; ite != ranges.end(); ++ite) {
    auto u = std::get<0>(*ite);
    auto v = std::get<1>(*ite);
    auto w = std::get<2>(*ite);
    if (u)
      std::cout << "traverse " << v << " to " << w << std::endl;
    else
      std::cout << "view " << v << " to " << w << std::endl;
  }
}

#else  //!ENABLE_CAA_DFS
TEST(TestCAAGraph, DFS) {
  using namespace bgl17;
  size_t n_vtx = 15;

  edge_list<undirected> A_list(n_vtx);
  A_list.push_back(0, 1);
  A_list.push_back(1, 2);
  A_list.push_back(2, 3);
  A_list.push_back(3, 4);
  A_list.push_back(4, 5);
  A_list.push_back(0, 6);
  A_list.push_back(6, 7);
  A_list.push_back(7, 8);
  A_list.push_back(8, 9);
  A_list.push_back(0, 10);
  A_list.push_back(10, 11);
  A_list.push_back(11, 12);
  A_list.push_back(12, 13);
  A_list.push_back(13, 14);
  A_list.push_back(14, 11);

  adjacency<0> A(A_list);

  dfs_edge_range ranges(A, 7);

  auto ite = ranges.begin();
  for (; ite != ranges.end(); ++ite) {
    auto u = std::get<0>(*ite);
    auto v = std::get<1>(*ite);
    auto w = std::get<2>(*ite);
    if (u)
      std::cout << "traverse " << v << " to " << w << std::endl;
    else
      std::cout << "view " << v << " to " << w << std::endl;
  }
}
#endif //ENABLE_CAA_DFS
