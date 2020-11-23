//#include "pch.h"
#include "vovos.hpp"

#include "graph/graph.hpp"
#include "graph/graph_utility.hpp"

#include <ranges>

#ifndef VOV_ADAPTOR_HPP
#  define VOV_ADAPTOR_HPP

//namespace ranges = ::ranges;

// It's valid to define things in std/std::graph to extend the definitions
// that already exit, as stated at
// https://en.cppreference.com/w/cpp/language/extending_std.

template <typename... Attributes>
struct std::graph::graph_traits<vov<Attributes...>> {
  using graph_type            = vov<Attributes...>;
  using graph_user_value_type = std::graph::empty_value;

  using vertex_set              = typename graph_type::outer_container;
  using vertex_outward_edge_set = typename graph_type::inner_container;

  using vertex_type            = typename vertex_set::value_type;
  using vertex_key_type        = typename vertex_set::size_type;
  using vertex_user_value_type = std::graph::empty_value;
  using vertex_value_type      = vertex_type;

  using vertex_range       = decltype(make_subrange2(std::declval<vertex_set&>()));
  using const_vertex_range = decltype(make_subrange2(std::declval<const vertex_set&>()));
  using vertex_size_type   = typename vertex_set::size_type;

  using edge_type            = typename vertex_outward_edge_set::value_type;
  using edge_key_type        = std::pair<vertex_key_type, vertex_key_type>;
  using edge_user_value_type = std::tuple<Attributes...>;
  using edge_value_type      = edge_type;
  using edge_size_type       = size_t;

  using vertex_outward_edge_range       = decltype(make_subrange2(std::declval<vertex_outward_edge_set&>()));
  using const_vertex_outward_edge_range = decltype(make_subrange2(std::declval<const vertex_outward_edge_set&>()));
  using vertex_outward_size_type        = typename vertex_outward_edge_set::size_type;

  // type aliases for Uniform API
  using vertex_edge_range       = vertex_outward_edge_range;
  using const_vertex_edge_range = const_vertex_outward_edge_range;
  using vertex_edge_size_type   = vertex_outward_size_type;
};


// Ideally, I'd use std::graph:: to prepend the symbols in that namespace rather than
// putting everyting in the namespace as shown, similar to what is done with the graph_traits
// above. However, I encountered compile errors when doing that and this works as expected.

template <typename... Attributes>
constexpr auto size(const vov<Attributes...>& g) noexcept -> std::graph::vertex_size_t<vov<Attributes...>> {
  return vertices_size(g);
}

template <typename... Attributes>
constexpr auto vertices_ssize(const vov<Attributes...>& g) noexcept -> std::graph::vertex_ssize_t<vov<Attributes...>> {
  return vertices_ssize(g);
}

