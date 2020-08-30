//#include <memory>
//#include <range/v3/all.hpp>
#include <concepts>
//#include <range/v3/range/concepts.hpp>
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
    3.  Directed Graph API (outgoing)
        a.  Types
        b.  Graph functions
        c.  Vertex functions
        d.  Edge functions
    4.  Directed Graph API (outgoing)
        a.  Types
        b.  Graph functions
        c.  Vertex functions
        d.  Edge functions
    5.  Concept definitions

   NOTES
    1.  The Uniform API is designed to work with both directed and undirected graphs. Undirected
        graphs use the types and functions as-is, whereas the undirected graph is allowed to alias
        a type/function to the outgoing form of the type or function. For instance, find_vertex_edge()
        would alias find_vertex_out_edge(). Additional notes:
        a.  An edge is associated with 2 vertices, so the concept of in/out isn't eliminated entirely
            for undirected graphs. It still has an in_vertex & out_vertex but there is no implied
            directionality with them.
        b.  A graph can be viewed as a range of ranges, with the vertices being the outer range and
            the edges being the inner range. [Andrew Lumsdaine]
    2.  Directed graphs are assumed to have outgoing types/functions, Incoming types/functions are
        optional and not typically used in algorithms. Directed graphs with both incoming and outgoing
        edges are bidirectional graphs.
    3.  The type of a key for a vertex is dependent on the underlying container for the vertices. For 
        non-mapped containers such as vector & deque, the index is used. For mapped containers such as
        map and unordered_map, the user-defined key is used. [This is is a design intent. It still 
        needs to be demonstrated to be used in a prototype.]
    4.  For a general-purpose graph, edge ranges/iterators should be defined for the graph and vertex. 
        Vertex edge iterators/ranges will only iterate through the edges for a vertex (uniform, incoming 
        or outgoing). Graph edge iterators/ranges will iterate over all edges in the graph, independent 
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

//
// Graph Traits
//
template <typename G>
struct graph_traits;

/* For exposition
template <typename G>
struct graph_traits
{
  using graph_type                  = typename G::graph_type;
  using const_graph_type            = typename G::const_graph_type;
  using graph_user_value_type       = typename graph_type::graph_user_value_type;
  using const_graph_user_value_type = typename graph_type::const_graph_user_value_type;
  using allocator_type              = typename graph_type::allocator_type;

  using vertex_type                  = typename graph_type::vertex_type;
  using const_vertex_type            = typename graph_type::const_vertex_type;
  using vertex_user_value_type       = typename graph_type::user_value_type;
  using const_vertex_user_value_type = typename graph_type::const_vertex_user_value_type;
  using vertex_allocator_type        = typename graph_type::vertex_allocator_type;
  using vertex_size_type             = typename graph_type::vertex_size_type;
  using vertex_ssize_type            = typename graph_type::vertex_ssize_type;
  using vertex_index_type            = typename graph_type::vertex_index_type;
  using vertex_key_type              = typename graph_type::vertex_key_type;
  using const_vertex_key_type        = typename graph_type::const_vertex_key_type;
  using vertex_value_type            = typename graph_type::vertex_value_type;

  using vertex_iterator       = typename graph_type::vertex_iterator;
  using const_vertex_iterator = typename graph_type::const_vertex_iterator;
  using vertex_range          = typename graph_type::vertex_range;
  using const_vertex_range    = typename graph_type::const_vertex_range;

  using edge_type                  = typename graph_type::edge_type;
  using const_edge_type            = typename graph_type::const_edge_type;
  using edge_user_value_type       = typename graph_type::edge_user_value_type;
  using const_edge_user_value_type = typename graph_type::const_edge_user_value_type;
  using edge_allocator_type        = typename graph_type::edge_allocator_type;
  using edge_size_type             = typename graph_type::edge_size_type;
  using edge_ssize_type            = typename graph_type::edge_ssize_type;
  using edge_index_type            = typename graph_type::IndexT;
  using edge_key_type              = typename graph_type::edge_key_type; // e.g. <from,to>
  using edge_value_type            = typename graph_type::edge_value_type;

  using edge_iterator       = typename graph_type::edge_iterator;
  using const_edge_iterator = typename graph_type::const_edge_iterator;
  using edge_range          = typename graph_type::edge_range;
  using const_edge_range    = typename graph_type::const_edge_range;

  using vertex_out_edge_size_type      = typename graph_type::vertex_out_edge_size_type;
  using vertex_out_edge_ssize_type     = typename graph_type::vertex_out_edge_ssize_type;
  using vertex_out_edge_iterator       = typename graph_type::vertex_out_edge_iterator;
  using const_vertex_out_edge_iterator = typename graph_type::const_vertex_out_edge_iterator;
  using vertex_out_edge_range          = typename graph_type::vertex_out_edge_range;
  using const_vertex_out_edge_range    = typename graph_type::const_vertex_out_edge_range;

  using vertex_in_edge_size_type      = typename graph_type::vertex_in_edge_size_type;
  using vertex_in_edge_ssize_type     = typename graph_type::vertex_in_edge_ssize_type;
  using vertex_in_edge_iterator       = typename graph_type::vertex_in_edge_iterator;
  using const_vertex_in_edge_iterator = typename graph_type::const_vertex_in_edge_iterator;
  using vertex_in_edge_range          = typename graph_type::vertex_in_edge_range;
  using const_vertex_in_edge_range    = typename graph_type::const_vertex_in_edge_range;

  using vertex_edge_size_type      = typename graph_type::vertex_edge_size_type;
  using vertex_edge_ssize_type     = typename graph_type::vertex_edge_ssize_type;
  using vertex_edge_iterator       = typename graph_type::vertex_edge_iterator;
  using const_vertex_edge_iterator = typename graph_type::const_vertex_edge_iterator;
  using vertex_edge_range          = typename graph_type::vertex_edge_range;
  using const_vertex_edge_range    = typename graph_type::const_vertex_edge_range;
};
*/

