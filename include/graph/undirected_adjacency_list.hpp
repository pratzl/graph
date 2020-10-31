//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include "ordered_pair.hpp"
#include <vector>
#include <range/v3/view/subrange.hpp>
#include <cassert>
#include <type_traits>

#ifndef UNDIRECTED_ADJ_LIST_HPP
#  define UNDIRECTED_ADJ_LIST_HPP

namespace std::graph {


///-------------------------------------------------------------------------------------
/// undirected_adjacency_list forward declarations
///
/// All vertices are kept in a single vector with an index for the first outward edge.
///
/// All edges are kept in doubly-linked lists for both the source and target vertices.
///

template <typename VV   = empty_value,
          typename EV   = empty_value,
          typename GV   = empty_value,
          integral KeyT = uint32_t,
          typename A    = allocator<char>>
class undirected_adjacency_list;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_vertex;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_edge;


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto size(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto ssize(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g) noexcept
      -> vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cbegin(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cend(undirected_adjacency_list<VV, EV, GV, KeyT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;

///-------------------------------------------------------------------------------------

// designator types
struct inward_list;
struct outward_list;

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_edge_list;

template <typename VV, typename EV, typename GV, integral KeyT, typename A, typename ListT>
class ual_edge_list_link;


///-------------------------------------------------------------------------------------
/// ual_edge_list
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
/// @tparam ListT  inward_list|outward_list. Which edge list this is for.
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_edge_list {
public:
  class iterator;
  class const_iterator;

  using graph_type = undirected_adjacency_list<VV, EV, GV, KeyT, A>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set            = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, KeyT, A>;

  using edge_list_type              = ual_edge_list<VV, EV, GV, KeyT, A>;
  using edge_list_inward_link_type  = ual_edge_list_link<VV, EV, GV, KeyT, A, inward_list>;
  using edge_list_outward_link_type = ual_edge_list_link<VV, EV, GV, KeyT, A, outward_list>;
  using edge_allocator_type         = typename allocator_traits<A>::template rebind_alloc<edge_type>;

  using value_type      = edge_type;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using reference       = value_type&;
  using const_reference = value_type const&;
  using size_type       = size_t;
  using difference_type = ptrdiff_t;

  using edge_range       = decltype(ranges::make_subrange(iterator(), iterator()));
  using const_edge_range = decltype(ranges::make_subrange(const_iterator(), const_iterator()));

public:
  class const_iterator {
  public:
    using iterator_category = bidirectional_iterator_tag;
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

    const_iterator& operator--();
    const_iterator  operator--(int);

    bool operator==(const_iterator const& rhs) const noexcept;
    bool operator!=(const_iterator const& rhs) const noexcept;

    friend void swap(const_iterator& lhs, const_iterator& rhs) noexcept {
      swap(lhs.vertex_key_, rhs.vertex_key_);
      swap(lhs.edge_, rhs.edge_);
    }

  protected:
    void advance();
    void retreat();

  protected:
    vertex_key_type vertex_key_ = numeric_limits<vertex_key_type>::max(); // owning vertex for the list we're in
    edge_type*      edge_       = nullptr;                                // current vertex (==nullptr for end)
    graph_type*     graph_      = nullptr;
  }; // end const_iterator

  class iterator : public const_iterator {
  public:
    using base_t            = const_iterator;
    using iterator_category = typename base_t::iterator_category;
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
    ~iterator() noexcept                   = default;

    iterator& operator=(iterator const& rhs) = default;

    iterator(graph_type const& g, vertex_type const& u, edge_type const* uv = nullptr) : const_iterator(g, u, uv) {}

    reference operator*() const;
    pointer   operator->() const;

    iterator& operator++();
    iterator  operator++(int);

    iterator& operator--();
    iterator  operator--(int);

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
  void link_front(edge_type& uv, ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link);

  template <typename ListT>
  void link_back(edge_type& uv, ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link);

  template <typename ListT>
  void unlink(edge_type& uv, ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link);

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
/// @tparam ListT  inward_list|outward_list. Which edge list this is for.
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A, typename ListT>
class ual_edge_list_link {
public:
  using graph_type = undirected_adjacency_list<VV, EV, GV, KeyT, A>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set            = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;

  using edge_type = ual_edge<VV, EV, GV, KeyT, A>;

  using edge_list_type      = ual_edge_list<VV, EV, GV, KeyT, A>;
  using edge_list_link_type = ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>;

public:
  ual_edge_list_link(vertex_key_type ukey) noexcept : vertex_key_(ukey) {}

  ual_edge_list_link() noexcept                          = default;
  ual_edge_list_link(ual_edge_list_link const&) noexcept = default;
  ual_edge_list_link(ual_edge_list_link&&) noexcept      = default;
  ~ual_edge_list_link() noexcept                         = default;
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
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_edge
      : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>
      , public ual_edge_list_link<VV, EV, GV, KeyT, A, inward_list>
      , public ual_edge_list_link<VV, EV, GV, KeyT, A, outward_list> {
public:
  using graph_type            = undirected_adjacency_list<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = GV;
  using base_type             = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;

  using vertex_type            = ual_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_key_type        = KeyT;
  using vertex_index           = KeyT;
  using vertex_user_value_type = VV;

  using edge_key_type        = unordered_pair<vertex_key_type, vertex_key_type>;
  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, KeyT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_index           = KeyT;
  using edge_size_type       = size_t;
  using edge_ssize_type      = ptrdiff_t;
#  ifdef FUTURE
  using edge_iterator       = typename edge_set::iterator;
  using const_edge_iterator = typename edge_set::const_iterator;
#  endif

  using edge_list_type              = ual_edge_list<VV, EV, GV, KeyT, A>;
  using edge_list_inward_link_type  = ual_edge_list_link<VV, EV, GV, KeyT, A, inward_list>;
  using edge_list_outward_link_type = ual_edge_list_link<VV, EV, GV, KeyT, A, outward_list>;

protected:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_edge() noexcept                = default;
  ual_edge(ual_edge const&) noexcept = default;
  ual_edge(ual_edge&&) noexcept      = default;
  ~ual_edge() noexcept;
  ual_edge& operator=(ual_edge&) noexcept = default;
  ual_edge& operator=(ual_edge&&) noexcept = default;

  ual_edge(graph_type&, vertex_type& u, vertex_type& v) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, edge_user_value_type const&) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, edge_user_value_type&&) noexcept;

  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, edge_user_value_type const&) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, edge_user_value_type&&) noexcept;

  void link_front(vertex_type&, vertex_type&) noexcept;
  void link_back(vertex_type&, vertex_type&) noexcept;
  void unlink(vertex_type&, vertex_type&) noexcept;

public:
  vertex_iterator       inward_vertex(graph_type&) noexcept;
  const_vertex_iterator inward_vertex(graph_type const&) const noexcept;
  vertex_key_type       inward_vertex_key(graph_type const&) const noexcept;

  vertex_iterator       outward_vertex(graph_type&) noexcept;
  const_vertex_iterator outward_vertex(graph_type const&) const noexcept;
  vertex_key_type       outward_vertex_key(graph_type const&) const noexcept;

  vertex_iterator       other_vertex(graph_type&, vertex_key_type const inward_or_outward_key) noexcept;
  const_vertex_iterator other_vertex(graph_type const&, vertex_key_type const inward_or_outward_key) const noexcept;
  vertex_key_type       other_vertex_key(graph_type const&, vertex_key_type const inward_or_outward_key) const noexcept;

  edge_key_type edge_key(graph_type const& g) const noexcept;

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
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class ual_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
public:
  using graph_type            = undirected_adjacency_list<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = GV;
  using base_type             = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_type            = ual_vertex<VV, EV, GV, KeyT, A>;
  using vertex_allocator_type  = typename allocator_traits<A>::template rebind_alloc<vertex_type>;
  using vertex_set             = vector<vertex_type, vertex_allocator_type>;
  using vertex_iterator        = typename vertex_set::iterator;
  using const_vertex_iterator  = typename vertex_set::const_iterator;
  using vertex_key_type        = KeyT;
  using vertex_index           = KeyT;
  using vertex_user_value_type = VV;

  using edge_user_value_type = EV;
  using edge_type            = ual_edge<VV, EV, GV, KeyT, A>;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_size_type       = typename edge_type::edge_size_type;

  using edge_list_type              = ual_edge_list<VV, EV, GV, KeyT, A>;
  using edge_list_inward_link_type  = ual_edge_list_link<VV, EV, GV, KeyT, A, inward_list>;
  using edge_list_outward_link_type = ual_edge_list_link<VV, EV, GV, KeyT, A, outward_list>;
  using vertex_edge_size_type       = typename edge_list_type::size_type;
  using vertex_edge_iterator        = typename edge_list_type::iterator;
  using const_vertex_edge_iterator  = typename edge_list_type::const_iterator;
  using vertex_edge_range           = typename edge_list_type::edge_range;
  using const_vertex_edge_range     = typename edge_list_type::const_edge_range;

public:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_vertex()                      = default;
  ual_vertex(ual_vertex const&)     = default;
  ual_vertex(ual_vertex&&) noexcept = default;
  ~ual_vertex() noexcept            = default;
  ual_vertex& operator=(ual_vertex const&) = default;
  ual_vertex& operator=(ual_vertex&&) noexcept = default;

  ual_vertex(vertex_set& vertices, vertex_index index);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const&);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&&) noexcept;

public:
  vertex_key_type       vertex_key(graph_type const&) const noexcept;
  vertex_edge_size_type edges_size() const noexcept;

  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator);
  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator, vertex_edge_iterator);
  void                 clear_edges(graph_type&);

  vertex_edge_iterator       edges_begin(graph_type&) noexcept;
  const_vertex_edge_iterator edges_begin(graph_type const&) const noexcept;
  const_vertex_edge_iterator edges_cbegin(graph_type const&) const noexcept;

  vertex_edge_iterator       edges_end(graph_type&) noexcept;
  const_vertex_edge_iterator edges_end(graph_type const&) const noexcept;
  const_vertex_edge_iterator edges_cend(graph_type const&) const noexcept;

  edge_type&       edge_front(graph_type&) noexcept;
  edge_type const& edge_front(graph_type const&) const noexcept;

  edge_type&       edge_back(graph_type&) noexcept;
  edge_type const& edge_back(graph_type const&) const noexcept;

  vertex_edge_iterator create_edge(graph_type&, vertex_type&);
  vertex_edge_iterator create_edge(graph_type&, vertex_type&, edge_user_value_type const&);
  vertex_edge_iterator create_edge(graph_type&, vertex_type&, edge_user_value_type&&);

  template <class EV2>
  vertex_edge_iterator create_edge(graph_type&,
                                   vertex_key_type,
                                   EV2 const&); // EV2 must be accepted by vertex_user_value_type constructor

