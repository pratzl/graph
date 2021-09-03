#pragma once
#include "../graph.hpp"

namespace std::graph {

template <typename G>
class adjacency_range_adaptor {
  using graph_type           = typename graph_traits<G>::graph_type;
  using vertex_type          = typename graph_traits<G>::vertex_type;
  using vertex_edge_range    = typename graph_traits<G>::vertex_edge_type;
  using vertex_edge_iterator = ranges::iterator_t<vertex_edge_range>;
};

} // namespace std::graph