//
// Uniform API Types
//

// Uniform API: Graph types
template <typename G>
using graph_value_t = typename graph_traits<G>::graph_user_value;
template <typename G>
using const_graph_value_t = typename graph_traits<G>::const_graph_user_value;

template <typename G>
using edge_range_t = typename graph_traits<G>::edge_range;
template <typename G>
using const_edge_range_t = typename graph_traits<G>::const_edge_range;

template <typename G>
using edge_iterator_t = typename graph_traits<G>::edge_iterator;
template <typename G>
using const_edge_iterator_t = typename graph_traits<G>::const_edge_iterator;

template <typename G>
using edge_sentinel_t = typename graph_traits<G>::edge_sentinel;

template <typename G>
using edge_size_t = typename graph_traits<G>::edge_size_type;
template <typename G>
using edge_ssize_t = typename graph_traits<G>::edge_ssize_type;

// Uniform API: Vertex types
template <typename G>
using vertex_t = typename graph_traits<G>::vertex_type;
template <typename G>
using const_vertex_t = typename graph_traits<G>::const_vertex_type;

template <typename G>
using vertex_key_t = typename graph_traits<G>::vertex_key_type;
template <typename G>
using const_vertex_key_t = const typename graph_traits<G>::const_vertex_key_type;

template <typename G>
using vertex_value_t = typename graph_traits<G>::vertex_user_value_type;
template <typename G>
using const_vertex_value_t = const typename graph_traits<G>::const_vertex_user_value_type;

template <typename G>
using vertex_range_t = typename graph_traits<G>::vertex_range;
template <typename G>
using const_vertex_range_t = typename graph_traits<G>::const_vertex_range;

template <typename G>
using vertex_iterator_t = typename graph_traits<G>::vertex_iterator;
template <typename G>
using const_vertex_iterator_t = typename graph_traits<G>::const_vertex_iterator;

template <typename G>
using vertex_sentinel_t = typename graph_traits<G>::vertex_sentinel;

template <typename G>
using vertex_size_t = typename graph_traits<G>::vertex_size_type;
template <typename G>
using vertex_ssize_t = typename graph_traits<G>::vertex_ssize_type;


// Uniform API: Edge types
template <typename G>
using edge_t = typename graph_traits<G>::edge_type;
template <typename G>
using const_edge_t = typename graph_traits<G>::const_edge_type;

template <typename G>
using edge_value_t = typename graph_traits<G>::edge_user_value_type;
template <typename G>
using const_edge_value_t = const typename graph_traits<G>::edge_user_value_type;

template <typename G>
using vertex_edge_range_t = typename graph_traits<G>::vertex_edge_range;
template <typename G>
using const_vertex_edge_range_t = typename graph_traits<G>::const_vertex_edge_range;

template <typename G>
using vertex_edge_iterator_t = typename graph_traits<G>::vertex_edge_iterator;
template <typename G>
using const_vertex_edge_iterator_t = typename graph_traits<G>::const_vertex_edge_iterator;

template <typename G>
using vertex_edge_sentinel_t = typename graph_traits<G>::vertex_edge_sentinel;

