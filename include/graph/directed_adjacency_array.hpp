//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <cassert>
#include <range/v3/view/subrange.hpp>

#ifndef COMPRESSED_ADJ_ARRAY_HPP
#  define COMPRESSED_ADJ_ARRAY_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// directed_adjacency_array forward declarations
///
/// All vertices are kept in a single vector with an index for the first outgoing edge.
///
/// All edges are kept in a single vector in the graph. Outgoing edges for a vertex are
/// stored contiguously. Edges for vertex v must come after the previous vertex's edges.
/// An edge holds the index for its outgoing vertex in the vertices vector, plus any
/// user-defined values.

template <typename VV     = empty_value,
          typename EV     = empty_value,
          typename GV     = empty_value,
          typename IndexT = uint32_t,
          typename A      = allocator<char>>
class daa_graph;

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class daa_vertex;

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class daa_edge;

template <typename VV     = empty_value,
          typename EV     = empty_value,
          typename GV     = empty_value,
          typename IndexT = uint32_t,
          typename A      = allocator<char>>
using directed_adjacency_array = daa_graph<VV, EV, GV, IndexT, A>;


///-------------------------------------------------------------------------------------
/// daa_edge
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class daa_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV> {
public:
  using graph_type            = daa_graph<VV, EV, GV, IndexT, A>;
  using graph_user_value_type = GV;
  using base_type             = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;

  using vertex_type            = daa_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = daa_edge<VV, EV, GV, IndexT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_index           = IndexT;

public:
  daa_edge()                    = default;
  daa_edge(daa_edge const&)     = default;
  daa_edge(daa_edge&&) noexcept = default;
  ~daa_edge()                   = default;
  daa_edge& operator=(daa_edge&) = default;
  daa_edge& operator=(daa_edge&&) = default;

  daa_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex);
  daa_edge(vertex_set const& vertices,
           vertex_iterator   in_vertex,
           vertex_iterator   out_vertex,
           edge_user_value_type const&);
  daa_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex, edge_user_value_type&&);

  vertex_iterator       in_vertex(graph_type&);
  const_vertex_iterator in_vertex(graph_type const&) const;
  vertex_key_type       in_vertex_key(graph_type const&) const;

  vertex_iterator       out_vertex(graph_type&);
  const_vertex_iterator out_vertex(graph_type const&) const;
  vertex_key_type       out_vertex_key(graph_type const&) const;

private:
  vertex_key_type in_vertex_;
  vertex_key_type out_vertex_;
};

///-------------------------------------------------------------------------------------
/// daa_vertex
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class daa_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
public:
  using graph_type            = daa_graph<VV, EV, GV, IndexT, A>;
  using graph_user_value_type = GV;
  using base_type             = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_type            = daa_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = daa_edge<VV, EV, GV, IndexT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = IndexT;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using vertex_edge_size_type      = typename edge_set::size_type;
  using vertex_edge_iterator       = edge_iterator;
  using const_vertex_edge_iterator = const_edge_iterator;

  using vertex_out_edge_size_type      = typename edge_set::size_type;
  using vertex_out_edge_iterator       = edge_iterator;
  using const_vertex_out_edge_iterator = const_edge_iterator;

public:
  daa_vertex()                      = default;
  daa_vertex(daa_vertex const&)     = default;
  daa_vertex(daa_vertex&&) noexcept = default;
  ~daa_vertex()                     = default;
  daa_vertex& operator=(daa_vertex const&) = default;
  daa_vertex& operator=(daa_vertex&&) = default;

  daa_vertex(vertex_set& vertices, vertex_index index);
  daa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  daa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&);

  void set_edge_begin(graph_type&, edge_iterator);

  vertex_edge_iterator       edge_begin(graph_type&);
  const_vertex_edge_iterator edge_begin(graph_type const&) const;
  const_vertex_edge_iterator edge_cbegin(graph_type const&) const;

  vertex_edge_iterator       edge_end(graph_type&);
  const_vertex_edge_iterator edge_end(graph_type const&) const;
  const_vertex_edge_iterator edge_cend(graph_type const&) const;

  edge_size_type edge_size(graph_type const&) const;

private:
  edge_index first_edge_ = numeric_limits<edge_index>::max();
};

