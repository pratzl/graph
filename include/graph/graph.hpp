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
            for undirected graphs. It still has an source_vertex & target_vertex but there is no implied
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

  // The following 4 range sets are used by algorithms
  using vertex_range               = ...;
  using const_vertex_range         = ...;

  using vertex_edge_range          = ...;
  using const_vertex_edge_range    = ...;
  
  using edge_range                 = ...; // recommended
  using const_edge_range           = ...; // recommended
  
  using vertex_vertex_range        = ...; // recommended
  using const_vertex_vertex_range  = ...; // recommended

  // The following are only for directed graphs
  using vertex_outward_edge_range         = ...; // optional
  using const_vertex_outward_edge_range   = ...; // optional
  
  using vertex_outward_vertex_range       = ...; // optional
  using const_vertex_outward_vertex_range = ...; // optional
  
  using vertex_inward_edge_range          = ...; // optional
  using const_vertex_inward_edge_range    = ...; // optional
  
  using vertex_inward_vertex_range        = ...; // optional
  using const_vertex_inward_vertex_range  = ...; // optional
};
*/

//
// Concept defintions, part 1
//

// clang-format off

// directed = edge key is ordered
template<typename G>
using is_directed = is_base_of<ordered_pair<typename graph_traits<G>::vertex_key_type, 
                                            typename graph_traits<G>::vertex_key_type>,
                               typename graph_traits<G>::edge_key_type>;
template<typename G>
inline constexpr bool is_directed_v = is_directed<G>::value;

template <typename G>
concept directed 
      =  is_base_of_v<ordered_pair<typename graph_traits<G>::vertex_key_type, 
                                   typename graph_traits<G>::vertex_key_type>,
                      typename graph_traits<G>::edge_key_type>;

// undirected = edge key is unordered
template <typename G>
using is_undirected = is_base_of<unordered_pair<typename graph_traits<G>::vertex_key_type, 
                                                typename graph_traits<G>::vertex_key_type>,
                                 typename graph_traits<G>::edge_key_type>;
template<typename G>
inline constexpr bool is_undirected_v = is_undirected<G>::value;

template <typename G>
concept undirected
      =  is_base_of_v<unordered_pair<typename graph_traits<G>::vertex_key_type, 
                                     typename graph_traits<G>::vertex_key_type>,
                      typename graph_traits<G>::edge_key_type>;

// directed or undirected
template<typename G>
inline constexpr bool is_directed_or_undirected_v = is_directed_v<G> || is_undirected_v<G>;

template <typename G>
concept directed_or_undirected = directed<G> || undirected<G>;

// iterator concepts
template <typename G, typename EI>
concept edge_iterator = 
  forward_iterator<EI> && 
  convertible_to<iter_value_t<EI>, 
                 add_lvalue_reference<
                    typename graph_traits<G>::edge_value_type>>;
template <typename G, typename EI>
concept const_edge_iterator = 
  forward_iterator<EI> && 
  convertible_to<iter_value_t<EI>, 
                 add_const<add_lvalue_reference<
                    typename graph_traits<G>::edge_value_type>>>;


// Requirements for extracting vertex values from external sources for graph construction
template <typename VRng, typename VValueFnc>
concept  vertex_value_extractor = ranges::input_range<VRng> &&
                                  invocable<VValueFnc, typename VRng::value_type>;

// Requirements for extracting edge values from external sources for graph construction
// ERng is a forward_range because it is traversed twice; once to get the max vertex_key
// and a second time to load the edges.
template <typename ERng, typename EKeyFnc, typename EValueFnc>
concept edge_value_extractor = ranges::forward_range<ERng> &&
                               invocable<EKeyFnc, typename ERng::value_type> &&
                               invocable<EValueFnc, typename ERng::value_type>;

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
template <typename G>
using vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_range>;

template <directed_or_undirected G>
using vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_range>;
template <directed_or_undirected G>
using vertex_difference_t = ranges::range_difference_t<typename graph_traits<G>::vertex_range>;


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
using edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::edge_range>;

