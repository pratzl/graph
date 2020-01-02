#include "pch.h"
#include "graph/undirected_adj_list.hpp"
//#include "graph/range/dfs.hpp"
//#include "graph/range/bfs.hpp"
//#include "graph/algorithm/shortest_paths.hpp"
#include "data_routes.hpp"

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

#if 0
using namespace std::graph; // Bring graph functions into global namespace
using Graph      = std::graph::undirected_adjacency_list<name_value, weight_value>;
using vtx_iter_t = std::graph::vertex_iterator_t<Graph>;
using vtx_key_t  = std::graph::vertex_key_t<Graph>;

struct route;
using Routes = routes_t;

vector<Graph::edge_value_type> ual_germany_edge_routes =
      to_edge_values<Graph>(germany_routes, germany_cities);

vertex_iterator_t<Graph> find_city(Graph& g, string_view const city_name) {
  return ::ranges::find_if(
        g, [&city_name](vertex_t<Graph>& u) { return u.name == city_name; });
}

Graph create_germany_routes_graph() {
  return Graph(
        ual_germany_edge_routes, germany_cities,
        [](Graph::edge_value_type const& er) { return er.first; },
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
  EXPECT_EQ(4, sizeof(G::vertex_type)); // vertex size = 4 bytes
  EXPECT_EQ(8, sizeof(G::edge_type));   // edge size = 8 bytes
}

TEST(TestUALGraph, TestEmptyGraph) {
  Graph g;
  EXPECT_EQ(0, vertices_size(g));
  EXPECT_EQ(0, edges_size(g));
  //EXPECT_EQ(sizeof(Graph::edge_size_type) + sizeof(name_value), sizeof(Graph::vertex_type));
  //EXPECT_EQ(sizeof(Graph::vertex_size_type) + sizeof(weight_value), sizeof(Graph::edge_type));
}
#endif
