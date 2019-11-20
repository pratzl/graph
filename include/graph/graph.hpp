// graph.hpp - more concrete defintions for graph data structures in the std
#pragma once
#include "graph_utility.hpp"

namespace std::graph {

template <typename ADJ, typename VV, typename EV>
class out_edge_set {};
template <typename ADJ, typename VV, typename EV>
class in_edge_set {};

template <typename ADJ, typename VV, typename EV>
class out_vertex_link {};
template <typename ADJ, typename VV, typename EV>
class in_vertex_link {};


//
// graph
//
/*template <typename ADJ, class GV, class VV, class EV, class A>
class graph {
public: // types
	using graph_user_value = GV;
};*/

} // namespace std::graph
