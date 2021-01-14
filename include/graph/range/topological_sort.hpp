//
//	Author: J. Phillip Ratzloff
//
//

#include "../graph.hpp"
#include <stack>
#include <vector>

#ifndef GRAPH_TOPO_SORT_HPP
#  define GRAPH_TOPO_SORT_HPP

namespace std {

// clang-format off
template <incidence_graph G, typename A = allocator<char>>
  requires integral<vertex_key_t<G>>
class topological_sort_vertex_range
// clang-format on
{
  using count_t   = vertex_key_t<G>;
  using visited_t = vector<count_t>;

public:
  topological_sort_vertex_range() = default;
  topological_sort_vertex_range();

public:
public:
public:
private:
  visited_t visit_count_;
  G&        graph_;
};


} // namespace std

#endif // GRAPH_TOPO_SORT_HPP