template <directed_or_undirected G>
using edge_size_t = ranges::range_size_t<typename graph_traits<G>::edge_range>;
template <directed_or_undirected G>
using edge_difference_t = ranges::range_difference_t<typename graph_traits<G>::edge_range>;


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
using vertex_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_edge_range>;

template <directed_or_undirected G>
using vertex_edge_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_edge_range>;
template <directed_or_undirected G>
using vertex_edge_difference_t = ranges::range_difference_t<typename graph_traits<G>::vertex_edge_range>;

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
using vertex_vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_vertex_range>;

template <directed_or_undirected G>
using vertex_vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_vertex_range>;
template <directed_or_undirected G>
using vertex_vertex_difference_t = ranges::range_difference_t<typename graph_traits<G>::vertex_vertex_range>;


//
// Uniform API Functions
//

// Graph properties
template <directed_or_undirected G>
constexpr auto graph_value(G& g) -> graph_value_t<G>&;
template <directed_or_undirected G>
constexpr auto graph_value(G const& g) -> const graph_value_t<G>&;

template <directed_or_undirected G>
constexpr bool contains_vertex(G const& g, vertex_key_t<G> ukey) noexcept;
template <directed_or_undirected G>
constexpr bool contains_edge(G const& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey);
template <directed_or_undirected G>
constexpr bool contains_edge(G const& g, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v);


// Vertex properties
template <directed_or_undirected G>
constexpr auto vertex_key(const G&, const_vertex_iterator_t<G> u) -> vertex_key_t<G>;

template <directed_or_undirected G>
constexpr auto vertex_value(G& g, vertex_iterator_t<G> u) -> vertex_value_t<G>&;
template <directed_or_undirected G>
constexpr auto vertex_value(const G& g, const_vertex_iterator_t<G> u) -> const vertex_value_t<G>&;

template <directed_or_undirected G>
constexpr auto degree(const G& g, const_vertex_iterator_t<G> u) noexcept -> vertex_edge_size_t<G>;

// range functions defined for graph to alias vertices(g) [see graph_utilities.hpp]

// Edge properties

// clang-format off
template <directed_or_undirected G>
constexpr auto edge_key(G& g, const_vertex_iterator_t<G> u, 
                              const_vertex_iterator_t<G> v) -> edge_key_t<G>;
template <directed_or_undirected G>
constexpr auto edge_key(G& g, vertex_key_t<G> ukey, 
                              vertex_key_t<G> vkey) -> edge_key_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto edge_key(G& g, EI uv) -> edge_key_t<G>;

template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto edge_value(G& g, EI u) -> edge_value_t<G>&;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto edge_value(const G& g, EI u) -> const edge_value_t<G>&;


template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto vertex(G& g, EI uv, const_vertex_iterator_t<G> source) 
  -> vertex_iterator_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex(const G&, EI uv, const_vertex_iterator_t<G> source) 
  -> const_vertex_iterator_t<G>;

template <directed G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto vertex(G& g, EI uv) -> vertex_iterator_t<G>;

template <directed G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex(const G&, EI uv) -> const_vertex_iterator_t<G>;


template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto vertex(G& g, EI uv, vertex_key_t<G> source_key) 
  -> vertex_iterator_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex(const G&, EI uv, vertex_key_t<G> source_key) 
  -> const_vertex_iterator_t<G>;

template <directed G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto vertex(G& g, EI uv) -> vertex_iterator_t<G>;

template <directed G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex(const G&, EI uv) -> const_vertex_iterator_t<G>;


template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex_key(const G&, EI uv, const_vertex_iterator_t<G> source) 
  -> vertex_key_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex_key(const G&, EI uv, vertex_key_t<G> source_key) 
  -> vertex_key_t<G>;

template <directed G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex_key(const G&, EI uv) -> vertex_key_t<G>;

template <directed G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto vertex_key(const G&, EI uv) -> vertex_key_t<G>;


