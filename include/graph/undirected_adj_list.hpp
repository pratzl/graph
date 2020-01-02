//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <range/v3/view/subrange.hpp>

#ifndef UNDIRECTED_ADJ_LIST_HPP
#  define UNDIRECTED_ADJ_LIST_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// undirected_adjacency_list forward declarations
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
class ual_graph;

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_vertex;

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_edge;

template <typename VV     = empty_value,
          typename EV     = empty_value,
          typename GV     = empty_value,
          typename IndexT = uint32_t,
          typename A      = allocator<char>>
using undirected_adjacency_list = ual_graph<VV, EV, GV, IndexT, A>;


///-------------------------------------------------------------------------------------
/// ual_edge
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV> {
public:
  using graph_type            = ual_graph<VV, EV, GV, IndexT, A>;
  using graph_user_value_type = GV;
  using base_t                = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;

  using vertex_type            = ual_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, IndexT, A>;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_index           = IndexT;

public:
  ual_edge()                    = default;
  ual_edge(ual_edge const&)     = default;
  ual_edge(ual_edge&&) noexcept = default;
  ~ual_edge()                   = default;
  ual_edge& operator=(ual_edge&) = default;
  ual_edge& operator=(ual_edge&&) = default;

  ual_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex);
  ual_edge(vertex_set const& vertices,
           vertex_iterator   in_vertex,
           vertex_iterator   out_vertex,
           edge_user_value_type const&);
  ual_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex, edge_user_value_type&&);

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
/// ual_vertex
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
public:
  using graph_type            = ual_graph<VV, EV, GV, IndexT, A>;
  using graph_user_value_type = GV;
  using base_t                = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_type            = ual_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, IndexT, A>;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = IndexT;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using vertex_edge_size_type      = typename edge_set::size_type;
  using vertex_edge_iterator       = typename edge_iterator;
  using const_vertex_edge_iterator = typename const_edge_iterator;

  using vertex_out_edge_size_type      = typename edge_set::size_type;
  using vertex_out_edge_iterator       = typename edge_iterator;
  using const_vertex_out_edge_iterator = typename const_edge_iterator;

public:
  ual_vertex()                      = default;
  ual_vertex(ual_vertex const&)     = default;
  ual_vertex(ual_vertex&&) noexcept = default;
  ~ual_vertex()                     = default;
  ual_vertex& operator=(ual_vertex const&) = default;
  ual_vertex& operator=(ual_vertex&&) = default;

  ual_vertex(vertex_set& vertices, vertex_index index);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&);

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

/// A simple undirected adjacency list (graph).
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_graph : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using graph_type            = ual_graph<VV, EV, GV, IndexT, A>;
  using base_t                = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_user_value_type = GV;
  using allocator_type        = A;

  using vertex_type            = ual_vertex<VV, EV, GV, IndexT, A>;
  using const_vertex_type      = const vertex_type;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_index           = IndexT;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;
  using vertex_range           = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set*>(nullptr)));
  using const_vertex_range     = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set const*>(nullptr)));

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, IndexT, A>;
  using const_edge_type      = const edge_type;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = IndexT;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using edge_key_type   = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type = pair<edge_key_type, edge_user_value_type>;

  using edge_range       = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set*>(nullptr)));
  using const_edge_range = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set const*>(nullptr)));

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
  ual_graph() = default;
  ual_graph(allocator_type alloc);
  ual_graph(graph_user_value_type const&, allocator_type alloc = allocator_type());
  ual_graph(graph_user_value_type&&, allocator_type alloc = allocator_type());

  // The following constructors will load edges (and vertices) into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in the edges.
  // Edges are scanned first to determine the highest number and the vertices are resized
  // to match the number.
  //
  // Accessor functions are used to return the edge_key_type, edge_user_value_type and
  // vertex_user_value_type.
  //
  // The order visited in the vertices determines their index (and key/identity) in the
  // internal vertices vector. The edge keys use those values and are also expected to be
  // ordered by their first (in) vertex key and an exception is thrown if they aren't in
  // order. For these reasons, unordered (hash) containers won't work.
  //

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng     The edge data range.
  /// @tparam EKeyFnc  Function object to return edge_key_type of the ERng::value_type.
  /// @tparam EPropFnc Function object to return the edge_user_value_type, or a type that
  ///                  edge_user_value_type is constructible from. If the return type is
  ///                  void or empty_value the edge_user_value_type default constructor
  ///                  will be used to initialize the value.
  /// @tparam VRng     The vertex data range.
  /// @tparam VPropFnc Function object to return the vertex_user_value_type, or a type that
  ///                  vertex_user_value_type is constructible from. If the return type is void
  ///                  or empty_value the vertex_user_value_type default constructor
  ///                  will be used to initialize the value.
  ///
  /// @param erng      The container of edge data.
  /// @param vrng      The container of vertex data.
  /// @param ekey_fnc  The edge key extractor function object.
  /// @param eprop_fnc The edge value extractor function object.
  /// @param vprop_fnc The vertex value extractor function object.
  /// @param alloc     The allocator to use for internal containers for vertices & edges.
  ///
  template <typename ERng, typename EKeyFnc, typename EPropFnc, typename VRng, typename VPropFnc>
  ual_graph(
        ERng const&     erng,
        VRng const&     vrng,
        EKeyFnc const&  ekey_fnc  = [](typename ERng::value_type const&) { return edge_key_type(); },
        EPropFnc const& eprop_fnc = [](typename ERng::value_type const&) { return empty_value(); },
        VPropFnc const& vprop_fnc = [](typename VRng::value_type const&) { return empty_value(); },
        A               alloc     = A());

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng     The edge data range.
  /// @tparam EKeyFnc  Function object to return edge_key_type of the ERng::value_type.
  /// @tparam EPropFnc Function object to return the edge_user_value_type, or a type that
  ///                  edge_user_value_type is constructible from. If the return type is
  ///                  void or empty_value the edge_user_value_type default constructor
  ///                  will be used to initialize the value.
  ///
  /// @param erng      The container of edge data.
  /// @param ekey_fnc  The edge key extractor function object.
  /// @param eprop_fnc The edge value extractor function object.
  /// @param alloc     The allocator to use for internal containers for vertices & edges.
  ///
  template <typename ERng, typename EKeyFnc, typename EPropFnc>
  ual_graph(
        ERng const&     erng,
        EKeyFnc const&  ekey_fnc  = [](typename ERng::value_type const&) { return edge_key_type(); },
        EPropFnc const& eprop_fnc = [](typename ERng::value_type const&) { return empty_value(); },
        A               alloc     = A());

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

#endif // UNDIRECTED_ADJ_LIST_HPP

#include "detail/undirected_adj_list_impl.hpp"
