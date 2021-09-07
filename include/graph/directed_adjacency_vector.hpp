//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <cassert>

#ifndef DIRECTED_ADJ_ARRAY_HPP
#  define DIRECTED_ADJ_ARRAY_HPP

namespace std::graph::containers {


///-------------------------------------------------------------------------------------
/// directed_adjacency_vector forward declarations
///
/// All vertices are kept in a single random-access container with an index for the
/// first outward edge in the edges container.
///
/// All edges are kept in a single random-access container in the graph. Outgoing edges
/// for a vertex are stored contiguously. Edges for vertex v must come after the
/// previous vertex's edges. An edge holds the index for its outward vertex in the
/// vertices container, plus any user-defined values.
///
/// A vector is used as the default container though any random-access container with
/// <T,A> (type, allocator) template parameters can be used.
///

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_vertex;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_edge;

// clang-format off
template <typename VV                                        = empty_value,
          typename EV                                        = empty_value,
          typename GV                                        = empty_value,
          integral KeyT                                      = uint32_t,
          template <typename V, typename A> class VContainer = vector,
          template <typename E, typename A> class EContainer = vector,
          typename Alloc                                     = allocator<char>>
class directed_adjacency_vector;
// clang-format on

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_const_vertex_vertex_iterator;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_vertex_vertex_iterator;


///-------------------------------------------------------------------------------------
/// dav_edge
///
/// @tparam VV   Vertex Value type. default = empty_value.
/// @tparam EV   Edge Value type. default = empty_value.
/// @tparam GV   Graph Value type. default = empty_value.
/// @tparam KeyT The type used for the vertex key, and index into edge container
/// @tparam A    Allocator. default = std::allocator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV> {
public:
  using base_type  = conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV>;
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;

  using vertex_type           = dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using vertex_value_type     = VV;
  using vertex_key_type       = KeyT;
  using vertex_index_type     = KeyT;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;

  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_difference_type = typename vertex_set::difference_type;

  using edge_type           = dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using edge_value_type     = EV;
  using edge_key_type       = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type     = KeyT;
  using edge_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_set            = EContainer<edge_type, edge_allocator_type>;

public:
  dav_edge()                    = default;
  dav_edge(const dav_edge&)     = default;
  dav_edge(dav_edge&&) noexcept = default;
  ~dav_edge() noexcept          = default;
  dav_edge& operator=(dav_edge&) = default;
  dav_edge& operator=(dav_edge&&) = default;

  dav_edge(vertex_key_type source_vertex_key, vertex_key_type target_vertex_key);
  dav_edge(vertex_key_type source_vertex_key, vertex_key_type target_vertex_key, const edge_value_type&);
  dav_edge(vertex_key_type source_vertex_key, vertex_key_type target_vertex_key, edge_value_type&&);

  vertex_iterator       source_vertex(graph_type&);
  const_vertex_iterator source_vertex(const graph_type&) const;
  vertex_key_type       source_vertex_key() const noexcept;

  vertex_iterator       target(graph_type&);
  const_vertex_iterator target(const graph_type&) const;
  vertex_key_type       target_key(const graph_type&) const noexcept;

  edge_key_type edge_key(const graph_type&) const noexcept;

  edge_value_type&       edge_value(graph_type&) noexcept;
  const edge_value_type& edge_value(const graph_type&) const noexcept;

private:
  vertex_key_type source_vertex_;
  vertex_key_type target_vertex_;
};