template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto target_vertex(G& g, EI uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto target_vertex(const G&, EI uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto target_vertex_key(const G&, EI uv) -> vertex_key_t<G>;

template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto source_vertex(G& g, EI uv) -> vertex_iterator_t<G>;
template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto source_vertex(const G&, EI uv) -> const_vertex_iterator_t<G>;

template <directed_or_undirected G, typename EI>
  requires const_edge_iterator<G,EI>
constexpr auto source_vertex_key(const G&, EI uv) -> vertex_key_t<G>;
// clang-format on


// Uniform API: Graph scope functions
template <directed_or_undirected G>
constexpr void swap(G& a, G& b);

template <directed_or_undirected G>
void clear(G& g);

template <directed_or_undirected G>
void reserve_vertices(G& g, vertex_size_t<G>) {}
template <directed_or_undirected G>
void reserve_edges(G& g, edge_size_t<G>);

template <directed_or_undirected G>
void resize_vertices(G& g, vertex_size_t<G>) {}

// Uniform API: Graph-Vertex range functions
// includes ranges functions: begin(g)/end(g), cbegin(g)/cend(g), size(g), ssize(g), empty(g)
template <directed_or_undirected G>
constexpr auto vertices(G& g) -> vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&) -> const_vertex_range_t<G>;

template <directed_or_undirected G>
constexpr auto find_vertex(G& g, vertex_key_t<G>) -> vertex_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_vertex(const G&, vertex_key_t<G>) -> const_vertex_iterator_t<G>;

// Uniform API: Graph-Edge range functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
template <directed_or_undirected G>
constexpr auto edges(G& g) -> edge_range_t<G>;
template <directed_or_undirected G>
constexpr auto edges(const G&) -> const_edge_range_t<G>;

template <directed_or_undirected G>
constexpr auto find_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_edge(const G&, const_vertex_iterator_t<G> u, const_vertex_iterator_t<G> v)
      -> const_edge_iterator_t<G>;

template <directed_or_undirected G>
constexpr auto find_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> edge_iterator_t<G>;
template <directed_or_undirected G>
constexpr auto find_edge(const G&, vertex_key_t<G> ukey, vertex_key_t<G> vkey) -> const_edge_iterator_t<G>;


// Uniform API: Edge Range functions

// Uniform API: Vertex-Edge range functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
template <directed_or_undirected G>
constexpr auto edges(G& g, vertex_iterator_t<G>& u) -> vertex_edge_range_t<G>;
template <directed_or_undirected G>
constexpr auto edges(const G&, const_vertex_iterator_t<G>& u) -> const_vertex_edge_range_t<G>;

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

// Uniform API: Vertex-Vertex range functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
template <directed_or_undirected G>
constexpr auto vertices(G& g, vertex_iterator_t<G> u) -> vertex_vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&, const_vertex_iterator_t<G> u) -> const_vertex_vertex_range_t<G>;

template <directed_or_undirected G>
constexpr auto vertices(G& g, vertex_key_t<G> ukey) -> vertex_vertex_range_t<G>;
template <directed_or_undirected G>
constexpr auto vertices(const G&, vertex_key_t<G> ukey) -> const_vertex_vertex_range_t<G>;

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

// Modifying vertex functions
// create vertex/vertices
template <directed_or_undirected G>
constexpr auto create_vertex(G& g) -> optional<vertex_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_vertex(G& g, const vertex_value_t<G>&) -> optional<vertex_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_vertex(G& g, vertex_value_t<G>&&) -> optional<vertex_iterator_t<G>>;

// clang-format off
template <directed_or_undirected G, typename VRng, typename VValueFnc>
  requires vertex_value_extractor<VRng, VValueFnc>
void create_vertices(G& g, const VRng& vrng, const VValueFnc& vvalue_fnc);
// clang-format on