template <typename... Attributes>
constexpr auto begin(vov<Attributes...>& g) -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto begin(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto cbegin(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.cbegin();
}

template <typename... Attributes>
constexpr auto end(vov<Attributes...>& g) -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto end(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto cend(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}


// Uniform API: Common functions (accepts graph, vertex and edge)
template <typename... Attributes>
constexpr auto value(std::graph::edge_t<vov<Attributes...>>& uv) -> std::graph::edge_value_t<vov<Attributes...>>& {
  return uv; // edge properties
}

template <typename... Attributes>
constexpr auto vertices(vov<Attributes...>& g) -> std::graph::vertex_range_t<vov<Attributes...>> {
  return make_subrange2(g);
}
template <typename... Attributes>
constexpr auto vertices(const vov<Attributes...>& g) -> std::graph::const_vertex_range_t<vov<Attributes...>> {
  return make_subrange2(g);
}

template <typename... Attributes>
constexpr auto vertices_size(const vov<Attributes...>& g) noexcept -> std::graph::vertex_size_t<vov<Attributes...>> {
  return g.size();
}

template <typename... Attributes>
constexpr auto vertices_ssize(const vov<Attributes...>& g) noexcept -> std::graph::vertex_size_t<vov<Attributes...>> {
  using ssize_t = std::graph::vertex_ssize_t<vov<Attributes...>>;
  return static_cast<ssize_t>(g.size());
}

template <typename... Attributes>
constexpr auto outward_vertex_key(const vov<Attributes...>& g, const std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::vertex_key_t<vov<Attributes...>> {
  return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>& g, const std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
  //return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                       g,
                          const std::graph::edge_t<vov<Attributes...>>&   uv,
                          const std::graph::vertex_t<vov<Attributes...>>& source)
      -> std::graph::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                     g,
                          const std::graph::edge_t<vov<Attributes...>>& uv,
                          std::graph::vertex_key_t<vov<Attributes...>>  source_key)
      -> std::graph::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
}


template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_key_t<vov<Attributes...>> {
  return static_cast<std::graph::vertex_key_t<vov<Attributes...>>>(&u - g.data());
}

template <typename... Attributes>
constexpr auto outward_vertex(vov<Attributes...>& g, std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + static_cast<typename vov<Attributes...>::difference_type>(vertex_key(g, uv));
}

template <typename... Attributes>
constexpr auto outward_vertex(const vov<Attributes...>& g, const std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>& g, std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(const vov<Attributes...>& g, const std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>&                             g,
                      std::graph::edge_t<vov<Attributes...>>&         uv,
                      const std::graph::vertex_t<vov<Attributes...>>& source)
      -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(const vov<Attributes...>&                       g,
                      const std::graph::edge_t<vov<Attributes...>>&   uv,
                      const std::graph::vertex_t<vov<Attributes...>>& source)
      -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto edge_key(const vov<Attributes...>& g, const std::graph::edge_t<vov<Attributes...>>& uv)
      -> std::graph::edge_key_t<vov<Attributes...>> {
  return std::graph::edge_key_t<vov<Attributes...>>(inward_vertex_key(g, uv), outward_vertex_key(g, uv));
}
template <typename... Attributes>
constexpr auto edge_key(const vov<Attributes...>&                       g,
                        const std::graph::vertex_t<vov<Attributes...>>& u,
                        const std::graph::vertex_t<vov<Attributes...>>& v)
      -> std::graph::edge_key_t<vov<Attributes...>> {
  return std::graph::edge_key_t<vov<Attributes...>>(vertex_key(g, u), vertex_key(g, v));
}


template <typename... Attributes>
constexpr auto vertices_begin(vov<Attributes...>& g) -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto vertices_begin(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto vertices_cbegin(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.cbegin();
}

template <typename... Attributes>
constexpr auto vertices_end(vov<Attributes...>& g) -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto vertices_end(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto vertices_cend(const vov<Attributes...>& g) -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}


template <typename... Attributes>
constexpr auto find_vertex(vov<Attributes...>& g, std::graph::vertex_key_t<vov<Attributes...>> key)
      -> std::graph::vertex_iterator_t<vov<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}
template <typename... Attributes>
constexpr auto find_vertex(const vov<Attributes...>& g, std::graph::vertex_key_t<vov<Attributes...>> key)
      -> std::graph::const_vertex_iterator_t<vov<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}

template <typename... Attributes>
void reserve_vertices(vov<Attributes...>& g, std::graph::vertex_size_t<vov<Attributes...>> n) {
  g.reserve(n);
}

template <typename... Attributes>
void resize_vertices(vov<Attributes...>& g, std::graph::vertex_size_t<vov<Attributes...>> n) {
  g.resize();
}

template <typename... Attributes>
constexpr auto edges(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_outward_edge_range_t<vov<Attributes...>> {
  return outward_edges(g, u);
}
template <typename... Attributes>
constexpr auto edges(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_range_t<vov<Attributes...>> {
  return outward_edges(g, u);
}

template <typename... Attributes>
constexpr auto edges_size(const vov<Attributes...>& g) noexcept -> std::graph::edge_size_t<vov<Attributes...>> {
  std::graph::edge_size_t<vov<Attributes...>> n = 0;
  for (auto& u : vertices(g))
    for (auto& uv : u)
      ++n;
  return n;
}
template <typename... Attributes>
constexpr auto edges_ssize(const vov<Attributes...>& g) noexcept -> std::graph::edge_ssize_t<vov<Attributes...>> {
  std::graph::edge_ssize_t<vov<Attributes...>> n = 0;
  for (auto& u : vertices(g))
    for (auto& uv : u)
      ++n;
  return n;
}

template <typename... Attributes>
constexpr auto edges_size(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u) noexcept
      -> std::graph::vertex_edge_size_t<vov<Attributes...>> {
  return u.size();
}

template <typename... Attributes>
constexpr auto edges_ssize(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u) noexcept
      -> std::graph::vertex_edge_ssize_t<vov<Attributes...>> {
  return static_cast<std::graph::vertex_edge_ssize_t<vov<Attributes...>>>(u.size());
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>&                       g,
                         std::graph::vertex_t<vov<Attributes...>>& u,
                         std::graph::vertex_t<vov<Attributes...>>& v)
      -> std::graph::edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, u, v);
}
template <typename... Attributes>
constexpr auto find_edge(const vov<Attributes...>&                       g,
                         const std::graph::vertex_t<vov<Attributes...>>& u,
                         const std::graph::vertex_t<vov<Attributes...>>& v)
      -> std::graph::const_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, u, v);
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>&                          g,
                         std::graph::vertex_key_t<vov<Attributes...>> ukey,
                         std::graph::vertex_key_t<vov<Attributes...>> vkey)
      -> std::graph::edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_edge(const vov<Attributes...>&                    g,
                         std::graph::vertex_key_t<vov<Attributes...>> ukey,
                         std::graph::vertex_key_t<vov<Attributes...>> vkey)
      -> std::graph::const_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

