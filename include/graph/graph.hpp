#include <concepts>
#include <ranges>
#include <type_traits>
#include <optional>
#include "ordered_pair.hpp"

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

*/

#ifndef GRAPH_FWD_HPP
#  define GRAPH_FWD_HPP

namespace std {

//
// Graph Traits
//
template <typename G>
struct graph_traits;

/* For exposition
template <typename G>
struct graph_traits
{
  using graph_type                 = G;
  using graph_value_type           = ...;
  using allocator_type             = ...; // used by containers in the graph

  using vertex_type                = ...;
  using vertex_key_type            = ...;
  using vertex_value_type          = ...;

  using edge_type                  = ...;
  using edge_key_type              = ...; // ordered_pair | unordered_pair
  using edge_value_type            = ...;

  using vertex_range               = ...;
  using vertex_edge_range          = ...;

  // The following are optional
  using edge_range                 = ...;
  using vertex_vertex_range        = ...;

  // The following are only for directed graphs and are optional
  using vertex_outward_edge_range   = ...; 
  using vertex_outward_vertex_range = ...; 
  using vertex_inward_edge_range    = ...; 
  using vertex_inward_vertex_range  = ...; 
};
*/

//
// Concept defintions, part 1
//

// clang-format off

// directed = edge key is ordered
template<typename G>
using is_directed = is_base_of<ordered_pair<typename graph_traits<G>::vertex_key_type, typename graph_traits<G>::vertex_key_type>,
                               typename graph_traits<G>::edge_key_type>;
template<typename G>
inline constexpr bool is_directed_v = is_directed<G>::value;

template <typename G>
concept directed 
      =  is_base_of_v<ordered_pair<typename graph_traits<G>::vertex_key_type, typename graph_traits<G>::vertex_key_type>,
                      typename graph_traits<G>::edge_key_type>;

// undirected = edge key is unordered
template <typename G>
using is_undirected = is_base_of<unordered_pair<typename graph_traits<G>::vertex_key_type, typename graph_traits<G>::vertex_key_type>,
                                 typename graph_traits<G>::edge_key_type>;
template<typename G>
inline constexpr bool is_undirected_v = is_undirected<G>::value;

template <typename G>
concept undirected
      =  is_base_of_v<unordered_pair<typename graph_traits<G>::vertex_key_type, typename graph_traits<G>::vertex_key_type>,
                      typename graph_traits<G>::edge_key_type>;

// directed or undirected
template<typename G>
inline constexpr bool is_directed_or_undirected_v = is_directed_v<G> || is_undirected_v<G>;

template <typename G>
concept directed_or_undirected = directed<G> || undirected<G>;

// clang-format on


//
// Uniform API Types
//

// Uniform API: Graph value types
template <directed_or_undirected G>
using graph_value_t = typename graph_traits<G>::graph_value_type;

template <directed_or_undirected G>
using graph_allocator_t = typename graph_traits<G>::allocator_type;

// Uniform API: Vertex value types
template <directed_or_undirected G>
using vertex_t = typename graph_traits<G>::vertex_type;

template <directed_or_undirected G>
using vertex_key_t = typename graph_traits<G>::vertex_key_type;

template <directed_or_undirected G>
using vertex_value_t = typename graph_traits<G>::vertex_value_type;

// Uniform API: Edge value types
template <directed_or_undirected G>
using edge_t = typename graph_traits<G>::edge_type;

template <directed_or_undirected G>
using edge_key_t = typename graph_traits<G>::edge_key_type;

template <directed_or_undirected G>
using edge_value_t = typename graph_traits<G>::edge_value_type;

// Uniform API: Graph-Vertices range types
template <directed_or_undirected G>
using vertex_range_t = typename graph_traits<G>::vertex_range;
template <directed_or_undirected G>
using const_vertex_range_t = typename graph_traits<G>::const_vertex_range;

template <typename G>
using vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_range>;
template <directed_or_undirected G>
using const_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_range>;

template <directed_or_undirected G>
using vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_range>;
template <directed_or_undirected G>
using vertex_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_range>;


// Uniform API: Graph-Edges range types
template <directed_or_undirected G>
using edge_range_t = typename graph_traits<G>::edge_range;
template <directed_or_undirected G>
using const_edge_range_t = typename graph_traits<G>::const_edge_range;

template <directed_or_undirected G>
using edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::edge_range>;
template <directed_or_undirected G>
using const_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_edge_range>;

template <directed_or_undirected G>
using edge_size_t = typename graph_traits<G>::edge_size_type;
template <directed_or_undirected G>
using edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::edge_range>;


// Uniform API: Vertex-Edges range types
template <directed_or_undirected G>
using vertex_edge_range_t = typename graph_traits<G>::vertex_edge_range;
template <directed_or_undirected G>
using const_vertex_edge_range_t = typename graph_traits<G>::const_vertex_edge_range;

template <directed_or_undirected G>
using vertex_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_edge_range>;
template <directed_or_undirected G>
using const_vertex_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_edge_range>;

template <directed_or_undirected G>
using vertex_edge_size_t = typename graph_traits<G>::vertex_edge_size_type;
template <directed_or_undirected G>
using vertex_edge_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_edge_range>;

// Uniform API: Vertex-Vertices range types
template <directed_or_undirected G>
using vertex_vertex_range_t = typename graph_traits<G>::vertex_vertex_range;
template <directed_or_undirected G>
using const_vertex_vertex_range_t = typename graph_traits<G>::const_vertex_vertex_range;

template <directed_or_undirected G>
using vertex_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_vertex_range>;
template <directed_or_undirected G>
using const_vertex_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::const_vertex_vertex_range>;

template <directed_or_undirected G>
using vertex_vertex_size_t = typename graph_traits<G>::vertex_vertex_size_type;
template <directed_or_undirected G>
using vertex_vertex_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_vertex_range>;


//
// Uniform API Functions
//

// Uniform API: Graph scope functions
template <directed_or_undirected G>
constexpr auto graph_value(G& g) -> graph_value_t<G>&;
template <directed_or_undirected G>
constexpr auto graph_value(G const& g) -> const graph_value_t<G>&;

// begin(g)
// end(g)
// size(g)
// ssize(g)
template <directed_or_undirected G>
constexpr auto edge_key(G& g, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v) -> edge_key_t<G>;
template <directed_or_undirected G>
constexpr auto edge_key(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> edge_key_t<G>;

template <directed_or_undirected G>
constexpr bool contains_vertex(G const& g, vertex_key_t<G> ukey) noexcept;
template <directed_or_undirected G>
constexpr bool contains_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey);
template <directed_or_undirected G>
constexpr bool contains_edge(G const& g, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v);

template <directed_or_undirected G>
constexpr void swap(G& a, G& b);

template <directed_or_undirected G>
constexpr void erase_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v);
template <directed_or_undirected G>
constexpr void erase_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey);

