#include "graph.hpp"
#include <string>

#ifndef GRAPH_UTILITY_HPP
#  define GRAPH_UTILITY_HPP

namespace std::graph {


//--------------------------------------------------------------------------------------
// graph_value<> - wraps scaler, union & reference user values for graph, vertex & edge
//
template <class T>
struct graph_value_wrapper {
  graph_value_wrapper()                           = default;
  graph_value_wrapper(graph_value_wrapper const&) = default;
  graph_value_wrapper& operator=(graph_value_wrapper const&) = default;
  graph_value_wrapper(graph_value_wrapper&& v) : value(move(v.value)) {}
  graph_value_wrapper(T const& v) : value(v) {}
  graph_value_wrapper(T&& v) : value(move(v)) {}

  T value = T();
};

template <class T>
struct graph_value_needs_wrap
      : integral_constant<bool,
                          is_scalar<T>::value || is_array<T>::value || is_union<T>::value || is_reference<T>::value> {};

template <class T>
constexpr auto user_value(T& v) -> T& {
  return v;
}
template <class T>
constexpr auto user_value(T const& v) -> T const& {
  return v;
}
template <class T>
constexpr auto user_value(graph_value_t<T>& v) -> T& {
  return v.value;
}
template <class T>
constexpr auto user_value(graph_value_t<T> const& v) -> T const& {
  return v.value;
}


//
// Common Property Values
//
struct empty_value {}; // empty graph|vertex|edge value

struct weight_value {
  int weight = 0;

  weight_value()                    = default;
  weight_value(weight_value const&) = default;
  weight_value& operator=(weight_value const&) = default;
  weight_value(int const& w) : weight(w) {}
};

struct name_value {
  string name;

  name_value()                  = default;
  name_value(name_value const&) = default;
  name_value& operator=(name_value const&) = default;
  name_value(string const& s) : name(s) {}
  name_value(string&& s) : name(move(s)) {}
};


namespace detail {

  template <class G>
  class const_vertex_vertex_iterator {
  public:
    using this_t = const_vertex_vertex_iterator<G>;

    using graph_type           = G;
    using vertex_type          = vertex_t<G>;
    using vertex_edge_iterator = vertex_edge_iterator_t<G>;

    using iterator_category = forward_iterator_tag;
    using value_type        = vertex_type;
    using size_type         = typename vertex_edge_iterator::size_type;
    using difference_type   = typename vertex_edge_iterator::difference_type;
    using pointer           = const value_type*;
    using reference         = const value_type&;

  public:
    const_vertex_vertex_iterator(graph_type& g, vertex_type& u, vertex_edge_iterator uv) : g_(&g), uv_(uv) {}

    const_vertex_vertex_iterator()                                    = default;
    const_vertex_vertex_iterator(const const_vertex_vertex_iterator&) = default;
    const_vertex_vertex_iterator(const_vertex_vertex_iterator&&)      = default;
    ~const_vertex_vertex_iterator()                                   = default;

    const_vertex_vertex_iterator& operator=(const_vertex_vertex_iterator const&) = default;
    const_vertex_vertex_iterator& operator=(const_vertex_vertex_iterator&&) = default;

  public:
    reference operator*() const { return vertex(*g_, *uv_); }
    pointer   operator->() const { return &vertex(*g_, *uv_); }

    const_vertex_vertex_iterator& operator++() { return *++uv_; }

    const_vertex_vertex_iterator operator++(int) {
      const_vertex_vertex_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    bool operator==(const_vertex_vertex_iterator const& rhs) const noexcept { return g_ == rhs.g_ && uv_ == rhs.uv_; }
    bool operator!=(const_vertex_vertex_iterator const& rhs) const noexcept { return !operator==(rhs); }

  protected:
    graph_type*               g_ = nullptr;
    vertex_edge_iterator_t<G> uv_;
  };

  template <class G>
  class vertex_vertex_iterator : public const_vertex_vertex_iterator<G> {
  public:
    using this_t = vertex_vertex_iterator<G>;
    using base_t = const_vertex_vertex_iterator<G>;

    using graph_type           = typename base_t::graph_type;
    using vertex_type          = typename base_t::vertex_type;
    using vertex_edge_iterator = typename base_t::vertex_edge_iterator;

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
    vertex_vertex_iterator(graph_type& g, vertex_type& u, vertex_edge_iterator uv) : base_t(g, u) {}

    vertex_vertex_iterator()                              = default;
    vertex_vertex_iterator(const vertex_vertex_iterator&) = default;
    vertex_vertex_iterator(vertex_vertex_iterator&&)      = default;
    ~vertex_vertex_iterator()                             = default;

    vertex_vertex_iterator& operator=(vertex_vertex_iterator const&) = default;
    vertex_vertex_iterator& operator=(vertex_vertex_iterator&&) = default;

  public:
    reference operator*() const { return vertex(*g_, *uv_); }
    pointer   operator->() const { return &vertex(*g_, *uv_); }

    vertex_vertex_iterator& operator++() { return *++uv_; }

    vertex_vertex_iterator operator++(int) {
      vertex_vertex_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    bool operator==(vertex_vertex_iterator const& rhs) const noexcept { return base_t::operator==(rhs); }
    bool operator!=(vertex_vertex_iterator const& rhs) const noexcept { return !operator==(rhs); }
  };

  template <typename G>
  class vertex_vertex_range {
  public:
    using graph_type           = G;
    using vertex_type          = vertex_t<G>;
    using vertex_edge_iterator = vertex_edge_iterator_t<G>;

    using value_type      = vertex_type;
    using size_type       = typename vertex_edge_iterator::size_type;
    using ssize_type      = typename vertex_edge_iterator::difference_type;
    using difference_type = typename vertex_edge_iterator::difference_type;

    using const_iterator = const_vertex_vertex_iterator<G>;
    using iterator       = vertex_vertex_iterator<G>;

  public:
    vertex_vertex_range(graph_type& g, vertex_type& u) : g_(&g_), u_(&u) {}

    vertex_vertex_range()                           = default;
    vertex_vertex_range(const vertex_vertex_range&) = default;
    vertex_vertex_range(vertex_vertex_range&&)      = default;
    ~vertex_vertex_range()                          = default;

    vertex_vertex_range& operator=(vertex_vertex_range const&) = default;
    vertex_vertex_range& operator=(vertex_vertex_range&&) = default;

  public:
    iterator       begin() { return iterator(*g_, edges_begin(*g_, *u_)); }
    const_iterator begin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }
    const_iterator cbegin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }

    iterator       end() { return iterator(*g_, edges_end(*g_, *u_)); }
    const_iterator end() const { return const_iterator(*g_, edges_end(*g_, *u_)); }
    const_iterator cend() const { return const_iterator(*g_, edges_end(*g_, *u_)); }

    size_type  size() const noexcept { return edges_size(*g_, *u_); }
    ssize_type ssize() const noexcept { return edges_ssize(*g_, *u_); }

    bool operator==(vertex_vertex_range const& rhs) const noexcept { return g_ == rhs.g_ && u_ == rhs.u_; }
    bool operator!=(vertex_vertex_range const& rhs) const noexcept { return !operator==(rhs); }

  private:
    graph_type*  g_ = nullptr;
    vertex_type* u_ = nullptr;
  };

} // namespace detail

} // namespace std::graph

#endif // GRAPH_UTILITY_HPP
