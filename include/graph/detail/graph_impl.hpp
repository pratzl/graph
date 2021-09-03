
#ifndef GRAPH_FWD_IMPL_HPP
#define GRAPH_FWD_IMPL_HPP

// This file contains default implementations that will work in most/all cases
// Individual graph structures can override if needed.

namespace std::graph {

//
// vertex Functions
//
template <typename G>
constexpr bool contains_vertex(G const& g, vertex_key_t<G> ukey) noexcept {
  return ukey >= 0 && ukey < static_cast<vertex_key_t<G>>(size(g));
}


} // namespace std::graph

#endif // GRAPH_FWD_IMPL_HPP
