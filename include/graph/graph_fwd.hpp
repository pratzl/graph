//#include <memory>
//#include <range/v3/all.hpp>
#include <concepts>
#include <range/v3/range/concepts.hpp>
#include <type_traits>

/* NOTES
	1.	edge functions default to out_edges on a directed graph. edge functions are the only functions 
		defined for undirected graphs. This allows generalized functions to apply to both undirected
		and directed by using "edge" functions.

   TODO
	1.	Extend compressed_adj_array to support:
		a.	in edges?
		d.	create_vertex & create_edge?
	2.	Add algorithms
	3.	Add reverse_graph (swap in/out edges)
	4.	Add more flexible adj_array to support 
		a.	adding/removing disjoint edges
	5.	Add adj_list
	6.	Add adj_matrix
*/

#ifndef GRAPH_FWD_HPP
#  define GRAPH_FWD_HPP

namespace std::graph {

//
// graph concepts
//
template <typename G>
concept graph_c = requires(G&& g){
	//vertices(g);
	//edges(g);
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

template <typename T>
concept arithmetic_c = is_arithmetic_v<T>;


//
// type traits
//
#  ifdef TODO
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
#  endif // TODO

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
using const_vertex_range_t = typename G::const_vertex_range;

template <graph_c G>
using vertex_iterator_t = typename G::vertex_iterator;
template <graph_c G>
using const_vertex_iterator_t = typename G::const_vertex_iterator;

template <graph_c G>
using vertex_sentinal_t = typename G::vertex_sentinal;

template <graph_c G>
using vertex_size_t = typename G::vertex_size_type;

// edge types
template <graph_c G>
using edge_t = typename G::edge_type;

template <graph_c G>
using edge_value_t = typename G::edge_user_value;

template <graph_c G>
using edge_range_t = typename G::edge_range;
template <graph_c G>
using const_edge_range_t = typename G::const_edge_range;

template <graph_c G>
using edge_iterator_t = typename G::edge_iterator;
template <graph_c G>
using const_edge_iterator_t = typename G::const_edge_iterator;

template <graph_c G>
using edge_sentinal_t = typename G::edge_sentinal;

template <graph_c G>
using edge_size_t = typename G::edge_size_type;


template <graph_c G>
using vertex_edge_range_t = typename G::vertex_edge_range;
template <graph_c G>
using const_vertex_edge_range_t = typename G::const_vertex_edge_range;

template <graph_c G>
using vertex_edge_iterator_t = typename G::vertex_edge_iterator;
template <graph_c G>
using const_vertex_edge_iterator_t = typename G::const_vertex_edge_iterator;

template <graph_c G>
using vertex_edge_sentinal_t = typename G::vertex_edge_sentinal;

template <graph_c G>
using vertex_edge_size_t = typename G::vertex_edge_size_type;


template <graph_c G>
using out_edge_range_t = typename G::out_edge_range;
template <graph_c G>
using const_out_edge_range_t = typename G::const_out_edge_range;

template <graph_c G>
using out_edge_iterator_t = typename G::out_edge_iterator;
template <graph_c G>
using const_out_edge_iterator_t = typename G::const_out_edge_iterator;

template <graph_c G>
using out_edge_sentinal_t = typename G::out_edge_sentinal;

template <graph_c G>
using out_edge_size_t = typename G::out_edge_size_type;


template <graph_c G>
using in_edge_range_t = typename G::in_edge_range;
template <graph_c G>
using const_in_edge_range_t = typename G::const_in_edge_range;

template <graph_c G>
using in_edge_iterator_t = typename G::in_edge_iterator;
template <graph_c G>
using const_in_edge_iterator_t = typename G::const_in_edge_iterator;

template <graph_c G>
using in_edge_sentinal_t = typename G::in_edge_sentinal;

template <graph_c G>
using in_edge_size_t = typename G::in_edge_size_type;


//
// common graph functions
//
template <typename T>
constexpr auto value(T& gve) noexcept -> decltype(user_value(gve)) {
  return user_value(gve);
}

//
// vertex Functions
//

template <graph_c G>
constexpr auto vertex_key(G const&, vertex_t<G> const& u) noexcept -> vertex_key_t<G>;


template <graph_c G>
constexpr auto edges(G& g, vertex_t<G>& u) noexcept -> vertex_edge_range_t<G>;
template <graph_c G>
constexpr auto edges(G const& g, vertex_t<G> const& u) noexcept -> const_vertex_edge_range_t<G>;

template <graph_c G>
constexpr auto begin(G& g, vertex_t<G>& u) noexcept -> vertex_edge_iterator_t<G>;
template <graph_c G>
constexpr auto begin(G const& g, vertex_t<G> const& u) noexcept -> const_vertex_edge_iterator_t<G>;
template <graph_c G>
constexpr auto cbegin(G const& g, vertex_t<G> const& u) noexcept -> const_vertex_edge_iterator_t<G>;

template <graph_c G>
constexpr auto end(G& g, vertex_t<G>& u) noexcept -> vertex_edge_iterator_t<G>;
template <graph_c G>
constexpr auto end(G const& g, vertex_t<G> const& u) noexcept -> const_vertex_edge_iterator_t<G>;
template <graph_c G>
constexpr auto cend(G const& g, vertex_t<G> const& u) noexcept -> const_vertex_edge_iterator_t<G>;


template <graph_c G>
constexpr auto edges_size(G& g, vertex_t<G>& u) noexcept -> vertex_edge_size_t<G>;

template <graph_c G>
constexpr auto edges_degree(G& g, vertex_t<G>& u) noexcept -> vertex_edge_size_t<G>;

template <graph_c G>
constexpr void clear_edges(G& g, vertex_t<G>& u);


template <graph_c G>
constexpr auto out_edges(G& g, vertex_t<G>& u) noexcept -> out_edge_range_t<G>;
template <graph_c G>
constexpr auto out_edges(G const& g, vertex_t<G> const& u) noexcept -> const_out_edge_range_t<G>;

template <graph_c G>
constexpr auto out_size(G const& g, vertex_t<G> const& u) noexcept -> out_edge_size_t<G>;

template <graph_c G>
constexpr auto out_degree(G const& g, vertex_t<G> const& u) noexcept -> out_edge_size_t<G>;

template <graph_c G>
constexpr void clear_out_edges(G& g, vertex_t<G>& u);


template <graph_c G>
constexpr auto in_edges(G& g, vertex_t<G>& u) noexcept -> in_edge_range_t<G>;
template <graph_c G>
constexpr auto in_edges(G const& g, vertex_t<G> const& u) noexcept -> const_in_edge_range_t<G>;

template <graph_c G>
constexpr auto in_size(G& g, vertex_t<G>& u) noexcept -> in_edge_size_t<G>;

template <graph_c G>
constexpr auto in_degree(G& g, vertex_t<G>& u) noexcept -> in_edge_size_t<G>;

template <graph_c G>
constexpr void clear_in_edges(G& g, vertex_t<G>& u);


template <graph_c G>
constexpr auto create_vertex(G& g) -> pair<vertex_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_vertex(G& g, vertex_value_t<G> const&) -> pair<vertex_iterator_t<G>, bool>;

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
constexpr auto find_vertex(G& g, vertex_key_t<G> const&) noexcept -> vertex_iterator_t<G>;
template <graph_c G>
constexpr auto find_vertex(G const& g, vertex_key_t<G> const&) noexcept -> const_vertex_iterator_t<G>;

//
// edge Functions
//
template <graph_c G>
constexpr auto vertex(G& g, edge_t<G>& uv) noexcept -> vertex_iterator_t<G>&;
template <graph_c G>
constexpr auto vertex(G const& g, edge_t<G> const& uv) noexcept -> vertex_t<G> const&;

template <graph_c G>
constexpr auto out_vertex(G& g, edge_t<G>& uv) noexcept -> vertex_iterator_t<G>&;
template <graph_c G>
constexpr auto out_vertex(G const& g, edge_t<G> const& uv) noexcept -> vertex_iterator_t<G> const&;

template <graph_c G>
constexpr auto in_vertex(G& g, edge_t<G>& uv) noexcept -> vertex_iterator_t<G>&;
template <graph_c G>
constexpr auto in_vertex(G const& g, edge_t<G> const& uv) noexcept -> vertex_iterator_t<G> const&;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> pair<vertex_edge_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&) -> pair<vertex_edge_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

template <graph_c G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

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
constexpr auto find_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) noexcept -> edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v) noexcept -> const_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) noexcept -> edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_edge(G const& g, vertex_key_t<G> const& ukey, vertex_key_t<G> const& vkey) noexcept
      -> const_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_out_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) noexcept -> out_edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_out_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v) noexcept
      -> const_out_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_out_edge(G& g, vertex_key_t<G> const& ukey, vertex_key_t<G> const& vkey) noexcept
      -> out_edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_out_edge(G const& g, vertex_key_t<G> const& ukey, vertex_key_t<G> const& vkey) noexcept
      -> const_out_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_in_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) noexcept -> in_edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_in_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v) noexcept
      -> const_in_edge_iterator_t<G>;

