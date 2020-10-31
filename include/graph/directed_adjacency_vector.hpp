//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include "ordered_pair.hpp"
#include <vector>
#include <cassert>

#ifndef DIRECTED_ADJ_ARRAY_HPP
#  define DIRECTED_ADJ_ARRAY_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// directed_adjacency_vector forward declarations
///
/// All vertices are kept in a single vector with an index for the first outward edge.
///
/// All edges are kept in a single vector in the graph. Outgoing edges for a vertex are
/// stored contiguously. Edges for vertex v must come after the previous vertex's edges.
/// An edge holds the index for its outward vertex in the vertices vector, plus any
/// user-defined values.

template <typename VV   = empty_value,
          typename EV   = empty_value,
          typename GV   = empty_value,
          integral KeyT = uint32_t,
          typename A    = allocator<char>>
class directed_adjacency_vector;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_vertex;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_edge;


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto size(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g) noexcept
      -> vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto ssize(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g) noexcept
      -> vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cbegin(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cend(directed_adjacency_vector<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;


///-------------------------------------------------------------------------------------
/// dav_edge
///
/// @tparam VV   Vertex Value type. default = empty_value.
/// @tparam EV   Edge Value type. default = empty_value.
/// @tparam GV   Graph Value type. default = empty_value.
/// @tparam KeyT The type used for the vertex key, and index into edge vector
/// @tparam A    Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV> {
public:
  using base_type  = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, A>;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_key_type        = KeyT;
  using vertex_index           = KeyT;
  using vertex_user_value_type = VV;

  using edge_key_type        = ordered_pair<vertex_key_type, vertex_key_type>;
  using edge_user_value_type = EV;
  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_index           = KeyT;

public:
  dav_edge()                    = default;
  dav_edge(dav_edge const&)     = default;
  dav_edge(dav_edge&&) noexcept = default;
  ~dav_edge() noexcept          = default;
  dav_edge& operator=(dav_edge&) = default;
  dav_edge& operator=(dav_edge&&) = default;

  dav_edge(vertex_set const& vertices, vertex_iterator inward_vertex, vertex_iterator outward_vertex);
  dav_edge(vertex_set const& vertices,
           vertex_iterator   inward_vertex,
           vertex_iterator   outward_vertex,
           edge_user_value_type const&);
  dav_edge(vertex_set const& vertices,
           vertex_iterator   inward_vertex,
           vertex_iterator   outward_vertex,
           edge_user_value_type&&);

  vertex_iterator       inward_vertex(graph_type&);
  const_vertex_iterator inward_vertex(graph_type const&) const;
  vertex_key_type       inward_vertex_key(graph_type const&) const;

  vertex_iterator       outward_vertex(graph_type&);
  const_vertex_iterator outward_vertex(graph_type const&) const;
  vertex_key_type       outward_vertex_key(graph_type const&) const;

  edge_key_type edge_key() const;

private:
  vertex_key_type inward_vertex_;
  vertex_key_type outward_vertex_;
};

///-------------------------------------------------------------------------------------
/// dav_vertex
///
/// @tparam VV   Vertex Value type. default = empty_value.
/// @tparam EV   Edge Value type. default = empty_value.
/// @tparam GV   Graph Value type. default = empty_value.
/// @tparam KeyT The type used for the vertex key, and index into edge vector
/// @tparam A    Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
public:
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, A>;
  using base_type  = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_key_type        = KeyT;
  using vertex_index           = KeyT;
  using vertex_user_value_type = VV;

  using edge_user_value_type = EV;
  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = KeyT;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using vertex_edge_size_type      = typename edge_set::size_type;
  using vertex_edge_iterator       = edge_iterator;
  using const_vertex_edge_iterator = const_edge_iterator;

  using vertex_outward_edge_size_type      = typename edge_set::size_type;
  using vertex_outward_edge_iterator       = edge_iterator;
  using const_vertex_outward_edge_iterator = const_edge_iterator;

public:
  dav_vertex() noexcept             = default;
  dav_vertex(dav_vertex const&)     = default;
  dav_vertex(dav_vertex&&) noexcept = default;
  ~dav_vertex() noexcept            = default;
  dav_vertex& operator=(dav_vertex const&) = default;
  dav_vertex& operator=(dav_vertex&&) = default;

  dav_vertex(vertex_set& vertices, vertex_index index);
  dav_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  dav_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&);

  void set_edge_begin(graph_type&, edge_iterator);

  vertex_edge_iterator       edges_begin(graph_type&);
  const_vertex_edge_iterator edges_begin(graph_type const&) const;
  const_vertex_edge_iterator edges_cbegin(graph_type const&) const;

  vertex_edge_iterator       edges_end(graph_type&);
  const_vertex_edge_iterator edges_end(graph_type const&) const;
  const_vertex_edge_iterator edges_cend(graph_type const&) const;

  edge_size_type edges_size(graph_type const&) const;

