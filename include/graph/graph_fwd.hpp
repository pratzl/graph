// graph_fwd.hpp - definitions from P1709Rx
#pragma once
#include <concepts>
#include <type_traits>

namespace std::graph {
//
// graph concepts
//
template <typename G>
concept graph_c = requires(G&& g) {
  vertices(g);
  edges(g);
  //value(g);  // value is optional
};

template <typename G>
concept directed_graph_c = true;
template <typename G>
concept bidirected_graph_c = true;
template <typename G>
concept undirected_graph_c = true;

template <typename G>
concept sparse_graph_c = true;
template <typename G>
concept dense_graph_c = true; // e.g. adjacency_matrix

template <typename V>
concept vertex_c = true;
template <typename V>
concept edge_c = true;

template <typename V>
concept vertex_iterator_c = true;
template <typename V>
concept edge_iterator_c = true;

template <typename V>
concept eraseable_c = true; // items can be erased
                            // (e.g. vertices or edges)

template <typename T>
concept arithmetic_c = is_arithmetic_v<T>;

// for DFS, BFS & TopoSort iterators
template <typename SI>
concept search_iterator_c = forward_iterator<SI>&& depth(SI);


//
// type traits
//
template <graph_c G>
struct is_adjacency_list;

template <graph_c G>
inline constexpr bool is_adjacency_list_v = is_adjacency_list<G>::value;

template <graph_c G>
struct is_adjacency_array;

template <graph_c G>
inline constexpr bool is_adjacency_array_v = is_adjacency_array<G>::value;

template <graph_c G>
struct is_adjacency_matrix;

template <graph_c G>
inline constexpr bool is_adjacency_matrix_v = is_adjacency_matrix<G>::value;


//
// types
//
template <graph_c G>
using graph_value_t = typename G::graph_user_value;

// vertex types
template <graph_c G>
using vertex_t = typename G::vertex_type;

template <graph_c G>
using vertex_key_t = typename G::vertex_key_type;

template <graph_c G>
using vertex_value_t = typename G::vertex_user_value;

template <graph_c G>
using vertex_range_t = typename G::vertex_range;

template <graph_c G>
using vertex_iterator_t = typename G::vertex_iterator;

template <graph_c G>
using vertex_sentinal_t = typename G::vertex_sentinal;

template <graph_c G>
using vertex_size_t = typename G::vertex_size_t;

// edge types
template <graph_c G>
using edge_t = typename G::edge_type;

template <graph_c G>
using edge_value_t = typename G::edge_user_value;

template <graph_c G>
using edge_range_t = typename G::edge_range;

template <graph_c G>
using edge_iterator_t = typename G::edge_iterator;

template <graph_c G>
using edge_sentinal_t = typename G::edge_sentinal;

template <graph_c G>
using edge_size_t = typename G::edge_size_type;

template <graph_c G>
using out_edge_range_t = typename G::out_edge_range;

template <graph_c G>
using out_edge_iterator_t = typename G::out_edge_iterator;

template <graph_c G>
using out_edge_sentinal_t = typename G::out_edge_sentinal;

template <graph_c G>
using out_edge_size_t = typename G::out_edge_size_type;

template <graph_c G>
using in_edge_range_t = typename G::in_edge_range;

template <graph_c G>
using in_edge_iterator_t = typename G::in_edge_iterator;

template <graph_c G>
using in_edge_sentinal_t = typename G::in_edge_sentinal;

template <graph_c G>
using in_edge_size_t = typename G::in_edge_size_type;


//
// common graph functions
//
template <typename T>
constexpr auto value(T& gve) noexcept -> decltype(get_user_value(gve));

//
// vertex Functions
//

template <graph_c G>
constexpr auto vertex_key(vertex_t<G>& u) noexcept -> vertex_key_t<G>&;

template <graph_c G>
constexpr auto out_edges(G& g, vertex_t<G>& u) noexcept -> out_edge_range_t<G>;

template <graph_c G>
constexpr auto out_size(G& g, vertex_t<G>& u) noexcept -> out_edge_size_t<G>;

template <graph_c G>
constexpr auto out_degree(G& g, vertex_t<G>& u) noexcept -> out_edge_size_t<G>;

template <graph_c G>
constexpr void clear_out_edges(G& g, vertex_t<G>& u);

template <graph_c G>
constexpr auto in_edges(G& g, vertex_t<G>& u) noexcept -> in_edge_range_t<G>;

template <graph_c G>
constexpr auto in_size(G& g, vertex_t<G>& u) noexcept -> in_edge_size_t<G>;

template <graph_c G>
constexpr auto in_degree(G& g, vertex_t<G>& u) noexcept -> in_edge_size_t<G>;

template <graph_c G>
constexpr void clear_in_edges(G& g, vertex_t<G>& u);

template <graph_c G>
constexpr auto create_vertex(G& g) -> pair<vertex_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_vertex(G& g, vertex_value_t<G>&) -> pair<vertex_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_vertex(G& g, vertex_value_t<G> &&) -> pair<vertex_iterator_t<G>, bool>;

template <graph_c G>
constexpr void erase_vertices(G& g, vertex_range_t<G>&);

template <graph_c G>
constexpr void erase_vertex(G& g, vertex_iterator_t<G>&);

template <graph_c G>
constexpr void erase_vertex(G& g, vertex_key_t<G>&);

template <graph_c G>
constexpr void clear_vertex(G& g, vertex_iterator_t<G>&);

template <graph_c G>
constexpr auto find_vertex(G& g, vertex_key_t<G>&) noexcept -> vertex_iterator_t<G>;

//
// edge Functions
//
template <graph_c G>
constexpr auto out_vertex(G& g, edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto out_vertex(G& g, out_edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto out_vertex(G& g, in_edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto in_vertex(G& g, edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto in_vertex(G& g, out_edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto in_vertex(G& g, in_edge_iterator_t<G>) noexcept -> vertex_iterator_t<G>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_iterator_t<G>, vertex_iterator_t<G>);

template <graph_c G>
constexpr auto create_edge(G& g, vertex_iterator_t<G>, vertex_iterator_t<G>, edge_value_t<G>&);

template <graph_c G>
constexpr auto create_edge(G& g, vertex_iterator_t<G>, vertex_iterator_t<G>, edge_value_t<G>&&);

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&);

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G>&);

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G>&&);

template <graph_c G>
constexpr void erase_edges(G& g, edge_range_t<G>);

template <graph_c G>
constexpr void erase_edges(G& g, out_edge_range_t<G>);

template <graph_c G>
constexpr void erase_edges(G& g, in_edge_range_t<G>);

template <graph_c G>
constexpr void erase_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v);

template <graph_c G>
constexpr void erase_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey);

template <graph_c G>
constexpr void erase_edge(G& g, edge_iterator_t<G> uv);

template <graph_c G>
constexpr void erase_edge(G& g, out_edge_iterator_t<G> uv);

template <graph_c G>
constexpr void erase_edge(G& g, in_edge_iterator_t<G> uv);

template <graph_c G>
constexpr auto find_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) noexcept -> edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) noexcept -> edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_out_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) noexcept -> out_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_out_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) noexcept -> out_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_in_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) noexcept -> in_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_in_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) noexcept -> in_edge_iterator_t<G>;

//
// graph Functions
//
template <graph_c G>
constexpr auto vertices(G& g) noexcept -> vertex_range_t<G>;

template <graph_c G>
constexpr auto vertices_size(G& g) noexcept -> vertex_size_t<G>;

template <graph_c G>
constexpr auto edges(G& g) noexcept -> edge_range_t<G>;

template <graph_c G>
constexpr auto edges_size(G& g) noexcept -> edge_size_t<G>;

template <graph_c G>
constexpr void clear(G& g);

template <graph_c G>
constexpr void swap(G& a, G& b);

} // namespace std::graph