/// A simple semi-mutable graph emphasizing performance and space.
///
/// daa_graph is a compressed adjacency array graph with the following characteristics:
///		1.	a forward-only directed graph
///		2.	user-defined value types for vertices, edges and the graph.
///		3.	iterating over vertices occurs in O(V) and over edges in O(E) time.
///		4.	minimum vertex size is sizeof(size_t) when empty_value is used for the value type
///		5.	minimum edge size is sizeof(size_t)*2 when empty_value is used for the value type
///		6.	vertices and edges are stored in separate vectors (2 total).
///		7.	After the graph is constructed, vertices and edges cannot be added or removed.
///			Properties may be modified.
///
/// The time to construct the graph is O(V) + 2*O(E). The edges are scanned twice, the first
/// time to identify the largest vertex index referenced (so the internal vertex vector is
/// allocated only once), and the second time to build the internal edges vector.
///
/// When constructing the daa_graph, vertices are identified by their index in the vertex
/// container passed. Edges refer to their in/out vertices using the vertex index. If more
/// vertices are referred to in the edges container in the constructor, then the internal
/// vertex vector will be sized to accomodate the largest vertex index used by the edges.
///
/// constructors accept a variety of containers, depending on the template parameters.
/// vertices accept containers with 2 template parameters, like vector<T,A> and deque<T,A>
/// Different constructors exist to accomoate edges in different container types, also based
/// on the number of template parameters. This accomodates both common std containers and
/// non-std containers.
///
/// Constructors are the only public functions that should be used directly. Use the
/// templatized graph free functions to work with the graph otherwise. While public member functions
/// (besides constructors) may work, there is no guarantee they will work on all implementations.
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class daa_graph : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using graph_type            = daa_graph<VV, EV, GV, IndexT, A>;
  using base_type             = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_user_value_type = GV;
  using allocator_type        = A;
  using graph_category        = sparse_graph_tag;

  using vertex_type            = daa_vertex<VV, EV, GV, IndexT, A>;
  using const_vertex_type      = const vertex_type;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;
  using vertex_range           = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set*>(0)));
  using const_vertex_range     = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set const*>(0)));

  using edge_user_value_type = EV;
  using edge_type            = daa_edge<VV, EV, GV, IndexT, A>;
  using const_edge_type      = const edge_type;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = IndexT;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using edge_key_type   = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type = pair<edge_key_type, edge_user_value_type>;

  using edge_range       = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set*>(0)));
  using const_edge_range = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set const*>(0)));

  using vertex_out_edge_range          = edge_range;
  using const_vertex_out_edge_range    = const_edge_range;
  using vertex_out_edge_iterator       = typename edge_range::iterator;
  using const_vertex_out_edge_iterator = typename const_edge_range::iterator;
  using vertex_out_edge_size_type      = typename edge_set::size_type;

  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;
  using vertex_edge_size_type      = vertex_out_edge_size_type;

