//#include "pch.h"
#include "vovos.hpp"

#include "graph/graph.hpp"
#include "graph/graph_utility.hpp"

#include <range/v3/view/subrange.hpp>
#include <range/v3/algorithm/find_if.hpp>

#ifndef VOV_ADAPTOR_HPP
#  define VOV_ADAPTOR_HPP

// It's valid to define things in std/std::graph to extend the definitions
// that already exit, as stated at
// https://en.cppreference.com/w/cpp/language/extending_std.

template <typename... Attributes>
struct std::graph::graph_traits<vov<Attributes...>> {
  using graph_type                  = vov<Attributes...>;
  using const_graph_type            = const graph_type;
  using graph_user_value_type       = std::graph::empty_value;
  using const_graph_user_value_type = const graph_user_value_type;

  using vertex_set        = typename graph_type::outer_container;
  using vertex_type       = typename vertex_set::value_type;
  using const_vertex_type = const vertex_type;

  using vertex_user_value_type       = std::graph::empty_value;
  using const_vertex_user_value_type = const vertex_user_value_type;
  using vertex_size_type             = typename vertex_set::size_type;
  using vertex_ssize_type            = make_signed_t<vertex_size_type>;
  using vertex_index_type            = vertex_size_type;
  using vertex_key_type              = vertex_size_type;
  using const_vertex_key_type        = const vertex_key_type;
  using vertex_value_type            = vertex_type;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range          = decltype(::ranges::make_subrange(std::declval<vertex_set&>()));
  using const_vertex_range    = decltype(::ranges::make_subrange(std::declval<vertex_set const&>()));

  using vertex_out_edge_set = typename graph_type::inner_container;
  using edge_type           = typename vertex_out_edge_set::value_type;
  using const_edge_type     = const edge_type;
  using edge_size_type      = size_t;
  using edge_ssize_type     = ptrdiff_t;

  using edge_value_type            = edge_type;
  using const_edge_value_type      = const edge_type;
  using edge_user_value_type       = std::tuple<Attributes...>;
  using const_edge_user_value_type = const edge_user_value_type;
  using vertex_out_edge_size_type  = typename vertex_out_edge_set::size_type;
  using vertex_out_edge_ssize_type = make_signed_t<vertex_out_edge_size_type>;

  using vertex_out_edge_iterator       = typename vertex_out_edge_set::iterator;
  using const_vertex_out_edge_iterator = typename vertex_out_edge_set::const_iterator;
  using vertex_out_edge_range          = decltype(::ranges::make_subrange(std::declval<vertex_out_edge_set&>()));
  using const_vertex_out_edge_range    = decltype(::ranges::make_subrange(std::declval<vertex_out_edge_set const&>()));

  // type aliases for Uniform API
  using vertex_edge_size_type      = vertex_out_edge_size_type;
  using vertex_edge_ssize_type     = vertex_out_edge_ssize_type;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;
  using vertex_edge_range          = vertex_out_edge_range;
  using const_vertex_edge_range    = const_vertex_out_edge_range;
};


