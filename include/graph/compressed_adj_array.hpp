#include "graph_utility.hpp"
#include <vector>
#include <range/v3/view/subrange.hpp>

#ifndef COMPRESSED_ADJ_ARRAY_HPP
#  define COMPRESSED_ADJ_ARRAY_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// compressed_adjacency_array forward declarations
///
/// All vertices are kept in a single vector with an index for the first outgoing edge.
///
/// All edges are kept in a single vector in the graph. Outgoing edges for a vertex are
/// stored contiguously. Edges for vertex v must come after the previous vertex's edges.
/// An edge holds the index for its outgoing vertex in the vertices vector, plus any
/// user-defined values.

template <typename VV = empty_value, typename EV = empty_value, typename GV = empty_value, typename A = allocator<char>>
class caa_graph;

template <typename VV, typename EV, typename GV, typename A>
class caa_vertex;

template <typename VV, typename EV, typename GV, typename A>
class caa_edge;

template <typename VV = empty_value, typename EV = empty_value, typename GV = empty_value, typename A = allocator<char>>
using compressed_adjacency_array = caa_graph<VV, EV, GV, A>;


///-------------------------------------------------------------------------------------
/// caa_edge
///
template <typename VV, typename EV, typename GV, typename A>
class caa_edge : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV> {
public:
  using graph_type            = caa_graph<VV, EV, GV, A>;
  using graph_user_value_type = GV;
  using base_t                = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;

  using vertex_type            = caa_vertex<VV, EV, GV, A>;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = typename vertex_set::size_type;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = caa_edge<VV, EV, GV, A>;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_index           = typename edge_set::size_type;

public:
  caa_edge()                    = default;
  caa_edge(caa_edge const&)     = default;
  caa_edge(caa_edge&&) noexcept = default;
  ~caa_edge()                   = default;
  caa_edge& operator=(caa_edge&) = default;
  caa_edge& operator=(caa_edge&&) = default;

  caa_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex);
  caa_edge(vertex_set const& vertices,
           vertex_iterator   in_vertex,
           vertex_iterator   out_vertex,
           edge_user_value_type const&);
  caa_edge(vertex_set const& vertices, vertex_iterator in_vertex, vertex_iterator out_vertex, edge_user_value_type&&);

  vertex_type&       in_vertex(graph_type&);
  vertex_type const& in_vertex(graph_type const&) const;
  vertex_key_type    in_vertex_key() const;

  vertex_type&       out_vertex(graph_type&);
  vertex_type const& out_vertex(graph_type const&) const;
  vertex_key_type    out_vertex_key() const;

private:
  vertex_index in_vertex_;
  vertex_index out_vertex_;
};

///-------------------------------------------------------------------------------------
/// caa_vertex
///
template <typename VV, typename EV, typename GV, typename A>
class caa_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
public:
  using graph_type            = caa_graph<VV, EV, GV, A>;
  using graph_user_value_type = GV;
  using base_t                = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_type            = caa_vertex<VV, EV, GV, A>;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_index           = typename vertex_set::size_type;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = caa_edge<VV, EV, GV, A>;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_index           = typename edge_set::size_type;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;

  using vertex_edge_size_type      = typename vertex_set::size_type;
  using vertex_edge_iterator       = typename edge_iterator;
  using const_vertex_edge_iterator = typename const_edge_iterator;

  using out_edge_size_type      = typename edge_set::size_type;
  using out_edge_iterator       = typename edge_iterator;
  using const_out_edge_iterator = typename const_edge_iterator;

public:
  caa_vertex()                      = default;
  caa_vertex(caa_vertex const&)     = default;
  caa_vertex(caa_vertex&&) noexcept = default;
  ~caa_vertex()                     = default;
  caa_vertex& operator=(caa_vertex const&) = default;
  caa_vertex& operator=(caa_vertex&&) = default;

  caa_vertex(vertex_set& vertices, vertex_index index);
  caa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  caa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&);

  void set_edge_begin(graph_type&, edge_iterator);

  vertex_edge_iterator       edge_begin(graph_type&);
  const_vertex_edge_iterator edge_begin(graph_type const&) const;
  const_vertex_edge_iterator edge_cbegin(graph_type const&) const;

  vertex_edge_iterator       edge_end(graph_type&);
  const_vertex_edge_iterator edge_end(graph_type const&) const;
  const_vertex_edge_iterator edge_cend(graph_type const&) const;

private:
  edge_index first_edge_ = numeric_limits<edge_index>::max();
};

