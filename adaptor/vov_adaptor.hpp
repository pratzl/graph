//#include "pch.h"
#include "vovos.hpp"

#include "graph/graph.hpp"
#include "graph/graph_utility.hpp"

#include <ranges>

#ifndef VOV_ADAPTOR_HPP
#  define VOV_ADAPTOR_HPP

// It's valid to define things in std to extend the definitions
// that already exit, as stated at
// https://en.cppreference.com/w/cpp/language/extending_std.

template <typename... Attributes>
struct std::graph_traits<vov<Attributes...>> {
  using graph_type       = vov<Attributes...>;
  using graph_value_type = std::empty_value;

  using vertex_set              = typename graph_type::outer_container;
  using vertex_outward_edge_set = typename graph_type::inner_container;

  using vertex_type       = typename vertex_set::value_type;
  using vertex_key_type   = typename vertex_set::size_type;
  using vertex_value_type = std::empty_value;

  using vertex_range       = vertex_set&;
  using const_vertex_range = const vertex_set&;
  using vertex_size_type   = typename vertex_set::size_type;

  using edge_type       = typename vertex_outward_edge_set::value_type;
  using edge_key_type   = std::pair<vertex_key_type, vertex_key_type>;
  using edge_value_type = std::tuple<Attributes...>;

  using vertex_outward_edge_range       = vertex_outward_edge_set&;
  using const_vertex_outward_edge_range = const vertex_outward_edge_set&;

  // type aliases for Uniform API
  using vertex_edge_range       = vertex_outward_edge_range;
  using const_vertex_edge_range = const_vertex_outward_edge_range;
};


// Uniform API: Common functions (accepts graph, vertex and edge)
template <typename... Attributes>
constexpr auto value(std::edge_t<vov<Attributes...>>& uv) -> std::edge_value_t<vov<Attributes...>>& {
  return uv; // edge properties
}

template <typename... Attributes>
constexpr auto vertices(vov<Attributes...>& g) -> std::vertex_range_t<vov<Attributes...>> {
  return g;
}
template <typename... Attributes>
constexpr auto vertices(const vov<Attributes...>& g) -> std::const_vertex_range_t<vov<Attributes...>> {
  return g;
}

template <typename... Attributes>
constexpr auto outward_vertex_key(const vov<Attributes...>& g, std::const_edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_key_t<vov<Attributes...>> {
  return std::get<0>(uv);
}
template <typename... Attributes>
constexpr auto outward_vertex_key(const vov<Attributes...>& g, std::const_vertex_edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_key_t<vov<Attributes...>> {
  return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>& g, std::const_edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
  //return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                        g,
                          std::const_edge_iterator_t<vov<Attributes...>>   uv,
                          std::const_vertex_iterator_t<vov<Attributes...>> source)
      -> std::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                      g,
                          std::const_edge_iterator_t<vov<Attributes...>> uv,
                          std::vertex_key_t<vov<Attributes...>> source_key) -> std::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, source_key);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                             g,
                          std::const_vertex_edge_iterator_t<vov<Attributes...>> uv,
                          std::const_vertex_iterator_t<vov<Attributes...>>      source)
      -> std::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>&                             g,
                          std::const_vertex_edge_iterator_t<vov<Attributes...>> uv,
                          std::vertex_key_t<vov<Attributes...>> source_key) -> std::vertex_key_t<vov<Attributes...>> {
  return outward_vertex_key(g, source_key);
}


template <typename... Attributes>
constexpr auto vertex_key(const vov<Attributes...>& g, std::const_vertex_iterator_t<vov<Attributes...>> u)
      -> std::vertex_key_t<vov<Attributes...>> {
  return static_cast<std::vertex_key_t<vov<Attributes...>>>(u - g.begin());
}

template <typename... Attributes>
constexpr auto outward_vertex(vov<Attributes...>& g, std::vertex_edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + static_cast<typename vov<Attributes...>::difference_type>(vertex_key(g, uv));
}

