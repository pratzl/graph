//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <range/v3/view/subrange.hpp>
#include <cassert>

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
struct in_list;
struct out_list;

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_edge_list;

template <typename VV, typename EV, typename GV, typename IndexT, typename A, typename ListT>
class ual_edge_list_link;


///-------------------------------------------------------------------------------------
/// ual_edge_list
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
/// @tparam ListT  in_list|out_list. Which edge list this is for.
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_edge_list {
public:
  class iterator;
  class const_iterator;

  using graph_type = ual_graph<VV, EV, GV, IndexT, A>;

  using vertex_type           = ual_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set            = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_index          = IndexT;
  using vertex_key_type       = vertex_index;

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, IndexT, A>;

  using edge_list_type          = ual_edge_list<VV, EV, GV, IndexT, A>;
  using edge_list_in_link_type  = ual_edge_list_link<VV, EV, GV, IndexT, A, in_list>;
  using edge_list_out_link_type = ual_edge_list_link<VV, EV, GV, IndexT, A, out_list>;
  using edge_allocator_type     = typename allocator_traits<typename A>::template rebind_alloc<edge_type>;

  using value_type      = edge_type;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using reference       = value_type&;
  using const_reference = value_type const&;
  using size_type       = size_t;
  using difference_type = ptrdiff_t;

  using edge_range       = decltype(::ranges::make_subrange(iterator(), iterator()));
  using const_edge_range = decltype(::ranges::make_subrange(const_iterator(), const_iterator()));

public:
  class const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge_list::edge_type;
    using difference_type   = ual_edge_list::difference_type;
    using size_type         = ual_edge_list::size_type;
    using pointer           = ual_edge_list::const_pointer;
    using reference         = ual_edge_list::const_reference;

    using graph_type  = ual_edge_list::graph_type;
    using vertex_type = ual_edge_list::vertex_type;
    using edge_type   = ual_edge_list::edge_type;

    const_iterator(graph_type const& g, vertex_type const& u, edge_type const* uv = nullptr) noexcept
          : vertex_key_(u.vertex_key(g)), edge_(const_cast<edge_type*>(uv)), graph_(const_cast<graph_type*>(&g)) {}

    const_iterator() noexcept                          = default;
    const_iterator(const_iterator const& rhs) noexcept = default;
    ~const_iterator() noexcept                         = default;

    const_iterator& operator=(const_iterator const& rhs) noexcept = default;

    reference operator*() const;
    pointer   operator->() const;

    const_iterator& operator++();
    const_iterator  operator++(int);

    bool operator==(const_iterator const& rhs) const noexcept;
    bool operator!=(const_iterator const& rhs) const noexcept;

    friend void swap(const_iterator& lhs, const_iterator& rhs) noexcept {
      swap(lhs.vertex_key_, rhs.vertex_key_);
      swap(lhs.edge_, rhs.edge_);
    }

  protected:
    void advance();

  protected:
    vertex_key_type vertex_key_ = numeric_limits<vertex_key_type>::max(); // owning vertex for the list we're in
    edge_type*      edge_       = nullptr;                                // current vertex (==nullptr for end)
    graph_type*     graph_      = nullptr;
  }; // end const_iterator

  class iterator : public const_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge_list::edge_type;
    using difference_type   = ual_edge_list::difference_type;
    using size_type         = ual_edge_list::size_type;
    using pointer           = ual_edge_list::pointer;
    using reference         = ual_edge_list::reference;

    using graph_type  = ual_edge_list::graph_type;
    using vertex_type = ual_edge_list::vertex_type;
    using edge_type   = ual_edge_list::edge_type;

    iterator() noexcept                    = default;
    iterator(iterator const& rhs) noexcept = default;
    ~iterator()                            = default;

    iterator& operator=(iterator const& rhs) = default;

    iterator(graph_type const& g, vertex_type const& u, edge_type const* uv = nullptr) : const_iterator(g, u, uv) {}

    reference operator*() const;
    pointer   operator->() const;

    iterator& operator++();
    iterator  operator++(int);

    friend void swap(iterator& lhs, iterator& rhs) {
      swap(lhs.vertex_key_, rhs.vertex_key_);
      swap(lhs.edge_, rhs.edge_);
    }
  }; // end iterator