template <directed_or_undirected G>
void clear(G& g);

template <directed_or_undirected G>
void reserve_vertices(G& g, vertex_size_t<G>) {}
template <directed_or_undirected G>
void reserve_edges(G& g, edge_size_t<G>);

template <directed_or_undirected G>
void resize_vertices(G& g, vertex_size_t<G>) {}

// Uniform API: Vertex & vertex range
template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_vertex_iterator_t<G> u) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto vertex_value(G& g, vertex_iterator_t<G> u) -> vertex_value_t<G>&;
template <directed_or_undirected G>
constexpr auto vertex_value(const G& g, const_vertex_iterator_t<G> u) -> const vertex_value_t<G>&;

template <directed_or_undirected G>
constexpr auto vertices(G& g) -> vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&) -> const_vertex_range_t<G>;

template <directed_or_undirected G>
constexpr auto vertices_size(const G&) noexcept -> vertex_size_t<G>;
template <directed_or_undirected G>
constexpr auto vertices_ssize(const G&) noexcept -> vertex_ssize_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex(G& g, vertex_key_t<G>) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex(const G&, vertex_key_t<G>) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto create_vertex(G& g) -> optional<vertex_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_vertex(G& g, const vertex_value_t<G>&) -> optional<vertex_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_vertex(G& g, vertex_value_t<G> &&) -> optional<vertex_iterator_t<G>>;

template <directed_or_undirected G>
constexpr void erase_vertex(G& g, vertex_iterator_t<G>);
template <directed_or_undirected G>
constexpr void erase_vertex(G& g, vertex_key_t<G>);
template <directed_or_undirected G>
constexpr void erase_vertices(G& g, vertex_range_t<G>);

template <directed_or_undirected G>
constexpr void clear_edges(G& g, vertex_iterator_t<G>);