template <typename G>
using vertex_edge_size_t = typename graph_traits<G>::vertex_edge_size_type;
template <typename G>
using vertex_edge_ssize_t = typename graph_traits<G>::vertex_edge_ssize_type;


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


// Uniform API: Graph-Vertex functions
template <typename G>
constexpr auto vertices(G& g) -> vertex_range_t<G>;
template <typename G>
constexpr auto vertices(G const& g) -> const_vertex_range_t<G>;

template <typename G>
constexpr auto vertices_size(G const& g) -> vertex_size_t<G>;
template <typename G>
constexpr auto vertices_ssize(G const& g) -> vertex_ssize_t<G>;

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
constexpr auto find_vertex(G& g, const_vertex_key_t<G>) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto find_vertex(G const& g, const_vertex_key_t<G>) -> const_vertex_iterator_t<G>;

template <typename G>
void reserve_vertices(G& g, vertex_size_t<G>) {}

template <typename G>
void resize_vertices(G& g, vertex_size_t<G>) {}


// Uniform API: Graph-Edge functions
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
constexpr auto edges_size(G const& g) -> edge_size_t<G>;
template <typename G>
constexpr auto edges_ssize(G const& g) -> edge_ssize_t<G>;

template <typename G>
constexpr auto find_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> edge_iterator_t<G>;
template <typename G>
constexpr auto find_edge(G const& g, const_vertex_t<G>& u, const_vertex_t<G>& v) -> const_edge_iterator_t<G>;

template <typename G>
constexpr auto find_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) -> edge_iterator_t<G>;
template <typename G>
constexpr auto find_edge(G const& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> const_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v);

template <typename G>
constexpr void erase_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey);

template <typename G>
constexpr void erase_edge(G& g, edge_iterator_t<G> uv);

template <typename G>
constexpr void erase_edges(G& g, edge_range_t<G>);

template <typename G>
void reserve_edges(G& g, edge_size_t<G>);


// Uniform API: Vertex functions
template <typename G>
constexpr auto vertex_key(G const&, const_vertex_t<G>& u) -> vertex_key_t<G>;

template <typename G>
constexpr auto create_vertex(G& g) -> pair<vertex_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_vertex(G& g, vertex_value_t<G> const&) -> pair<vertex_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_vertex(G& g, vertex_value_t<G> &&) -> pair<vertex_iterator_t<G>, bool>;

template <typename G>
constexpr void erase_vertex(G& g, vertex_iterator_t<G>&);
template <typename G>
constexpr void erase_vertex(G& g, vertex_key_t<G>&);
template <typename G>
constexpr void erase_vertices(G& g, vertex_range_t<G>&);

