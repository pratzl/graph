//
//	Author: J. Phillip Ratzloff
//
#include "graph_utility.hpp"
#include <vector>
#include <ranges>
#include <cassert>
#include <type_traits>

#ifndef UNDIRECTED_ADJ_LIST_HPP
#  define UNDIRECTED_ADJ_LIST_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// undirected_adjacency_list forward declarations
///
/// All vertices are kept in a single random-access container with an index for the
/// first outward edge.
///
/// All edges are kept in doubly-linked lists for both the inward and outward vertices.
///
/// A vector is used as the default container though any random-access container with
/// <T,A> (type, allocator) template parameters can be used.
///

template <typename VV                                        = empty_value,
          typename EV                                        = empty_value,
          typename GV                                        = empty_value,
          integral KeyT                                      = uint32_t,
          template <typename V, typename A> class VContainer = vector,
          typename Alloc                                     = allocator<char>>
class undirected_adjacency_list;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_edge;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_const_vertex_vertex_iterator;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex_vertex_iterator;


///-------------------------------------------------------------------------------------

// designator types
struct inward_list;
struct outward_list;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex_edge_list;

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename A,
          typename ListT>
class ual_vertex_edge_list_link;


///-------------------------------------------------------------------------------------
/// ual_vertex_edge_list
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
/// @tparam ListT  inward_list|outward_list. Which edge list this is for.
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex_edge_list {
public:
  class iterator;
  class const_iterator;

  using graph_type = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;

  using edge_value_type = EV;
  using edge_type       = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;

  using edge_list_type                    = ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_edge_list_inward_link_type = ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, inward_list>;
  using vertex_edge_list_outward_link_type =
        ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, outward_list>;
  using edge_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;

  using value_type      = edge_type;
  using pointer         = value_type*;
  using const_pointer   = value_type const*;
  using reference       = value_type&;
  using const_reference = const value_type&;
  using size_type       = size_t;
  using difference_type = ptrdiff_t;

  using edge_range       = ranges::subrange<iterator, iterator, ranges::subrange_kind::sized>;
  using const_edge_range = ranges::subrange<const_iterator, const_iterator, ranges::subrange_kind::sized>;

public:
  class const_iterator {
  public:
    using iterator_category = bidirectional_iterator_tag;
    using value_type        = ual_vertex_edge_list::edge_type;
    using difference_type   = ual_vertex_edge_list::difference_type;
    using size_type         = ual_vertex_edge_list::size_type;
    using pointer           = ual_vertex_edge_list::const_pointer;
    using const_pointer     = ual_vertex_edge_list::const_pointer;
    using reference         = ual_vertex_edge_list::const_reference;
    using const_reference   = ual_vertex_edge_list::const_reference;

    using graph_type  = ual_vertex_edge_list::graph_type;
    using vertex_type = ual_vertex_edge_list::vertex_type;
    using edge_type   = ual_vertex_edge_list::edge_type;

    const_iterator(const graph_type& g, vertex_key_type ukey, const edge_type* uv = nullptr) noexcept
          : vertex_key_(ukey), edge_(const_cast<edge_type*>(uv)), graph_(const_cast<graph_type*>(&g)) {}

    const_iterator() noexcept                          = default;
    const_iterator(const const_iterator& rhs) noexcept = default;
    const_iterator(const_iterator&&) noexcept          = default;
    ~const_iterator() noexcept                         = default;

    const_iterator& operator=(const const_iterator&) noexcept = default;
    const_iterator& operator=(const_iterator&&) noexcept = default;

    reference operator*() const;
    pointer   operator->() const;

    const_iterator& operator++();
    const_iterator  operator++(int);

    const_iterator& operator--();
    const_iterator  operator--(int);

    bool operator==(const const_iterator& rhs) const noexcept;
    bool operator!=(const const_iterator& rhs) const noexcept;

    friend void swap(const_iterator& lhs, const_iterator& rhs) noexcept {
      swap(lhs.vertex_key_, rhs.vertex_key_);
      swap(lhs.edge_, rhs.edge_);
    }

    graph_type&       graph() { return *graph_; }
    const graph_type& graph() const { return *graph_; }

    vertex_key_type source_key() const { return vertex_key_; }

  protected:
    void advance();
    void retreat();

  protected:
    vertex_key_type vertex_key_ = numeric_limits<vertex_key_type>::max(); // source vertex for the list we're in
    edge_type*      edge_       = nullptr;                                // current edge (==nullptr for end)
    graph_type*     graph_      = nullptr;
  }; // end const_iterator

  class iterator : public const_iterator {
  public:
    using base_t            = const_iterator;
    using iterator_category = typename base_t::iterator_category;
    using value_type        = ual_vertex_edge_list::edge_type;
    using difference_type   = ual_vertex_edge_list::difference_type;
    using size_type         = ual_vertex_edge_list::size_type;
    using pointer           = ual_vertex_edge_list::pointer;
    using const_pointer     = ual_vertex_edge_list::const_pointer;
    using reference         = ual_vertex_edge_list::reference;
    using const_reference   = ual_vertex_edge_list::const_reference;

    using graph_type  = ual_vertex_edge_list::graph_type;
    using vertex_type = ual_vertex_edge_list::vertex_type;
    using edge_type   = ual_vertex_edge_list::edge_type;

    iterator() noexcept                = default;
    iterator(const iterator&) noexcept = default;
    iterator(iterator&&) noexcept      = default;
    ~iterator() noexcept               = default;

    iterator& operator=(const iterator&) = default;
    iterator& operator=(iterator&&) = default;

    iterator(const graph_type& g, vertex_key_type ukey, const edge_type* uv = nullptr) : const_iterator(g, ukey, uv) {}

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
  ual_vertex_edge_list() noexcept                            = default;
  ual_vertex_edge_list(const ual_vertex_edge_list&) noexcept = default;
  ~ual_vertex_edge_list() noexcept                           = default;
  ual_vertex_edge_list& operator=(const ual_vertex_edge_list&) noexcept = default;

  ual_vertex_edge_list(ual_vertex_edge_list&& rhs) noexcept
        : head_(move(rhs.head_)), tail_(move(rhs.tail_)), size_(move(rhs.size_)) {
    rhs.head_ = rhs.tail_ = nullptr;
    rhs.size_             = 0;
  }
  ual_vertex_edge_list& operator=(ual_vertex_edge_list&& rhs) noexcept = default;

  size_type size() const noexcept;
  bool      empty() const noexcept;

  edge_type&       front();
  const edge_type& front() const;

  edge_type&       back();
  const edge_type& back() const;

public:
  template <typename ListT>
  void link_front(edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link);

  template <typename ListT>
  void link_back(edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link);

  template <typename ListT>
  void unlink(edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link);

  iterator       begin(graph_type& g, vertex_key_type ukey) noexcept;
  const_iterator begin(const graph_type& g, vertex_key_type ukey) const noexcept;
  const_iterator cbegin(const graph_type& g, vertex_key_type ukey) const noexcept;

  iterator       end(graph_type& g, vertex_key_type ukey) noexcept;
  const_iterator end(const graph_type& g, vertex_key_type ukey) const noexcept;
  const_iterator cend(const graph_type& g, vertex_key_type ukey) const noexcept;

  edge_range       edges(graph_type& g, vertex_key_type ukey) noexcept;
  const_edge_range edges(const graph_type& g, vertex_key_type ukey) const noexcept;

private:
  edge_type* head_ = nullptr;
  edge_type* tail_ = nullptr;
  size_type  size_ = 0;
};

///-------------------------------------------------------------------------------------
/// ual_vertex_edge_list_link
///
/// @tparam VV     Vertex Value type. default = empty_value.
/// @tparam EV     Edge Value type. default = empty_value.
/// @tparam GV     Graph Value type. default = empty_value.
/// @tparam IntexT The type used for vertex & edge index into the internal vectors.
/// @tparam A      Allocator. default = std::allocator
/// @tparam ListT  inward_list|outward_list. Which edge list this is for.
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc,
          typename ListT>
class ual_vertex_edge_list_link {
public:
  using graph_type = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;

  using edge_type = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;

  using edge_list_type      = ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using edge_list_link_type = ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>;

public:
  ual_vertex_edge_list_link(vertex_key_type ukey) noexcept : vertex_key_(ukey) {}

  ual_vertex_edge_list_link() noexcept                                 = default;
  ual_vertex_edge_list_link(const ual_vertex_edge_list_link&) noexcept = default;
  ual_vertex_edge_list_link(ual_vertex_edge_list_link&&) noexcept      = default;
  ~ual_vertex_edge_list_link() noexcept                                = default;
  ual_vertex_edge_list_link& operator=(const ual_vertex_edge_list_link&) noexcept = default;
  ual_vertex_edge_list_link& operator=(ual_vertex_edge_list_link&&) noexcept = default;

public:
  vertex_key_type       vertex_key() const noexcept { return vertex_key_; }
  const_vertex_iterator vertex(const graph_type& g) const { return g.vertices().begin() + vertex_key_; }
  vertex_iterator       vertex(graph_type& g) { return g.vertices().begin() + vertex_key_; }

  edge_type*       next() noexcept { return next_; }
  const edge_type* next() const noexcept { return next_; }
  edge_type*       prev() noexcept { return prev_; }
  const edge_type* prev() const noexcept { return prev_; }

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
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_edge
      : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV>
      , public ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, inward_list>
      , public ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, outward_list> {
public:
  using graph_type       = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using graph_value_type = GV;
  using base_type        = conditional_t<graph_value_needs_wrap<EV>::value, graph_value_wrapper<EV>, EV>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;
  using vertex_value_type     = VV;

  using edge_key_type        = pair<vertex_key_type, vertex_key_type>;
  using edge_value_type      = EV;
  using edge_type            = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;
  using edge_allocator_type  = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_index           = KeyT;
  using edge_size_type       = size_t;
  using edge_difference_type = ptrdiff_t;

  using edge_list_type                    = ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_edge_list_inward_link_type = ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, inward_list>;
  using vertex_edge_list_outward_link_type =
        ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, outward_list>;

protected:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_edge() noexcept                = default;
  ual_edge(const ual_edge&) noexcept = default;
  ual_edge(ual_edge&&) noexcept      = default;
  ~ual_edge() noexcept;
  ual_edge& operator=(ual_edge&) noexcept = default;
  ual_edge& operator=(ual_edge&&) noexcept = default;

#  if 1
  ual_edge(graph_type&, vertex_key_type ukey, vertex_key_type vkey) noexcept;
  ual_edge(graph_type&, vertex_key_type ukey, vertex_key_type vkey, const edge_value_type&) noexcept;
  ual_edge(graph_type&, vertex_key_type ukey, vertex_key_type vkey, edge_value_type&&) noexcept;
#  else
  ual_edge(graph_type&, vertex_type& u, vertex_type& v) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, const edge_value_type&) noexcept;
  ual_edge(graph_type&, vertex_type& u, vertex_type& v, edge_value_type&&) noexcept;
#  endif

  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, const edge_value_type&) noexcept;
  ual_edge(graph_type&, vertex_iterator ui, vertex_iterator vi, edge_value_type&&) noexcept;

  void link_front(vertex_type&, vertex_type&) noexcept;
  void link_back(vertex_type&, vertex_type&) noexcept;
  void unlink(vertex_type&, vertex_type&) noexcept;

public:
  vertex_iterator       source_vertex(graph_type&) noexcept;
  const_vertex_iterator source_vertex(const graph_type&) const noexcept;
  vertex_key_type       source_vertex_key(const graph_type&) const noexcept;

  vertex_iterator       target_vertex(graph_type&) noexcept;
  const_vertex_iterator target_vertex(const graph_type&) const noexcept;
  vertex_key_type       target_vertex_key(const graph_type&) const noexcept;

  vertex_iterator       other_vertex(graph_type&, const_vertex_iterator other) noexcept;
  const_vertex_iterator other_vertex(const graph_type&, const_vertex_iterator other) const noexcept;
  vertex_iterator       other_vertex(graph_type&, vertex_key_type other_key) noexcept;
  const_vertex_iterator other_vertex(const graph_type&, vertex_key_type other_key) const noexcept;

  vertex_key_type other_vertex_key(const graph_type&, const_vertex_iterator other) const noexcept;
  vertex_key_type other_vertex_key(const graph_type&, vertex_key_type other_key) const noexcept;

  edge_key_type edge_key(const graph_type& g) const noexcept;

  friend graph_type;     // the graph is the one to create & destroy edges because it owns the allocator
  friend vertex_type;    // vertex can also destroy its own edges
  friend edge_list_type; // for delete, when clearing the list
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
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV> {
public:
  using graph_type       = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using graph_value_type = GV;
  using base_type        = conditional_t<graph_value_needs_wrap<VV>::value, graph_value_wrapper<VV>, VV>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;
  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_key_type       = KeyT;
  using vertex_index          = KeyT;
  using vertex_value_type     = VV;

  using edge_value_type      = EV;
  using edge_type            = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;
  using edge_allocator_type  = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_size_type       = typename edge_type::edge_size_type;
  using edge_difference_type = typename edge_type::edge_difference_type;

  using vertex_edge_list_type             = ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_edge_list_inward_link_type = ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, inward_list>;
  using vertex_edge_list_outward_link_type =
        ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, outward_list>;
  using vertex_edge_size_type      = typename vertex_edge_list_type::size_type;
  using vertex_edge_iterator       = typename vertex_edge_list_type::iterator;
  using const_vertex_edge_iterator = typename vertex_edge_list_type::const_iterator;
  using vertex_edge_range          = typename vertex_edge_list_type::edge_range;
  using const_vertex_edge_range    = typename vertex_edge_list_type::const_edge_range;

  using vertex_vertex_iterator       = ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>;
  using const_vertex_vertex_iterator = ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_vertex_range =
        ranges::subrange<vertex_vertex_iterator, vertex_vertex_iterator, ranges::subrange_kind::sized>;
  using const_vertex_vertex_range =
        ranges::subrange<const_vertex_vertex_iterator, const_vertex_vertex_iterator, ranges::subrange_kind::sized>;
  using vertex_vertex_size_type       = edge_size_type;
  using vertex_vertex_difference_type = edge_difference_type;

public:
  // noexcept is only defined for move ctor & assignment b/c the user-defined value type could
  // throw an exception in other cases
  ual_vertex()                      = default;
  ual_vertex(const ual_vertex&)     = default;
  ual_vertex(ual_vertex&&) noexcept = default;
  ~ual_vertex() noexcept            = default;
  ual_vertex& operator=(const ual_vertex&) = default;
  ual_vertex& operator=(ual_vertex&&) noexcept = default;

  ual_vertex(vertex_set& vertices, vertex_index index);
  ual_vertex(vertex_set& vertices, vertex_index index, const vertex_value_type&);
  ual_vertex(vertex_set& vertices, vertex_index index, vertex_value_type&&) noexcept;

public:
#  if 0
  vertex_key_type       vertex_key(const graph_type&) const noexcept;
#  endif
  vertex_edge_size_type edges_size() const;

  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator);
  vertex_edge_iterator erase_edge(graph_type&, vertex_edge_iterator, vertex_edge_iterator);
  void                 clear_edges(graph_type&);

  vertex_edge_iterator       edges_begin(graph_type&, vertex_key_type ukey) noexcept;
  const_vertex_edge_iterator edges_begin(const graph_type&, vertex_key_type ukey) const noexcept;
  const_vertex_edge_iterator edges_cbegin(const graph_type&, vertex_key_type ukey) const noexcept;

  vertex_edge_iterator       edges_end(graph_type&, vertex_key_type ukey) noexcept;
  const_vertex_edge_iterator edges_end(const graph_type&, vertex_key_type ukey) const noexcept;
  const_vertex_edge_iterator edges_cend(const graph_type&, vertex_key_type ukey) const noexcept;

  vertex_edge_range       edges(graph_type&, vertex_key_type ukey);
  const_vertex_edge_range edges(const graph_type&, vertex_key_type ukey) const;

  edge_type&       edge_front(graph_type&) noexcept;
  const edge_type& edge_front(const graph_type&) const noexcept;

  edge_type&       edge_back(graph_type&) noexcept;
  const edge_type& edge_back(const graph_type&) const noexcept;

  vertex_vertex_iterator       vertices_begin(graph_type&, vertex_key_type ukey);
  const_vertex_vertex_iterator vertices_begin(const graph_type&, vertex_key_type ukey) const;
  const_vertex_vertex_iterator vertices_cbegin(const graph_type&, vertex_key_type ukey) const;

  vertex_vertex_iterator       vertices_end(graph_type&, vertex_key_type ukey);
  const_vertex_vertex_iterator vertices_end(const graph_type&, vertex_key_type ukey) const;
  const_vertex_vertex_iterator vertices_cend(const graph_type&, vertex_key_type ukey) const;

  vertex_vertex_range       vertices(graph_type&, vertex_key_type ukey);
  const_vertex_vertex_range vertices(const graph_type&, vertex_key_type ukey) const;

  vertex_vertex_size_type vertices_size(const graph_type&) const;

protected:
  void erase_edge(graph_type&, edge_type*);

  vertex_edge_iterator e_begin(graph_type const&, vertex_key_type ukey) const;
  vertex_edge_iterator e_end(graph_type const&, vertex_key_type ukey) const;

private:
  vertex_edge_list_type edges_;
  friend edge_type;
  friend vertex_edge_list_inward_link_type;
  friend vertex_edge_list_outward_link_type;
};

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_const_vertex_vertex_iterator {
public:
  using this_t = ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>;

  using graph_type = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;

  using vertex_type           = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_value_type     = VV;
  using vertex_key_type       = KeyT;
  using vertex_allocator_type = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set            = VContainer<vertex_type, vertex_allocator_type>;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;

  using edge_list_type             = ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_edge_size_type      = typename edge_list_type::size_type;
  using vertex_edge_iterator       = typename edge_list_type::iterator;
  using const_vertex_edge_iterator = typename edge_list_type::const_iterator;

  using edge_type       = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;
  using edge_value_type = typename edge_type::edge_value_type;
  using edge_key_type   = typename edge_type::edge_key_type; // <from,to>

  using iterator_category = bidirectional_iterator_tag;
  using value_type        = vertex_type;
  using size_type         = typename edge_list_type::size_type;
  using difference_type   = typename edge_list_type::difference_type;
  using pointer           = const value_type*;
  using const_pointer     = const value_type*;
  using reference         = const value_type&;
  using const_reference   = const value_type&;

public:
  constexpr ual_const_vertex_vertex_iterator(vertex_edge_iterator const& uv);

  constexpr ual_const_vertex_vertex_iterator()                                        = default;
  constexpr ual_const_vertex_vertex_iterator(const ual_const_vertex_vertex_iterator&) = default;
  constexpr ual_const_vertex_vertex_iterator(ual_const_vertex_vertex_iterator&&)      = default;
  ~ual_const_vertex_vertex_iterator()                                                 = default;

  constexpr ual_const_vertex_vertex_iterator& operator=(const ual_const_vertex_vertex_iterator&) = default;
  constexpr ual_const_vertex_vertex_iterator& operator=(ual_const_vertex_vertex_iterator&&) = default;

public:
  constexpr graph_type&       graph() noexcept;
  constexpr const graph_type& graph() const noexcept;

  constexpr const_vertex_iterator other_vertex() const;
  constexpr vertex_key_type       other_vertex_key() const;

  constexpr reference operator*() const noexcept;
  constexpr pointer   operator->() const noexcept;

  constexpr ual_const_vertex_vertex_iterator& operator++() noexcept;
  constexpr ual_const_vertex_vertex_iterator  operator++(int) noexcept;

  constexpr ual_const_vertex_vertex_iterator& operator--() noexcept;
  constexpr ual_const_vertex_vertex_iterator  operator--(int) noexcept;

  constexpr bool operator==(const ual_const_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator!=(const ual_const_vertex_vertex_iterator& rhs) const noexcept;

  friend void swap(ual_const_vertex_vertex_iterator& lhs, ual_const_vertex_vertex_iterator& rhs) {
    swap(lhs.uv_, rhs.uv_);
  }

protected:
  vertex_edge_iterator uv_;
};

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
class ual_vertex_vertex_iterator : public ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc> {
public:
  using this_t = ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>;
  using base_t = ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>;

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
  using const_pointer     = const value_type*;
  using reference         = value_type&;
  using const_reference   = const value_type&;

protected:
  using base_t::uv_;

public:
  constexpr ual_vertex_vertex_iterator(vertex_edge_iterator const& uv);

  constexpr ual_vertex_vertex_iterator()                                  = default;
  constexpr ual_vertex_vertex_iterator(const ual_vertex_vertex_iterator&) = default;
  constexpr ual_vertex_vertex_iterator(ual_vertex_vertex_iterator&&)      = default;
  ~ual_vertex_vertex_iterator()                                           = default;

  constexpr ual_vertex_vertex_iterator& operator=(const ual_vertex_vertex_iterator&) = default;
  constexpr ual_vertex_vertex_iterator& operator=(ual_vertex_vertex_iterator&&) = default;

public:
  constexpr vertex_iterator other_vertex();

  constexpr reference operator*() const;
  constexpr pointer   operator->() const;

  constexpr ual_vertex_vertex_iterator& operator++();
  constexpr ual_vertex_vertex_iterator  operator++(int);

  constexpr ual_vertex_vertex_iterator& operator--() noexcept;
  constexpr ual_vertex_vertex_iterator  operator--(int) noexcept;

  constexpr bool operator==(const ual_vertex_vertex_iterator& rhs) const noexcept;
  constexpr bool operator!=(const ual_vertex_vertex_iterator& rhs) const noexcept;

  friend void swap(ual_vertex_vertex_iterator& lhs, ual_vertex_vertex_iterator& rhs) { swap(lhs.uv_, rhs.uv_); }
};

/// A simple undirected adjacency list (graph).
///
/// @tparam VV              Vertex Value type. default = empty_value.
/// @tparam EV              Edge Value type. default = empty_value.
/// @tparam GV              Graph Value type. default = empty_value.
/// @tparam IntexT          The type used for vertex & edge index into the internal vectors.
/// @tparam VContainer<V,A> Random-access container type used to store vertices (V) with allocator (A).
/// @tparam Alloc           Allocator. default = std::allocator
///
// clang-format off
template <typename                                VV,
          typename                                EV,
          typename                                GV,
          integral                                KeyT,
          template <typename V, typename A> class VContainer,
          typename                                Alloc>
class undirected_adjacency_list 
  : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV>
// clang-format on
{
public:
  using base_type        = conditional_t<graph_value_needs_wrap<GV>::value, graph_value_wrapper<GV>, GV>;
  using graph_type       = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using graph_value_type = GV;
  using allocator_type   = Alloc;

  using vertex_type            = ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>;
  using vertex_value_type      = VV;
  using vertex_key_type        = KeyT;
  using vertex_index_type      = KeyT;
  using vertex_allocator_type  = typename allocator_traits<Alloc>::template rebind_alloc<vertex_type>;
  using vertex_set             = VContainer<vertex_type, vertex_allocator_type>;
  using vertex_size_type       = typename vertex_set::size_type;
  using vertex_difference_type = typename vertex_set::difference_type;

  using vertex_iterator       = typename vertex_set::iterator;
  using const_vertex_iterator = typename vertex_set::const_iterator;
  using vertex_range          = vertex_set&;
  using const_vertex_range    = const vertex_set&;

  using edge_type            = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;
  using edge_value_type      = EV;
  using edge_allocator_type  = typename allocator_traits<Alloc>::template rebind_alloc<edge_type>;
  using edge_key_type        = pair<vertex_key_type, vertex_key_type>; // <from,to>
  using edge_size_type       = typename edge_type::edge_size_type;
  using edge_difference_type = typename edge_type::edge_difference_type;
  // edge_set, edge_index_type n/a

  using vertex_edge_iterator        = typename vertex_type::vertex_edge_iterator;
  using const_vertex_edge_iterator  = typename vertex_type::const_vertex_edge_iterator;
  using vertex_edge_range           = typename vertex_type::vertex_edge_range;
  using const_vertex_edge_range     = typename vertex_type::const_vertex_edge_range;
  using vertex_edge_size_type       = typename vertex_edge_iterator::size_type;
  using vertex_edge_difference_type = typename vertex_edge_iterator::difference_type;

  class edge_iterator;       // (defined below)
  class const_edge_iterator; // (defined below)
  using edge_range       = ranges::subrange<edge_iterator, edge_iterator, ranges::subrange_kind::sized>;
  using const_edge_range = ranges::subrange<const_edge_iterator, const_edge_iterator, ranges::subrange_kind::sized>;

  class const_edge_iterator {
  public:
    using iterator_category = forward_iterator_tag;
    using value_type        = ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>;
    using size_type         = size_t;
    using difference_type   = ptrdiff_t;
    using pointer           = value_type const*;
    using reference         = const value_type&;

  public:
    const_edge_iterator(const graph_type& g, vertex_iterator u) : g_(&const_cast<graph_type&>(g)), u_(u) {
      advance_vertex();
    }
    const_edge_iterator(const graph_type& g, vertex_iterator u, vertex_edge_iterator uv)
          : g_(&const_cast<graph_type&>(g)), u_(u), uv_(uv) {}

    const_edge_iterator() noexcept                               = default;
    const_edge_iterator(const const_edge_iterator& rhs) noexcept = default;
    ~const_edge_iterator() noexcept                              = default;
    const_edge_iterator& operator=(const const_edge_iterator& rhs) = default;

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

    bool operator==(const const_edge_iterator& rhs) const noexcept { return uv_ == rhs.uv_ && u_ == rhs.u_; }
    bool operator!=(const const_edge_iterator& rhs) const noexcept { return !operator==(rhs); }

  protected:
    void advance_edge() {
      // next edge for current vertex
      vertex_key_type ukey = vertex_key(*g_, u_);
      if (++uv_ != u_->edges_end(*g_, ukey))
        return;

      // find next vertex with edge(s)
      ++u_;
      advance_vertex();
    }

    void advance_vertex() {
      // at exit, if u_ != g.vertices().end() then uv_ will refer to a valid edge
      for (; u_ != g_->vertices().end(); ++u_) {
        if (u_->edges_size() > 0) {
          vertex_key_type ukey = vertex_key(*g_, u_);
          uv_                  = u_->edges_begin(*g_, ukey);
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
    edge_iterator(const edge_iterator& rhs) noexcept : const_edge_iterator(rhs) {}
    edge_iterator(const_edge_iterator& rhs) : const_edge_iterator(rhs) {}
    ~edge_iterator() {}
    edge_iterator& operator=(const edge_iterator& rhs) noexcept {
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
  undirected_adjacency_list()                                         = default;
  undirected_adjacency_list(undirected_adjacency_list&& rhs) noexcept = default;
  undirected_adjacency_list(const undirected_adjacency_list&)         = default;

  // clang-format off
  undirected_adjacency_list(const allocator_type& alloc);
  undirected_adjacency_list(const graph_value_type&, 
                            const allocator_type& alloc = allocator_type());
  undirected_adjacency_list(graph_value_type&&, 
                            const allocator_type& alloc = allocator_type());
  // clang-format on

  // The following constructors will load edges (and vertices) into the graph
  //
  // The number of vertices is guaranteed to match the highest vertex key in
  // the edges. Edges are scanned first to determine the highest number and
  // the vertices are resized to match the number.
  //
  // Accessor functions are used to return the edge_key_type,
  // edge_value_type and vertex_value_type.
  //
  // The order visited in the vertices determines their index (and
  // key/identity) in the internal vertices container. The edge keys use those
  // values and are also expected to be ordered by their first (in) vertex
  // key and an exception is thrown if they aren't in order. For these
  // reasons, unordered (hash) containers won't work.
  //

  /// Constructor that takes edge & vertex ranges to create the graph.
  ///
  /// @tparam ERng      The edge data range.
  /// @tparam EKeyFnc   Function object to return edge_key_type of the
  ///                   ERng::value_type.
  /// @tparam EValueFnc Function object to return the edge_value_type, or
  ///                   a type that edge_value_type is constructible
  ///                   from. If the return type is void or empty_value the
  ///                   edge_value_type default constructor
  ///                   will be used to initialize the value.
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
    requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> 
          &&  vertex_value_extractor<VRng, VValueFnc>
  undirected_adjacency_list(const ERng&      erng,
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
  ///                   ekey_fnc(ERng::value_type) -> undirected_adjacency_list::edge_key_type
  /// @param evalue_fnc The edge value extractor functor:
  ///                   evalue_fnc(ERng::value_type) -> edge_value_t<G>.
  /// @param alloc      The allocator to use for internal containers for
  ///                   vertices & edges.
  ///
  // clang-format off
  template <typename ERng, typename EKeyFnc, typename EValueFnc>
    requires edge_value_extractor<ERng, EKeyFnc, EValueFnc>
  undirected_adjacency_list(const ERng&      erng, 
                            const EKeyFnc&   ekey_fnc, 
                            const EValueFnc& evalue_fnc, 
                            const GV&        gv    = GV(), 
                            const Alloc&     alloc = Alloc());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with edge values.
  ///
  /// @param ilist Initializer list of tuples with source_vertex_key,
  ///              target_vertex_key and the edge value.
  /// @param alloc Allocator.
  ///
  // clang-format off
  undirected_adjacency_list(
    const initializer_list<
          tuple<vertex_key_type, vertex_key_type, edge_value_type>>& ilist,
    const Alloc&                                                     alloc = Alloc());
  // clang-format on

  /// Constructor for easy creation of a graph that takes an initializer
  /// list with edge values.
  ///
  /// @param ilist Initializer list of tuples with source_vertex_key and
  ///              target_vertex_key.
  /// @param alloc Allocator.
  ///
  // clang-format off
  undirected_adjacency_list(
    const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist,
    const Alloc&                                                     alloc = Alloc());
  // clang-format on

  ~undirected_adjacency_list();

  undirected_adjacency_list& operator=(const undirected_adjacency_list&) = default;
  undirected_adjacency_list& operator=(undirected_adjacency_list&&) = default;

public:
  constexpr edge_allocator_type edge_allocator() const noexcept;

  constexpr vertex_set&       vertices();
  constexpr const vertex_set& vertices() const;

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

  edge_range       edges() { return {edges_begin(), edges_end(), edges_size_}; }
  const_edge_range edges() const { return {edges_begin(), edges_end(), edges_size_}; }

protected:
  void reserve_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type);
  void resize_vertices(vertex_size_type, const vertex_value_type&);

  vertex_iterator create_vertex();
  vertex_iterator create_vertex(vertex_value_type&&);

  template <class VV2>
  vertex_iterator create_vertex(const VV2&); // vertex_value_type must be constructable from VV2

public:
  vertex_edge_iterator create_edge(vertex_key_type, vertex_key_type);
  vertex_edge_iterator create_edge(vertex_key_type, vertex_key_type, edge_value_type&&);

  template <class EV2>
  vertex_edge_iterator create_edge(vertex_key_type,
                                   vertex_key_type,
                                   const EV2&); // EV2 must be accepted by vertex_value_type constructor

  vertex_edge_iterator create_edge(vertex_iterator, vertex_iterator);
  vertex_edge_iterator create_edge(vertex_iterator, vertex_iterator, edge_value_type&&);

  template <class EV2>
  vertex_edge_iterator create_edge(vertex_iterator,
                                   vertex_iterator,
                                   const EV2&); // EV2 must be accepted by vertex_value_type constructor


  edge_iterator erase_edge(edge_iterator);

public:
  void clear();
  void swap(undirected_adjacency_list&);

protected:
  //vertex_iterator finalize_outward_edges(vertex_range);
  void throw_unordered_edges() const;

private:
  vertex_set          vertices_;
  edge_size_type      edges_size_ = 0;
  edge_allocator_type edge_alloc_;
  friend vertex_type;
};

#  ifdef CPO
// clang-format off
template <typename                                VV,
          typename                                EV,
          typename                                GV,
          integral                                KeyT,
          template <typename V, typename A> class VContainer,
          typename                                Alloc>
struct graph_traits<undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>>
// clang-format on
{
  using graph_type       = undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>;
  using graph_value_type = typename graph_type::graph_value_type;
  using allocator_type   = typename graph_type::allocator_type;

  using vertex_type       = typename graph_type::vertex_type;
  using vertex_key_type   = typename graph_type::vertex_key_type;
  using vertex_value_type = typename graph_type::vertex_value_type;

  using edge_type       = typename graph_type::edge_type;
  using edge_key_type   = pair<vertex_key_type, vertex_key_type>;
  using edge_value_type = typename graph_type::edge_value_type;

  using vertex_range       = typename graph_type::vertex_range;
  using const_vertex_range = typename graph_type::const_vertex_range;

  using edge_range       = typename graph_type::edge_range;
  using const_edge_range = typename graph_type::const_edge_range;

  using vertex_edge_range       = typename vertex_type::vertex_edge_range;
  using const_vertex_edge_range = typename vertex_type::const_vertex_edge_range;

  using vertex_vertex_range       = typename vertex_type::vertex_vertex_range;
  using const_vertex_vertex_range = typename vertex_type::const_vertex_vertex_range;
};
#  endif

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_HPP

#include "detail/undirected_adjacency_list_api.hpp"
#include "detail/undirected_adjacency_list_impl.hpp"