// Uniform API: Graph-Edge range functions
template <directed_or_undirected G>
constexpr auto edge_key(G& g, const_edge_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
constexpr auto edge_value(G& g, edge_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed_or_undirected G>
constexpr auto vertex(G& g, edge_iterator_t<G> uv, const_vertex_iterator_t<G> source) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto vertex(const G&, const_edge_iterator_t<G> uv, const_vertex_iterator_t<G> source)
      -> const_vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(G& g, edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(const G&, const_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto vertex(G& g, edge_iterator_t<G> uv, vertex_key_t<G> source_key) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto vertex(const G&, const_edge_iterator_t<G> uv, vertex_key_t<G> source_key) -> const_vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(G& g, edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(const G&, const_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_edge_iterator_t<G> uv, const_vertex_iterator_t<G> source) -> vertex_key_t<G>;
template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_edge_iterator_t<G> uv, vertex_key_t<G> source_key) -> vertex_key_t<G>;
template <directed G>
constexpr auto vertex_key(const G&, const_edge_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
constexpr auto vertex_key(const G&, const_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto outward_vertex(G& g, edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto outward_vertex(const G&, const_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto outward_vertex_key(const G&, const_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto inward_vertex(G& g, edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto inward_vertex(const G&, const_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto inward_vertex_key(const G&, const_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto edges(G& g) -> edge_range_t<G>;
template <directed_or_undirected G>
constexpr auto edges(const G&) -> const_edge_range_t<G>;

template <directed_or_undirected G>
constexpr auto edges_size(const G&) noexcept -> edge_size_t<G>;
template <directed_or_undirected G>
constexpr auto edges_ssize(const G&) noexcept -> edge_ssize_t<G>;

template <directed_or_undirected G>
constexpr auto find_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_edge(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_edge_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto find_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_edge(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> const_edge_iterator_t<G>;

template <directed_or_undirected G>
constexpr void swap(edge_iterator_t<G>& u, edge_iterator_t<G>& v);
template <directed_or_undirected G>
constexpr void swap(const_edge_iterator_t<G>& u, const_edge_iterator_t<G>& v);

template <directed_or_undirected G>
constexpr void erase_edge(G& g, edge_iterator_t<G> uv);
template <directed_or_undirected G>
constexpr void erase_edges(G& g, edge_range_t<G>);


// Uniform API: Edge Range functions

// Uniform API: Vertex-Edge range functions
// clang-format off
template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_key(G& g, const_vertex_edge_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(G& g, vertex_edge_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_edge_iterator_t<G> uv, const_vertex_iterator_t<G> source) 
      -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_edge_iterator_t<G> uv, const_vertex_iterator_t<G> source)
      -> const_vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_edge_iterator_t<G> uv, vertex_key_t<G> source_key)
      -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_edge_iterator_t<G> uv, vertex_key_t<G> source_key)
      -> const_vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_edge_iterator_t<G> uv, vertex_key_t<G> source_key) 
      -> vertex_key_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_edge_iterator_t<G> uv, const_vertex_iterator_t<G> source)
      -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_edge_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto outward_vertex(G& g, vertex_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex(const G&, const_vertex_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex_key(const G&, const_vertex_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_edge_iterator_t<G> uv) -> vertex_key_t<G>;
// clang-format on

template <directed_or_undirected G>
constexpr auto edges(G& g, vertex_iterator_t<G>& u) -> vertex_edge_range_t<G>;
template <directed_or_undirected G>
constexpr auto edges(const G&, const_vertex_iterator_t<G>& u) -> const_vertex_edge_range_t<G>;

template <directed_or_undirected G>
constexpr auto edges_size(const G&, const_vertex_iterator_t<G> u) -> vertex_edge_size_t<G>;
template <directed_or_undirected G>
constexpr auto edges_ssize(const G&, const_vertex_iterator_t<G> u) -> vertex_edge_ssize_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> vertex_edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex_edge(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_edge_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex_edge(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_edge_iterator_t<G>;

template <directed_or_undirected G>
constexpr void swap(vertex_edge_iterator_t<G>& u, vertex_edge_iterator_t<G>& v);
template <directed_or_undirected G>
constexpr void swap(const_vertex_edge_iterator_t<G>& u, const_vertex_edge_iterator_t<G>& v);

template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G>&)
      -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G> &&)
      -> optional<vertex_edge_iterator_t<G>>;

template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>) -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&)
      -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G> &&)
      -> optional<vertex_edge_iterator_t<G>>;

template <directed_or_undirected G>
constexpr auto erase_edge(G& g, vertex_edge_iterator_t<G> uv) -> vertex_edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto erase_edges(G& g, vertex_edge_range_t<G> uv) -> vertex_edge_iterator_t<G>;

// Uniform API: Vertex-Vertex range functions
template <directed_or_undirected G>
constexpr auto edge_key(G& g, const_vertex_vertex_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
constexpr auto edge_value(G& g, vertex_vertex_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed_or_undirected G>
constexpr auto vertex(G& g, vertex_vertex_iterator_t<G> uv, const_vertex_iterator_t<G> source) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto vertex(const G&, const_vertex_vertex_iterator_t<G> uv, const_vertex_iterator_t<G> source)
      -> const_vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(G& g, vertex_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
requires(!is_same_v<const_vertex_vertex_iterator_t<G>, const_edge_iterator_t<G>>) constexpr auto vertex(
      const G&, const_vertex_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
requires(!is_same_v<vertex_vertex_iterator_t<G>, edge_iterator_t<G>>) constexpr auto vertex(
      G& g, vertex_vertex_iterator_t<G> uv, vertex_key_t<G> source_key) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto vertex(const G&, const_vertex_vertex_iterator_t<G> uv, vertex_key_t<G> source_key)
      -> const_vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(G& g, vertex_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
constexpr auto vertex(const G&, const_vertex_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv, vertex_key_t<G> source_key)
      -> vertex_key_t<G>;
template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv, const_vertex_iterator_t<G> source)
      -> vertex_key_t<G>;
template <directed G>
constexpr auto vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
constexpr auto vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto outward_vertex(G& g, vertex_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto outward_vertex(const G&, const_vertex_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto outward_vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto inward_vertex(G& g, vertex_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto inward_vertex(const G&, const_vertex_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto inward_vertex_key(const G&, const_vertex_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto vertices(G& g, vertex_iterator_t<G> u) -> vertex_vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&, const_vertex_iterator_t<G> u) -> const_vertex_vertex_range_t<G>;

template <directed_or_undirected G>
constexpr auto vertices(G& g, vertex_key_t<G> ukey) -> vertex_vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&, vertex_key_t<G> ukey) -> const_vertex_vertex_range_t<G>;

template <directed_or_undirected G>
constexpr auto vertices_size(const G&, const_vertex_iterator_t<G> u) -> vertex_vertex_size_t<G>;
template <directed_or_undirected G>
constexpr auto vertices_ssize(const G&, const_vertex_iterator_t<G> u) -> vertex_vertex_ssize_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex_vertex(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> vertex_vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex_vertex(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex_vertex(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex_vertex(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_vertex_iterator_t<G>;

template <directed_or_undirected G>
constexpr void swap(vertex_vertex_iterator_t<G>& u, vertex_vertex_iterator_t<G>& v);
template <directed_or_undirected G>
constexpr void swap(const_vertex_vertex_iterator_t<G>& u, const_vertex_vertex_iterator_t<G>& v);

template <directed_or_undirected G>
constexpr auto erase_edge(G& g, vertex_vertex_iterator_t<G> uv) -> vertex_vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto erase_edges(G& g, vertex_vertex_range_t<G> uv) -> vertex_vertex_iterator_t<G>;

//
// Directed API (outward)
//

// Directed API (outward): Types
template <directed G>
using vertex_outward_size_t = typename graph_traits<G>::vertex_outward_size_type;
template <directed G>
using vertex_outward_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_outward_edge_range>;

template <directed G>
using vertex_outward_edge_range_t = typename graph_traits<G>::vertex_outward_edge_range;
template <directed G>
using const_vertex_outward_edge_range_t = typename graph_traits<G>::const_vertex_outward_edge_range;

template <directed G>
using vertex_outward_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_outward_edge_range>;
template <directed G>
using const_vertex_outward_edge_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_outward_edge_range>;

template <directed G>
using vertex_outward_vertex_range_t = typename graph_traits<G>::vertex_outward_vertex_range;
template <directed G>
using const_vertex_outward_vertex_range_t = typename graph_traits<G>::const_vertex_outward_vertex_range;

template <directed G>
using vertex_outward_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_outward_vertex_range>;
template <directed G>
using const_vertex_outward_vertex_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_outward_vertex_range>;


// Directed API (outward): common outward functions
template <directed G>
constexpr auto outward_size(const G&, const_vertex_iterator_t<G> u) -> vertex_outward_size_t<G>;
template <directed G>
constexpr auto outward_ssize(const G&, const_vertex_iterator_t<G> u) -> vertex_outward_ssize_t<G>;


// Directed API (outward): Vertex-Edge functions
// clang-format off
template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_key(G& g, const_vertex_outward_edge_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(G& g, vertex_outward_edge_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto outward_vertex(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex_key(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_outward_edge_iterator_t<G> uv) -> vertex_key_t<G>;
// clang-format on

template <directed G>
constexpr auto outward_edges(G& g, vertex_iterator_t<G> u) -> vertex_outward_edge_range_t<G>;
template <directed G>
constexpr auto outward_edges(const G&, const_vertex_iterator_t<G> u) -> const_vertex_outward_edge_range_t<G>;

template <directed G>
constexpr auto find_outward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> vertex_outward_edge_iterator_t<G>;
template <directed G>
constexpr auto find_outward_edge(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_outward_edge_iterator_t<G>;

template <directed G>
constexpr auto find_outward_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_outward_edge_iterator_t<G>;
template <directed G>
constexpr auto find_outward_edge(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_outward_edge_iterator_t<G>;

template <directed G>
constexpr auto erase_edge(G& g, vertex_outward_edge_iterator_t<G> uv) -> vertex_outward_edge_iterator_t<G>;

template <directed G>
constexpr void erase_edges(G& g, vertex_outward_edge_range_t<G>);

template <directed G>
constexpr void clear_outward_edges(G& g, vertex_iterator_t<G>& u);

template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> optional<vertex_outward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G>&)
      -> optional<vertex_outward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G> &&)
      -> optional<vertex_outward_edge_iterator_t<G>>;

template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>)
      -> optional<vertex_outward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&)
      -> optional<vertex_outward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_outward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G> &&)
      -> optional<vertex_outward_edge_iterator_t<G>>;

// Directed API (outward): Vertex-Vertex functions
// clang-format off
template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_key(G& g, const_vertex_outward_vertex_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(G& g, vertex_outward_vertex_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_outward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_outward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto outward_vertex(G& g, vertex_outward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto outward_vertex_key(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_outward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_outward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_outward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_outward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;
// clang-format on

template <directed G>
constexpr auto outward_vertices(G& g, vertex_iterator_t<G> u) -> vertex_outward_vertex_range_t<G>;
template <directed G>
constexpr auto outward_vertices(const G&, const_vertex_iterator_t<G> u) -> const_vertex_outward_vertex_range_t<G>;

template <directed G>
constexpr auto find_outward_vertex(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> vertex_outward_vertex_iterator_t<G>;
template <directed G>
constexpr auto find_outward_vertex(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_outward_vertex_iterator_t<G>;

template <directed G>
constexpr auto find_outward_vertex(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> vertex_outward_vertex_iterator_t<G>;
template <directed G>
constexpr auto find_outward_vertex(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_outward_vertex_iterator_t<G>;


// Directed API (outward): Edge functions

//
// Directed API (inward)
//

// Directed API (inward): Types
template <directed G>
using vertex_inward_vertex_range_t = typename graph_traits<G>::vertex_inward_vertex_range;
template <directed G>
using const_vertex_inward_vertex_range_t = typename graph_traits<G>::const_vertex_inward_vertex_range;

template <directed G>
using vertex_inward_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_inward_vertex_range>;
template <directed G>
using const_vertex_inward_vertex_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_inward_vertex_range>;

template <directed G>
using vertex_inward_edge_range_t = typename graph_traits<G>::vertex_inward_edge_range;
template <directed G>
using const_vertex_inward_edge_range_t = typename graph_traits<G>::const_vertex_inward_edge_range;

template <directed G>
using vertex_inward_edge_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_inward_edge_range>;
template <directed G>
using const_vertex_inward_edge_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_inward_edge_range>;

template <directed G>
using vertex_inward_size_t = typename graph_traits<G>::vertex_inward_size_type;
template <directed G>
using vertex_inward_ssize_t = ranges::range_difference_t<typename graph_traits<G>::vertex_inward_edge_range>;


// Directed API (inward): common functions
template <directed G>
constexpr auto inward_size(G& g, const_vertex_iterator_t<G> u) -> vertex_inward_size_t<G>;
template <directed G>
constexpr auto inward_ssize(G& g, const_vertex_iterator_t<G> u) -> vertex_inward_ssize_t<G>;


// Directed API (inward): Vertex-Edge functions
// clang-format off
template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_key(G& g, const_vertex_inward_edge_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(G& g, vertex_inward_edge_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_edge_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_edge_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_inward_edge_iterator_t<G> uv) -> vertex_key_t<G>;
// clang-format on

template <directed G>
constexpr auto inward_edges(G& g, vertex_iterator_t<G> u) -> vertex_inward_edge_range_t<G>;
template <directed G>
constexpr auto inward_edges(const G&, const_vertex_iterator_t<G> u) -> const_vertex_inward_edge_range_t<G>;

template <directed G>
constexpr auto find_inward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> vertex_inward_edge_iterator_t<G>;
template <directed G>
constexpr auto find_inward_edge(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_inward_edge_iterator_t<G>;

template <directed G>
constexpr auto find_inward_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> vertex_inward_edge_iterator_t<G>;
template <directed G>
constexpr auto find_inward_edge(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_inward_edge_iterator_t<G>;

template <directed G>
constexpr auto erase_edge(G& g, vertex_inward_edge_iterator_t<G> uv) -> vertex_inward_edge_iterator_t<G>;

template <directed G>
constexpr void erase_edges(G& g, vertex_inward_edge_range_t<G>);

template <directed G>
constexpr void clear_inward_edges(G& g, vertex_iterator_t<G> u);

template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> optional<vertex_inward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G>&)
      -> optional<vertex_inward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G> &&)
      -> optional<vertex_inward_edge_iterator_t<G>>;

template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>) -> optional<vertex_inward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&)
      -> optional<vertex_inward_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_inward_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G> &&)
      -> optional<vertex_inward_edge_iterator_t<G>>;

// Directed API (inward): Vertex-Vertex functions
// clang-format off
template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_key(G& g, const_vertex_inward_vertex_iterator_t<G> uv) -> edge_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(G& g, vertex_inward_vertex_iterator_t<G>& u) -> edge_value_t<G>&;
template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto edge_value(const G& g, const_edge_iterator_t<G>& u) -> const edge_value_t<G>&;

template <directed G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_inward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto vertex(G& g, vertex_inward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;
template <directed G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto vertex_key(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_inward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<vertex_inward_vertex_iterator_t<G>, edge_iterator_t<G>>)
constexpr auto inward_vertex(G& g, vertex_inward_vertex_iterator_t<G> uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G>
  requires (!is_same_v<const_vertex_inward_vertex_iterator_t<G>, const_edge_iterator_t<G>>)
constexpr auto inward_vertex_key(const G&, const_vertex_inward_vertex_iterator_t<G> uv) -> vertex_key_t<G>;
// clang-format on

template <directed G>
constexpr auto inward_vertices(G& g, vertex_iterator_t<G> u) -> vertex_inward_vertex_range_t<G>;
template <directed G>
constexpr auto inward_vertices(const G&, const_vertex_iterator_t<G> u) -> const_vertex_inward_vertex_range_t<G>;

template <directed G>
constexpr auto find_inward_vertex(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v)
      -> vertex_inward_vertex_iterator_t<G>;
template <directed G>
constexpr auto find_inward_vertex(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_vertex_inward_vertex_iterator_t<G>;

template <directed G>
constexpr auto find_inward_vertex(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> vertex_inward_vertex_iterator_t<G>;
template <directed G>
constexpr auto find_inward_vertex(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey)
      -> const_vertex_inward_vertex_iterator_t<G>;


// Directed API (inward): Edge functions


//
// type traits
//

template <typename G, typename Rng>
struct is_vertex_range
      : public bool_constant<is_convertible_v<Rng, const_vertex_range_t<G>> ||
                             is_convertible_v<Rng, const_vertex_vertex_range_t<G>>> {};
template <typename G, typename Rng>
inline constexpr bool is_vertex_range_v = is_vertex_range<G, Rng>::value;

template <typename G, typename Iterator>
struct is_vertex_iterator
      : public bool_constant<is_convertible_v<Iterator, const_vertex_iterator_t<G>> ||
                             is_convertible_v<Iterator, const_vertex_vertex_iterator_t<G>>> {};
template <typename G, typename Iterator>
inline constexpr bool is_vertex_iterator_v = is_vertex_iterator<G, Iterator>::value;


template <typename G, typename Rng>
struct is_edge_range
      : public bool_constant<is_convertible_v<Rng, const_edge_range_t<G>> ||
                             is_convertible_v<Rng, const_vertex_edge_range_t<G>> ||
                             is_convertible_v<Rng, const_vertex_outward_edge_range_t<G>> ||
                             is_convertible_v<Rng, const_vertex_inward_edge_range_t<G>>> {};
template <typename G, typename Rng>
inline constexpr bool is_edge_range_v = is_edge_range<G, Rng>::value;

template <typename G, typename Iterator>
struct is_edge_iterator
      : public bool_constant<is_convertible_v<Iterator, const_edge_iterator_t<G>> ||
                             is_convertible_v<Iterator, const_vertex_edge_iterator_t<G>> ||
                             is_convertible_v<Iterator, const_vertex_outward_edge_iterator_t<G>> ||
                             is_convertible_v<Iterator, const_vertex_inward_edge_iterator_t<G>>> {};
template <typename G, typename Iterator>
inline constexpr bool is_edge_iterator_v = is_edge_iterator<G, Iterator>::value;

template <typename G, typename Rng>
using is_vertex_vertex_range = bool_constant<true>;
template <typename G, typename Iterator>
using is_vertex_vertex_iterator = bool_constant<true>;

//
// Concept definitions, part 2
//

// clang-format off

template<typename G>
concept graph_value_types = requires(G& g) {
  graph_traits<G>::graph_type;
  graph_traits<G>::graph_value_type;
  graph_traits<G>::allocator_type; // e.g. allocator<char>
  is_same_v<G, graph_traits<G>::graph_type>;
};

template<typename G>
concept vertex_value_types = requires(G& g) {
  graph_traits<G>::vertex_type;
  graph_traits<G>::vertex_key_type;
  graph_traits<G>::vertex_value_type;
};

template<typename G>
concept edge_value_types = requires(G& g) {
  graph_traits<G>::edge_type;
  graph_traits<G>::edge_key_type;
  graph_traits<G>::edge_value_type;
  graph_traits<G>::vertex_key_type;

  // edge_key_type = ordered_pair<K,K> | unordered_pair<K,K>
  derived_from<edge_key_t<G>, ordered_pair<vertex_key_t<G>,vertex_key_t<G>>> ||
  derived_from<edge_key_t<G>, unordered_pair<vertex_key_t<G>,vertex_key_t<G>>>;
};

template<typename R>
concept graph_standard_range = ranges::sized_range<R> && semiregular<R> && requires(R& r) {
  { cbegin(r) } -> same_as<ranges::iterator_t<const R>>;
  { cend(r) } -> same_as<ranges::sentinel_t<const R>>;
  { ssize(r) } -> same_as<ranges::range_difference_t<R>>;
  { empty(r) } -> same_as<bool>;
};

template<typename G, typename VR>
concept vertex_range = ranges::random_access_range<VR>
                    && graph_standard_range<VR>
                    && requires(VR& vr, ranges::iterator_t<VR> u) {
                      is_same_v<vertex_t<G>, ranges::range_value_t<VR>>;
                      { vertex_key(vr,u) } -> same_as<vertex_key_t<G>>;
                      { vertex_value(vr,u) } -> same_as<vertex_value_t<G>&>;
                    };

template<typename G, typename R>
concept edge_range = ranges::forward_range<R>
                  && graph_standard_range<R>
                  && requires(G& g, ranges::iterator_t<R> uv) {
                      { edge_key(g,uv) }   -> same_as<edge_key_t<G>>;
                      { edge_value(g,uv) } -> same_as<edge_value_t<G>&>;
                  };

template<typename G, typename R>
concept incidence_edge_range = edge_range<G,R>
                            && requires(R& r) {
                              true;
                              //is_same<range_value_t<R>, edge_t<G>;
                              //{ degree(r) } -> same_as<ranges::range_difference_t<VL>>;
                            };

template<typename G, typename R>
concept adjacency_edge_range = edge_range<G,R>
                            && requires(R& r) {
                              true;
                              //is_same<range_value_t<R>, vertex_t<G>;
                              //{ degree(r) } -> same_as<ranges::range_difference_t<VL>>;
                            };

template <typename G>
concept vertex_list_graph = vertex_range<G,typename graph_traits<G>::vertex_range> // vertex_range is a vertex_range
                         && vertex_range<G,G>                             // graph is an alias of the vertex_range
                         && graph_value_types<G>
                         && vertex_value_types<G>
                         && requires(G&&                  g, 
                                     vertex_key_t<G>      ukey, 
                                     vertex_iterator_t<G> u, 
                                     vertex_iterator_t<G> v) {
  { graph_value(g) } -> same_as<graph_value_t<G>&>;
  { vertices(g) } -> same_as<vertex_range_t<G>>;
  { find_vertex(g,ukey) } -> same_as<vertex_iterator_t<G>>;
  { contains_vertex(g, ukey) } -> same_as<bool>;
  swap(u,v);
};

template <typename G>
concept edge_list_graph = true;

template <typename G>
concept incidence_graph = ranges::forward_range<edge_range_t<G>>;
                       //&& edge_value_types<G>;

template<typename G>
concept incidence_graph_new = vertex_list_graph<G>
                           && edge_range<G, typename graph_traits<G>::vertex_edge_range>
                           && edge_value_types<G>;

template <typename G>
concept adjacency_graph = vertex_list_graph<G>
                        && edge_range<G, typename graph_traits<G>::vertex_vertex_range>
                        && edge_value_types<G>;

template <typename G>
concept adjacency_matrix = requires(vertex_iterator_t<G> u, vertex_iterator_t<G> v) {
  { contains_edge(u,v) } -> same_as<bool>;
  { contains_edge(vertex_key(u),vertex_key(v)) } -> same_as<bool>;
};

template <typename G, typename Rng>
concept vertex_path = true;
template <typename G, typename Rng>
concept edge_path = true;

template <typename G, typename Rng>
concept vertex_cycle = true;

template <typename G>
concept incremental_vertex_graph = true; // add_vertex
template <typename G>
concept decremental_vertex_graph = true; // remove_vertex
template <typename G>
concept dynamic_vertex_graph = incremental_vertex_graph<G> && decremental_vertex_graph<G>;
template <typename G>
concept static_vertex_graph = !incremental_vertex_graph<G> && !decremental_vertex_graph<G>;

template <typename G>
concept incremental_edge_graph = true;
/*template <typename G, typename K=vertex_key_t<G>, typename I=vertex_iterator_t<G>>
concept incremental_edge_graph = requires(G& g, K ukey, K vkey, I u, I v) {
      vertex_edge_iterator_t<G>; 
      { create_edge(g, ukey, vkey) } -> std::convertible_to<optional<vertex_edge_iterator_t<G>>>;
      { create_edge(g, u,    v) }    -> std::convertible_to<optional<vertex_edge_iterator_t<G>>>;
      // consider: edges with required value that aren't default-constructable.
      // We need to support either this, or creation with a value, to have a complete definition.
};*/
template <typename G>
concept decremental_edge_graph = true; // remove_edge
template <typename G>
concept dynamic_edge_graph = incremental_edge_graph<G> && decremental_edge_graph<G>;
template <typename G>
concept static_edge_graph = !incremental_edge_graph<G> && !decremental_edge_graph<G>;

template <typename G>
concept dynamic_graph = dynamic_vertex_graph<G> && dynamic_edge_graph<G>;
template <typename G>
concept static_graph = static_vertex_graph<G> && static_edge_graph<G>;


// Requirements for extracting vertex values from external sources for graph construction
template <typename VRng, typename VValueFnc>
concept vertex_range_extractor = ranges::input_range<VRng>&& invocable<VValueFnc, typename VRng::value_type>;

// Requirements for extracting edge values from external sources for graph construction
// ERng is a forward_range because it is traversed twice; once to get the max vertex_key
// and a second time to load the edges.
template <typename ERng, typename EKeyFnc, typename EValueFnc>
concept edge_range_extractor = ranges::forward_range<ERng>&& invocable<EKeyFnc, typename ERng::value_type>&&
                                                             invocable<EValueFnc, typename ERng::value_type>;

// clang-format on

} // namespace std

#endif // GRAPH_FWD_HPP

#include "detail/graph_impl.hpp"