/// A simple semi-mutable graph emphasizing performance and space.
///
/// caa_graph is a compressed adjacency array graph with the following characteristics:
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
/// When constructing the caa_graph, vertices are identified by their index in the vertex
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
/// @tparam VV Vertex Value type. default = empty_value.
/// @tparam EV Edge Value type. default = empty_value.
/// @tparam GV Graph Value type. default = empty_value.
/// @tparam A  Allocator. default = std::allocator
//
template <typename VV, typename EV, typename GV, typename A>
class caa_graph : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using graph_type            = caa_graph<VV, EV, GV, A>;
  using base_t                = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_user_value_type = GV;
  using allocator_type        = A;

  using vertex_type            = caa_vertex<VV, EV, GV, A>;
  using vertex_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_index           = typename vertex_set::size_type;
  using vertex_user_value_type = VV;
  using vertex_key_type        = vertex_index;
  using vertex_range           = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set*>(nullptr)));
  using const_vertex_range     = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set const*>(nullptr)));

  using edge_user_value_type = EV;
  using edge_type            = caa_edge<VV, EV, GV, A>;
  using edge_allocator_type  = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;
  using edge_set             = vector<edge_type, edge_allocator_type>;
  using edge_iterator        = typename edge_set::iterator;
  using const_edge_iterator  = typename edge_set::const_iterator;
  using edge_size_type       = typename edge_set::size_type;

  using edge_key_type   = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type = pair<edge_key_type, edge_user_value_type>;

  using edge_range              = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set*>(nullptr)));
  using const_edge_range        = decltype(::ranges::make_subrange(*reinterpret_cast<edge_set const*>(nullptr)));
  using vertex_edge_range       = edge_range;
  using const_vertex_edge_range = const_edge_range;
  using out_edge_range          = edge_range;
  using const_out_edge_range    = const_edge_range;

  using vertex_out_edge_iterator       = typename out_edge_range::iterator;
  using const_vertex_out_edge_iterator = typename const_out_edge_range::iterator;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;