// erase/clear vertex/vertices
template <directed_or_undirected G>
constexpr void erase_vertex(G& g, vertex_iterator_t<G>);
template <directed_or_undirected G>
constexpr void erase_vertex(G& g, vertex_key_t<G>);
template <directed_or_undirected G>
constexpr void erase_vertices(G& g, vertex_range_t<G>);


// create edge/edges
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v) -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G>&)
      -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v, edge_value_t<G>&&)
      -> optional<vertex_edge_iterator_t<G>>;

template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>) -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&)
      -> optional<vertex_edge_iterator_t<G>>;
template <directed_or_undirected G>
constexpr auto create_edge(G& g, vertex_key_t<G>, vertex_key_t<G>, edge_value_t<G>&&)
      -> optional<vertex_edge_iterator_t<G>>;

// clang-format off
template <directed_or_undirected G, 
          typename               ERng, 
          typename               EKeyFnc, 
          typename               EValueFnc>
  requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> 
void create_edges(G&               g,
                  const ERng&      rng,
                  const EKeyFnc&   ekey_fnc,
                  const EValueFnc& evalue_fnc);

template <directed_or_undirected G, 
          typename               ERng, 
          typename               EKeyFnc, 
          typename               EValueFnc, 
          typename               VRng, 
          typename               VValueFnc>
  requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> &&
           vertex_value_extractor<VRng, VValueFnc>
void create_edges(G&               g,
                  const ERng&      erng,
                  const VRng&      vrng,
                  const EKeyFnc&   ekey_fnc,
                  const EValueFnc& evalue_fnc,
                  const VValueFnc& vvalue_fnc);
// clang-format on

// erase edge/edges
// clang-format off
template <directed_or_undirected G>
constexpr void erase_edge(G& g, vertex_iterator_t<G> u, vertex_iterator_t<G> v);
template <directed_or_undirected G>
constexpr void erase_edge(G& g, vertex_key_t<G> ukey, vertex_key_t<G> vkey);

template <directed_or_undirected G, typename EI>
  requires edge_iterator<G,EI>
constexpr auto erase_edge(G& g, EI uv) -> EI;

template <directed_or_undirected G, typename ER>
  requires edge_iterator<G, ranges::iterator_t<ER>>
constexpr void erase_edges(G& g, ER uv_rng);

template <directed_or_undirected G>
constexpr void clear_edges(G& g, vertex_iterator_t<G>);
// clang-format on


//
// Directed API (outward)
//

// Directed API (outward): Types
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
using vertex_outward_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_outward_edge_range>;

template <directed G>
using vertex_outward_edge_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_outward_edge_range>;
template <directed G>
using vertex_outward_edge_difference_t =
      ranges::range_difference_t<typename graph_traits<G>::vertex_outward_edge_range>;


template <directed G>
using vertex_outward_vertex_range_t = typename graph_traits<G>::vertex_outward_vertex_range;
template <directed G>
using const_vertex_outward_vertex_range_t = typename graph_traits<G>::const_vertex_outward_vertex_range;

template <directed G>
using vertex_outward_vertex_iterator_t = ranges::iterator_t<typename graph_traits<G>::vertex_outward_vertex_range>;
template <directed G>
using const_vertex_outward_vertex_iterator_t =
      ranges::iterator_t<typename graph_traits<G>::const_vertex_outward_vertex_range>;
template <directed G>
using vertex_outward_vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_outward_vertex_range>;

template <directed G>
using vertex_outward_vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_outward_vertex_range>;
template <directed G>
using vertex_outward_vertex_difference_t =
      ranges::range_difference_t<typename graph_traits<G>::vertex_outward_vertex_range>;


// Directed API (outward): common outward functions
template <directed G>
constexpr auto outward_degree(const G& g, const_vertex_iterator_t<G> u) noexcept -> vertex_outward_edge_size_t<G>;


// Directed API (outward): Vertex-Edge functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
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

// Directed API (outward): Vertex-Vertex functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
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

// Directed API (outward): Modifying functions
template <directed G>
constexpr void clear_outward_edges(G& g, vertex_iterator_t<G> u);