// Uniform API: Vertex-Edge functions
template <typename G>
constexpr auto edges(G& g, vertex_t<G>& u) -> vertex_edge_range_t<G>;
template <typename G>
constexpr auto edges(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_range_t<G>;

template <typename G>
constexpr auto begin(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto begin(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto cbegin(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto end(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto end(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto cend(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_begin(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_begin(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cbegin(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_end(G& g, vertex_t<G>& u) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_end(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto edges_cend(G const& g, const_vertex_t<G>& u) -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto edges_size(G const& g, const_vertex_t<G>& u) -> vertex_edge_size_t<G>;
template <typename G>
constexpr auto edges_ssize(G const& g, const_vertex_t<G>& u) -> vertex_edge_ssize_t<G>;

template <typename G>
constexpr auto edges_degree(G const& g, const_vertex_t<G>& u) -> vertex_edge_size_t<G>;

template <typename G>
constexpr auto find_vertex_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto find_vertex_edge(G const& g, const_vertex_t<G>& u, const_vertex_t<G>& v)
      -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto find_vertex_edge(G& g, vertex_key_t<G>& ukey, vertex_key_t<G>& vkey) -> vertex_edge_iterator_t<G>;
template <typename G>
constexpr auto find_vertex_edge(G const& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> const_vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_edge_iterator_t<G> uv) -> vertex_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edges(G& g, vertex_edge_range_t<G> uv) -> vertex_edge_iterator_t<G>;

template <typename G>
constexpr void clear_edges(G& g, vertex_t<G>&);


// Uniform API: Edge functions
template <typename G>
constexpr auto vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertex(G const& g, const_edge_t<G>& uv) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertex(G& g, edge_t<G>& uv, const_vertex_t<G>& source) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto vertex(G const& g, const_edge_t<G>& uv, const_vertex_t<G>& source) -> const_vertex_iterator_t<G>;

template <typename G>
constexpr auto vertex_key(G const& g, const_edge_t<G>& uv) -> vertex_key_t<G>;
template <typename G>
constexpr auto vertex_key(G const& g, const_edge_t<G>& uv, vertex_key_t<G> source_key) -> vertex_key_t<G>;
template <typename G>
constexpr auto vertex_key(G const& g, const_edge_t<G>& uv, const_vertex_t<G>& source) -> vertex_key_t<G>;

template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_t<G>& u, vertex_t<G>& v, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&) -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G>&)
      -> pair<vertex_edge_iterator_t<G>, bool>;
template <typename G>
constexpr auto create_edge(G& g, vertex_key_t<G>&, vertex_key_t<G>&, edge_value_t<G> &&)
      -> pair<vertex_edge_iterator_t<G>, bool>;

//
// Directed API (outward)
//

// Directed API (outward): Types
template <typename G>
using vertex_out_edge_range_t = typename graph_traits<G>::vertex_out_edge_range;
template <typename G>
using const_vertex_out_edge_range_t = typename graph_traits<G>::const_vertex_out_edge_range;

template <typename G>
using vertex_out_edge_iterator_t = typename graph_traits<G>::vertex_out_edge_iterator;
template <typename G>
using const_vertex_out_edge_iterator_t = typename graph_traits<G>::const_vertex_out_edge_iterator;

template <typename G>
using vertex_out_edge_sentinel_t = typename graph_traits<G>::vertex_out_edge_sentinel;

template <typename G>
using vertex_out_edge_size_t = typename graph_traits<G>::vertex_out_edge_size_type;
template <typename G>
using vertex_out_edge_ssize_t = typename graph_traits<G>::vertex_out_edge_ssize_type;


// Directed API (outward): Vertex-Edge functions
template <typename G>
constexpr auto out_edges(G& g, vertex_t<G>& u) -> vertex_out_edge_range_t<G>;
template <typename G>
constexpr auto out_edges(G const& g, const_vertex_t<G>& u) -> const_vertex_out_edge_range_t<G>;

template <typename G>
constexpr auto out_begin(G& g, vertex_t<G>& u) -> vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto out_begin(G const& g, const_vertex_t<G>& u) -> const_vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto out_cbegin(G const& g, const_vertex_t<G>& u) -> const_vertex_out_edge_iterator_t<G>;

template <typename G>
constexpr auto out_end(G& g, vertex_t<G>& u) -> vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto out_end(G const& g, const_vertex_t<G>& u) -> const_vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto out_cend(G const& g, const_vertex_t<G>& u) -> const_vertex_out_edge_iterator_t<G>;

template <typename G>
constexpr auto out_size(G const& g, const_vertex_t<G>& u) -> vertex_out_edge_size_t<G>;
template <typename G>
constexpr auto out_ssize(G const& g, const_vertex_t<G>& u) -> vertex_out_edge_ssize_t<G>;
template <typename G>
constexpr auto out_degree(G const& g, const_vertex_t<G>& u) -> vertex_out_edge_size_t<G>;

template <typename G>
constexpr auto find_out_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto find_out_edge(G const& g, const_vertex_t<G>& u, const_vertex_t<G>& v)
      -> const_vertex_out_edge_iterator_t<G>;

template <typename G>
constexpr auto find_out_edge(G& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> vertex_out_edge_iterator_t<G>;
template <typename G>
constexpr auto find_out_edge(G const& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> const_vertex_out_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_out_edge_iterator_t<G> uv) -> vertex_out_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edges(G& g, vertex_out_edge_range_t<G>);

template <typename G>
constexpr void clear_out_edges(G& g, vertex_t<G>& u);

// Directed API (outgoing): Edge functions
template <typename G>
constexpr auto out_vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto out_vertex(G const& g, const_edge_t<G>& uv) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto out_vertex_key(G const& g, const_edge_t<G>& uv) -> vertex_key_t<G>;

//
// Directed API (inward)
//

// Directed API (inward): Types
template <typename G>
using vertex_in_edge_range_t = typename graph_traits<G>::vertex_in_edge_range;
template <typename G>
using const_vertex_in_edge_range_t = typename graph_traits<G>::const_vertex_in_edge_range;

template <typename G>
using vertex_in_edge_iterator_t = typename graph_traits<G>::vertex_in_edge_iterator;
template <typename G>
using const_vertex_in_edge_iterator_t = typename graph_traits<G>::const_vertex_in_edge_iterator;

template <typename G>
using vertex_in_edge_sentinel_t = typename graph_traits<G>::vertex_in_edge_sentinel;

template <typename G>
using vertex_in_edge_size_t = typename graph_traits<G>::vertex_in_edge_size_type;
template <typename G>
using vertex_in_edge_ssize_t = typename graph_traits<G>::vertex_in_edge_ssize_type;


// Directed API (inward): Vertex-Edge functions
template <typename G>
constexpr auto in_edges(G& g, vertex_t<G>& u) -> vertex_in_edge_range_t<G>;
template <typename G>
constexpr auto in_edges(G const& g, const_vertex_t<G>& u) -> const_vertex_in_edge_range_t<G>;

template <typename G>
constexpr auto in_begin(G& g, vertex_t<G>& u) -> vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto in_begin(G const& g, const_vertex_t<G>& u) -> const_vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto in_cbegin(G const& g, const_vertex_t<G>& u) -> const_vertex_in_edge_iterator_t<G>;

template <typename G>
constexpr auto in_end(G& g, vertex_t<G>& u) -> vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto in_end(G const& g, const_vertex_t<G>& u) -> const_vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto in_cend(G const& g, const_vertex_t<G>& u) -> const_vertex_in_edge_iterator_t<G>;

template <typename G>
constexpr auto in_size(G& g, vertex_t<G>& u) -> vertex_in_edge_size_t<G>;
template <typename G>
constexpr auto in_ssize(G& g, vertex_t<G>& u) -> vertex_in_edge_ssize_t<G>;
template <typename G>
constexpr auto in_degree(G& g, vertex_t<G>& u) -> vertex_in_edge_size_t<G>;

template <typename G>
constexpr auto find_in_edge(G& g, vertex_t<G>& u, vertex_t<G>& v) -> vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto find_in_edge(G const& g, const_vertex_t<G>& u, const_vertex_t<G>& v)
      -> const_vertex_in_edge_iterator_t<G>;

template <typename G>
constexpr auto find_in_edge(G& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> vertex_in_edge_iterator_t<G>;
template <typename G>
constexpr auto find_in_edge(G const& g, const_vertex_key_t<G>& ukey, const_vertex_key_t<G>& vkey)
      -> const_vertex_in_edge_iterator_t<G>;

template <typename G>
constexpr auto erase_edge(G& g, vertex_in_edge_iterator_t<G> uv) -> vertex_in_edge_iterator_t<G>;

template <typename G>
constexpr void erase_edges(G& g, vertex_in_edge_range_t<G>);

template <typename G>
constexpr void clear_in_edges(G& g, vertex_t<G>& u);

// Directed API (incoming): Edge functions
template <typename G>
constexpr auto in_vertex(G& g, edge_t<G>& uv) -> vertex_iterator_t<G>;
template <typename G>
constexpr auto in_vertex(G const& g, const_edge_t<G>& uv) -> const_vertex_iterator_t<G>;
template <typename G>
constexpr auto in_vertex_key(G const& g, const_edge_t<G>& uv) -> vertex_key_t<G>;


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
  //{ out_edges<G>(g, u) } -> vertex_out_edge_range_t<G>;
};

template <typename G>
concept inward_directed_graph = requires(G&& g, vertex_t<G>& u) {
  true;
  //{ in_edges(g, u) } -> vertex_in_edge_range_t<G>;
};

template <typename G>
concept directed_graph = uniform_graph<G> && outward_directed_graph<G>;

template <typename G>
concept bidirected_graph = uniform_graph<G> && outward_directed_graph<G>&& inward_directed_graph<G>;

template <typename G>
concept undirected_graph = uniform_graph<G> && !outward_directed_graph<G> && !inward_directed_graph<G>;

template <typename V>
concept vertex_c = true; // (vertex_c --> vertex: name conflict with vertex(g,uv))
template <typename V>
concept edge_c = true;

template <typename T>
concept arithmetic = is_arithmetic_v<T>;


// for DFS, BFS & TopoSort ranges
template <typename G>
concept searchable_graph = requires(G&& g, vertex_iterator_t<G>& u, vertex_edge_iterator_t<G>& uv) {
#if 1
      true;
#else
  ::ranges::template input_range<G>;
  ::ranges::template input_iterator<vertex_iterator_t<G>>;
  ::ranges::template input_iterator<vertex_edge_iterator_t<G>>;
  //::ranges::forward_range<vertex_t<G>>; // vertex begin/end require graph parameter so it doesn't apply
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
