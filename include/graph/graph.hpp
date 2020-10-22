//#include <memory>
//#include <range/v3/all.hpp>
#include <concepts>
//#include <range/v3/range/concepts.hpp>
#include <range/v3/view/subrange.hpp>
#include <type_traits>
#include <utility> // pair

/* 
    This header's organization
    1.  Uniform API Types
        a.  Graph types
        b.  Vertex types
        c.  Edge types
    2.  Uniform API Functions
        a.  Graph functions
        b.  Vertex functions
        c.  Edge functions
    3.  Directed Graph API (outward)
        a.  Types
        b.  Graph functions
        c.  Vertex functions
        d.  Edge functions
    4.  Directed Graph API (outward)
        a.  Types
        b.  Graph functions
        c.  Vertex functions
        d.  Edge functions
    5.  Concept definitions

   NOTES
    1.  The Uniform API is designed to work with both directed and undirected graphs. Undirected
        graphs use the types and functions as-is, whereas the undirected graph is allowed to alias
        a type/function to the outward form of the type or function. For instance, find_vertex_edge()
        would alias find_vertex_outward_edge(). Additional notes:
        a.  An edge is associated with 2 vertices, so the concept of in/out isn't eliminated entirely
            for undirected graphs. It still has an inward_vertex & outward_vertex but there is no implied
            directionality with them.
        b.  A graph can be viewed as a range of ranges, with the vertices being the outer range and
            the edges being the inner range. [Andrew Lumsdaine]
    2.  Directed graphs are assumed to have outward types/functions, Incoming types/functions are
        optional and not typically used in algorithms. Directed graphs with both inward and outward
        edges are bidirectional graphs.
    3.  The type of a key for a vertex is dependent on the underlying container for the vertices. For 
        non-mapped containers such as vector & deque, the index is used. For mapped containers such as
        map and unordered_map, the user-defined key is used. [This is is a design intent. It still 
        needs to be demonstrated to be used in a prototype.]
    4.  For a general-purpose graph, edge ranges/iterators should be defined for the graph and vertex. 
        Vertex edge iterators/ranges will only iterate through the edges for a vertex (uniform, inward 
        or outward). Graph edge iterators/ranges will iterate over all edges in the graph, independent 
        of an specific vertex. 
    5.  The common begin(g)/end(g), find(g,v) and find_if(g,p) assume the range of vertices for a graph
        (not edges).

   TODO
	1.	Extend compressed_adj_array to support:
		a.	in edges?
	2.	Add reverse_graph (swap in/out edges)
	3.	Add adj_matrix impl

*/

#ifndef GRAPH_FWD_HPP
#  define GRAPH_FWD_HPP