protected:
  void erase_edge(graph_type&, edge_type*);

private:
  edge_list_type edges_;
  friend edge_type;
  friend edge_list_inward_link_type;
  friend edge_list_outward_link_type;
};

/// A simple undirected adjacency list (graph).
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
class undirected_adjacency_list : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
public:
  using base_type             = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_type            = undirected_adjacency_list<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = GV;
  using allocator_type        = A;

  using vertex_type            = ual_vertex<VV, EV, GV, KeyT, A>;
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

  using edge_type            = ual_edge<VV, EV, GV, KeyT, A>;
  using edge_user_value_type = EV;
  using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;
  using edge_key_type        = unordered_pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_value_type      = pair<edge_key_type, edge_user_value_type>;
  using edge_size_type       = typename edge_type::edge_size_type;
  using edge_ssize_type      = typename edge_type::edge_ssize_type;
  // edge_set, edge_index_type n/a

  using vertex_edge_iterator       = typename vertex_type::vertex_edge_iterator;
  using const_vertex_edge_iterator = typename vertex_type::const_vertex_edge_iterator;
  using vertex_edge_range          = typename vertex_type::vertex_edge_range;
  using const_vertex_edge_range    = typename vertex_type::const_vertex_edge_range;
  using vertex_edge_size_type      = typename vertex_edge_iterator::size_type;
  using vertex_edge_ssize_type     = typename vertex_edge_iterator::difference_type;

  class edge_iterator;       // (defined below)
  class const_edge_iterator; // (defined below)
  using edge_range       = decltype(ranges::make_subrange(edge_iterator(), edge_iterator()));
  using const_edge_range = decltype(ranges::make_subrange(const_edge_iterator(), const_edge_iterator()));

  class const_edge_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge<VV, EV, GV, KeyT, A>;
    using size_type         = size_t;
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
      if (++uv_ != u_->edges_end(*g_))
        return;

      // find next vertex with edge(s)
      ++u_;
      advance_vertex();
    }

    void advance_vertex() {
      // at exit, if u_ != g.vertices().end() then uv_ will refer to a valid edge
      for (; u_ != g_->vertices().end(); ++u_) {
        if (u_->edges_size() > 0) {
          uv_ = u_->edges_begin(*g_);
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
    using base_t            = const_edge_iterator;
    using iterator_category = typename base_t::iterator_category;
    using value_type        = typename base_t::value_type;
    using size_type         = typename base_t::size_type;
    using difference_type   = typename base_t::difference_type;
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
  undirected_adjacency_list() noexcept(noexcept(allocator_type())) = default;
  undirected_adjacency_list(allocator_type const& alloc) noexcept;
  undirected_adjacency_list(graph_user_value_type const&, allocator_type const& alloc = allocator_type());
  undirected_adjacency_list(graph_user_value_type&&, allocator_type const& alloc = allocator_type());


  // The following constructors will load edges (and vertices) into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in
  // the edges. Edges are scanned first to determine the highest number and
  // the vertices are resized to match the number.
  //
  // Accessor functions are used to return the edge_key_type,
  // edge_user_value_type and vertex_user_value_type.
  //
  // The order visited in the vertices determines their index (and
  // key/identity) in the internal vertices vector. The edge keys use those
  // values and are also expected to be ordered by their first (in) vertex
  // key and an exception is thrown if they aren't in order. For these
  // reasons, unordered (hash) containers won't work.
  //

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_user_value_type, or
  ///                   a type that edge_user_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_user_value_type default constructor
  ///                   will be used to initialize the value.
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
  ///                   ekey_fnc(ERng::value_type) -> undirected_adjacency_list::edge_key_type
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
  undirected_adjacency_list(ERng const&      erng,
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
  ///                   ekey_fnc(ERng::value_type) -> undirected_adjacency_list::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, typename EKeyFnc, typename EValueFnc>
    requires edge_range_extractor<ERng, EKeyFnc, EValueFnc>
  undirected_adjacency_list(ERng const&      erng, 
                            EKeyFnc const&   ekey_fnc, 
                            EValueFnc const& evalue_fnc, 
                            GV const&        gv    = GV(), 
                            A const&         alloc = A());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with edge values.
  ///
  /// @param ilist Initializer list of tuples with inward_vertex_key,
  ///              outward_vertex_key and the edge value.
  /// @param alloc Allocator.
  ///
  undirected_adjacency_list(
        initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist,
        A const&                                                                               alloc = A());

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with edge values.
  ///
  /// @param ilist Initializer list of tuples with inward_vertex_key and
  ///              outward_vertex_key.
  /// @param alloc Allocator.
  ///
  undirected_adjacency_list(initializer_list<tuple<vertex_key_type, vertex_key_type>> const& ilist,
                            A const&                                                         alloc = A());

  ~undirected_adjacency_list();

  undirected_adjacency_list& operator=(undirected_adjacency_list const&) = default;
  undirected_adjacency_list& operator=(undirected_adjacency_list&&) = default;

public:
  constexpr edge_allocator_type edge_allocator() const noexcept;

  constexpr vertex_set&       vertices();
  constexpr vertex_set const& vertices() const;

  constexpr vertex_iterator       begin();
  constexpr const_vertex_iterator begin() const;
  constexpr const_vertex_iterator cbegin() const;

  constexpr vertex_iterator       end();
  constexpr const_vertex_iterator end() const;
  constexpr const_vertex_iterator cend() const;

  vertex_iterator       find_vertex(vertex_key_type);
  const_vertex_iterator find_vertex(vertex_key_type) const;

  constexpr edge_size_type edges_size() const noexcept;

  constexpr edge_iterator       edges_begin() { return edge_iterator(*this, begin()); }
  constexpr const_edge_iterator edges_begin() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).begin());
  }
  constexpr const_edge_iterator edges_cbegin() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).cbegin());
  }

  constexpr edge_iterator       edges_end() { return edge_iterator(*this, end()); }
  constexpr const_edge_iterator edges_end() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).end());
  }
  constexpr const_edge_iterator edges_cend() const {
    return const_edge_iterator(*this, const_cast<graph_type&>(*this).end());
  }

  edge_range       edges() { return ranges::make_subrange(edges_begin(), edges_end()); }
  const_edge_range edges() const { return ranges::make_subrange(edges_begin(), edges_end()); }

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, vertex_user_value_type const&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_user_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(VV2 const&); // vertex_user_value_type must be constructable from VV2