public:
  ual_edge_list() noexcept                     = default;
  ual_edge_list(ual_edge_list const&) noexcept = default;
  ~ual_edge_list() noexcept                    = default;
  ual_edge_list& operator=(ual_edge_list const&) noexcept = default;

  ual_edge_list(ual_edge_list&& rhs) noexcept : head_(move(rhs.head_)), tail_(move(rhs.tail_)), size_(move(rhs.size_)) {
    rhs.head_ = rhs.tail_ = nullptr;
    rhs.size_             = 0;
  }
  ual_edge_list& operator=(ual_edge_list&& rhs) noexcept = default;

  size_type size() const noexcept;
  bool      empty() const noexcept;

  edge_type&       front();
  edge_type const& front() const;

  edge_type&       back();
  edge_type const& back() const;


public:
  template <typename ListT>
  void link_front(edge_type& uv, ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link);

  template <typename ListT>
  void link_back(edge_type& uv, ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link);

  template <typename ListT>
  void unlink(edge_type& uv, ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link);

  iterator       begin(graph_type& g, vertex_type& u) noexcept;
  const_iterator begin(graph_type const& g, vertex_type const& u) const noexcept;
  const_iterator cbegin(graph_type const& g, vertex_type const& u) const noexcept;

  iterator       end(graph_type& g, vertex_type& u) noexcept;
  const_iterator end(graph_type const& g, vertex_type const& u) const noexcept;
  const_iterator cend(graph_type const& g, vertex_type const& u) const noexcept;

private:
  edge_type* head_ = nullptr;
  edge_type* tail_ = nullptr;
  size_type  size_ = 0;
};

///-------------------------------------------------------------------------------------
/// ual_edge_list_link
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
/// @tparam ListT  in_list|out_list. Which edge list this is for.
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A, typename ListT>
class ual_edge_list_link {
public:
  using graph_type = ual_graph<VV, EV, GV, IndexT, A>;

  using vertex_type           = ual_vertex<VV, EV, GV, IndexT, A>;
  using vertex_allocator_type = typename allocator_traits<typename A>::template rebind_alloc<vertex_type>;
  using vertex_set            = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_index          = IndexT;
  using vertex_key_type       = vertex_index;

  using edge_type = ual_edge<VV, EV, GV, IndexT, A>;

  using edge_list_type      = ual_edge_list<VV, EV, GV, IndexT, A>;
  using edge_list_link_type = ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>;

public:
  ual_edge_list_link(vertex_key_type ukey) noexcept : vertex_key_(ukey) {}

  ual_edge_list_link() noexcept                          = default;
  ual_edge_list_link(ual_edge_list_link const&) noexcept = default;
  ual_edge_list_link(ual_edge_list_link&&) noexcept      = default;
  ual_edge_list_link& operator=(ual_edge_list_link const&) noexcept = default;
  ual_edge_list_link& operator=(ual_edge_list_link&&) noexcept = default;

public:
  vertex_key_type       vertex_key() const noexcept { return vertex_key_; }
  const_vertex_iterator vertex(graph_type const& g) const { return g.vertices().begin() + vertex_key_; }
  vertex_iterator       vertex(graph_type& g) { return g.vertices().begin() + vertex_key_; }

  edge_type*       next() noexcept { return next_; }
  edge_type const* next() const noexcept { return next_; }
  edge_type*       prev() noexcept { return prev_; }
  edge_type const* prev() const noexcept { return prev_; }

private:
  vertex_key_type vertex_key_ = numeric_limits<vertex_key_type>::max();
  edge_type*      next_       = nullptr;
  edge_type*      prev_       = nullptr;