///-------------------------------------------------------------------------------------
/// dav_vertex
///
/// @tparam VV   Vertex Value type. default = empty_value.
/// @tparam EV   Edge Value type. default = empty_value.
/// @tparam GV   Graph Value type. default = empty_value.
/// @tparam KeyT The type used for the vertex key, and index into edge container
/// @tparam A    Allocator. default = std::allocator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV> {
public:
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using base_type  = conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV>;

  using vertex_type           = dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using vertex_value_type     = VV;
  using vertex_key_type       = KeyT;
  using vertex_index_type     = KeyT;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;

  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_difference_type = typename vertex_set::difference_type;

  using edge_type           = dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using edge_value_type     = EV;
  using edge_key_type       = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type     = KeyT;
  using edge_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_set            = EContainer<edge_type, edge_allocator_type>;

  using edge_range           = edge_set&;
  using const_edge_range     = const edge_set&;
  using edge_iterator        = ranges::iterator_t<edge_range>;
  using const_edge_iterator  = ranges::iterator_t<const_edge_range>;
  using edge_size_type       = ranges::range_size_t<edge_range>;
  using edge_difference_type = ranges::range_difference_t<edge_range>;

  using vertex_edge_range_type       = ranges::subrange<edge_iterator, edge_iterator, ranges::subrange_kind::sized>;
  using const_vertex_edge_range_type = ranges::subrange<edge_iterator, edge_iterator, ranges::subrange_kind::sized>;

public:
  dav_vertex() noexcept             = default;
  dav_vertex(const dav_vertex&)     = default;
  dav_vertex(dav_vertex&&) noexcept = default;
  ~dav_vertex() noexcept            = default;
  dav_vertex& operator=(const dav_vertex&) = default;
  dav_vertex& operator=(dav_vertex&&) = default;

  dav_vertex(vertex_set& vertices, vertex_index_type index);
  dav_vertex(vertex_set& vertices, vertex_index_type index, const vertex_value_type&);
  dav_vertex(vertex_set& vertices, vertex_index_type index, vertex_value_type&&);

  void            set_edge_begin(graph_type&, edge_iterator);
  void            set_edge_begin(edge_index_type);
  edge_index_type edge_begin_index() const;

public:
  // CPO member functions
  vertex_value_type&       vertex_value(graph_type&);
  const vertex_value_type& vertex_value(graph_type const&) const;

