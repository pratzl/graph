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
class dav_const_vertex_vertex_iterator;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_vertex_vertex_iterator;


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
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
class dav_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV> {
public:
  using base_type  = conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV>;
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, A>;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_user_value_type = VV;
  using vertex_key_type        = KeyT;
  using vertex_index_type      = KeyT;
  using vertex_value_type      = vertex_type;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_size_type      = typename vertex_set::size_type;
  using vertex_ssize_type     = typename vertex_set::difference_type;

  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = EV;
  using edge_key_type        = ordered_pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type      = KeyT;
  using edge_value_type      = pair<edge_key_type, edge_user_value_type>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;

public:
  dav_edge()                    = default;
  dav_edge(const dav_edge&)     = default;
  dav_edge(dav_edge&&) noexcept = default;
  ~dav_edge() noexcept          = default;
  dav_edge& operator=(dav_edge&) = default;
  dav_edge& operator=(dav_edge&&) = default;

  dav_edge(const vertex_set& vertices, vertex_iterator inward_vertex, vertex_iterator outward_vertex);
  dav_edge(const vertex_set& vertices,
           vertex_iterator   inward_vertex,
           vertex_iterator   outward_vertex,
           const edge_user_value_type&);
  dav_edge(const vertex_set& vertices,
           vertex_iterator   inward_vertex,
           vertex_iterator   outward_vertex,
           edge_user_value_type&&);

  vertex_iterator       inward_vertex(graph_type&);
  const_vertex_iterator inward_vertex(const graph_type&) const;
  vertex_key_type       inward_vertex_key(const graph_type&) const;

  vertex_iterator       outward_vertex(graph_type&);
  const_vertex_iterator outward_vertex(const graph_type&) const;
  vertex_key_type       outward_vertex_key(const graph_type&) const;

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
class dav_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV> {
public:
  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, A>;
  using base_type  = conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV>;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_user_value_type = VV;
  using vertex_key_type        = KeyT;
  using vertex_index_type      = KeyT;
  using vertex_value_type      = vertex_type;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_size_type      = typename vertex_set::size_type;
  using vertex_ssize_type     = typename vertex_set::difference_type;

  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = EV;
  using edge_key_type        = ordered_pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type      = KeyT;
  using edge_value_type      = pair<edge_key_type, edge_user_value_type>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;

  using edge_range          = decltype(make_subrange2(declval<edge_set&>()));
  using const_edge_range    = decltype(make_subrange2(declval<const edge_set&>()));
  using edge_iterator       = ranges::iterator_t<edge_range>;
  using const_edge_iterator = ranges::iterator_t<const_edge_range>;
  using edge_size_type      = ranges::range_size_t<edge_range>;
  using edge_ssize_type     = ranges::range_difference_t<edge_range>;

  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;
  using vertex_edge_iterator       = edge_iterator;
  using const_vertex_edge_iterator = const_edge_iterator;
  using vertex_edge_size_type      = edge_size_type;
  using vertex_edge_ssize_type     = edge_ssize_type;

  using vertex_vertex_iterator       = dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>;
  using const_vertex_vertex_iterator = dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>;
  using vertex_vertex_range          = decltype(make_subrange2(vertex_vertex_iterator(), vertex_vertex_iterator()));
  using const_vertex_vertex_range =
        decltype(make_subrange2(const_vertex_vertex_iterator(), const_vertex_vertex_iterator()));
  using vertex_vertex_size_type  = edge_size_type;
  using vertex_vertex_ssize_type = edge_ssize_type;

public:
  dav_vertex() noexcept             = default;
  dav_vertex(const dav_vertex&)     = default;
  dav_vertex(dav_vertex&&) noexcept = default;
  ~dav_vertex() noexcept            = default;
  dav_vertex& operator=(const dav_vertex&) = default;
  dav_vertex& operator=(dav_vertex&&) = default;

  dav_vertex(vertex_set& vertices, vertex_index_type index);
  dav_vertex(vertex_set& vertices, vertex_index_type index, const vertex_user_value_type&);
  dav_vertex(vertex_set& vertices, vertex_index_type index, vertex_user_value_type&&);