template <graph_c G>
constexpr auto find_in_edge(G& g, vertex_key_t<G> const& ukey, vertex_key_t<G> const& vkey) noexcept
      -> in_edge_iterator_t<G>;
template <graph_c G>
constexpr auto find_in_edge(G const& g, vertex_key_t<G> const& ukey, vertex_key_t<G> const& vkey) noexcept
      -> const_in_edge_iterator_t<G>;

//
// graph Functions
//
template <graph_c G>
constexpr auto vertices(G& g) noexcept -> vertex_range_t<G>;
template <graph_c G>
constexpr auto vertices(G const& g) noexcept -> const_vertex_range_t<G>;

template <graph_c G>
constexpr auto vertices_size(G const& g) noexcept -> vertex_size_t<G>;

template <graph_c G>
constexpr auto begin(G& g) noexcept -> vertex_iterator_t<G>;
template <graph_c G>
constexpr auto begin(G const& g) noexcept -> const_vertex_iterator_t<G>;
template <graph_c G>
constexpr auto cbegin(G const& g) noexcept -> const_vertex_iterator_t<G>;

template <graph_c G>
constexpr auto end(G& g) noexcept -> vertex_iterator_t<G>;
template <graph_c G>
constexpr auto end(G const& g) noexcept -> const_vertex_iterator_t<G>;
template <graph_c G>
constexpr auto cend(G const& g) noexcept -> const_vertex_iterator_t<G>;