template <typename... Attributes>
constexpr auto outward_vertex(const vov<Attributes...>& g, std::const_vertex_edge_iterator_t<vov<Attributes...>> uv)
      -> std::const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>& g, std::edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(const vov<Attributes...>& g, std::const_edge_iterator_t<vov<Attributes...>> uv)
      -> std::const_vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>&                              g,
                      std::edge_iterator_t<vov<Attributes...>>         uv,
                      std::const_vertex_iterator_t<vov<Attributes...>> source)
      -> std::vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(const vov<Attributes...>&                        g,
                      std::const_edge_iterator_t<vov<Attributes...>>   uv,
                      std::const_vertex_iterator_t<vov<Attributes...>> source)
      -> std::const_vertex_iterator_t<vov<Attributes...>> {
  return outward_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto edge_key(const vov<Attributes...>& g, const std::edge_t<vov<Attributes...>>& uv)
      -> std::edge_key_t<vov<Attributes...>> {
  return std::edge_key_t<vov<Attributes...>>(inward_vertex_key(g, uv), outward_vertex_key(g, uv));
}
template <typename... Attributes>
constexpr auto edge_key(const vov<Attributes...>&                        g,
                        std::const_vertex_iterator_t<vov<Attributes...>> u,
                        std::const_vertex_iterator_t<vov<Attributes...>> v) -> std::edge_key_t<vov<Attributes...>> {
  return std::edge_key_t<vov<Attributes...>>(vertex_key(g, u), vertex_key(g, v));
}
template <typename... Attributes>
constexpr auto edge_key(const vov<Attributes...>&             g,
                        std::vertex_key_t<vov<Attributes...>> ukey,
                        std::vertex_key_t<vov<Attributes...>> vkey) -> std::edge_key_t<vov<Attributes...>> {
  return std::edge_key_t<vov<Attributes...>>(ukey, vkey);
}

template <typename... Attributes>
constexpr auto find_vertex(vov<Attributes...>& g, std::vertex_key_t<vov<Attributes...>> key)
      -> std::vertex_iterator_t<vov<Attributes...>> {
  return key < size(g) ? (g.graph().begin() + key) : g.graph.end();
}
template <typename... Attributes>
constexpr auto find_vertex(const vov<Attributes...>& g, std::vertex_key_t<vov<Attributes...>> key)
      -> std::const_vertex_iterator_t<vov<Attributes...>> {
  return key < size(g) ? (g.graph().begin() + key) : g.graph.end();
}

template <typename... Attributes>
void reserve_vertices(vov<Attributes...>& g, std::vertex_size_t<vov<Attributes...>> n) {
  g.reserve(n);
}

template <typename... Attributes>
void resize_vertices(vov<Attributes...>& g, std::vertex_size_t<vov<Attributes...>> n) {
  g.resize();
}

template <typename... Attributes>
constexpr auto edges(vov<Attributes...>& g, std::vertex_t<vov<Attributes...>>& u)
      -> std::vertex_outward_edge_range_t<vov<Attributes...>> {
  return outward_edges(g, u);
}
template <typename... Attributes>
constexpr auto edges(const vov<Attributes...>& g, const std::vertex_t<vov<Attributes...>>& u)
      -> std::const_vertex_outward_edge_range_t<vov<Attributes...>> {
  return outward_edges(g, u);
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>&                        g,
                         std::vertex_iterator_t<vov<Attributes...>> u,
                         std::vertex_iterator_t<vov<Attributes...>> v) -> std::edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, u, v);
}
template <typename... Attributes>
constexpr auto find_edge(const vov<Attributes...>&                        g,
                         std::const_vertex_iterator_t<vov<Attributes...>> u,
                         std::const_vertex_iterator_t<vov<Attributes...>> v)
      -> std::const_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, u, v);
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>&                   g,
                         std::vertex_key_t<vov<Attributes...>> ukey,
                         std::vertex_key_t<vov<Attributes...>> vkey) -> std::edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_edge(const vov<Attributes...>&             g,
                         std::vertex_key_t<vov<Attributes...>> ukey,
                         std::vertex_key_t<vov<Attributes...>> vkey) -> std::const_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

// Directed API (outward): Vertex functions
template <typename... Attributes>
constexpr auto outward_edges(vov<Attributes...>& g, std::vertex_iterator_t<vov<Attributes...>> u)
      -> std::vertex_outward_edge_range_t<vov<Attributes...>>& {
  return *u;
}
template <typename... Attributes>
constexpr auto outward_edges(const vov<Attributes...>& g, std::const_vertex_iterator_t<vov<Attributes...>> u)
      -> std::const_vertex_outward_edge_range_t<vov<Attributes...>>& {
  return *u;
}


template <typename... Attributes>
constexpr auto
find_outward_edge(vov<Attributes...>& g, std::vertex_t<vov<Attributes...>>& u, std::vertex_t<vov<Attributes...>>& v)
      -> std::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}
template <typename... Attributes>
constexpr auto find_outward_edge(const vov<Attributes...>&                g,
                                 const std::vertex_t<vov<Attributes...>>& u,
                                 const std::vertex_t<vov<Attributes...>>& v)
      -> std::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename... Attributes>
constexpr auto find_outward_edge(vov<Attributes...>&                   g,
                                 std::vertex_key_t<vov<Attributes...>> ukey,
                                 std::vertex_key_t<vov<Attributes...>> vkey)
      -> std::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_outward_edge(const vov<Attributes...>&             g,
                                 std::vertex_key_t<vov<Attributes...>> ukey,
                                 std::vertex_key_t<vov<Attributes...>> vkey)
      -> std::const_vertex_outward_edge_iterator_t<vov<Attributes...>> {
  return find_outward_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

template <typename... Attributes>
constexpr void erase_edges(vov<Attributes...>& g, std::vertex_outward_edge_range_t<vov<Attributes...>> uv_rng) {
  erase(uv_rng);
}

template <typename... Attributes>
constexpr void clear_outward_edges(vov<Attributes...>& g, std::vertex_t<vov<Attributes...>>& u) {
  u.clear();
}

// Directed API (outward): Edge functions
template <typename... Attributes>
constexpr auto erase_edge(vov<Attributes...>& g, std::vertex_outward_edge_iterator_t<vov<Attributes...>> uv)
      -> std::vertex_outward_edge_iterator_t<vov<Attributes...>> {
  using G             = vov<Attributes...>;
  std::vertex_t<G>& u = inward_vertex(g, uv);
  u.erase(uv);
}

#endif //VOV_ADAPTOR_HPP
