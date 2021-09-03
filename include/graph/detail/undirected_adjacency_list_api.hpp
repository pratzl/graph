//
//	Author: J. Phillip Ratzloff
//

#ifndef UNDIRECTED_ADJ_LIST_API_HPP
#define UNDIRECTED_ADJ_LIST_API_HPP

#include <stdexcept>

namespace std::graph {

#ifdef CPO
///-------------------------------------------------------------------------------------
/// undirected_adjacency_list graph API
///

// Helper functions

//
// Uniform API: Common functions (accepts graph, vertex and edge)
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto graph_value(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g)
      -> const graph_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(g);
}

//
// API vertex functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return static_cast<vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>>(
        u - g.vertices().begin());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex_value(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                            vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex_value(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                            const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> const vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*u);
}

// (clear, create_vertex, erase_vertex not supported because the graph is immutable)

//
// Uniform API: Edge functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&         g,
                        const edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& uv)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv.edge_key(g);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> v)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>(vertex_key(g, u), vertex_key(g, v));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edge_value(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                          edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edge_value(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                         g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>         uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>   uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>    source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>          source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
           const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>   uv,
           const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex_key(g, source_key);
}

// (create_edge & erase_edge)

//
// Uniform API: Graph-Vertex range functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g)
      -> vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.vertices();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g)
      -> const_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.vertices();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&              g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.find_vertex(key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&        g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.find_vertex(key);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void reserve_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&               g,
                      vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> n) {
  g.reserve_vertices(n);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&               g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> n) {
  g.resize(n);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                       g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>         n,
                     const vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& val) {
  g.resize(n, val);
}


//
// Uniform API: Graph-Edge range functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g)
      -> edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.edges();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g)
      -> const_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return g.edges();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> v)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> v)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&              g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> vkey)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&        g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> vkey)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}


//
// Uniform API: Vertex-Edge range functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                         g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>  uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>           source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source_key);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
           const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
           const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>      source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
           const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv,
           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>                 source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->other_vertex_key(g, source_key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edge_value(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
edge_value(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
           const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const edge_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                     vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return u->edges(g, vertex_key(g, u));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                     const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> const_vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return u->edges(g, vertex_key(g, u));
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> v)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                    g,
                 const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& u,
                 const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>& v)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&              g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> vkey)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&        g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> vkey)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto erase_edge(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>* uv_ptr = &*uv;
  ++uv;
  delete uv_ptr;
  return uv;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto erase_edges(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                     g,
                           vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv_rng)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {

  vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv = ranges::begin(uv_rng);
  while (uv != ranges::end(uv_rng))
    uv = erase_edge(g, uv);

  return uv;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr void clear_edges(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                           vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u) {
  u->clear_edges(g);
}


//
// Uniform API: Vertex-Vertex range functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                          g,
           const_vertex_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return u.other_vertex_key();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                        vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return u->vertices(g, vertex_key(g, u));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> u)
      -> const_vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return u->vertices(g, vertex_key(g, u));
}


//
// API edge functions
//

//
// Directed API (inward & outward)
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto target_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                             edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto target_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                             const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto target_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                             vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
target_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
              const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
target_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                  const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
target_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                  const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->target_vertex_key(g);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto source_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                             edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto source_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                             const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto source_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                             vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
source_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
              const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
source_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                 g,
                  const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr auto
source_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&                        g,
                  const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> {
  return uv->source_vertex_key(g);
}


//
// Directed API (outward): Vertex-Edge functions
//

// todo: outward_vertices


#  if 0
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g, vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->target_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                      const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->target_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                          const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->target_vertex_key(g);
}
#  endif


//
// API graph functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void contains_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>&              g,
                     vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>> ukey) {
  return ukey >= 0 && ukey < g.vertices().size();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void clear(undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>& g) {
  g.clear();
}


//
// API: common container functions
//

#endif // CPO

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_API_HPP