// Directed API (outward): Vertex functions
template <typename... Attributes>
constexpr auto outward_edges(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_outward_edge_range_t<vov<Attributes...>> {
  return u;
}
template <typename... Attributes>
constexpr auto outward_edges(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_range_t<vov<Attributes...>> {
  return u;
}

template <typename... Attributes>
constexpr auto begin(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto begin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto cbegin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto end(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto end(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto cend(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}


template <typename... Attributes>
constexpr auto edges_begin(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto edges_begin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto edges_cbegin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto edges_end(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto edges_end(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto edges_cend(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}


template <typename... Attributes>
constexpr auto outward_begin(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto outward_begin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto outward_cbegin(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto outward_end(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto outward_end(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto outward_cend(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}

template <typename... Attributes>
constexpr auto outward_size(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u) noexcept
      -> std::graph::vertex_outward_size_t<vov<Attributes...>> {
  return u.size();
}
template <typename... Attributes>
constexpr auto outward_ssize(const vov<Attributes...>& g, const std::graph::vertex_t<vov<Attributes...>>& u) noexcept
      -> std::graph::vertex_outward_edge_ssize_t<vov<Attributes...>> {
  return static_cast<std::graph::vertex_outward_edge_ssize_t<vov<Attributes...>>>(u.size());
}

template <typename... Attributes>
constexpr auto find_outward_edge(vov<Attributes...>&                       g,
                                 std::graph::vertex_t<vov<Attributes...>>& u,
                                 std::graph::vertex_t<vov<Attributes...>>& v)
      -> std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}
template <typename... Attributes>
constexpr auto find_outward_edge(const vov<Attributes...>&                       g,
                                 const std::graph::vertex_t<vov<Attributes...>>& u,
                                 const std::graph::vertex_t<vov<Attributes...>>& v)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename... Attributes>
constexpr auto find_outward_edge(vov<Attributes...>&                          g,
                                 std::graph::vertex_key_t<vov<Attributes...>> ukey,
                                 std::graph::vertex_key_t<vov<Attributes...>> vkey)
      -> std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_outward_edge(const vov<Attributes...>&                    g,
                                 std::graph::vertex_key_t<vov<Attributes...>> ukey,
                                 std::graph::vertex_key_t<vov<Attributes...>> vkey)
      -> std::graph::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

template <typename... Attributes>
constexpr void erase_edges(vov<Attributes...>& g, std::graph::vertex_outward_edge_range_t<vov<Attributes...>> uv_rng) {
  erase(uv_rng);
}

template <typename... Attributes>
constexpr void clear_outward_edges(vov<Attributes...>& g, std::graph::vertex_t<vov<Attributes...>>& u) {
  u.clear();
}

// Directed API (outward): Edge functions
template <typename... Attributes>
constexpr auto erase_edge(vov<Attributes...>& g, std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> uv)
      -> std::graph::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  using G                    = vov<Attributes...>;
  std::graph::vertex_t<G>& u = inward_vertex(g, uv);
  u.erase(uv);
}

#endif //VOV_ADAPTOR_HPP