  friend edge_list_type;
  friend edge_type;
};


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
class ual_edge
      : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>
      , public ual_edge_list_link<VV, EV, GV, IndexT, A, in_list>
      , public ual_edge_list_link<VV, EV, GV, IndexT, A, out_list> {
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
  using edge_index           = IndexT;
  using edge_size_type       = IndexT;
#  ifdef FUTURE
  using edge_iterator       = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
#  endif

  using edge_list_type          = ual_edge_list<VV, EV, GV, IndexT, A>;
  using edge_list_in_link_type  = ual_edge_list_link<VV, EV, GV, IndexT, A, in_list>;
  using edge_list_out_link_type = ual_edge_list_link<VV, EV, GV, IndexT, A, out_list>;

protected:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_edge()                    = default;
  ual_edge(ual_edge const&)     = default;
  ual_edge(ual_edge&&) noexcept = default;
  ual_edge& operator=(ual_edge&) = default;
  ual_edge& operator=(ual_edge&&) noexcept = default;

  ual_edge(graph_type&, vertex_type& u, vertex_type& v) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, edge_user_value_type const&) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, edge_user_value_type&&) noexcept;

  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, edge_user_value_type const&) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, edge_user_value_type&&) noexcept;

  ~ual_edge();

  void link_front(vertex_type&, vertex_type&);
  void link_back(vertex_type&, vertex_type&);
  void unlink(vertex_type&, vertex_type&);

public:
  vertex_iterator       in_vertex(graph_type&) noexcept;
  const_vertex_iterator in_vertex(graph_type const&) const noexcept;
  vertex_key_type       in_vertex_key(graph_type const&) const noexcept;

  vertex_iterator       out_vertex(graph_type&) noexcept;
  const_vertex_iterator out_vertex(graph_type const&) const noexcept;
  vertex_key_type       out_vertex_key(graph_type const&) const noexcept;

  vertex_iterator       other_vertex(graph_type&, vertex_key_type const in_or_out_key) noexcept;
  const_vertex_iterator other_vertex(graph_type const&, vertex_key_type const in_or_out_key) const noexcept;
  vertex_key_type       other_vertex_key(graph_type const&, vertex_key_type const in_or_out_key) const noexcept;

  friend graph_type;  // the graph is the one to create & destroy edges because it owns the allocator
  friend vertex_type; // vertex can also destroy its own edges
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
  using edge_size_type       = IndexT;

  using edge_list_type             = ual_edge_list<VV, EV, GV, IndexT, A>;
  using edge_list_in_link_type     = ual_edge_list_link<VV, EV, GV, IndexT, A, in_list>;
  using edge_list_out_link_type    = ual_edge_list_link<VV, EV, GV, IndexT, A, out_list>;
  using vertex_edge_size_type      = typename edge_list_type::size_type;
  using vertex_edge_iterator       = typename edge_list_type::iterator;
  using const_vertex_edge_iterator = typename edge_list_type::const_iterator;
  using vertex_edge_range          = typename edge_list_type::edge_range;
  using const_vertex_edge_range    = typename edge_list_type::const_edge_range;

public:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_vertex()                      = default;
  ual_vertex(ual_vertex const&)     = default;
  ual_vertex(ual_vertex&&) noexcept = default;
  ~ual_vertex()                     = default;
  ual_vertex& operator=(ual_vertex const&) = default;
  ual_vertex& operator=(ual_vertex&&) noexcept = default;

  ual_vertex(vertex_set& vertices, vertex_index index);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&) noexcept;