template <graph_c G>
void reserve_vertices(G& g, vertex_size_t<G>) {}

template <graph_c G>
void resize_vertices(G& g, vertex_size_t<G>) {}
template <graph_c G>
void resize_vertices(G& g, vertex_size_t<G>, vertex_value_t<G> const&) {}


template <graph_c G>
constexpr auto edges(G& g) noexcept -> edge_range_t<G>;
template <graph_c G>
constexpr auto edges(G const& g) noexcept -> const_edge_range_t<G>;

template <graph_c G>
constexpr auto edges_size(G const& g) noexcept -> edge_size_t<G>;

template <graph_c G>
void reserve_edges(G& g, edge_size_t<G>) {}

template <graph_c G>
void clear(G& g);

#  ifdef TODO
template <graph_c G>
constexpr void swap(G& a, G& b);
#  endif


// for DFS, BFS & TopoSort ranges
template <typename G>
concept searchable_graph_c = requires(G&& g, vertex_iterator_t<G> u, vertex_edge_iterator_t<G> uv) {
  ::ranges::forward_range<G>;
  //::ranges::forward_range<vertex_t<G>>; // vertex begin/end require graph parameter so it doesn't apply
  ::ranges::forward_range<vertex_range_t<G>>;
  ::ranges::forward_range<vertex_edge_iterator_t<G>>;
  integral<vertex_key_t<G>>;
  { vertices(g) } ->vertex_range_t<G>;
  { edges(g, *u) } ->vertex_edge_range_t<G>;
  { vertex(g, *uv) } ->vertex_iterator_t<G>;
  { vertex_key(g,*u) } ->vertex_key_t<G>;
};



//
// Common Property Values
//
struct empty_value {}; // empty graph|vertex|edge value

struct weight_value {
  int weight = 0;

  weight_value()                    = default;
  weight_value(weight_value const&) = default;
  weight_value& operator=(weight_value const&) = default;
  weight_value(int const& w) : weight(w) {}
};

struct name_value {
  string name;

  name_value()                  = default;
  name_value(name_value const&) = default;
  name_value& operator=(name_value const&) = default;
  name_value(string const& s) : name(s) {}
  name_value(string&& s) : name(move(s)) {}
};


} // namespace std::graph

#endif // GRAPH_FWD_HPP

#include "detail/graph_fwd_impl.hpp"