  void set_edge_begin(graph_type&, edge_iterator);

  vertex_edge_iterator       edges_begin(graph_type&);
  const_vertex_edge_iterator edges_begin(const graph_type&) const;
  const_vertex_edge_iterator edges_cbegin(const graph_type&) const;

  vertex_edge_iterator       edges_end(graph_type&);
  const_vertex_edge_iterator edges_end(const graph_type&) const;
  const_vertex_edge_iterator edges_cend(const graph_type&) const;

  edge_size_type edges_size(const graph_type&) const;

  vertex_vertex_iterator       vertices_begin(graph_type&);
  const_vertex_vertex_iterator vertices_begin(const graph_type&) const;
  const_vertex_vertex_iterator vertices_cbegin(const graph_type&) const;

  vertex_vertex_iterator       vertices_end(graph_type&);
  const_vertex_vertex_iterator vertices_end(const graph_type&) const;
  const_vertex_vertex_iterator vertices_cend(const graph_type&) const;

  vertex_vertex_size_type vertices_size(const graph_type&) const;

private:
  edge_index_type first_edge_ = numeric_limits<edge_index_type>::max();

  vertex_edge_iterator e_begin(graph_type const&) const;
  vertex_edge_iterator e_end(graph_type const&) const;
};

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_const_vertex_vertex_iterator {
public:
  using this_t = dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>;

  using graph_type = directed_adjacency_vector<VV, EV, GV, KeyT, A>;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_user_value_type = VV;
  using vertex_key_type        = KeyT;
  using vertex_value_type      = vertex_type;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;

  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = typename edge_type::edge_user_value_type;
  using edge_key_type        = typename edge_type::edge_key_type; // <from,to>
  using edge_value_type      = typename edge_type::edge_value_type;
  using edge_set             = typename edge_type::edge_set;

  using vertex_edge_iterator       = typename edge_set::iterator;
  using const_vertex_edge_iterator = typename edge_set::const_iterator;

  using iterator_category = random_access_iterator_tag;
  using value_type        = vertex_type;
  using size_type         = typename edge_set::size_type;
  using ssize_type        = typename edge_set::difference_type;
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

  reference operator[](const difference_type distance) const noexcept { return *uv_[distance].outward_vertex(*g_); }

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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class dav_vertex_vertex_iterator : public dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A> {
public:
  using this_t = dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>;
  using base_t = dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>;

  using graph_type           = typename base_t::graph_type;
  using vertex_type          = typename base_t::vertex_type;
  using vertex_edge_iterator = typename base_t::vertex_edge_iterator;
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
class directed_adjacency_vector : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV> {
public:
  using base_type             = conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV>;
  using graph_type            = directed_adjacency_vector<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = GV;
  using allocator_type        = A;

  using vertex_type            = dav_vertex<VV, EV, GV, KeyT, A>;
  using vertex_user_value_type = VV;
  using vertex_key_type        = KeyT;
  using vertex_index_type      = KeyT;
  using vertex_value_type      = vertex_type;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;

  using vertex_range          = decltype(make_subrange2(declval<vertex_set&>()));
  using const_vertex_range    = decltype(make_subrange2(declval<const vertex_set&>()));
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_size_type      = ranges::range_size_t<vertex_range>;
  using vertex_ssize_type     = ranges::range_difference_t<vertex_range>;

  using edge_type            = dav_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = EV;
  using edge_key_type        = ordered_pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_index_type      = KeyT;
  using edge_value_type      = pair<edge_key_type, edge_user_value_type>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;

  using edge_range          = decltype(make_subrange2(declval<edge_set&>()));
  using const_edge_range    = decltype(make_subrange2(declval<const edge_set&>()));
  using edge_iterator       = ranges::iterator_t<edge_range>;
  using const_edge_iterator = ranges::iterator_t<const_edge_range>;
  using edge_size_type      = ranges::range_size_t<edge_range>;
  using edge_ssize_type     = ranges::range_difference_t<edge_range>;

  using vertex_outward_edge_range          = edge_range;
  using const_vertex_outward_edge_range    = const_edge_range;
  using vertex_outward_edge_iterator       = edge_iterator;
  using const_vertex_outward_edge_iterator = const_edge_iterator;
  using vertex_outward_size_type           = edge_size_type;
  using vertex_outward_edge_ssize_type     = edge_ssize_type;

  using vertex_edge_range          = edge_range;
  using const_vertex_edge_range    = const_edge_range;
  using vertex_edge_iterator       = edge_iterator;
  using const_vertex_edge_iterator = const_edge_iterator;
  using vertex_edge_size_type      = edge_size_type;
  using vertex_edge_ssize_type     = edge_ssize_type;

public:
  directed_adjacency_vector() noexcept(noexcept(allocator_type()))    = default;
  directed_adjacency_vector(directed_adjacency_vector&& rhs) noexcept = default;
  directed_adjacency_vector(const directed_adjacency_vector&)         = default;

  directed_adjacency_vector(const allocator_type& alloc) noexcept;
  directed_adjacency_vector(const graph_user_value_type&, const allocator_type& alloc = allocator_type());
  directed_adjacency_vector(graph_user_value_type&&, const allocator_type& alloc = allocator_type());

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
  directed_adjacency_vector(const ERng&     erng,
                           const VRng&      vrng,
                           const EKeyFnc&   ekey_fnc,
                           const EValueFnc& evalue_fnc,
                           const VValueFnc& vvalue_fnc,
                           const GV&        gv    = GV(),
                           const A&         alloc = A());
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
  directed_adjacency_vector(const ERng&     rng, 
                            const EKeyFnc&   ekey_fnc, 
                            const EValueFnc& evalue_fnc, 
                            const GV&        gv = GV(), 
                            const A&         alloc = A());
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
        const initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>>& ilist,
        const A&                                                                               alloc = A());

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with a tuple with 2 edge elements.
  ///
  /// @param ilist Initializer list of tuples with inward_vertex_key and
  ///              outward_vertex_key.
  /// @param alloc Allocator.
  ///
  directed_adjacency_vector(const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist,
                            const A&                                                         alloc = A());

  ~directed_adjacency_vector() = default;

  directed_adjacency_vector& operator=(const directed_adjacency_vector&) = default;
  directed_adjacency_vector& operator=(directed_adjacency_vector&&) = default;

public:
  constexpr vertex_set&       vertices();
  constexpr const vertex_set& vertices() const;

  constexpr vertex_iterator       begin();
  constexpr const_vertex_iterator begin() const;
  constexpr const_vertex_iterator cbegin() const;

  constexpr vertex_iterator       end();
  constexpr const_vertex_iterator end() const;
  constexpr const_vertex_iterator cend() const;

  constexpr edge_set&       edges();
  constexpr const edge_set& edges() const;

  vertex_iterator       find_vertex(vertex_key_type);
  const_vertex_iterator find_vertex(vertex_key_type) const;

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, const vertex_user_value_type&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_user_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(const VV2&); // vertex_user_value_type must be constructable from VV2

protected:
  void reserve_edges(edge_size_type);

  edge_iterator create_edge(vertex_key_type, vertex_key_type);
  edge_iterator create_edge(vertex_key_type, vertex_key_type, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type,
                            vertex_key_type,
                            const EV2&); // EV2 must be accepted by vertex_user_value_type constructor

public:
  void clear();
  void swap(directed_adjacency_vector&);

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
  using vertex_outward_size_type        = typename graph_type::vertex_outward_size_type;

  using vertex_edge_range       = vertex_outward_edge_range;
  using const_vertex_edge_range = const_vertex_outward_edge_range;
  using vertex_edge_size_type   = vertex_outward_size_type;

  using vertex_vertex_range       = typename vertex_type::vertex_vertex_range;
  using const_vertex_vertex_range = typename vertex_type::const_vertex_vertex_range;
  using vertex_vertex_size_type   = size_t;
};


} // namespace std::graph

#endif // DIRECTED_ADJ_ARRAY_HPP

#include "detail/directed_adjacency_vector_impl.hpp"