// also see erase_edge(g,uv), erase_edges(g,uv_rng)


//
// Directed API (inward)
//

// Directed API (inward): Types
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
using vertex_inward_edge_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_inward_edge_range>;

template <directed G>
using vertex_inward_edge_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_inward_edge_range>;
template <directed G>
using vertex_inward_edge_difference_t = ranges::range_difference_t<typename graph_traits<G>::vertex_inward_edge_range>;


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
using vertex_inward_vertex_sentinel_t = ranges::sentinel_t<typename graph_traits<G>::vertex_inward_vertex_range>;

template <directed G>
using vertex_inward_vertex_size_t = ranges::range_size_t<typename graph_traits<G>::vertex_inward_vertex_range>;
template <directed G>
using vertex_inward_vertex_difference_t =
      ranges::range_difference_t<typename graph_traits<G>::vertex_inward_vertex_range>;


// Directed API (outward): common outward functions
template <directed G>
constexpr auto inward_degree(const G& g, const_vertex_iterator_t<G> u) noexcept -> vertex_inward_edge_size_t<G>;

// Directed API (inward): Vertex-Edge functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
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

// Directed API (inward): Vertex-Vertex functions
// includes ranges functions: begin(r)/end(r), cbegin(r)/cend(r), size(r), ssize(r), empty(r)
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


// Directed API (inward): Modifying Functions

template <directed G>
constexpr void clear_inward_edges(G& g, vertex_iterator_t<G> u);

// also see erase_edge(g,uv), erase_edges(g,uv_rng)

//
// type traits
//

//
// Concept definitions, part 2
//

// clang-format off

template<typename G>
concept graph_value_types = 
  requires(G& g1, G& g2) {
    graph_traits<G>::graph_type;
    graph_traits<G>::graph_value_type;
    graph_traits<G>::allocator_type;
    is_same_v<G, graph_traits<G>::graph_type>;
    semiregular<graph_value_t<G>>;
    { graph_value(g1) } -> convertible_to<graph_value_t<G>&>;
    { swap(g1,g2) };
  };

template<typename G>
concept vertex_value_types = 
  requires(G& g) {
    graph_traits<G>::vertex_type;
    graph_traits<G>::vertex_key_type;
    graph_traits<G>::vertex_value_type;
    semiregular<vertex_value_t<G>>;
  };

template<typename G>
concept edge_value_types = 
  requires(G& g) {
    graph_traits<G>::edge_type;
    graph_traits<G>::edge_key_type;
    graph_traits<G>::edge_value_type;
    graph_traits<G>::vertex_key_type;
    semiregular<edge_value_t<G>>;

    // edge_key_type is ordered_pair<K,K> | unordered_pair<K,K>
    derived_from<edge_key_t<G>, ordered_pair<vertex_key_t<G>,vertex_key_t<G>>> ||
    derived_from<edge_key_t<G>, unordered_pair<vertex_key_t<G>,vertex_key_t<G>>>;
  };

template<typename G, typename VR>
concept vertex_range = 
  ranges::forward_range<VR> &&
  ranges::sized_range<VR> &&
  requires(VR& vr, ranges::iterator_t<VR> u, vertex_key_t<G> ukey) {
    is_same_v<vertex_t<G>, ranges::range_value_t<VR>>;
    { vertex_key(vr,u) } -> convertible_to<vertex_key_t<G>>;
    { vertex_value(vr,u) } -> convertible_to<vertex_value_t<G>&>;
    { find_vertex(vr,ukey) } -> convertible_to<vertex_iterator_t<G>>;
    { contains_vertex(vr, ukey) } -> convertible_to<bool>;
    { size(vr) } -> convertible_to<ranges::range_size_t<VR>>;
    { ssize(vr) } -> convertible_to<ranges::range_difference_t<VR>>;
    { empty(vr) } -> convertible_to<bool>;
  };

