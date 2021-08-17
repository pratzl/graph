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

    CHANGES FROM D1709R3b
    1.  All graph functions are assumed to use Customization Point Objects (CPO)
    2.  Use function calls to define graph, vertex & edge types (as in std::ranges::iterator_t)
    3.  Remove graph_traits (no longer needed b/c of #2)
    4.  Tightened up & simplified concepts
    5.  Remove functions related to creation or erasing of objects
    6.  Consolidate const & non-const versions into single definition b/c of CPO.
*/

#ifndef GRAPH2_FWD_HPP
#  define GRAPH2_FWD_HPP

namespace std {

// Graph types
template <typename G>
using graph_value_t = decltype(graph_value(declval<G&>()));

// Vertex types
template <typename G>
using vertex_range_t = decltype(vertices(declval<G&>()));
template <typename G>
using vertex_iterator_t = typename ranges::iterator_t<vertex_range_t<G>>;

// Vertex value types
template <typename G>
using vertex_t = typename ranges::range_value_t<vertex_range_t<G>>;
template <typename G>
using vertex_key_t = decltype(vertex_key(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_value_t = decltype(vertex_value(declval<G&>(), declval(vertex_iterator_t<G&>())));

// edge range types
template <typename G>
using edge_range_t = decltype(edges(declval<G&>()));
template <typename G>
using vertex_edge_range_t = decltype(edges(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_vertex_range_t = decltype(vertices(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_outward_edge_range_t = decltype(outward_edges(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_outward_vertex_range_t = decltype(outward_vertices(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_inward_edge_range_t = decltype(inward_edges(declval<G&>(), declval(vertex_iterator_t<G&>())));
template <typename G>
using vertex_inward_vertex_range_t = decltype(inward_vertices(declval<G&>(), declval(vertex_iterator_t<G&>())));

// edge value types (EI is an iterator of one of the previous edge ranges)
template <typename G, typename ER>
using edge_t = typename ranges::range_value_t<ER>;
template <typename G, typename EI>
using edge_key_t = decltype(edge_key(declval<G&>(), declval<EI>())); // e.g. pair<vertex_key_t<G>,vertex_key_t<G>>
template <typename G, typename EI>
using edge_value_t = decltype(edge_value(declval<G&>(), declval<EI>()));

// clang-format off
// iterator concepts
template<typename G, typename VI>
concept vertex_iterator = requires(G&& g, VI u) {
  forward_iterator<VI>;
  { vertex_key(g, u) };
};

template<typename G, typename EI>
concept edge_iterator = requires(G&& g, EI uv) {
  forward_iterator<EI>;
  { edge_key(g, uv) };
};

// graph concepts
template<typename G>
concept vertex_list_graph = requires(G&&                  g,
                                     vertex_range_t<G>    vr, 
                                     vertex_iterator_t<G> u, 
                                     vertex_key_t<G>      ukey) {
    ranges::forward_range<vertex_range_t<G>>;
    ranges::sized_range<vertex_range_t<G>>;
    vertex_t<G>;
    vertex_key_t<G>;
    { empty(vr) } -> convertible_to<bool>;
    { find_vertex(g,ukey) } -> convertible_to<vertex_iterator_t<G>>;
  };

template<typename G, typename ER>
concept basic_edge_range = 
  requires(G&&                    g, 
           ER                     er,
           ranges::iterator_t<ER> uv, 
           vertex_iterator_t<G>   u, 
           vertex_iterator_t<G>   src, 
           vertex_key_t<G>        src_key) {
    ranges::forward_range<ER>;
    ranges::sized_range<ER>;
    edge_t<G, ER>;
    edge_key_t<G, ranges::iterator_t<ER>>;
    //{ vertex(g, uv) } -> convertible_to<vertex_iterator_t<G>>;
    //{ vertex_key(g, uv) } -> convertible_to<vertex_iterator_t<G>>;
    { vertex(g, uv, src) } -> convertible_to<vertex_iterator_t<G>>;
    { vertex_key(g, uv, src_key) } -> convertible_to<vertex_key_t<G>>;
    { target_vertex(g, uv) } -> convertible_to<vertex_iterator_t<G>>;
    { target_vertex_key(g, uv) } -> convertible_to<vertex_key_t<G>>;
    { source_vertex(g, uv) } -> convertible_to<vertex_iterator_t<G>>;
    { source_vertex_key(g, uv) } -> convertible_to<vertex_key_t<G>>;
    { empty(er) } -> convertible_to<bool>;
  };

template <typename G>
concept incidence_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_edge_range_t<G>>;
    { degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_edge_range_t<G>>>;
    { find_vertex_edge(g, u, v) } -> convertible_to<ranges::iterator_t<vertex_edge_range_t<G>>>;
    { find_vertex_edge(g, ukey, vkey) } 
                                  -> convertible_to<ranges::iterator_t<vertex_edge_range_t<G>>>;
  };

template <typename G>
concept adjacency_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_vertex_range_t<G>>;
    { degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_vertex_range_t<G>>>;
    { find_vertex_vertex(g,u,v) }-> convertible_to<ranges::iterator_t<vertex_vertex_range_t<G>>>;
    { find_vertex_vertex(g,ukey,vkey) } 
                                 -> convertible_to<ranges::iterator_t<vertex_vertex_range_t<G>>>;
  };

template <typename G>
concept edge_list_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, edge_range_t<G>>;
    { find_edge(g, u, v) }       -> convertible_to<ranges::iterator_t<edge_range_t<G>>>;
    { find_edge(g, ukey, vkey) } -> convertible_to<ranges::iterator_t<edge_range_t<G>>>;
  };

template<typename G>
concept outward_incidence_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_outward_edge_range_t<G>>;
    { outward_degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_outward_edge_range_t<G>>>;
    { find_vertex_outward_edge(g, u, v) }       
          -> convertible_to<ranges::iterator_t<vertex_outward_edge_range_t<G>>>;
    { find_vertex_outward_edge(g, ukey, vkey) } 
          -> convertible_to<ranges::iterator_t<vertex_outward_edge_range_t<G>>>;
  };

template <typename G>
concept outward_adjacency_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_outward_vertex_range_t<G>>;
    { outward_degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_outward_edge_range_t<G>>>;
    { find_vertex_outward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_outward_vertex_range_t<G>>>;
    { find_vertex_outward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_outward_vertex_range_t<G>>>;
  };

template<typename G>
concept inward_incidence_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_inward_edge_range_t<G>>;
    { inward_degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_inward_edge_range_t<G>>>;
    { find_vertex_inward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_inward_edge_range_t<G>>>;
    { find_vertex_inward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_inward_edge_range_t<G>>>;
  };

template <typename G>
concept inward_adjacency_graph = 
  requires(G&&                  g, 
           vertex_iterator_t<G> u, 
           vertex_iterator_t<G> v,
           vertex_key_t<G>      ukey,
           vertex_key_t<G>      vkey) {
    vertex_list_graph<G>;
    basic_edge_range<G, vertex_inward_vertex_range_t<G>>;
    { inward_degree(g,u) } -> convertible_to<ranges::range_size_t<vertex_inward_vertex_range_t<G>>>;
    { find_vertex_inward_edge(g, u, v) }       
      -> convertible_to<ranges::iterator_t<vertex_inward_vertex_range_t<G>>>;
    { find_vertex_inward_edge(g, ukey, vkey) } 
      -> convertible_to<ranges::iterator_t<vertex_inward_vertex_range_t<G>>>;
  };

template <typename G>
concept adjacency_matrix = 
  requires(G&& g) {
    true; // used to tag algorithms that can take advantage of matrix layout
  };


// misc functionality concepts
template<typename G>
concept has_graph_value = requires(G&& g) {
    semiregular<graph_value_t<G>>;
  };

template<typename G>
concept has_vertex_value = requires(G&& g) {
    semiregular<vertex_value_t<G>>;
  };
  
template<typename G, typename EI>
concept has_edge_value = requires(G&& g, EI uv) {
    semiregular<edge_value_t<G,EI>>;
  };

template<typename G, typename EI>
concept has_contains_vertex = requires(G&&             g, 
                                       vertex_key_t<G> ukey) {
    { contains_vertex(g, ukey) } -> convertible_to<bool>;
};
  
template<typename G, typename EI>
concept has_contains_edge = requires(G&&                  g, 
                                     vertex_iterator_t<G> u, 
                                     vertex_iterator_t<G> v,
                                     vertex_key_t<G>      ukey,
                                     vertex_key_t<G>      vkey) {
  { contains_edge(g, u, v) -> convertible_to<bool> };
  { contains_edge(g, ukey, vkey) -> convertible_to<bool> };
};
  
// path and cycle concepts
template <typename G, typename Path>
concept vertex_path = 
          ranges::forward_range<Path> && 
          convertible_to<ranges::range_value_t<Path>, vertex_iterator_t<G>>;

template <typename G, typename Path>
concept edge_path = 
          ranges::forward_range<Path> && 
          edge_iterator<G,ranges::range_value_t<Path>>; 

template <typename G, typename Path>
concept vertex_cycle = 
          ranges::forward_range<Path> && 
          convertible_to<ranges::range_value_t<Path>, vertex_iterator_t<G>>;

template <typename G, typename Path>
concept edge_cycle = 
          ranges::forward_range<Path> && 
          edge_iterator<G,ranges::range_value_t<Path>>;

// clang-format on

} // namespace std

#endif // GRAPH2_FWD_HPP