public:
  vertex_key_type       vertex_key(graph_type const&) const noexcept;
  vertex_edge_size_type edge_size() const noexcept;

  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator);
  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator, vertex_edge_iterator);
  void                 clear_edges(graph_type&);

  vertex_edge_iterator       edge_begin(graph_type&) noexcept;
  const_vertex_edge_iterator edge_begin(graph_type const&) const noexcept;
  const_vertex_edge_iterator edge_cbegin(graph_type const&) const noexcept;

  vertex_edge_iterator       edge_end(graph_type&) noexcept;
  const_vertex_edge_iterator edge_end(graph_type const&) const noexcept;
  const_vertex_edge_iterator edge_cend(graph_type const&) const noexcept;

  vertex_edge_iterator create_edge(graph_type&, vertex_type&);
  vertex_edge_iterator create_edge(graph_type&, vertex_type&, edge_user_value_type const&);
  vertex_edge_iterator create_edge(graph_type&, vertex_type&, edge_user_value_type&&);

  template <class EV2>
  vertex_edge_iterator create_edge(graph_type&,
                                   vertex_key_type const&,
                                   EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

protected:
  void erase_edge(graph_type&, edge_type*);

private:
  edge_list_type edges_;
  friend edge_type;
  friend edge_list_in_link_type;
  friend edge_list_out_link_type;
};

/// A simple undirected adjacency list (graph).
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
class ual_graph : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using graph_type            = ual_graph<VV, EV, GV, IndexT, A>;
  using base_t                = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_user_value_type = GV;
  using allocator_type        = A;
  using graph_category        = sparse_graph_tag;

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

  using edge_key_type   = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type = pair<edge_key_type, edge_user_value_type>;

  using vertex_edge_iterator       = typename vertex_type::vertex_edge_iterator;
  using const_vertex_edge_iterator = typename vertex_type::const_vertex_edge_iterator;
  using vertex_edge_size_type      = typename vertex_type::vertex_edge_size_type;
  using vertex_edge_range          = typename vertex_type::vertex_edge_range;
  using const_vertex_edge_range    = typename vertex_type::const_vertex_edge_range;

  class edge_iterator;
  class const_edge_iterator;
  using edge_range       = decltype(::ranges::make_subrange(edge_iterator(), edge_iterator()));
  using const_edge_range = decltype(::ranges::make_subrange(const_edge_iterator(), const_edge_iterator()));
  using edge_size_type   = size_t;

  class const_edge_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge<VV, EV, GV, IndexT, A>;
    using difference_type   = ptrdiff_t;
    using pointer           = value_type const*;
    using reference         = value_type const&;

  public:
    const_edge_iterator(graph_type const& g, vertex_iterator u) : g_(&const_cast<graph_type&>(g)), u_(u) {
      advance_vertex();
    }
    const_edge_iterator(graph_type const& g, vertex_iterator u, vertex_edge_iterator uv)
          : g_(&const_cast<graph_type&>(g)), u_(u), uv_(uv) {}

    const_edge_iterator() noexcept                               = default;
    const_edge_iterator(const_edge_iterator const& rhs) noexcept = default;
    ~const_edge_iterator() noexcept                              = default;
    const_edge_iterator& operator=(const_edge_iterator const& rhs) = default;

    reference operator*() const { return *uv_; }
    pointer   operator->() const { return &*uv_; }

    const_edge_iterator& operator++() {
      advance_edge();
      return *this;
    }
    const_edge_iterator operator++(int) {
      const_edge_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    bool operator==(const_edge_iterator const& rhs) const noexcept { return uv_ == rhs.uv_ && u_ == rhs.u_; }
    bool operator!=(const_edge_iterator const& rhs) const noexcept { return !operator==(rhs); }

  protected:
    void advance_edge() {
      // next edge for current vertex
      if (++uv_ != u_->edge_end(*g_))
        return;

      // find next vertex with edge(s)
      ++u_;
      advance_vertex();
    }

    void advance_vertex() {
      // at exit, if u_ != g.vertices().end() then uv_ will refer to a valid edge
      for (; u_ != g_->vertices().end(); ++u_) {
        if (u_->edge_size() > 0) {
          uv_ = u_->edge_begin(*g_);
          return;
        }
      }
    }

  protected:
    graph_type*          g_;
    vertex_iterator      u_;
    vertex_edge_iterator uv_;
  };

  class edge_iterator : public const_edge_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge<VV, EV, GV, IndexT, A>;
    using difference_type   = ptrdiff_t;
    using pointer           = value_type*;
    using reference         = value_type&;

  public:
    edge_iterator(graph_type& g, vertex_iterator u) noexcept : const_edge_iterator(g, u) {}
    edge_iterator(graph_type& g, vertex_iterator u, vertex_edge_iterator uv) : const_edge_iterator(g, u, uv) {}

    edge_iterator() noexcept : const_edge_iterator(){};
    edge_iterator(edge_iterator const& rhs) noexcept : const_edge_iterator(rhs) {}
    edge_iterator(const_edge_iterator& rhs) : const_edge_iterator(rhs) {}
    ~edge_iterator() {}
    edge_iterator& operator=(edge_iterator const& rhs) noexcept {
      const_edge_iterator::operator=(rhs);
      return *this;
    }

    reference operator*() const { return *this->uv_; }
    pointer   operator->() const { return &*this->uv_; }

    edge_iterator& operator++() {
      this->advance_edge();
      return *this;
    }
    edge_iterator operator++(int) {
      edge_iterator tmp(*this);
      ++*this;
      return tmp;
    }
  };

