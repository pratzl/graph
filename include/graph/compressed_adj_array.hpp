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

  caa_edge(vertex_set const& vertices, vertex_iterator out_vertex, vertex_iterator in_vertex);
  caa_edge(vertex_set const& vertices,
           vertex_iterator   out_vertex,
           vertex_iterator   in_vertex,
           edge_user_value_type const&);
  caa_edge(vertex_set const& vertices, vertex_iterator out_vertex, vertex_iterator in_vertex, edge_user_value_type&);

  vertex_type&       out_vertex(graph_type&);
  vertex_type const& out_vertex(graph_type const&) const;

private:
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

private:
  edge_index first_edge_ = numeric_limits<edge_index>::max();
};

///-------------------------------------------------------------------------------------
/// caa_graph definitions
///
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

  // constructors taking vertices
  template <template <class, class> class VCont,
            class VV2,
            class A2> // container must be ordered by edge_key_type.first; for vector, deque
  caa_graph(VCont<VV2, A2> const& vcont, allocator_type alloc = allocator_type())
        : caa_graph(vcont, vector<edge_key_type, A2>(), alloc) {}

  // constructors taking edges
  template <
        template <class, class>
        class ECont,
        class
        A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={vector, deque, ...}
  caa_graph(ECont<edge_key_type, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  template <
        template <class, class>
        class ECont,
        class A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={vector, deque}
  caa_graph(ECont<edge_value_type, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  template <template <class, class, class> class ECont,
            class Cmp,
            class A3> // econt must be ordered by edge_key_type.first; VCont={vector, deque, ...}, for ECont={set}
  caa_graph(ECont<edge_key_type, Cmp, A3> const& econt, allocator_type alloc = allocator_type())
        : caa_graph(vector<vertex_user_value_type, A3>(), econt, alloc) {}

  template <template <class, class> class VCont,
            template <class, class, class, class>
            class ECont,
            class Cmp,
            class A2,
            class A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={map}
  caa_graph(ECont<edge_key_type, edge_user_value_type, Cmp, A3> const& econt, allocator_type alloc = allocator_type());


  // constructors taking vertices and edges
  template <
        template <class, class>
        class VCont,
        template <class, class>
        class ECont,
        class VV2,
        class A2,
        class
        A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={vector, deque, ...}
  caa_graph(VCont<VV2, A2> const&           vcont,
            ECont<edge_key_type, A3> const& econt,
            allocator_type                  alloc = allocator_type());

  template <
        template <class, class>
        class VCont,
        template <class, class>
        class ECont,
        class VV2,
        class EV2,
        class A2,
        class A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={vector, deque}
  caa_graph(VCont<VV2, A2> const&                      vcont,
            ECont<pair<edge_key_type, EV2>, A3> const& econt,
            allocator_type                             alloc = allocator_type());

  template <template <class, class> class VCont,
            template <class, class, class>
            class ECont,
            class VV2,
            class Cmp,
            class A2,
            class A3> // econt must be ordered by edge_key_type.first; VCont={vector, deque, ...}, for ECont={set}
  caa_graph(VCont<VV2, A2> const&                vcont,
            ECont<edge_key_type, Cmp, A3> const& econt,
            allocator_type                       alloc = allocator_type());

  template <template <class, class> class VCont,
            template <class, class, class, class>
            class ECont,
            class VV2,
            class EV2,
            class Cmp,
            class A2,
            class A3> // econt must be ordered by edge_key_type.first; for VCont={vector, deque, ...}, ECont={map}
  caa_graph(VCont<VV2, A2> const&                     vcont,
            ECont<edge_key_type, EV2, Cmp, A3> const& econt,
            allocator_type                            alloc = allocator_type());

public:
  constexpr vertex_set&       vertices();
  constexpr vertex_set const& vertices() const;

  void                  reserve_vertices(vertex_size_type);
  void                  resize_vertices(vertex_size_type);
  void                  resize_vertices(vertex_size_type, vertex_user_value_type const&);
  vertex_iterator       find_vertex(vertex_key_type const&);
  const_vertex_iterator find_vertex(vertex_key_type const&) const;
  vertex_iterator       create_vertex();
  vertex_iterator       create_vertex(vertex_user_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(VV2 const&); // vertex_user_value_type must be constructable from VV2

public:
  constexpr edge_set&       edges();
  constexpr edge_set const& edges() const;
  void                      reserve_edges(edge_size_type);
  edge_iterator             create_edge(vertex_key_type const&, vertex_key_type const&);
  edge_iterator             create_edge(vertex_key_type const&, vertex_key_type const&, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type const&,
                            vertex_key_type const&,
                            EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

public:
  void clear();

protected:
  void throw_unordered_edges() const;

private:
  vertex_set     vertices_;
  edge_set       edges_;
  allocator_type alloc_;
};


} // namespace std::graph

#endif // COMPRESSED_ADJ_ARRAY_HPP

#include "detail/compressed_adj_array_impl.hpp"
