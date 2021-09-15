
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


#if 0
template <typename G, typename EI, typename VI>
constexpr auto other_vertex(G&& g, EI uv, VI src) {
  return src == source(g, uv) ? target(g, uv) : source(g, uv);
}


template <typename G>
constexpr auto find_vertex_edge(G&& g, vertex_iterator_t<G> u, vertex_iterator_t<G> look_for) noexcept {
  auto src    = u;
  auto uv     = ranges::begin(edges(forward<G>(g), u));
  auto uv_end = ranges::end(edges(forward<G>(g), u));
  for (; uv != uv_end; ++uv) {
    if (other_vertex(forward<G>(g), uv, src) == look_for)
      return uv;
    src = other_vertex(forward<G>(g), uv, src); // advance to target of uv (new src)
  }
  return uv; // end
}
#endif //0


} // namespace std::graph

#endif // GRAPH_FWD_IMPL_HPP