template<typename G, typename R>
concept basic_edge_range = 
  ranges::forward_range<R> &&
  requires(G&                                    g, 
           R&                                    r,
           ranges::iterator_t<R>                 uv, 
           ranges::iterator_t<vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_range_t<G>> src, 
           vertex_key_t<G>                       src_key) {
    { vertex(g, uv, src) } -> convertible_to<ranges::iterator_t<vertex_range_t<G>>>;
    { vertex_key(g, uv, src_key) } -> convertible_to<ranges::iterator_t<vertex_range_t<G>>>;
    { target_vertex(g, uv) } -> convertible_to<ranges::iterator_t<vertex_range_t<G>>>;
    { target_vertex_key(g, uv) } -> convertible_to<vertex_key_t<G>>;
    { source_vertex(g, uv) } -> convertible_to<ranges::iterator_t<vertex_range_t<G>>>;
    { source_vertex_key(g, uv) } -> convertible_to<vertex_key_t<G>>;
    { size(r) } -> convertible_to<ranges::range_size_t<R>>;
    { ssize(r) } -> convertible_to<ranges::range_difference_t<R>>;
    { empty(r) } -> convertible_to<bool>;
  };

template<typename G, typename R>
concept incidence_edge_range = 
  basic_edge_range<G,R> &&
  ranges::sized_range<R> &&
  is_same_v<ranges::range_value_t<R>, edge_t<G>> &&
  requires(G& g, R& r, ranges::iterator_t<R> uv) {
    { edge_key(g,uv) }   -> same_as<edge_key_t<G>>;
    { edge_value(g,uv) } -> same_as<edge_value_t<G>&>;
    //{ degree(r) } -> same_as<decltype(ranges::size(r))>;
  };

template<typename G, typename R>
concept adjacency_edge_range = 
  basic_edge_range<G,R> &&
  ranges::sized_range<R> &&
  is_same_v<ranges::range_value_t<R>, vertex_t<G>> &&
  requires(R& r) {
    true;
    //{ degree(r) } -> same_as<decltype(ranges::size(r))>;
  };

template <typename G>
concept vertex_list_graph = 
  vertex_range<G, vertex_range_t<G>> && // vertex_range is a vertex_range
  vertex_range<G,G> &&                  // graph is an alias of vertex_range
  graph_value_types<G> &&
  vertex_value_types<G> &&
  requires(G& g) {
    { vertices(g) } -> same_as<vertex_range_t<G>>;
  };

template <typename G>
concept incidence_graph = 
  requires(G&                                    g, 
           ranges::iterator_t<vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey) {
    vertex_list_graph<G>;
    incidence_edge_range<G, vertex_edge_range_t<G>>;
    edge_value_types<G>;
    { edges(g,u) }                      -> convertible_to<vertex_edge_range_t<G>>;
    { find_vertex_edge(g, u, v) }       -> convertible_to<ranges::iterator_t<vertex_edge_range_t<G>>>;
    { find_vertex_edge(g, ukey, vkey) } -> convertible_to<ranges::iterator_t<vertex_edge_range_t<G>>>;
  };

template <typename G>
concept adjacency_graph = 
  vertex_list_graph<G> &&
  adjacency_edge_range<G, vertex_vertex_range_t<G>> &&
  requires(G&                                    g, 
           ranges::iterator_t<vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey) {
    { vertices(g,u) }                   -> convertible_to<vertex_vertex_range_t<G>>;
    { find_vertex_vertex(g,u,v) }       -> convertible_to<ranges::iterator_t<vertex_vertex_range_t<G>>>;
    { find_vertex_vertex(g,ukey,vkey) } -> convertible_to<ranges::iterator_t<vertex_vertex_range_t<G>>>;
  };