private:
  edge_index first_edge_ = numeric_limits<edge_index>::max();
};

/// A simple semi-mutable graph emphasizing performance and space.
///
/// directed_adjacency_vector is a compressed adjacency array graph with the following characteristics:
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
/// When constructing the directed_adjacency_vector, vertices are identified by their index in the vertex
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
/// @tparam VV   Vertex Value type. default = empty_value.
/// @tparam EV   Edge Value type. default = empty_value.
/// @tparam GV   Graph Value type. default = empty_value.
/// @tparam KeyT The type used for the vertex key, and index into edge vector
/// @tparam A    Allocator. default = std::allocator
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class directed_adjacency_vector : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using base_type             = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_type            = directed_adjacency_vector<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = GV;
  using allocator_type        = A;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_user_value_type = VV;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_ssize_type      = typename vertex_set::difference_type;
  using vertex_key_type        = KeyT;
  using vertex_index_type      = KeyT;
  using vertex_value_type      = vertex_type;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range          = decltype(ranges::make_subrange(declval<vertex_set&>()));
  using const_vertex_range    = decltype(ranges::make_subrange(declval<vertex_set const&>()));

  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = EV;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_size_type       = typename edge_set::size_type;
  using edge_ssize_type      = typename edge_set::difference_type;
  using edge_index_type      = KeyT;
  using edge_key_type        = ordered_pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type      = pair<edge_key_type, edge_user_value_type>;

  using edge_iterator       = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
  using edge_range          = decltype(ranges::make_subrange(declval<edge_set&>()));
  using const_edge_range    = decltype(ranges::make_subrange(declval<edge_set const&>()));

  using vertex_outward_edge_size_type      = typename edge_set::size_type;
  using vertex_outward_edge_ssize_type     = typename edge_set::difference_type;
  using vertex_outward_edge_iterator       = typename edge_range::iterator;
  using const_vertex_outward_edge_iterator = typename const_edge_range::iterator;
  using vertex_outward_edge_range          = edge_range;
  using const_vertex_outward_edge_range    = const_edge_range;

  using vertex_edge_size_type      = vertex_outward_edge_size_type;
  using vertex_edge_ssize_type     = vertex_outward_edge_ssize_type;
  using vertex_edge_iterator       = vertex_outward_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_outward_edge_iterator;
  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;