namespace std::graph {

namespace ranges = ::ranges; // future = std::ranges

//
// Graph Traits
//
template <typename G>
struct graph_traits;

/* For exposition
template <typename G>
struct graph_traits
{
  // graph types
  using graph_type                  = ...; // all graph types
  using graph_user_value_type       = ...; // all graph types
  using allocator_type              = ...; // all graph types

  // vertex types - only if vertices defined for graph
  using vertex_type                  = ...; // all graph types
  using vertex_key_type              = ...; // 
  using vertex_user_value_type       = ...; // all graph types
  using vertex_value_type            = ...; // all graph types

  // vertex collection types - only if graph-vertices defined
  using vertex_range                 = ...; // 
  using const_vertex_range           = ...; // 
  using vertex_size_type             = ...; // 

  // edge types 
  using edge_type                  = ...; // 
  using edge_key_type              = ...; // (e.g. pair<vertex_key_t,vertex_key_t>)
  using edge_user_value_type       = ...; // 
  using edge_value_type            = ...; // 

  // graph-edge collection types - only if graph-edges defined by graph
  using edge_range                 = ...; // only if graph-edges defined
  using const_edge_range           = ...; // only if graph-edges defined
  using edge_size_type             = ...; // all graph types

  // vertex-edge collection types - directed_graph & undirected_graph
  using vertex_edge_range          = ...; 
  using const_vertex_edge_range    = ...; 
  using vertex_edge_size_type      = ...; 

  // vertex-out-edge collection types - outward_directed_graph & bidirected_graph
  using vertex_outward_edge_range          = ...; 
  using const_vertex_outward_edge_range    = ...; 
  using vertex_outward_edge_size_type      = ...; 

  // vertex-in-edge collection types - inward_directed_graph & bidirected_graph
  using vertex_inward_edge_range          = ...; 
  using const_vertex_inward_edge_range    = ...; 
  using vertex_inward_edge_size_type      = ...; 
};
*/

//
// Uniform API Types
//

// Uniform API: Graph types
template <typename G>
using graph_value_t = typename graph_traits<G>::graph_user_value;

// Uniform API: Vertex types
template <typename G>
using vertex_t = typename graph_traits<G>::vertex_type;

template <typename G>
using vertex_key_t = typename graph_traits<G>::vertex_key_type;

template <typename G>
using vertex_value_t = typename graph_traits<G>::vertex_user_value_type;

// Uniform API: Edge Types
template <typename G>
using edge_t = typename graph_traits<G>::edge_type;

template <typename G>
using edge_key_t = typename graph_traits<G>::edge_key_type;

template <typename G>
using edge_value_t = typename graph_traits<G>::edge_user_value_type;

// Uniform API: Graph-Vertex range types
template <typename G>
using vertex_range_t = typename graph_traits<G>::vertex_range;
template <typename G>
using const_vertex_range_t = typename graph_traits<G>::const_vertex_range;

template <typename G>
using vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_range>;
template <typename G>
using const_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_range>;

template <typename G>
using vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_range>;

template <typename G>
using vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_range>;
template <typename G>
using vertex_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_range>;


// Uniform API: Graph-Edge range types
template <typename G>
using edge_range_t = typename graph_traits<G>::edge_range;
template <typename G>
using const_edge_range_t = typename graph_traits<G>::const_edge_range;

template <typename G>
using edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::edge_range>;
template <typename G>
using const_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_edge_range>;

template <typename G>
using edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::edge_range>;

template <typename G>
using edge_size_t = typename graph_traits<G>::edge_size_type;
template <typename G>
using edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::edge_range>;


// Uniform API: Vertex-Edge range types
template <typename G>
using vertex_edge_range_t = typename graph_traits<G>::vertex_edge_range;
template <typename G>
using const_vertex_edge_range_t = typename graph_traits<G>::const_vertex_edge_range;

template <typename G>
using vertex_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_edge_range>;
template <typename G>
using const_vertex_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_edge_range>;

template <typename G>
using vertex_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_edge_range>;

template <typename G>
using vertex_edge_size_t = typename graph_traits<G>::vertex_edge_size_type;
template <typename G>
using vertex_edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_edge_range>;

// Uniform API: Vertex-Vertex range types
template <typename G>
using vertex_vertex_range_t = typename graph_traits<G>::vertex_vertex_range;
template <typename G>
using const_vertex_vertex_range_t = typename graph_traits<G>::const_vertex_vertex_range;

template <typename G>
using vertex_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_vertex_range>;
template <typename G>
using const_vertex_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_vertex_range>;

template <typename G>
using vertex_vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_vertex_range>;

template <typename G>
using vertex_vertex_size_t = typename graph_traits<G>::vertex_vertex_size_type;
template <typename G>
using vertex_vertex_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_vertex_range>;


//
// Uniform API Functions
//

// Uniform API: Common functions (accepts graph, vertex and edge)
template <typename T>
constexpr auto value(T& gve) -> decltype(user_value(gve)) {
  return user_value(gve);
}

template <typename G>
void clear(G& g);
template <typename G>
void clear(G& g, graph_value_t<G>&&);
template <typename G>
void clear(G& g, graph_value_t<G> const&);

template <typename G>
constexpr void swap(G& a, G& b);


// Uniform API: Vertex functions
template <typename G>
constexpr auto vertex_key(G const&, vertex_t<G> const& u) -> vertex_key_t<G>;

template <typename G>
constexpr auto create_vertex(G& g) -> pair<vertex_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_vertex(G& g, vertex_value_t<G> const&) -> pair<vertex_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_vertex(G& g, vertex_value_t<G> &&) -> pair<vertex_iterator_t<G>, bool>;

template <typename G>
constexpr void erase_vertex(G& g, vertex_iterator_t<G>&);
template <typename G>
constexpr void erase_vertex(G& g, vertex_key_t<G>);
template <typename G>
constexpr void erase_vertices(G& g, vertex_range_t<G>&);


// Uniform API: Edge functions
template <typename G>
constexpr auto vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertex(G const& g, edge_t<G> const& uv) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertex(G& g, edge_t<G>& uv, vertex_t<G> const& source) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertex(G const& g, edge_t<G> const& uv, vertex_t<G> const& source) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertex_key(G const& g, edge_t<G> const& uv) -> vertex_key_t<G>;
template <typename G>
constexpr auto vertex_key(G const& g, edge_t<G> const& uv, vertex_key_t<G> source_key) -> vertex_key_t<G>;
template <typename G>
constexpr auto vertex_key(G const& g, edge_t<G> const& uv, vertex_t<G> const& source) -> vertex_key_t<G>;

template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>) -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;


