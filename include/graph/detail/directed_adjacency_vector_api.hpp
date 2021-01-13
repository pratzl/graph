//
//	Author: J. Phillip Ratzloff
//
#include <ranges>
#include <stdexcept>

#ifndef DIRECTED_ADJ_ARRAY_API_HPP
#  define DIRECTED_ADJ_ARRAY_API_HPP

namespace std {

///-------------------------------------------------------------------------------------
/// directed_adjacency_vector graph API
///

// Helper Functions

//
// Uniform API: Graph functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void contains_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                     vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey) {
  return ukey >= 0 && ukey < g.vertices().size();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void clear(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g) {
  g.clear();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto graph_value(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> graph_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto graph_value(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> const graph_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
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
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
           const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return static_cast<vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>>(
        u - g.vertices().begin());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_value(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
             vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(*u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_value(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
             const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> const vertex_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
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
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
         const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->edge_key(g);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
         const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
         const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>(vertex_key(g, u),
                                                                                                vertex_key(g, u));
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>(ukey, vkey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edge_value(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
           edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> edge_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edge_value(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
           const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> const edge_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                         g,
       edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>         uv,
       const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> source)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
       const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>   uv,
       const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> source)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
       edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
       vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>    source_key)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
       const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
       vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>          source_key)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
           const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
           vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>     source)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
           const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
           vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>          source_key)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}

// (create_edge & erase_edge not supported because the graph is immutable)


//
// Uniform API: Graph-Vertex range functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto vertices(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return detail::make_subrange(g.vertices());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto vertices(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> const_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return detail::make_subrange(g.vertices());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertices_size(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g) noexcept
      -> vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.vertices().size();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertices_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g) noexcept
      -> vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  using ssize_t = vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>;
  return static_cast<ssize_t>(g.vertices().size());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto find_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                           vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> key)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.find_vertex(key);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto find_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                           vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> key)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.find_vertex(key);
}

// (reserve_vertices & resize_vertices not supported because the graph is immutable)

//
// Uniform API: Graph-Edge range functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edges(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.edges();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edges(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g)
      -> const_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.edges();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edges_size(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g) noexcept
      -> edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.edges().size();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edges_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>& g) noexcept
      -> edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  using ssize_t = edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>;
  return static_cast<ssize_t>(g.edges().size());
}

// (erase_edge & reserve_edges not supported because the graph is immutable)

//
// Uniform API: Vertex-Edge range functions
//
#  if 0 // edge_iterator == vertex_edge_iterator; don't define to avoid duplicate definitions
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                          const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
                          vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>            source)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                          const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv,
                          vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>                 source_key)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto edge_value(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                          vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> edge_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(*uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto edge_value(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                          const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> const edge_value_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>& {
  return user_value(*uv);
}
#  endif

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto edges(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                     vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return outward_edges(g, u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edges(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
      const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> const_vertex_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return outward_edges(g, u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edges_size(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
           const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return outward_size(g, u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
edges_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
            const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return static_cast<vertex_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>>(
        outward_size(g, u));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
          vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
          vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, u, v);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
          const vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
          const vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, u, v);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto find_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto find_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> const_vertex_outward_edge_iterator_t<
            directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

// (create_edge, erase_edge and clear_edges aren't supported because the graph is immutable)


//
// Uniform API: Vertex-Vertex range functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                          g,
           const_vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return u.outward_vertex_key();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto vertices(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                        vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.outward_vertices(u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertices(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
         const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> const_vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.outward_vertices(u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto vertices(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey)
      -> vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.find_vertex(ukey)->vertices();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto vertices(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey)
      -> const_vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.find_vertex(ukey)->vertices();
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertices_size(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
              const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return edges_size(g, u);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
vertices_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
               const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return edges_ssize(g, u);
}


//
// Directed API (inward & outward)
//

//
// Directed API (outward): Vertex functions
//
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
outward_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
               vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto outward_vertex(
      const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
      const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
outward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
                   const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}

#  if 0 // edge_iterator == vertex_edge_iterator; don't define to avoid duplicate definitions
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto outward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                                  const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->outward_vertex_key();
}
#  endif

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
inward_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
              edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
inward_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
              const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
inward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                 g,
                  const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex_key();
}

#  if 0 // edge_iterator == vertex_edge_iterator; don't define to avoid duplicate definitions
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto inward_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                             vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto inward_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                             const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
constexpr auto inward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                        g,
                                 const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return uv->inward_vertex_key();
}
#  endif

//
// Directed API (outward): Vertex-Edge functions
//

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
outward_edges(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
              vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> vertex_outward_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.outward_edges(u);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
outward_edges(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
              const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u)
      -> const_vertex_outward_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return g.outward_edges(u);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto outward_size(
      const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
      const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u) noexcept
      -> vertex_outward_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  using vertex_edge_size_type =
        vertex_edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>;
  if (&*u != &g.vertices().back())
    return (u + 1)->edge_begin_index() - u->edge_begin_index();
  else
    return static_cast<vertex_edge_size_type>(g.edges().size()) - u->edge_begin_index();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto outward_ssize(
      const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
      const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u) noexcept
      -> vertex_outward_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  using ssize_t = vertex_outward_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>>;
  return static_cast<ssize_t>(outward_size(g));
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_outward_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                  vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
                  vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  auto out_edges = outward_edges(g, u);
  for (auto uv = begin(out_edges); uv != end(out_edges); ++uv)
    if (outward_vertex(g, uv) == v)
      return uv;
  return end(out_edges);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_outward_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                  const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
                  const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> const_vertex_outward_edge_iterator_t<
            directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  auto out_edges = outward_edges(g, u);
  for (auto uv = begin(out_edges); uv != end(out_edges); ++uv)
    if (outward_vertex(g, uv) == v)
      return uv;
  return end(out_edges);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_outward_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                  vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                  vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, g.find_vertex(ukey), g.find_vertex(vkey));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_outward_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                  vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                  vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> const_vertex_outward_edge_iterator_t<
            directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, g.find_vertex(ukey), g.find_vertex(vkey));
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_vertex_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                 vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
                 vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, u, v);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_vertex_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&                   g,
                 const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> u,
                 const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> v)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, u, v);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_vertex_edge(directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&              g,
                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, ukey, vkey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr auto
find_vertex_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&        g,
                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> ukey,
                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> vkey)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>> {
  return find_outward_edge(g, ukey, vkey);
}


//
// API: common container functions
//


//
// API graph functions
//

} // namespace std

#endif // DIRECTED_ADJ_ARRAY_API_HPP