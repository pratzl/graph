//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <cassert>
#include <range/v3/view/subrange.hpp>

#ifndef DIRECTED_ADJ_ARRAY_HPP
#  define DIRECTED_ADJ_ARRAY_HPP

namespace std::graph {

// clang-format off
// Input vertex data requires a range of data & a function to extract & return 
// the vertex value.
template <typename VRng, typename VValueFnc>
concept daa_vertex_data = ::ranges::input_range<VRng>
                        && invocable<VValueFnc, typename VRng::value_type>;

// Input edge data requires a range of data & functions to extract & return 
// the edge key and edge value. ERng is a forward_range because it is 
// is traversed twice; once to get the max vertex_key and a second time to
// load the edges.
template <typename ERng, typename EKeyFnc, typename EValueFnc>
concept daa_edge_data = ::ranges::forward_range<ERng>
                        && invocable<EKeyFnc, typename ERng::value_type>
                        && invocable<EValueFnc, typename ERng::value_type>;
// clang-format on

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
  ~daa_edge() noexcept          = default;
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
  daa_vertex() noexcept             = default;
  daa_vertex(daa_vertex const&)     = default;
  daa_vertex(daa_vertex&&) noexcept = default;
  ~daa_vertex() noexcept            = default;
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
  using base_type                   = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_type                  = daa_graph<VV, EV, GV, IndexT, A>;
  using const_graph_type            = const graph_type;
  using graph_user_value_type       = GV;
  using const_graph_user_value_type = const GV;
  using allocator_type              = A;

  using vertex_type                  = daa_vertex<VV, EV, GV, IndexT, A>;
  using const_vertex_type            = const vertex_type;
  using vertex_user_value_type       = VV;
  using const_vertex_user_value_type = const VV;
  using vertex_allocator_type        = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set                   = vector<vertex_type, vertex_allocator_type>;
  using vertex_size_type             = typename vertex_set::size_type;
  using vertex_index_type            = IndexT;
  using vertex_key_type              = vertex_index_type;
  using const_vertex_key_type        = const vertex_key_type;
  using vertex_value_type            = vertex_type;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range          = decltype(::ranges::make_subrange(declval<vertex_set&>()));
  using const_vertex_range    = decltype(::ranges::make_subrange(declval<vertex_set const&>()));

  using edge_type                  = daa_edge<VV, EV, GV, IndexT, A>;
  using const_edge_type            = const edge_type;
  using edge_user_value_type       = EV;
  using const_edge_user_value_type = const EV;
  using edge_allocator_type        = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set                   = vector<edge_type, edge_allocator_type>;
  using edge_size_type             = typename edge_set::size_type;
  using edge_index_type            = IndexT;
  using edge_key_type              = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type            = pair<edge_key_type, edge_user_value_type>;

  using edge_iterator       = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
  using edge_range          = decltype(::ranges::make_subrange(declval<edge_set&>()));
  using const_edge_range    = decltype(::ranges::make_subrange(declval<edge_set const&>()));

  using vertex_out_edge_size_type      = typename edge_set::size_type;
  using vertex_out_edge_iterator       = typename edge_range::iterator;
  using const_vertex_out_edge_iterator = typename const_edge_range::iterator;
  using vertex_out_edge_range          = edge_range;
  using const_vertex_out_edge_range    = const_edge_range;

  using vertex_edge_size_type      = vertex_out_edge_size_type;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;
  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;

public:
  daa_graph() noexcept(noexcept(allocator_type())) = default;
  daa_graph(daa_graph&& rhs) noexcept              = default;
  daa_graph(daa_graph const&)                      = default;
  ~daa_graph()                                     = default;

