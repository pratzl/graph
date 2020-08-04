//
// Author: Phil Ratzloff
//
//
// TODO
//
// ISSUES / QUESTIONS
//

#include "../graph.hpp"
#include "../range/dfs.hpp"
#include <vector>

#ifndef GRAPH_TRANSITIVE_CLOSURE_HPP
#  define GRAPH_TRANSITIVE_CLOSURE_HPP

namespace std::graph {

template <directed_graph G>
struct reaches {
  vertex_iterator_t<G> from;
  vertex_iterator_t<G> to;
};

//template <directed_graph G, output_iterator<reaches<vertex_iterator_t<G>>> OutIter>
//void transitive_closure(G& g, OutIter result_iter);

/// Transitive closure returns all vertices that can be reached from a source vertex, for all source
/// vertices. This algorithm specializes on a sparse graph. Complexity is same as DFS: V(VE).
template <directed_graph G, typename OutIter, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ::ranges::random_access_range<vertex_range_t<G>> constexpr void
dfs_transitive_closure(G& g, OutIter result_iter, A alloc = A()) {
  using reach_t = reaches<G>;
  auto first    = begin(g);
  for (auto ui = first; ui != end(g); ++ui) {
    for (auto& v : dfs_vertex_range(g, ui, alloc)) {
      vertex_iterator_t<G> vi =
            first +
            vertex_key(
                  g,
                  v); // dfs should return a vertex_iterator_t? we can't be guaranteed of vertex& --> iterator conversion for all graphs
      if (vi != ui)
        *result_iter = reach_t{ui, vi};
    }
  }
}

template <typename ExecutionPolicy, directed_graph G, typename OutIter, typename A = allocator<bool>>
requires integral<vertex_key_t<G>>&& ::ranges::random_access_range<vertex_range_t<G>> constexpr void
warshall_transitive_closure(ExecutionPolicy&& policy, G& g, OutIter result_iter, A alloc = A());

/// Transitive closure returns all vertices that can be reached from a source vertex, for all source
/// vertices. This algorithm specializes on a dense graph using Warshall's algorithm.
/// Complexity is O(n^3).
//
/// ToDo: Implement parallel option
///
template <directed_graph G, typename OutIter, typename A = allocator<bool>>
requires integral<vertex_key_t<G>>&& ::ranges::random_access_range<vertex_range_t<G>> constexpr void
warshall_transitive_closure(G& g, OutIter result_iter, A alloc = A()) {
  size_t const V = vertices_size(g);

  vector<bool> reach(V * V, alloc);
  for (vertex_t<G>& u : g) {
    size_t uoff = vertex_key(g, u) * V;
    for (auto& uv : edges(g, u))
      reach[uoff + vertex_key(g, uv, u)] = true;
  }

  for (vertex_key_t<G> k = 0; k < V; ++k) {
    size_t koff = k * V;
    for (vertex_key_t<G> ukey = 0; ukey < V; ++ukey) {
      size_t uoff = ukey * V;
      for (vertex_key_t<G> vkey = 0; vkey < V; ++vkey) {
        reach[uoff + vkey] = reach[uoff + vkey] || (reach[uoff + k] && reach[koff + vkey]);
      }
    }
  }

  for (vertex_key_t<G> ukey = 0; ukey < V; ++ukey) {
    size_t uoff = ukey * V;
    for (vertex_key_t<G> vkey = 0; vkey < V; ++vkey)
      if (reach[uoff + vkey])
        *result_iter = {begin(g) + ukey, begin(g) + vkey};
  }
}

} // namespace std::graph

#endif //GRAPH_TRANSITIVE_CLOSURE_HPP
