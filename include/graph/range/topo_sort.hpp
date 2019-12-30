//
//	Author: J. Phillip Ratzloff
//
//

#include "../graph_fwd.hpp"
#include <stack>
#include <vector>

#ifndef GRAPH_TOPO_SORT_HPP
#  define GRAPH_TOPO_SORT_HPP

namespace std::graph {

template <searchable_graph_c G, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ::ranges::contiguous_range<vertex_range_t<G>> 
class vertex_topological_sort {
    using count_t = vertex_key_t<G>;
    using visited_t = vector<count_t>;

public:
    vertex_topological_sort() = default;
  vertex_topological_sort();

public:
public:
public:
private:
  visited_t visit_count_;
  G&        graph_;
};



} // namespace std::graph

#endif // GRAPH_TOPO_SORT_HPP