public:
  edge_iterator create_edge(vertex_key_type, vertex_key_type);
  edge_iterator create_edge(vertex_key_type, vertex_key_type, edge_user_value_type&&);

  template <class EV2>
  edge_iterator create_edge(vertex_key_type,
                            vertex_key_type,
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
  //vertex_iterator finalize_outward_edges(vertex_range);
  void throw_unordered_edges() const;

private:
  vertex_set          vertices_;
  edge_size_type      edges_size_ = 0;
  edge_allocator_type edge_alloc_;
  friend vertex_type;
};

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
struct graph_traits<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using graph_type            = undirected_adjacency_list<VV, EV, GV, KeyT, A>;
  using graph_user_value_type = typename graph_type::graph_user_value_type;
  using allocator_type        = typename graph_type::allocator_type;

  using vertex_type            = typename graph_type::vertex_type;
  using vertex_key_type        = typename graph_type::vertex_key_type;
  using vertex_user_value_type = typename graph_type::vertex_user_value_type;
  using vertex_value_type      = typename graph_type::vertex_value_type;

  using edge_type            = typename graph_type::edge_type;
  using edge_key_type        = typename graph_type::edge_key_type; // undirected_pair<from,to>
  using edge_user_value_type = typename graph_type::edge_user_value_type;
  using edge_value_type      = typename graph_type::edge_value_type;

  using vertex_range       = typename graph_type::vertex_range;
  using const_vertex_range = typename graph_type::const_vertex_range;
  using vertex_size_type   = typename graph_type::vertex_size_type;

  using vertex_edge_range       = typename graph_type::vertex_edge_range;
  using const_vertex_edge_range = typename graph_type::const_vertex_edge_range;
  using vertex_edge_size_type   = typename graph_type::vertex_edge_size_type;

  using edge_range       = typename graph_type::edge_range;
  using const_edge_range = typename graph_type::const_edge_range;
  using edge_size_type   = typename graph_type::edge_size_type;
};

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_HPP

#include "detail/undirected_adjacency_list_impl.hpp"