public:
  ual_graph() = default;
  ual_graph(allocator_type alloc);
  ual_graph(graph_user_value_type const&, allocator_type alloc = allocator_type());
  ual_graph(graph_user_value_type&&, allocator_type alloc = allocator_type()) noexcept;
  ~ual_graph();

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
  constexpr edge_allocator_type edge_allocator() const noexcept;

  constexpr vertex_set&       vertices() noexcept;
  constexpr vertex_set const& vertices() const noexcept;

  constexpr vertex_iterator       begin() noexcept;
  constexpr const_vertex_iterator begin() const noexcept;
  constexpr const_vertex_iterator cbegin() const noexcept;

  constexpr vertex_iterator       end() noexcept;
  constexpr const_vertex_iterator end() const noexcept;
  constexpr const_vertex_iterator cend() const noexcept;

  vertex_iterator       find_vertex(vertex_key_type const&);
  const_vertex_iterator find_vertex(vertex_key_type const&) const;

  constexpr edge_size_type edges_size() const noexcept;

  constexpr edge_iterator       edge_begin() { return edge_iterator(*this, begin()); }
  constexpr const_edge_iterator edge_begin() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).begin());
  }
  constexpr const_edge_iterator edge_cbegin() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).cbegin());
  }

  constexpr edge_iterator       edge_end() { return edge_iterator(*this, end()); }
  constexpr const_edge_iterator edge_end() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).end());
  }
  constexpr const_edge_iterator edge_cend() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).end());
  }

  edge_range       edges() { return ::ranges::make_subrange(edge_begin(), edge_end()); }
  const_edge_range edges() const { return ::ranges::make_subrange(edge_begin(), edge_end()); }

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, vertex_user_value_type const&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_user_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(VV2 const&); // vertex_user_value_type must be constructable from VV2

public:
  edge_iterator create_edge(vertex_key_type const&, vertex_key_type const&);
  edge_iterator create_edge(vertex_key_type const&, vertex_key_type const&, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type const&,
                            vertex_key_type const&,
                            EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

  edge_iterator create_edge(vertex_iterator, vertex_iterator);
  edge_iterator create_edge(vertex_iterator, vertex_iterator, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_iterator,
                            vertex_iterator,
                            EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor


  const_edge_iterator erase_edge(const_edge_iterator);

public:
  void clear();

protected:
  //vertex_iterator finalize_out_edges(vertex_range);
  void throw_unordered_edges() const;

private:
  vertex_set          vertices_;
  edge_size_type      edges_size_ = 0;
  edge_allocator_type edge_alloc_;
  friend vertex_type;
};

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_HPP

#include "detail/undirected_adj_list_impl.hpp"