public:
  daa_graph()                 = default;
  daa_graph(daa_graph&& rhs)  = default;
  daa_graph(daa_graph const&) = default;

  daa_graph(allocator_type alloc);
  daa_graph(graph_user_value_type const&, allocator_type alloc = allocator_type());
  daa_graph(graph_user_value_type&&, allocator_type alloc = allocator_type());

  daa_graph& operator=(daa_graph const&) = default;
  daa_graph& operator=(daa_graph&&) = default;

  // The following constructors will load edges (and vertices) into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in
  // the edges. Edges are scanned first to determine the highest number and
  // the vertices are resized to match the number.
  //
  // Accessor functions are used to return the edge_key_type,
  // edge_user_value_type and vertex_user_value_type.
  //
  // The order visited in the vertices determines their index
  // (and key/identity) in the internal vertices vector. The edge keys use
  // those values and are also expected to be ordered by their first (in)
  // vertex key and an exception is thrown if they aren't in
  // order. For these reasons, unordered (hash) containers won't work.
  //

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng     The edge data range.
  /// @tparam EKeyFnc  Function object to return edge_key_type of the
  ///                  ERng::value_type.
  /// @tparam EPropFnc Function object to return the edge_user_value_type, or
  ///                  a type that edge_user_value_type is constructible
  ///                  from. If the return type is void or empty_value the
  ///                  edge_user_value_type default constructor will be used
  ///                  to initialize the value.
  /// @tparam VRng     The vertex data range.
  /// @tparam VPropFnc Function object to return the vertex_user_value_type,
  ///                  or a type that vertex_user_value_type is constructible
  ///                  from. If the return type is void or empty_value the
  ///                  vertex_user_value_type default constructor will be
  ///                  used to initialize the value.
  ///
  /// @param erng      The container of edge data.
  /// @param vrng      The container of vertex data.
  /// @param ekey_fnc  The edge key extractor function object.
  /// @param eprop_fnc The edge value extractor function object.
  /// @param vprop_fnc The vertex value extractor function object.
  /// @param alloc     The allocator to use for internal containers for
  ///                  vertices & edges.
  ///
  template <typename ERng, typename EKeyFnc, typename EPropFnc, typename VRng, typename VPropFnc>
  daa_graph(ERng const&     erng,
            VRng const&     vrng,
            EKeyFnc const&  ekey_fnc,  //= [](typename ERng::value_type const&) { return edge_key_type(); },
            EPropFnc const& eprop_fnc, //= [](typename ERng::value_type const&) { return empty_value(); },
            VPropFnc const& vprop_fnc, //= [](typename VRng::value_type const&) { return empty_value(); },
            GV const&       gv    = GV(),
            A               alloc = A());

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng     The edge data range.
  /// @tparam EKeyFnc  Function object to return edge_key_type of the
  ///                  ERng::value_type.
  /// @tparam EPropFnc Function object to return the edge_user_value_type, or
  ///                  a type that edge_user_value_type is constructible
  ///                  from. If the return type is void or empty_value the
  ///                  edge_user_value_type default constructor will be used
  ///                  to initialize the value.
  ///
  /// @param erng      The container of edge data.
  /// @param ekey_fnc  The edge key extractor function object.
  /// @param eprop_fnc The edge value extractor function object.
  /// @param alloc     The allocator to use for internal containers for
  ///                  vertices & edges.
  ///
  template <typename ERng, typename EKeyFnc, typename EPropFnc>
  daa_graph(ERng const&     erng,
            EKeyFnc const&  ekey_fnc,  // = [](typename ERng::value_type const&) { return edge_key_type(); },
            EPropFnc const& eprop_fnc, // = [](typename ERng::value_type const&) { return empty_value(); },
            GV const&       gv    = GV(),
            A               alloc = A());

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a struct/class/tuple with 3 edge elements: in_vertex_key,
  /// out_vertex_key, edge_value.
  /// Values must be convertible to vertex_key_t<G> & edge_value_t<G>.
  ///
  /// @tparam T The struct/class type used.
  ///
  /// @param ilist Initializer list of T
  /// @param alloc Allocator.
  ///
  template <typename T>
  daa_graph(initializer_list<T> const& ilist, A alloc = A());

public:
  constexpr vertex_set&       vertices();
  constexpr vertex_set const& vertices() const;

  constexpr vertex_iterator       begin();
  constexpr const_vertex_iterator begin() const;
  constexpr const_vertex_iterator cbegin() const;

  constexpr vertex_iterator       end();
  constexpr const_vertex_iterator end() const;
  constexpr const_vertex_iterator cend() const;

  constexpr edge_set&       edges();
  constexpr edge_set const& edges() const;

  vertex_iterator       find_vertex(vertex_key_type const&);
  const_vertex_iterator find_vertex(vertex_key_type const&) const;

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, vertex_user_value_type const&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_user_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(VV2 const&); // vertex_user_value_type must be constructable from VV2

protected:
  void reserve_edges(edge_size_type);

  edge_iterator create_edge(vertex_key_type const&, vertex_key_type const&);
  edge_iterator create_edge(vertex_key_type const&, vertex_key_type const&, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type const&,
                            vertex_key_type const&,
                            EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

public:
  void clear();

protected:
  vertex_iterator finalize_out_edges(vertex_range);
  void            throw_unordered_edges() const;

private:
  vertex_set     vertices_;
  edge_set       edges_;
  allocator_type alloc_;
};


} // namespace std::graph

#endif // COMPRESSED_ADJ_ARRAY_HPP

#include "detail/directed_adjacency_array_impl.hpp"