// Uniform API: Graph-Vertex range functions
template <typename G>
constexpr auto vertices(G& g) -> vertex_range_t<G>;
template <typename G>
constexpr auto vertices(G const& g) -> const_vertex_range_t<G>;

template <typename G>
constexpr auto vertices_size(G const& g) noexcept -> vertex_size_t<G>;

template <typename G>
constexpr auto vertices_ssize(G const& g) noexcept -> vertex_ssize_t<G>;

template <typename G>
constexpr auto begin(G& g) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto begin(G const& g) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto cbegin(G const& g) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto end(G& g) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto end(G const& g) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto cend(G const& g) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertices_begin(G& g) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertices_begin(G const& g) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto vertices_cbegin(G const& g) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertices_end(G& g) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertices_end(G const& g) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto vertices_cend(G const& g) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto find_vertex(G& g, vertex_key_t<G>) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto find_vertex(G const& g, vertex_key_t<G>) -> const_vertex_iterator_t<G>;

template <typename G>
void reserve_vertices(G& g, vertex_size_t<G>) {}

template <typename G>
void resize_vertices(G& g, vertex_size_t<G>) {}


// Uniform API: Graph-Edge range functions
template <typename G>
constexpr auto edges(G& g) -> edge_range_t<G>;
template <typename G>
constexpr auto edges(G const& g) -> const_edge_range_t<G>;

template <typename G>
constexpr auto edges_begin(G& g) -> edge_iterator_t<G>;
template <typename G>
constexpr auto edges_begin(G const& g) -> const_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cbegin(G const& g) -> const_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_end(G& g) -> edge_iterator_t<G>;
template <typename G>
constexpr auto edges_end(G const& g) -> const_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cend(G const& g) -> const_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_size(G const& g) noexcept -> edge_size_t<G>;
template <typename G>
constexpr auto edges_ssize(G const& g) noexcept -> edge_ssize_t<G>;

template <typename G>
constexpr auto find_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> edge_iterator_t<G>;
template <typename G>
constexpr auto find_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v) -> const_edge_iterator_t<G>;

template <typename G>
constexpr auto find_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> edge_iterator_t<G>;
template <typename G>
constexpr auto find_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> const_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v);

template <typename G>
constexpr void erase_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey);

template <typename G>
constexpr void erase_edge(G& g, edge_iterator_t<G> uv);

template <typename G>
constexpr void erase_edges(G& g, edge_range_t<G>);

template <typename G>
void reserve_edges(G& g, edge_size_t<G>);


// Uniform API: Vertex-Edge range functions
template <typename G>
constexpr auto edges(G& g, vertex_t<G>& u) -> vertex_edge_range_t<G>;
template <typename G>
constexpr auto edges(G const& g, vertex_t<G> const& u) -> const_vertex_edge_range_t<G>;

