#include "pch.h"
#include "graph/compressed_adj_array.hpp"
#include "graph/range/dfs.hpp"
#include "graph/range/bfs.hpp"
#include "test_data.hpp"

using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::cout;
using std::endl;
using std::is_same;

using namespace std::graph; // Bring graph functions into global namespace

#if 0
struct dbl_weight_value {
  double weight = 0.0;

  dbl_weight_value()                        = default;
  dbl_weight_value(dbl_weight_value const&) = default;
  dbl_weight_value& operator=(dbl_weight_value const&) = default;
  dbl_weight_value(double const& w) : weight(w) {}
};


using DollarGraph = std::graph::compressed_adjacency_array<name_value, dbl_weight_value>;
using data::dollar_edges;
using data::dollar_vertices;

vector<DollarGraph::edge_value_type> ual_dollar_edges = to_data_edge_values<DollarGraph>(dollar_edges, dollar_vertices);

DollarGraph create_dollar_ual_graph() {
  return DollarGraph(
        ual_dollar_edges, dollar_vertices, [](DollarGraph::edge_value_type const& uv) { return uv.first; },
        [](DollarGraph::edge_value_type const& er) { return er.second; },
        [](string const& label) -> string const& { return label; });
}



TEST(TestCycleDirected, DollarStructure) {
  DollarGraph g = create_dollar_graph();
  EXPECT_EQ(dollar_vertices.size(), vertices_size(g));
  EXPECT_EQ(dollar_edges.size(), edges_size(g));
}
#endif
