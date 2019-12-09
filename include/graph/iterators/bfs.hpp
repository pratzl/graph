//
//	Author: J. Phillip Ratzloff
//
// breadth-first search graph algorithms for vertices and edges.
//
// The algorithms are designed to work with both directed & undirected graphs
// by using general functions such as vertex() and edges() instead of out_vertex()
// and out_edges().
//
// Assumes the graph structure remains stable during the duration of iteration.
//
// TODO
//  1.  Add designation for forward range
//  2.  Add concepts for requirements
//      a.  integer vertex_key
//      b.  vertices in contiguous space? (vertex or deque)
//      c.  max vertex key is number of vertices
//  3.  Test with const graph
//  4.  Make more compatible with Ranges
//  5.  Add range functions to create the range
//
// ISSUES / QUESTIONS
//  1.  Range holds the state, not the iterator. is there a better design?
//      a.  begin() returns current state of iteration (non-standard)
//      b.  range is potentially heavy-weight, based on the number of vertices in the graph
//          and the longest DFS path.
//      c.  the current design could be useful for multi-threading, assuming the stack &
//          visited members are guarded with locks.
//

#include "../graph_fwd.hpp"
#include <stack>
#include <vector>

#ifndef GRAPH_BFS_HPP
#  define GRAPH_BFS_HPP

namespace std::graph {



} // namespace std::graph

#endif // GRAPH_BFS_HPP