template <typename G>
constexpr auto begin(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto begin(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto cbegin(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto end(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto end(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto cend(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_begin(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_begin(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cbegin(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_end(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_end(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cend(G const& g, vertex_t<G> const& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_size(G const& g, vertex_t<G> const& u) noexcept -> vertex_edge_size_t<G>;
template <typename G>
constexpr auto edges_ssize(G const& g, vertex_t<G> const& u) noexcept -> vertex_edge_ssize_t<G>;

template <typename G>
constexpr auto find_vertex_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto find_vertex_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v)
      -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto find_vertex_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto find_vertex_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_edge_iterator_t<G> uv) -> vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edges(G& g, vertex_edge_range_t<G> uv) -> vertex_edge_iterator_t<G>;

template <typename G>
constexpr void clear_edges(G& g, vertex_t<G>&);

// Uniform API: Vertex-Vertex range functions
template <typename G>
constexpr auto vertices(G& g, vertex_t<G>& u) -> vertex_vertex_range_t<G>;
template <typename G>
constexpr auto vertices(G const& g, vertex_t<G> const& u) -> const_vertex_vertex_range_t<G>;

template <typename G>
constexpr auto vertices(G& g, vertex_key_t<G> u) -> vertex_vertex_range_t<G>;
template <typename G>
constexpr auto vertices(G const& g, vertex_key_t<G> u) -> const_vertex_vertex_range_t<G>;

template <typename G>
constexpr auto vertices_size(G const& g, vertex_t<G> const& u) noexcept -> vertex_vertex_size_t<G>;
template <typename G>
constexpr auto vertices_size(G const& g, vertex_key_t<G> u) noexcept -> vertex_vertex_size_t<G>;

template <typename G>
constexpr auto vertices_ssize(G const& g, vertex_t<G> const& u) noexcept -> vertex_vertex_ssize_t<G>;
template <typename G>
constexpr auto vertices_ssize(G const& g, vertex_key_t<G> u) noexcept -> vertex_vertex_ssize_t<G>;


//
// Directed API (outward)
//

// Directed API (outward): Types
template <typename G>
using vertex_outward_edge_range_t = typename graph_traits<G>::vertex_outward_edge_range;
template <typename G>
using const_vertex_outward_edge_range_t = typename graph_traits<G>::const_vertex_outward_edge_range;

template <typename G>
using vertex_outward_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_outward_edge_range>;
template <typename G>
using const_vertex_outward_edge_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_outward_edge_range>;

template <typename G>
using vertex_outward_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_outward_edge_range>;

template <typename G>
using vertex_outward_edge_size_t = typename graph_traits<G>::vertex_outward_edge_size_type;
template <typename G>
using vertex_outward_edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_outward_edge_range>;


// Directed API (outward): Vertex-Edge functions
template <typename G>
constexpr auto outward_vertices(G& g, vertex_t<G>& u) -> vertex_range_t<G>;
template <typename G>
constexpr auto outward_vertices(G const& g, vertex_t<G> const& u) -> const_vertex_range_t<G>;

template <typename G>
constexpr auto outward_edges(G& g, vertex_t<G>& u) -> vertex_outward_edge_range_t<G>;
template <typename G>
constexpr auto outward_edges(G const& g, vertex_t<G> const& u) -> const_vertex_outward_edge_range_t<G>;

template <typename G>
constexpr auto outward_begin(G& g, vertex_t<G>& u) -> vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto outward_begin(G const& g, vertex_t<G> const& u) -> const_vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto outward_cbegin(G const& g, vertex_t<G> const& u) -> const_vertex_outward_edge_iterator_t<G>;

template <typename G>
constexpr auto outward_end(G& g, vertex_t<G>& u) -> vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto outward_end(G const& g, vertex_t<G> const& u) -> const_vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto outward_cend(G const& g, vertex_t<G> const& u) -> const_vertex_outward_edge_iterator_t<G>;

template <typename G>
constexpr auto outward_size(G const& g, vertex_t<G> const& u) noexcept -> vertex_outward_edge_size_t<G>;
template <typename G>
constexpr auto outward_ssize(G const& g, vertex_t<G> const& u) noexcept -> vertex_outward_edge_ssize_t<G>;

template <typename G>
constexpr auto find_outward_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto find_outward_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v)
      -> const_vertex_outward_edge_iterator_t<G>;

template <typename G>
constexpr auto find_outward_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_outward_edge_iterator_t<G>;
template <typename G>
constexpr auto find_outward_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_outward_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_outward_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edges(G& g, vertex_outward_edge_range_t<G>);

template <typename G>
constexpr void clear_outward_edges(G& g, vertex_t<G>& u);

// Directed API (outward): Edge functions
template <typename G>
constexpr auto outward_vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto outward_vertex(G const& g, edge_t<G> const& uv) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto outward_vertex_key(G const& g, edge_t<G> const& uv) -> vertex_key_t<G>;

//
// Directed API (inward)
//

// Directed API (inward): Types
template <typename G>
using vertex_inward_edge_range_t = typename graph_traits<G>::vertex_inward_edge_range;
template <typename G>
using const_vertex_inward_edge_range_t = typename graph_traits<G>::const_vertex_inward_edge_range;

template <typename G>
using vertex_inward_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_inward_edge_range>;
template <typename G>
using const_vertex_inward_edge_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_inward_edge_range>;

template <typename G>
using vertex_inward_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_inward_edge_range>;

template <typename G>
using vertex_inward_edge_size_t = typename graph_traits<G>::vertex_inward_edge_size_type;
template <typename G>
using vertex_inward_edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_inward_edge_range>;


// Directed API (inward): Vertex-Edge functions
template <typename G>
constexpr auto inward_vertices(G& g, vertex_t<G>& u) -> vertex_range_t<G>;
template <typename G>
constexpr auto inward_vertices(G const& g, vertex_t<G> const& u) -> const_vertex_range_t<G>;

template <typename G>
constexpr auto inward_edges(G& g, vertex_t<G>& u) -> vertex_inward_edge_range_t<G>;
template <typename G>
constexpr auto inward_edges(G const& g, vertex_t<G> const& u) -> const_vertex_inward_edge_range_t<G>;

template <typename G>
constexpr auto inward_begin(G& g, vertex_t<G>& u) -> vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto inward_begin(G const& g, vertex_t<G> const& u) -> const_vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto inward_cbegin(G const& g, vertex_t<G> const& u) -> const_vertex_inward_edge_iterator_t<G>;

template <typename G>
constexpr auto inward_end(G& g, vertex_t<G>& u) -> vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto inward_end(G const& g, vertex_t<G> const& u) -> const_vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto inward_cend(G const& g, vertex_t<G> const& u) -> const_vertex_inward_edge_iterator_t<G>;

template <typename G>
constexpr auto inward_size(G& g, vertex_t<G>& u) noexcept -> vertex_inward_edge_size_t<G>;
template <typename G>
constexpr auto inward_ssize(G& g, vertex_t<G>& u) noexcept -> vertex_inward_edge_ssize_t<G>;

template <typename G>
constexpr auto find_inward_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto find_inward_edge(G const& g, vertex_t<G> const& u, vertex_t<G> const& v)
      -> const_vertex_inward_edge_iterator_t<G>;

template <typename G>
constexpr auto find_inward_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_inward_edge_iterator_t<G>;
template <typename G>
constexpr auto find_inward_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_inward_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_inward_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edges(G& g, vertex_inward_edge_range_t<G>);

template <typename G>
constexpr void clear_inward_edges(G& g, vertex_t<G>& u);

// Directed API (inward): Edge functions
template <typename G>
constexpr auto inward_vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto inward_vertex(G const& g, edge_t<G> const& uv) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto inward_vertex_key(G const& g, edge_t<G> const& uv) -> vertex_key_t<G>;


//
// Concept definitions
//

// clang-format off
template <typename G>
concept uniform_graph = requires(G&& g, vertex_t<G>& u) {
  true;
  //{ edges(g) } -> edge_range_t<G>;
  //{ edges(g, u) } ->vertex_edge_range_t<G>;
};

template <typename G>
concept outward_directed_graph = requires(G&& g, vertex_t<G>& u) {
  true;
  //{ outward_edges<G>(g, u) } -> vertex_outward_edge_range_t<G>;
};

template <typename G>
concept inward_directed_graph = requires(G&& g, vertex_t<G>& u) {
  true;
  //{ inward_edges(g, u) } -> vertex_inward_edge_range_t<G>;
};

template <typename G>
concept directed_graph = uniform_graph<G> && outward_directed_graph<G>;

template <typename G>
concept bidirected_graph = uniform_graph<G> && outward_directed_graph<G>&& inward_directed_graph<G>;

template <typename G>
concept undirected_graph = uniform_graph<G> && !outward_directed_graph<G> && !inward_directed_graph<G>;

template <typename V>
concept graph_vertex = true;
template <typename V>
concept graph_edge = true;

template <typename T>
concept arithmetic = is_arithmetic_v<T>;

// Requirements for extracting vertex values from external sources for graph construction
template <typename VRng, typename VValueFnc>
concept vertex_range_extractor 
        = ranges::input_range<VRng>
       && invocable<VValueFnc, typename VRng::value_type>;

// Requirements for extracting edge values from external sources for graph construction
// ERng is a forward_range because it is traversed twice; once to get the max vertex_key 
// and a second time to load the edges.
template <typename ERng, typename EKeyFnc, typename EValueFnc>
concept edge_range_extractor 
        = ranges::forward_range<ERng>
       && invocable<EKeyFnc, typename ERng::value_type>
       && invocable<EValueFnc, typename ERng::value_type>;

// for DFS, BFS & TopoSort ranges
template <typename G>
concept searchable_graph = requires(G&& g, vertex_iterator_t<G>& u, vertex_edge_iterator_t<G>& uv) {
#if 1
      true;
#else
  ranges::template input_range<G>;
  ranges::template input_iterator<vertex_iterator_t<G>>;
  ranges::template input_iterator<vertex_edge_iterator_t<G>>;
  //ranges::forward_range<vertex_t<G>>; // vertex begin/end require graph parameter so it doesn't apply
  { vertices(g) } -> convertible_to<vertex_range_t<G>>;
  { begin(g, *u) } -> convertible_to<vertex_edge_iterator_t<G>>;
  { end(g, *u) } -> convertible_to<vertex_edge_iterator_t<G>>;
  { vertex(g, *uv, *u) } -> convertible_to<vertex_iterator_t<G>>;
  { vertex_key(g, *u) } -> convertible_to<vertex_key_t<G>>;
#endif
};

// clang-format on

} // namespace std::graph

#endif // GRAPH_FWD_HPP

#include "detail/graph_impl.hpp"
