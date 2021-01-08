//
// Author: Phil Ratzloff
//
//
// TODO
//
// ISSUES / QUESTIONS
//

#include "../graph.hpp"
#include "../range/depth_first_search.hpp"
#include <vector>

#ifndef GRAPH_TRANSITIVE_CLOSURE_HPP
#  define GRAPH_TRANSITIVE_CLOSURE_HPP

namespace std {

template <directed G>
struct reaches {
  vertex_iterator_t<G> from;
  vertex_iterator_t<G> to;
};

//template <directed_graph G, output_iterator<reaches<vertex_iterator_t<G>>> OutIter>
//void transitive_closure(G& g, OutIter result_iter);

/// Transitive closure returns all vertices that can be reached from a source vertex, for all source
/// vertices. This algorithm specializes on a sparse graph. Complexity is same as DFS: V(VE).
// clang-format off
template <incidence_graph G, typename OutIter, typename A = allocator<char>>
  requires integral<vertex_key_t<G>> && ranges::random_access_range<vertex_range_t<G>> constexpr 
void dfs_transitive_closure(G& g, OutIter result_iter, A alloc = A())
// clang-format on
{
  using reach_t = reaches<G>;
  using dfs_t   = depth_first_search_vertex_range<G, A>;
  for (auto ui = begin(g); ui != end(g); ++ui) {
    dfs_t dfs(g, ui, alloc);
    for (typename dfs_t::iterator vi_dfs = begin(dfs); vi_dfs != end(dfs); ++vi_dfs) {
      vertex_iterator_t<G> vi = vi_dfs.operator->();
      if (vi != ui)
        *result_iter = reach_t{ui, vi};
    }
  }
}

// clang-format off
template <typename ExecutionPolicy, incidence_graph G, typename OutIter, typename A = allocator<bool>>
  requires directed<G> && integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>> constexpr 
void warshall_transitive_closure(ExecutionPolicy&& policy, G& g, OutIter result_iter, A alloc = A());
// clang-format on

/// Transitive closure returns all vertices that can be reached from a source vertex, for all source
/// vertices. This algorithm specializes on a dense graph using Warshall's algorithm.
/// Complexity is O(n^3).
//
/// ToDo: Implement parallel option
///
// clang-format off
template <incidence_graph G, typename OutIter, typename A = allocator<bool>>
  requires directed<G> && integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>> constexpr 
void warshall_transitive_closure(G& g, OutIter result_iter, A alloc = A())
// clang-format on
{
  size_t const V = vertices_size(g);

  vector<bool> reach(V * V, alloc);
  for (vertex_iterator_t<G> ui = begin(g); ui != end(g); ++ui) {
    size_t uoff = vertex_key(g, ui) * V;
    for (vertex_edge_iterator_t<G> uv = ranges::begin(edges(g, ui)); uv != ranges::end(edges(g, ui)); ++uv)
      reach[uoff + vertex_key(g, uv, ui)] = true;
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

} // namespace std

#endif //GRAPH_TRANSITIVE_CLOSURE_HPP