private:
  edge_index_type first_edge_ = numeric_limits<edge_index_type>::max();
};

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_const_vertex_vertex_iterator {
public:
  using this_t = dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;

  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;

  using vertex_type           = dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using vertex_value_type     = VV;
  using vertex_key_type       = KeyT;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;

  using edge_type       = dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using edge_value_type = typename edge_type::edge_value_type;
  using edge_key_type   = typename edge_type::edge_key_type; // <from,to>
  using edge_set        = typename edge_type::edge_set;

  using vertex_edge_iterator       = typename edge_set::iterator;
  using const_vertex_edge_iterator = typename edge_set::const_iterator;

  using iterator_category = random_access_iterator_tag;
  using value_type        = vertex_type;
  using size_type         = typename edge_set::size_type;
  using difference_type   = typename edge_set::difference_type;
  using pointer           = const value_type*;
  using reference         = const value_type&;

public:
  constexpr dav_const_vertex_vertex_iterator(graph_type& g, vertex_edge_iterator uv);

  constexpr dav_const_vertex_vertex_iterator()                                        = default;
  constexpr dav_const_vertex_vertex_iterator(const dav_const_vertex_vertex_iterator&) = default;
  constexpr dav_const_vertex_vertex_iterator(dav_const_vertex_vertex_iterator&&)      = default;
  ~dav_const_vertex_vertex_iterator()                                                 = default;

  constexpr dav_const_vertex_vertex_iterator& operator=(const dav_const_vertex_vertex_iterator&) = default;
  constexpr dav_const_vertex_vertex_iterator& operator=(dav_const_vertex_vertex_iterator&&) = default;

public:
  constexpr const_vertex_iterator target_vertex(graph_type const&) const;
  constexpr vertex_key_type       target_vertex_key() const;

  constexpr reference operator*() const noexcept;
  constexpr pointer   operator->() const noexcept;

  constexpr dav_const_vertex_vertex_iterator& operator++() noexcept;
  constexpr dav_const_vertex_vertex_iterator  operator++(int) noexcept;
  constexpr dav_const_vertex_vertex_iterator& operator+=(const difference_type distance) noexcept;
  constexpr dav_const_vertex_vertex_iterator  operator+(const difference_type distance) const noexcept;

  constexpr dav_const_vertex_vertex_iterator& operator--() noexcept;
  constexpr dav_const_vertex_vertex_iterator  operator--(int) noexcept;
  constexpr dav_const_vertex_vertex_iterator& operator-=(const difference_type distance) noexcept;
  constexpr dav_const_vertex_vertex_iterator  operator-(const difference_type distance) const noexcept;

  reference operator[](const difference_type distance) const noexcept { return *uv_[distance].target_vertex(*g_); }

  constexpr bool operator==(const dav_const_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator!=(const dav_const_vertex_vertex_iterator& rhs) const noexcept;

  constexpr bool operator>(const dav_const_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator<=(const dav_const_vertex_vertex_iterator& rhs) const noexcept;

  constexpr bool operator<(const dav_const_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator>=(const dav_const_vertex_vertex_iterator& rhs) const noexcept;


  friend void swap(dav_const_vertex_vertex_iterator& lhs, dav_const_vertex_vertex_iterator& rhs) {
    swap(lhs.g_, rhs.g_);
    swap(lhs.uv_, rhs.uv_);
  }

protected:
  graph_type*          g_ = nullptr;
  vertex_edge_iterator uv_;
};

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
class dav_vertex_vertex_iterator
      : public dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc> {
public:
  using this_t = dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using base_t = dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;

  using graph_type           = typename base_t::graph_type;
  using vertex_type          = typename base_t::vertex_type;
  using vertex_edge_iterator = typename base_t::vertex_edge_iterator;
  using vertex_iterator      = typename base_t::vertex_iterator;
  using edge_type            = typename base_t::edge_type;

  using iterator_category = typename base_t::iterator_category;
  using value_type        = typename base_t::value_type;
  using size_type         = typename base_t::size_type;
  using difference_type   = typename base_t::difference_type;
  using pointer           = value_type*;
  using reference         = value_type&;

protected:
  using base_t::g_;
  using base_t::uv_;

public:
  constexpr dav_vertex_vertex_iterator(graph_type& g, vertex_edge_iterator uv);

  constexpr dav_vertex_vertex_iterator()                                  = default;
  constexpr dav_vertex_vertex_iterator(const dav_vertex_vertex_iterator&) = default;
  constexpr dav_vertex_vertex_iterator(dav_vertex_vertex_iterator&&)      = default;
  ~dav_vertex_vertex_iterator()                                           = default;

  constexpr dav_vertex_vertex_iterator& operator=(const dav_vertex_vertex_iterator&) = default;
  constexpr dav_vertex_vertex_iterator& operator=(dav_vertex_vertex_iterator&&) = default;

public:
  constexpr vertex_iterator target_vertex(graph_type&);

  constexpr reference operator*() const;
  constexpr pointer   operator->() const;

  constexpr dav_vertex_vertex_iterator& operator++();
  constexpr dav_vertex_vertex_iterator  operator++(int);
  constexpr dav_vertex_vertex_iterator& operator+=(const difference_type distance) noexcept;
  constexpr dav_vertex_vertex_iterator  operator+(const difference_type distance) const noexcept;

  constexpr dav_vertex_vertex_iterator& operator--() noexcept;
  constexpr dav_vertex_vertex_iterator  operator--(int) noexcept;
  constexpr dav_vertex_vertex_iterator& operator-=(const difference_type distance) noexcept;
  constexpr dav_vertex_vertex_iterator  operator-(const difference_type distance) const noexcept;

  reference operator[](const difference_type distance) const noexcept;

  constexpr bool operator==(const dav_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator!=(const dav_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator>(const dav_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator<=(const dav_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator<(const dav_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator>=(const dav_vertex_vertex_iterator& rhs) const noexcept;

  friend void swap(dav_vertex_vertex_iterator& lhs, dav_vertex_vertex_iterator& rhs) {
    swap(lhs.g_, rhs.g_);
    swap(lhs.uv_, rhs.uv_);
  }
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
/// time to identify the largest vertex index referenced (so the internal vertex container is
/// allocated only once), and the second time to build the internal edges container.
///
/// When constructing the directed_adjacency_vector, vertices are identified by their index in the vertex
/// container passed. Edges refer to their in/out vertices using the vertex index. If more
/// vertices are referred to in the edges container in the constructor, then the internal
/// vertex container will be sized to accomodate the largest vertex index used by the edges.
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
/// @tparam VV              Vertex Value type. default = empty_value.
/// @tparam EV              Edge Value type. default = empty_value.
/// @tparam GV              Graph Value type. default = empty_value.
/// @tparam KeyT            The type used for the vertex key, and index into edge container
/// @tparam VContainer<V,A> Random-access container type used to store vertices (V) with allocator (A).
/// @tparam EContainer<E,A> Random-access Container type used to store edges (E) with allocator (A).
/// @tparam Alloc           Allocator. default = std::allocator
//
// clang-format off
template <typename                                VV,
          typename                                EV,
          typename                                GV,
          integral                                KeyT,
          template <typename V, typename A> class VContainer,
          template <typename E, typename A> class EContainer,
          typename                                Alloc>
class directed_adjacency_vector 
  : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV>
// clang-format on
{
public:
  using base_type        = conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV>;
  using graph_type       = directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using graph_value_type = GV;
  using allocator_type   = Alloc;

  using vertex_type           = dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using vertex_value_type     = VV;
  using vertex_key_type       = KeyT;
  using vertex_index_type     = KeyT;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;

  using vertex_range           = vertex_set&;
  using const_vertex_range     = const vertex_set&;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = ranges::range_size_t<vertex_range>;
  using vertex_difference_type = ranges::range_difference_t<vertex_range>;
  using vertex_subrange        = decltype(detail::make_subrange(declval<vertex_set&>()));

  using edge_type           = dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using edge_value_type     = EV;
  using edge_key_type       = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type     = KeyT;
  using edge_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_set            = EContainer<edge_type, edge_allocator_type>;

  using edge_range           = edge_set&;
  using const_edge_range     = const edge_set&;
  using edge_iterator        = ranges::iterator_t<edge_range>;
  using const_edge_iterator  = ranges::iterator_t<const_edge_range>;
  using edge_size_type       = ranges::range_size_t<edge_range>;
  using edge_difference_type = ranges::range_difference_t<edge_range>;

  using vertex_outward_size_type       = edge_size_type;
  using vertex_outward_difference_type = edge_difference_type;

  using vertex_outward_edge_range = ranges::subrange<edge_iterator, edge_iterator, ranges::subrange_kind::sized>;
  using const_vertex_outward_edge_range =
        ranges::subrange<const_edge_iterator, const_edge_iterator, ranges::subrange_kind::sized>;
  using vertex_outward_edge_iterator       = edge_iterator;
  using const_vertex_outward_edge_iterator = const_edge_iterator;

  using vertex_outward_vertex_iterator = dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using const_vertex_outward_vertex_iterator =
        dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>;
  using vertex_outward_vertex_range =
        ranges::subrange<vertex_outward_vertex_iterator, vertex_outward_vertex_iterator, ranges::subrange_kind::sized>;
  using const_vertex_outward_vertex_range = ranges::subrange<const_vertex_outward_vertex_iterator,
                                                             const_vertex_outward_vertex_iterator,
                                                             ranges::subrange_kind::sized>;

  using vertex_edge_range          = vertex_outward_edge_range;
  using const_vertex_edge_range    = const_vertex_outward_edge_range;
  using vertex_edge_iterator       = vertex_outward_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_outward_edge_iterator;

public:
  directed_adjacency_vector()                                         = default;
  directed_adjacency_vector(directed_adjacency_vector&& rhs) noexcept = default;
  directed_adjacency_vector(const directed_adjacency_vector&)         = default;

  // clang-format off
  directed_adjacency_vector(const allocator_type& alloc);
  directed_adjacency_vector(const graph_value_type&, 
                            const allocator_type& alloc = allocator_type());
  directed_adjacency_vector(graph_value_type&&, 
                            const allocator_type& alloc = allocator_type());
  // clang-format on

  // The following constructors will load edges (and vertices) into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in
  // the edges. Edges are scanned first to determine the highest number and
  // the vertices are resized to match the number.
  //
  // Accessor functions are used to return the edge_key_type and
  // vertex_value_type.
  //
  // The order visited in the vertices determines their index
  // (and key/identity) in the internal vertices container. The edge keys use
  // those values and are also expected to be ordered by their first (in)
  // vertex key and an exception is thrown if they aren't in
  // order. For these reasons, unordered (hash) containers won't work.
  //

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_value_type, or
  ///                   a type that edge_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_value_type default constructor will be used
  ///                   to initialize the value.
  /// @tparam VRng      The vertex data range.
  /// @tparam VValueFnc Function object to return the vertex_value_type,
  ///                   or a type that vertex_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   vertex_value_type default constructor will be
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
    requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> 
          && vertex_value_extractor<VRng, VValueFnc>
  directed_adjacency_vector(const ERng&      erng,
                            const VRng&      vrng,
                            const EKeyFnc&   ekey_fnc,
                            const EValueFnc& evalue_fnc,
                            const VValueFnc& vvalue_fnc,
                            const GV&        gv    = GV(),
                            const Alloc&     alloc = Alloc());
  // clang-format on

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_value_type, or
  ///                   a type that edge_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_value_type default constructor will be used
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
    requires edge_value_extractor<ERng, EKeyFnc, EValueFnc>
  directed_adjacency_vector(const ERng&      rng, 
                            const EKeyFnc&   ekey_fnc, 
                            const EValueFnc& evalue_fnc, 
                            const GV&        gv = GV(), 
                            const Alloc&     alloc = Alloc());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 3 edge elements: source_vertex_key,
  /// target_vertex_key and edge_value.
  ///
  /// @param ilist Initializer list of tuples with source_vertex_key,
  ///              target_vertex_key and the edge value.
  /// @param alloc Allocator.
  ///
  // clang-format off
  directed_adjacency_vector(
    const initializer_list<
          tuple<vertex_key_type, vertex_key_type, edge_value_type>>& ilist,
    const Alloc&                                                     alloc = Alloc());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 2 edge elements.
  ///
  /// @param ilist Initializer list of tuples with source_vertex_key and
  ///              target_vertex_key.
  /// @param alloc Allocator.
  ///
  // clang-format off
  directed_adjacency_vector(
    const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist,
    const Alloc&                                                     alloc = Alloc());
  // clang-format on

  ~directed_adjacency_vector() = default;

  directed_adjacency_vector& operator=(const directed_adjacency_vector&) = default;
  directed_adjacency_vector& operator=(directed_adjacency_vector&&) = default;

public:
  constexpr graph_value_type&       graph_value();
  constexpr const graph_value_type& graph_value() const;

  constexpr vertex_set&       vertices();
  constexpr const vertex_set& vertices() const;

  constexpr edge_set&       edges();
  constexpr const edge_set& edges() const;

  vertex_iterator       find_vertex(vertex_key_type);
  const_vertex_iterator find_vertex(vertex_key_type) const;

  constexpr vertex_edge_range       outward_edges(vertex_iterator u);
  constexpr const_vertex_edge_range outward_edges(const_vertex_iterator u) const;

  constexpr vertex_outward_vertex_range       outward_vertices(vertex_iterator u);
  constexpr const_vertex_outward_vertex_range outward_vertices(const_vertex_iterator u) const;

  constexpr allocator_type allocator() const;

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, const vertex_value_type&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(const VV2&); // vertex_value_type must be constructable from VV2

protected:
  void reserve_edges(edge_size_type);

  edge_iterator create_edge(vertex_key_type, vertex_key_type);
  edge_iterator create_edge(vertex_key_type, vertex_key_type, edge_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type,
                            vertex_key_type,
                            const EV2&); // EV2 must be accepted by vertex_value_type constructor

public:
  void clear();
  void swap(directed_adjacency_vector&);

protected:
  vertex_iterator finalize_outward_edges(vertex_subrange);
  void            throw_unordered_edges() const;

private:
  vertex_set     vertices_;
  edge_set       edges_;
  allocator_type alloc_;
};


} // namespace std::graph::containers

#endif // DIRECTED_ADJ_ARRAY_HPP

#include "detail/directed_adjacency_vector_api.hpp"
#include "detail/directed_adjacency_vector_impl.hpp"
