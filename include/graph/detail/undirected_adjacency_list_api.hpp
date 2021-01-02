//
//	Author: J. Phillip Ratzloff
//

#ifndef UNDIRECTED_ADJ_LIST_API_HPP
#define UNDIRECTED_ADJ_LIST_API_HPP

#include <stdexcept>

namespace std {
///-------------------------------------------------------------------------------------
/// undirected_adjacency_list graph API
///

// Helper functions

//
// Uniform API: Common functions (accepts graph, vertex and edge)
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto graph_value(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const graph_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(g);
}

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return static_cast<vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>>(u - g.vertices().begin());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_value(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                            vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_value(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                            const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*u);
}

// (clear, create_vertex, erase_vertex not supported because the graph is immutable)

//
// Uniform API: Edge functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&         g,
                        const edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& uv)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv.edge_key(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>(vertex_key(g, u), vertex_key(g, v));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_value(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                          edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_value(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                         g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>         uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>   uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>    source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>          source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>   uv,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>          source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source_key);
}

// (create_edge & erase_edge)

//
// Uniform API: Graph-Vertex range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.vertices().size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void reserve_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&               g,
                      vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> n) {
  g.reserve_vertices(n);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&               g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> n) {
  g.resize(n);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                       g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>         n,
                     const vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& val) {
  g.resize(n, val);
}


//
// Uniform API: Graph-Edge range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges_size();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.edges().size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}


//
// Uniform API: Vertex-Edge range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                         g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>  uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>      source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>           source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>                 source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>      source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>                 source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_value(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_value(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& {
  return user_value(*uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                     vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges(g, vertex_key(g, u));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                     const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges(g, vertex_key(g, u));
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_size();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u) noexcept
      -> vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(u->edges_size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                    g,
                                const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& u,
                                const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& v)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto erase_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>* uv_ptr = &*uv;
  ++uv;
  delete uv_ptr;
  return uv;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto erase_edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                     g,
                           vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv_rng)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {

  vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv = ranges::begin(uv_rng);
  while (uv != ranges::end(uv_rng))
    uv = erase_edge(g, uv);

  return uv;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr void clear_edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u) {
  u->clear_edges(g);
}


//
// Uniform API: Vertex-Vertex range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                          g,
                          const_vertex_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u.other_vertex_key();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->vertices(g, vertex_key(g, u));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->vertices(g, vertex_key(g, u));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                             const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edges_size(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                              const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edges_ssize(g, u);
}


//
// API edge functions
//

//
// Directed API (inward & outward)
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                              edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                              const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                              vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                              const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                                  const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                                  const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex_key(g);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                             edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                             const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                             vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                             const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                                 const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                                 const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex_key(g);
}


//
// Directed API (outward): Vertex-Edge functions
//

// todo: outward_vertices


#if 0
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g, vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                      const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                          const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex_key(g);
}
#endif


//
// API graph functions
//


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void clear(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) {
  g.clear();
}


//
// API: common container functions
//

} // namespace std

#endif // UNDIRECTED_ADJ_LIST_API_HPP