  daa_graph(allocator_type const& alloc) noexcept;
  daa_graph(graph_user_value_type const&, allocator_type const& alloc = allocator_type());
  daa_graph(graph_user_value_type&&, allocator_type const& alloc = allocator_type());

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
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_user_value_type, or
  ///                   a type that edge_user_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_user_value_type default constructor will be used
  ///                   to initialize the value.
  /// @tparam VRng      The vertex data range.
  /// @tparam VValueFnc Function object to return the vertex_user_value_type,
  ///                   or a type that vertex_user_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   vertex_user_value_type default constructor will be
  ///                   used to initialize the value.
  ///
  /// @param erng       The container of edge data.
  /// @param vrng       The container of vertex data.
  /// @param ekey_fnc   The edge key extractor functor:
  ///                   ekey_fnc(ERng::value_type) -> daa_graph::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param vvalue_fnc The vertex value extractor functor:
  ///                   vvalue_fnc(VRng::value_type) -> vertex_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
    requires daa_edge_data<ERng, EKeyFnc, EValueFnc> && daa_vertex_data<VRng, VValueFnc>
  daa_graph(ERng const&      erng,
            VRng const&      vrng,
            EKeyFnc const&   ekey_fnc,
            EValueFnc const& evalue_fnc,
            VValueFnc const& vvalue_fnc,
            GV const&        gv    = GV(),
            A const&         alloc = A());
  // clang-format on

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_user_value_type, or
  ///                   a type that edge_user_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_user_value_type default constructor will be used
  ///                   to initialize the value.
  ///
  /// @param erng       The container of edge data.
  /// @param ekey_fnc   The edge key extractor functor:
  ///                   ekey_fnc(ERng::value_type) -> daa_graph::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, typename EKeyFnc, typename EValueFnc>
    requires daa_edge_data<ERng, EKeyFnc, EValueFnc>
  daa_graph(ERng const& erng, EKeyFnc const& ekey_fnc, EValueFnc const& evalue_fnc, GV const& gv = GV(), A const& alloc = A());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 3 edge elements: in_vertex_key,
  /// out_vertex_key and edge_value.
  ///
  /// @param ilist Initializer list of tuples with in_vertex_key,
  ///              out_vertex_key and the edge value.
  /// @param alloc Allocator.
  ///
  daa_graph(initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist,
            A const&                                                                               alloc = A());

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 2 edge elements.
  ///
  /// @param ilist Initializer list of tuples with in_vertex_key and
  ///              out_vertex_key.
  /// @param alloc Allocator.
  ///
  daa_graph(initializer_list<tuple<vertex_key_type, vertex_key_type>> const& ilist, A const& alloc = A());

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

  vertex_iterator       find_vertex(const_vertex_key_type);
  const_vertex_iterator find_vertex(const_vertex_key_type) const;

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


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
struct graph_traits<daa_graph<VV, EV, GV, IndexT, A>> {
  using graph_type                  = daa_graph<VV, EV, GV, IndexT, A>;
  using const_graph_type            = const graph_type;
  using graph_user_value_type       = GV;
  using const_graph_user_value_type = const GV;
  using allocator_type              = A;

  using vertex_type                  = daa_vertex<VV, EV, GV, IndexT, A>;
  using const_vertex_type            = const vertex_type;
  using vertex_user_value_type       = VV;
  using const_vertex_user_value_type = const VV;
  using vertex_allocator_type        = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set                   = vector<vertex_type, vertex_allocator_type>;
  using vertex_size_type             = typename vertex_set::size_type;
  using vertex_index_type            = IndexT;
  using vertex_key_type              = vertex_index_type;
  using const_vertex_key_type        = const vertex_key_type;
  using vertex_value_type            = vertex_type;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range          = decltype(::ranges::make_subrange(declval<vertex_set&>()));
  using const_vertex_range    = decltype(::ranges::make_subrange(declval<vertex_set const&>()));

  using edge_type                  = daa_edge<VV, EV, GV, IndexT, A>;
  using const_edge_type            = const edge_type;
  using edge_user_value_type       = EV;
  using const_edge_user_value_type = const EV;
  using edge_allocator_type        = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set                   = vector<edge_type, edge_allocator_type>;
  using edge_size_type             = typename edge_set::size_type;
  using edge_index_type            = IndexT;
  using edge_key_type              = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type            = pair<edge_key_type, edge_user_value_type>;

  using edge_iterator       = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
  using edge_range          = decltype(::ranges::make_subrange(declval<edge_set&>()));
  using const_edge_range    = decltype(::ranges::make_subrange(declval<edge_set const&>()));

  using vertex_out_edge_size_type      = typename edge_set::size_type;
  using vertex_out_edge_iterator       = typename edge_range::iterator;
  using const_vertex_out_edge_iterator = typename const_edge_range::iterator;
  using vertex_out_edge_range          = edge_range;
  using const_vertex_out_edge_range    = const_edge_range;

  using vertex_edge_size_type      = vertex_out_edge_size_type;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;
  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;
};


} // namespace std::graph

#endif // DIRECTED_ADJ_ARRAY_HPP

#include "detail/directed_adjacency_array_impl.hpp"