public:
  directed_adjacency_vector() noexcept(noexcept(allocator_type()))    = default;
  directed_adjacency_vector(directed_adjacency_vector&& rhs) noexcept = default;
  directed_adjacency_vector(directed_adjacency_vector const&)         = default;

  directed_adjacency_vector(allocator_type const& alloc) noexcept;
  directed_adjacency_vector(graph_user_value_type const&, allocator_type const& alloc = allocator_type());
  directed_adjacency_vector(graph_user_value_type&&, allocator_type const& alloc = allocator_type());

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
  ///                   ekey_fnc(ERng::value_type) -> directed_adjacency_vector::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param vvalue_fnc The vertex value extractor functor:
  ///                   vvalue_fnc(VRng::value_type) -> vertex_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, 
            typename EKeyFnc, 
            typename EValueFnc, 
            typename VRng, 
            typename VValueFnc>
    requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> 
          && vertex_range_extractor<VRng, VValueFnc>
  directed_adjacency_vector(ERng const&     erng,
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
  ///                   ekey_fnc(ERng::value_type) -> directed_adjacency_vector::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, typename EKeyFnc, typename EValueFnc>
    requires edge_range_extractor<ERng, EKeyFnc, EValueFnc>
  directed_adjacency_vector(ERng const&     rng, 
                           EKeyFnc const&   ekey_fnc, 
                           EValueFnc const& evalue_fnc, 
                           GV const&        gv = GV(), 
                           A const&         alloc = A());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 3 edge elements: inward_vertex_key,
  /// outward_vertex_key and edge_value.
  ///
  /// @param ilist Initializer list of tuples with inward_vertex_key,
  ///              outward_vertex_key and the edge value.
  /// @param alloc Allocator.
  ///
  directed_adjacency_vector(
        initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist,
        A const&                                                                               alloc = A());

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 2 edge elements.
  ///
  /// @param ilist Initializer list of tuples with inward_vertex_key and
  ///              outward_vertex_key.
  /// @param alloc Allocator.
  ///
  directed_adjacency_vector(initializer_list<tuple<vertex_key_type, vertex_key_type>> const& ilist,
                            A const&                                                         alloc = A());

  ~directed_adjacency_vector() = default;

  directed_adjacency_vector& operator=(directed_adjacency_vector const&) = default;
  directed_adjacency_vector& operator=(directed_adjacency_vector&&) = default;

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

  vertex_iterator       find_vertex(vertex_key_type);
  const_vertex_iterator find_vertex(vertex_key_type) const;

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

  edge_iterator create_edge(vertex_key_type, vertex_key_type);
  edge_iterator create_edge(vertex_key_type, vertex_key_type, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type,
                            vertex_key_type,
                            EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

public:
  void clear();

protected:
  vertex_iterator finalize_outward_edges(vertex_range);
  void            throw_unordered_edges() const;

private:
  vertex_set     vertices_;
  edge_set       edges_;
  allocator_type alloc_;
};


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
struct graph_traits<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  using graph_type            = directed_adjacency_vector<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = typename graph_type::graph_user_value_type;
  using allocator_type        = typename graph_type::allocator_type;

  using vertex_type            = typename graph_type::vertex_type;
  using vertex_key_type        = typename graph_type::vertex_key_type;
  using vertex_user_value_type = typename graph_type::vertex_user_value_type;
  using vertex_value_type      = typename graph_type::vertex_value_type;

  using edge_type            = typename graph_type::edge_type;
  using edge_key_type        = typename graph_type::edge_key_type; // ordered_pair<from,to>
  using edge_user_value_type = typename graph_type::edge_user_value_type;
  using edge_value_type      = typename graph_type::edge_value_type;

  using vertex_range       = typename graph_type::vertex_range;
  using const_vertex_range = typename graph_type::const_vertex_range;
  using vertex_size_type   = typename graph_type::vertex_size_type;

  using edge_range       = typename graph_type::edge_range;
  using const_edge_range = typename graph_type::const_edge_range;
  using edge_size_type   = typename graph_type::edge_size_type;

  using vertex_outward_edge_range       = typename graph_type::vertex_outward_edge_range;
  using const_vertex_outward_edge_range = typename graph_type::const_vertex_outward_edge_range;
  using vertex_outward_edge_size_type   = typename graph_type::vertex_outward_edge_size_type;

  using vertex_edge_range       = vertex_outward_edge_range;
  using const_vertex_edge_range = const_vertex_outward_edge_range;
  using vertex_edge_size_type   = vertex_outward_edge_size_type;

  using vertex_vertex_range       = void;
  using const_vertex_vertex_range = void;
  using vertex_vertex_size_type   = void;
};


} // namespace std::graph

#endif // DIRECTED_ADJ_ARRAY_HPP

#include "detail/directed_adjacency_vector_impl.hpp"