template <typename G>
concept edge_list_graph = 
  vertex_list_graph<G> &&
  basic_edge_range<G, edge_range_t<G>> &&
  is_same_v<ranges::range_value_t<edge_range_t<G>>, edge_t<G>> &&
  edge_value_types<G> &&
  requires(G&                                    g, 
           ranges::iterator_t<vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey) {
    { edges(g) }                 -> convertible_to<edge_range_t<G>>;
    { find_edge(g, u, v) }       -> convertible_to<ranges::iterator_t<edge_range_t<G>>>;
    { find_edge(g, ukey, vkey) } -> convertible_to<ranges::iterator_t<edge_range_t<G>>>;
  };

template<typename G>
concept outward_incidence_graph = 
  vertex_list_graph<G> &&
  incidence_edge_range<G, vertex_outward_edge_range_t<G>> &&
  edge_value_types<G> &&
  requires(G&                                                 g, 
           ranges::iterator_t<vertex_outward_edge_range_t<G>> u, 
           ranges::iterator_t<vertex_outward_edge_range_t<G>> v,
           vertex_key_t<G>                                    ukey,
           vertex_key_t<G>                                    vkey) {
    { vertex_outward_edges(g,u) }               
      -> convertible_to<vertex_outward_edge_range_t<G>>;
    { find_vertex_outward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_outward_edge_range_t<G>>>;
    { find_vertex_outward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_outward_edge_range_t<G>>>;
  };

template <typename G>
concept outward_adjacency_graph = 
  vertex_list_graph<G> &&
  adjacency_edge_range<G, vertex_outward_vertex_range_t<G>> &&
  edge_value_types<G> &&
  requires(G&                                                   g, 
           ranges::iterator_t<vertex_outward_vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_outward_vertex_range_t<G>> v,
           vertex_key_t<G>                                      ukey,
           vertex_key_t<G>                                      vkey) {
    { vertex_outward_edges(g,u) }               
      -> convertible_to<vertex_outward_vertex_range_t<G>>;
    { find_vertex_outward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_outward_vertex_range_t<G>>>;
    { find_vertex_outward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_outward_vertex_range_t<G>>>;
  };

template<typename G>
concept inward_incidence_graph = 
  vertex_list_graph<G> &&
  incidence_edge_range<G, vertex_inward_edge_range_t<G>> &&
  edge_value_types<G> &&
  requires(G&                                                g, 
           ranges::iterator_t<vertex_inward_edge_range_t<G>> u, 
           ranges::iterator_t<vertex_inward_edge_range_t<G>> v,
           vertex_key_t<G>                                   ukey,
           vertex_key_t<G>                                   vkey) {
    { vertex_inward_edges(g,u) }               
      -> convertible_to<vertex_inward_edge_range_t<G>>;
    { find_vertex_inward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_inward_edge_range_t<G>>>;
    { find_vertex_inward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_inward_edge_range_t<G>>>;
  };

template <typename G>
concept inward_adjacency_graph = 
  vertex_list_graph<G> &&
  adjacency_edge_range<G, vertex_inward_vertex_range_t<G>> &&
  edge_value_types<G> &&
  requires(G&                                                  g, 
           ranges::iterator_t<vertex_inward_vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_inward_vertex_range_t<G>> v,
           vertex_key_t<G>                                     ukey,
           vertex_key_t<G>                                     vkey) {
    { vertex_inward_edges(g,u) }               
      -> convertible_to<vertex_inward_vertex_range_t<G>>;
    { find_vertex_inward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_inward_vertex_range_t<G>>>;
    { find_vertex_inward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_inward_vertex_range_t<G>>>;
  };

template <typename G>
concept adjacency_matrix = 
  requires(ranges::iterator_t<vertex_range_t<G>> u, 
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey) {
    { contains_edge(u,v) } -> convertible_to<bool>;
    { contains_edge(ukey,vkey) } -> convertible_to<bool>;
  };

template <typename G>
concept incremental_vertex_graph = 
  requires(G&                  g, 
          vertex_value_t<G>&& val) {
    { create_vertex(g) } -> convertible_to<optional<ranges::iterator_t<vertex_range_t<G>>>>;
    { create_vertex(g,val) } -> convertible_to<optional<ranges::iterator_t<vertex_range_t<G>>>>;
    { create_vertex(g,move(val)) } 
      -> convertible_to<optional<ranges::iterator_t<vertex_range_t<G>>>>;
    //{create_vertices(g,vrng,vvalue_fnc)};
  };

template <typename G>
concept decremental_vertex_graph = 
  requires(G&                                    g,
           ranges::iterator_t<vertex_range_t<G>> u,
           vertex_key_t<G>                       ukey,
           vertex_range_t<G>&                    r) {
    { erase_vertex(g, u) };
    { erase_vertex(g, ukey) };
    { erase_vertices(g, r) };
  };

template <typename G>
concept dynamic_vertex_graph = incremental_vertex_graph<G> && 
                               decremental_vertex_graph<G>;
template <typename G>
concept static_vertex_graph = !incremental_vertex_graph<G> && 
                              !decremental_vertex_graph<G>;

template <typename G>
concept incremental_edge_graph = 
  requires(G&                                    g,
           ranges::iterator_t<vertex_range_t<G>> u,
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey,
           edge_value_t<G>&&                     val) {
    {create_edge(g,u,v)} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;
    {create_edge(g,u,v,val)} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;
    {create_edge(g,u,v,move(val))} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;
    
    {create_edge(g,ukey,vkey)} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;
    {create_edge(g,ukey,vkey,val)} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;
    {create_edge(g,ukey,vkey,move(val))} -> convertible_to<optional<vertex_edge_iterator_t<G>>>;

    //{create_edges(g,erng,ekey_fnc,evalue_fnc)};
    //{create_edges(g,erng,vrng,ekey_fnc,evalue_fnc,vvalue_fnc)};
  };
  // consider: edges with required value that aren't default-constructable.
  // We need to support either this, or creation with a value, to have a complete definition.

template <typename G, typename ER>
concept decremental_edge_range = 
  requires(G&                                    g,
           ER                                    er,
           ranges::iterator_t<ER>                uv) {
    is_same_v<ranges::range_value_t<ER>, edge_value_t<G>>;
    { erase_edge(g,uv) } -> std::same_as<optional<ranges::iterator_t<ER>>>;
    { erase_edges(g,er) };
  };

template <typename G>
concept decremental_edge_graph = 
  (decremental_edge_range<G,vertex_edge_range_t<G>> || 
    decremental_edge_range<G,vertex_vertex_range_t<G>> || 
    decremental_edge_range<G,vertex_edge_range_t<G>>) &&
  requires(G&                                    g,
           ranges::iterator_t<vertex_range_t<G>> u,
           ranges::iterator_t<vertex_range_t<G>> v,
           vertex_key_t<G>                       ukey,
           vertex_key_t<G>                       vkey) {
    { erase_edge(g,u,v) };
    { erase_edge(g,ukey,vkey) };
  };
template <typename G>
concept dynamic_edge_graph = incremental_edge_graph<G> && decremental_edge_graph<G>;
template <typename G>
concept static_edge_graph = !incremental_edge_graph<G> && !decremental_edge_graph<G>;

template <typename G>
concept dynamic_graph = dynamic_vertex_graph<G> && dynamic_edge_graph<G>;
template <typename G>
concept static_graph = static_vertex_graph<G> && static_edge_graph<G>;


template <typename G, typename Path>
concept vertex_path = ranges::forward_range<Path> && 
                      convertible_to<ranges::range_value_t<Path>, vertex_iterator_t<G>>;

template <typename G, typename Path>
concept edge_path = ranges::forward_range<Path> && 
                      (const_edge_iterator<G,ranges::range_value_t<Path>> || 
                        edge_iterator<G,ranges::range_value_t<Path>>); 

template <typename G, typename Path>
concept vertex_cycle = ranges::forward_range<Path> && 
                      convertible_to<ranges::range_value_t<Path>, vertex_iterator_t<G>>;

// clang-format on

} // namespace std

#endif // GRAPH_FWD_HPP

#include "detail/graph_impl.hpp"