public:
  caa_graph() = default;
  caa_graph(allocator_type alloc);
  caa_graph(graph_user_value_type const&, allocator_type alloc = allocator_type());
  caa_graph(graph_user_value_type&&, allocator_type alloc = allocator_type());

  // The following constructors will load vertices and/or edges into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in the edges. Edges are scanned first to
  // determine the highest number and the vertices are resized to match the number.
  //
  // The containers are designed for vector, deque, set & map, though any container that matches their template
  // signature and general behavior can be used. The main criteria is for the edges to be ordered by the "first" value
  // in edge_key_value_type. The caller is responsible to assure vertex key values in edge_key_value_type match
  // the associated vertex values passed (if vertices are supplied).
  //
  //		ECont<class,class>: supports edges stored in {vertex,deque} or any similar forward_only container.
  //		forward_list would also work. The first type can be either edge_key_type or edge_value_type, the second is an
  //		allocator.
  //
  //		ECont<edge_key_type, Cmp, A3> is for set
  //		ECont<edge_key_type, edge_user_value_type, Cmp, A3> is for map.
  //
  // unordered_map and unordered_set are not supported because edges must be ordered by the first vertex_key (src).
  //

  /// Constructor that takes only vertex values to create the graph.
  ///
  /// @tparam VCont<VV2,A2>
  ///             The container that holds the vertex values. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam VV2 The value type used for constructing the vertex's value. The graph's VV type must accept VV2
  ///             in its constructor.
  /// @tparam A2  The allocator type used for VCont.
  ///
  /// @param vcont The container of vertex values.
  ///
  template <template <class, class> class VCont, class VV2, class A2>
  caa_graph(VCont<VV2, A2> const& vcont, allocator_type alloc = allocator_type())
        : caa_graph(vcont, vector<edge_key_type, A2>(), alloc) {}

  /// Constructor that takes only edge keys to create the graph.
  ///
  /// @tparam ECont<edge_key_type,A3>
  ///             The container that holds the edges. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters. Edge values are assigned
  ///			  their default value.
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param econt A container of edge_key_type. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class ECont, class A3>
  caa_graph(ECont<edge_key_type, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  /// Constructor that takes only edge key+values to create the graph.
  ///
  /// @tparam ECont<edge_value_type,A3>
  ///             The container that holds the edges. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param econt A container of edge_value_type. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class ECont, class A3>
  caa_graph(ECont<edge_value_type, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  /// Constructor that takes only edge key+values to create the graph.
  ///
  /// @tparam ECont<edge_key_type,Cmp,A3>
  ///             The container that holds the edges. Intended for std::set but could be used for any
  ///             other non-std container with similar template parameters.
  /// @tparam Cmp The comparator used for ECont (not used)
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param econt A container of edge_key_type. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class, class> class ECont, class Cmp, class A3>
  caa_graph(ECont<edge_key_type, Cmp, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  /// Constructor that takes edge key+values to create the graph.
  ///
  /// @tparam ECont<edge_key_type,edge_user_value_type,Cmp,A3>
  ///             The container that holds the edges. Intended for std::map but could be used for any
  ///             other non-std container with similar template parameters.
  /// @tparam Cmp The comparator used for ECont (not used)
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param econt A container of pair<edge_key_type,edge_user_value_type>. The container must be ordered by
  ///              edge_key_type::first (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class, class, class> class ECont, class Cmp, class A2, class A3>
  caa_graph(ECont<edge_key_type, edge_user_value_type, Cmp, A3> const& econt, allocator_type alloc = allocator_type());


  /// Constructor that takes vertex values and edge keys to create the graph.
  ///
  /// @tparam VCont<VV2,A2>
  ///             The container that holds the vertex values. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam VV2 The value type used for constructing the vertex's value. The graph's VV type must accept VV2
  ///             in its constructor.
  /// @tparam A2  The allocator type used for VCont.
  ///
  /// @tparam ECont<edge_key_type,A3>
  ///             The container that holds the edges. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters. Edge values are assigned
  ///			  their default value.
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param vcont The container of vertex values.
  /// @param econt A container of edge_key_type. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class VCont, template <class, class> class ECont, class VV2, class A2, class A3>
  caa_graph(VCont<VV2, A2> const&           vcont,
            ECont<edge_key_type, A3> const& econt,
            allocator_type                  alloc = allocator_type());

  /// Constructor that takes vertex values and edge key+values to create the graph.
  ///
  /// @tparam VCont<VV2,A2>
  ///             The container that holds the vertex values. Allows vector and deque, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam VV2 The value type used for constructing the vertex's value. The graph's VV type must accept VV2
  ///             in its constructor.
  /// @tparam A2  The allocator type used for VCont.
  ///
  /// @tparam ECont<pair<edge_key_type, edge-value>,A3>
  ///             The container that holds the edges. Intended for std::vector or std::deque but could be
  ///			  used for any other container with similar template parameters.
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param vcont The container of vertex values.
  /// @param econt A container of pair<edge_key_type, edge-value>. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class VCont,
            template <class, class>
            class ECont,
            class VV2,
            class EV2,
            class A2,
            class A3>
  caa_graph(VCont<VV2, A2> const&                      vcont,
            ECont<pair<edge_key_type, EV2>, A3> const& econt,
            allocator_type                             alloc = allocator_type());

  /// Constructor that takes vertex values and edge key+values to create the graph.
  ///
  /// @tparam VCont<VV2,A2>
  ///             The container that holds the vertex values. Allows set, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam VV2 The value type used for constructing the vertex's value. The graph's VV type must accept VV2
  ///             in its constructor.
  /// @tparam A2  The allocator type used for VCont.
  ///
  /// @tparam ECont<edge_key_type,Cmp,A3>
  ///             The container that holds the edges. Intended for std::set but could be
  ///			  used for any other container with similar template parameters.
  /// @tparam Cmp The comparator used for ECont (only needed for ECont definition)
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param vcont The container of vertex values.
  /// @param econt A container of pair<edge_key_type, edge-value>. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class VCont,
            template <class, class, class>
            class ECont,
            class VV2,
            class Cmp,
            class A2,
            class A3>
  caa_graph(VCont<VV2, A2> const&                vcont,
            ECont<edge_key_type, Cmp, A3> const& econt,
            allocator_type                       alloc = allocator_type());

  /// Constructor that takes vertex values and edge key+values to create the graph.
  ///
  /// @tparam VCont<VV2,A2>
  ///             The container that holds the vertex values. Allows set, but could also allow
  ///             other containers with the same number of template parameters.
  /// @tparam VV2 The value type used for constructing the vertex's value. The graph's VV type must accept VV2
  ///             in its constructor.
  /// @tparam A2  The allocator type used for VCont.
  ///
  /// @tparam ECont<edge_key_type,edge-value,Cmp,A3>
  ///             The container that holds the edge keys and values. Intended for std::map but could be
  ///			  used for any other container with similar template parameters.
  /// @tparam Cmp The comparator used for ECont (only needed for ECont definition)
  /// @tparam A3  The allocator type used for ECont (only needed for ECont definition)
  ///
  /// @param vcont The container of vertex values.
  /// @param econt A container of edge_key_type, edge-value. The container must be ordered by edge_key_type::first
  ///			  (a domain_exception is thrown if it isn't).
  /// @param alloc The allocator used for the internal vertex and edge vectors.
  ///
  template <template <class, class> class VCont,
            template <class, class, class, class>
            class ECont,
            class VV2,
            class EV2,
            class Cmp,
            class A2,
            class A3>
  caa_graph(VCont<VV2, A2> const&                     vcont,
            ECont<edge_key_type, EV2, Cmp, A3> const& econt,
            allocator_type                            alloc = allocator_type());

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

#include "detail/compressed_adj_array_impl.hpp"