// Ideally, I'd use std::graph:: to prepend the symbols in that namespace rather than
// putting everyting in the namespace as shown, similar to what is done with the graph_traits
// above. However, I encountered compile errors when doing that and this works as expected.
namespace std::graph {

// Uniform API: Common functions (accepts graph, vertex and edge)
template <typename... Attributes>
constexpr auto value(edge_t<vov<Attributes...>>& uv) -> edge_value_t<vov<Attributes...>>& {
  return uv; // edge properties
}

template <typename... Attributes>
constexpr auto vertices(vov<Attributes...>& g) -> vertex_range_t<vov<Attributes...>> {
  return ::ranges::make_subrange(g);
}
template <typename... Attributes>
constexpr auto vertices(vov<Attributes...> const& g) -> const_vertex_range_t<vov<Attributes...>> {
  return ::ranges::make_subrange(g);
}

template <typename... Attributes>
constexpr auto vertices_size(vov<Attributes...> const& g) -> vertex_size_t<vov<Attributes...>> {
  return g.size();
}

template <typename... Attributes>
constexpr auto vertices_ssize(vov<Attributes...> const& g) -> vertex_size_t<vov<Attributes...>> {
  using ssize_t = vertex_ssize_t<vov<Attributes...>>;
  return static_cast<ssize_t>(g.size());
}

template <typename... Attributes>
constexpr auto out_vertex_key(vov<Attributes...> const& g, const_edge_t<vov<Attributes...>>& uv)
      -> vertex_key_t<vov<Attributes...>> {
  return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(vov<Attributes...> const& g, const_edge_t<vov<Attributes...>>& uv)
      -> vertex_key_t<vov<Attributes...>> {
  return out_vertex_key(g, uv);
  //return std::get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex_key(vov<Attributes...> const&           g,
                          const_edge_t<vov<Attributes...>>&   uv,
                          const_vertex_t<vov<Attributes...>>& source) -> vertex_key_t<vov<Attributes...>> {
  return out_vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(vov<Attributes...> const&         g,
                          const_edge_t<vov<Attributes...>>& uv,
                          vertex_key_t<vov<Attributes...>>  source_key) -> vertex_key_t<vov<Attributes...>> {
  return out_vertex_key(g, uv);
}


template <typename... Attributes>
constexpr auto vertex_key(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_key_t<vov<Attributes...>> {
  return static_cast<vertex_key_t<vov<Attributes...>>>(&u - g.data());
}

template <typename... Attributes>
constexpr auto out_vertex(vov<Attributes...>& g, edge_t<vov<Attributes...>>& uv)
      -> vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + static_cast<typename vov<Attributes...>::difference_type>(vertex_key(g, uv));
}

template <typename... Attributes>
constexpr auto out_vertex(vov<Attributes...> const& g, const_edge_t<vov<Attributes...>>& uv)
      -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin() + vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>& g, edge_t<vov<Attributes...>>& uv) -> vertex_iterator_t<vov<Attributes...>> {
  return out_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...> const& g, const_edge_t<vov<Attributes...>>& uv)
      -> const_vertex_iterator_t<vov<Attributes...>> {
  return out_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...>& g, edge_t<vov<Attributes...>>& uv, const_vertex_t<vov<Attributes...>>& source)
      -> vertex_iterator_t<vov<Attributes...>> {
  return out_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov<Attributes...> const&           g,
                      const_edge_t<vov<Attributes...>>&   uv,
                      const_vertex_t<vov<Attributes...>>& source) -> const_vertex_iterator_t<vov<Attributes...>> {
  return out_vertex(g, uv);
}


template <typename... Attributes>
constexpr auto begin(vov<Attributes...>& g) -> vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto begin(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto cbegin(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.cbegin();
}

template <typename... Attributes>
constexpr auto end(vov<Attributes...>& g) -> vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto end(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto cend(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}


template <typename... Attributes>
constexpr auto vertices_begin(vov<Attributes...>& g) -> vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto vertices_begin(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto vertices_cbegin(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.cbegin();
}

template <typename... Attributes>
constexpr auto vertices_end(vov<Attributes...>& g) -> vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto vertices_end(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto vertices_cend(vov<Attributes...> const& g) -> const_vertex_iterator_t<vov<Attributes...>> {
  return g.end();
}


template <typename... Attributes>
constexpr auto find_vertex(vov<Attributes...>& g, const_vertex_key_t<vov<Attributes...>> key)
      -> vertex_iterator_t<vov<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}
template <typename... Attributes>
constexpr auto find_vertex(vov<Attributes...> const& g, const_vertex_key_t<vov<Attributes...>> key)
      -> const_vertex_iterator_t<vov<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}

template <typename... Attributes>
void reserve_vertices(vov<Attributes...>& g, vertex_size_t<vov<Attributes...>> n) {
  g.reserve(n);
}

template <typename... Attributes>
void resize_vertices(vov<Attributes...>& g, vertex_size_t<vov<Attributes...>> n) {
  g.resize();
}

template <typename... Attributes>
constexpr auto edges(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_range_t<vov<Attributes...>> {
  return out_edges(g, u);
}
template <typename... Attributes>
constexpr auto edges(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_range_t<vov<Attributes...>> {
  return out_edges(g, u);
}

template <typename... Attributes>
constexpr auto edges_size(vov<Attributes...> const& g) -> edge_size_t<vov<Attributes...>> {
  edge_size_t<vov<Attributes...>> n = 0;
  for (auto& u : vertices(g))
    for (auto& uv : u)
      ++n;
  return n;
}
template <typename... Attributes>
constexpr auto edges_ssize(vov<Attributes...> const& g) -> edge_ssize_t<vov<Attributes...>> {
  edge_ssize_t<vov<Attributes...>> n = 0;
  for (auto& u : vertices(g))
    for (auto& uv : u)
      ++n;
  return n;
}

template <typename... Attributes>
constexpr auto edges_size(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_size_t<vov<Attributes...>> {
  return u.size();
}

template <typename... Attributes>
constexpr auto edges_ssize(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_ssize_t<vov<Attributes...>> {
  return static_cast<vertex_edge_ssize_t<vov<Attributes...>>>(u.size());
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u, vertex_t<vov<Attributes...>>& v)
      -> edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, u, v);
}
template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...> const&           g,
                         const_vertex_t<vov<Attributes...>>& u,
                         const_vertex_t<vov<Attributes...>>& v) -> const_edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, u, v);
}

template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...>&               g,
                         vertex_key_t<vov<Attributes...>>& ukey,
                         vertex_key_t<vov<Attributes...>>& vkey) -> edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_edge(vov<Attributes...> const&               g,
                         vertex_key_t<vov<Attributes...>> const& ukey,
                         vertex_key_t<vov<Attributes...>> const& vkey) -> const_edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

// Directed API (outgoing): Vertex functions
template <typename... Attributes>
constexpr auto out_edges(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_range_t<vov<Attributes...>> {
  return u;
}
template <typename... Attributes>
constexpr auto out_edges(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_range_t<vov<Attributes...>> {
  return u;
}

template <typename... Attributes>
constexpr auto begin(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto begin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto cbegin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto end(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto end(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto cend(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}


template <typename... Attributes>
constexpr auto edges_begin(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto edges_begin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto edges_cbegin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto edges_end(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto edges_end(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto edges_cend(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}


template <typename... Attributes>
constexpr auto out_begin(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto out_begin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto out_cbegin(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto out_end(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto out_end(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto out_cend(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return u.end();
}

template <typename... Attributes>
constexpr auto out_size(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_size_t<vov<Attributes...>> {
  return u.size();
}
template <typename... Attributes>
constexpr auto out_ssize(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_ssize_t<vov<Attributes...>> {
  return static_cast<vertex_out_edge_ssize_t<vov<Attributes...>>>(u.size());
}
template <typename... Attributes>
constexpr auto out_degree(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u)
      -> vertex_out_edge_size_t<vov<Attributes...>> {
  return u.size();
}

template <typename... Attributes>
constexpr auto find_out_edge(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u, vertex_t<vov<Attributes...>>& v)
      -> vertex_out_edge_iterator_t<vov<Attributes...>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}
template <typename... Attributes>
constexpr auto
find_out_edge(vov<Attributes...> const& g, const_vertex_t<vov<Attributes...>>& u, const_vertex_t<vov<Attributes...>>& v)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename... Attributes>
constexpr auto find_out_edge(vov<Attributes...>&                     g,
                             const_vertex_key_t<vov<Attributes...>>& ukey,
                             const_vertex_key_t<vov<Attributes...>>& vkey)
      -> vertex_out_edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_out_edge(vov<Attributes...> const&               g,
                             const_vertex_key_t<vov<Attributes...>>& ukey,
                             const_vertex_key_t<vov<Attributes...>>& vkey)
      -> const_vertex_out_edge_iterator_t<vov<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

template <typename... Attributes>
constexpr void erase_edges(vov<Attributes...>& g, vertex_out_edge_range_t<vov<Attributes...>> uv_rng) {
  erase(uv_rng);
}

template <typename... Attributes>
constexpr void clear_out_edges(vov<Attributes...>& g, vertex_t<vov<Attributes...>>& u) {
  u.clear();
}

// Directed API (outgoing): Edge functions
template <typename... Attributes>
constexpr auto erase_edge(vov<Attributes...>& g, vertex_out_edge_iterator_t<vov<Attributes...>> uv)
      -> vertex_out_edge_iterator_t<vov<Attributes...>> {
  using G        = vov<Attributes...>;
  vertex_t<G>& u = in_vertex(g, uv);
  u.erase(uv);
}

} // namespace std::graph

#endif //VOV_ADAPTOR_HPP
