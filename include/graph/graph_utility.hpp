#include "graph.hpp"
#include <string>
#include <algorithm>

#ifndef GRAPH_UTILITY_HPP
#  define GRAPH_UTILITY_HPP

namespace std::graph {

//--------------------------------------------------------------------------------------
// graph_value<> - wraps scaler, union & reference user values for graph, vertex & edge
//
template <class T>
struct graph_value_wrapper {
  constexpr graph_value_wrapper()                 = default;
  graph_value_wrapper(const graph_value_wrapper&) = default;
  graph_value_wrapper& operator=(const graph_value_wrapper&) = default;
  graph_value_wrapper(graph_value_wrapper&& v) : value(move(v.value)) {}
  graph_value_wrapper(const T& v) : value(v) {}
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
constexpr auto user_value(const T& v) -> const T& {
  return v;
}
template <class T>
constexpr auto user_value(graph_value_t<T>& v) -> T& {
  return v.value;
}
template <class T>
constexpr auto user_value(const graph_value_t<T>& v) -> const T& {
  return v.value;
}


//
// Common Property Values
//
struct empty_value {}; // empty graph|vertex|edge value

struct weight_value {
  int weight = 0;

  constexpr weight_value()          = default;
  weight_value(const weight_value&) = default;
  weight_value& operator=(const weight_value&) = default;
  weight_value(const int& w) : weight(w) {}
};

struct name_value {
  string name;

  name_value()                  = default;
  name_value(const name_value&) = default;
  name_value& operator=(const name_value&) = default;
  name_value(const string& s) : name(s) {}
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
    using size_type         = ::size_t;
    using difference_type   = typename vertex_edge_iterator::difference_type;
    using pointer           = const value_type*;
    using reference         = const value_type&;

  public:
    constexpr const_vertex_vertex_iterator(graph_type& g, vertex_edge_iterator uv) : g_(&g), uv_(uv) {}

    constexpr const_vertex_vertex_iterator()                                    = default;
    constexpr const_vertex_vertex_iterator(const const_vertex_vertex_iterator&) = default;
    constexpr const_vertex_vertex_iterator(const_vertex_vertex_iterator&&)      = default;
    ~const_vertex_vertex_iterator()                                             = default;

    constexpr const_vertex_vertex_iterator& operator=(const const_vertex_vertex_iterator&) = default;
    constexpr const_vertex_vertex_iterator& operator=(const_vertex_vertex_iterator&&) = default;

  public:
    constexpr reference operator*() const { return *outward_vertex(*g_, *uv_); }
    constexpr pointer   operator->() const { return &*outward_vertex(*g_, &*uv_); }

    constexpr const_vertex_vertex_iterator& operator++() { return *++uv_; }

    constexpr const_vertex_vertex_iterator operator++(int) {
      const_vertex_vertex_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    constexpr bool operator==(const const_vertex_vertex_iterator& rhs) const noexcept {
      return g_ == rhs.g_ && uv_ == rhs.uv_;
    }
    constexpr bool operator!=(const const_vertex_vertex_iterator& rhs) const noexcept { return !operator==(rhs); }

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
    constexpr vertex_vertex_iterator(graph_type& g, vertex_edge_iterator uv) : base_t(g, uv) {}

    constexpr vertex_vertex_iterator()                              = default;
    constexpr vertex_vertex_iterator(const vertex_vertex_iterator&) = default;
    constexpr vertex_vertex_iterator(vertex_vertex_iterator&&)      = default;
    ~vertex_vertex_iterator()                                       = default;

    constexpr vertex_vertex_iterator& operator=(const vertex_vertex_iterator&) = default;
    constexpr vertex_vertex_iterator& operator=(vertex_vertex_iterator&&) = default;

  public:
    constexpr reference operator*() const { return *outward_vertex(*g_, *uv_); }
    constexpr pointer   operator->() const { return &*outward_vertex(*g_, &*uv_); }

    constexpr vertex_vertex_iterator& operator++() {
      ++uv_;
      return *this;
    }

    constexpr vertex_vertex_iterator operator++(int) {
      vertex_vertex_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    constexpr bool operator==(const vertex_vertex_iterator& rhs) const noexcept { return base_t::operator==(rhs); }
    constexpr bool operator!=(const vertex_vertex_iterator& rhs) const noexcept { return !operator==(rhs); }
  };


  template <typename G>
  class const_vertex_vertex_range {
  public:
    using graph_type              = G;
    using vertex_type             = vertex_t<G>;
    using const_vertex_edge_range = const_vertex_edge_range_t<G>;

    using value_type      = vertex_type;
    using size_type       = ranges::range_size_t<const_vertex_edge_range>;
    using ssize_type      = ranges::range_difference_t<const_vertex_edge_range>;
    using difference_type = ranges::range_difference_t<const_vertex_edge_range>;

    using const_iterator = const_vertex_vertex_iterator<G>;
    using iterator       = const_vertex_vertex_iterator<G>;

  public:
    constexpr const_vertex_vertex_range(graph_type& g, vertex_type& u) : g_(&g_), u_(&u) {}

    constexpr const_vertex_vertex_range()                                 = default;
    constexpr const_vertex_vertex_range(const const_vertex_vertex_range&) = default;
    constexpr const_vertex_vertex_range(const_vertex_vertex_range&&)      = default;
    ~const_vertex_vertex_range()                                          = default;

    constexpr const_vertex_vertex_range& operator=(const const_vertex_vertex_range&) = default;
    constexpr const_vertex_vertex_range& operator=(const_vertex_vertex_range&&) = default;

  public:
    constexpr const_iterator begin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }
    constexpr const_iterator cbegin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }

    constexpr const_iterator end() const { return const_iterator(*g_, edges_end(*g_, *u_)); }
    constexpr const_iterator cend() const { return const_iterator(*g_, edges_end(*g_, *u_)); }

    constexpr size_type  size() const noexcept { return edges_size(*g_, *u_); }
    constexpr ssize_type ssize() const noexcept { return edges_ssize(*g_, *u_); }

    constexpr bool operator==(const const_vertex_vertex_range& rhs) const noexcept {
      return g_ == rhs.g_ && u_ == rhs.u_;
    }
    constexpr bool operator!=(const const_vertex_vertex_range& rhs) const noexcept { return !operator==(rhs); }

  protected:
    graph_type*  g_ = nullptr;
    vertex_type* u_ = nullptr;
  };

  template <typename G>
  class vertex_vertex_range : public const_vertex_vertex_range<G> {
    using base_t = const_vertex_vertex_range<G>;
    using this_t = vertex_vertex_range<G>;

    using graph_type           = G;
    using vertex_type          = vertex_t<G>;
    using vertex_edge_iterator = vertex_edge_iterator_t<G>;

    using value_type      = typename base_t::value_type;
    using size_type       = typename base_t::size_type;
    using ssize_type      = typename base_t::ssize_type;
    using difference_type = typename base_t::difference_type;

    using const_iterator = const_vertex_vertex_iterator<G>;
    using iterator       = vertex_vertex_iterator<G>;

  protected:
    using base_t::g_;
    using base_t::u_;

  public:
    constexpr vertex_vertex_range(graph_type& g, vertex_type& u) : base_t(g, u) {}

    constexpr vertex_vertex_range()                           = default;
    constexpr vertex_vertex_range(const vertex_vertex_range&) = default;
    constexpr vertex_vertex_range(vertex_vertex_range&&)      = default;
    ~vertex_vertex_range()                                    = default;

    constexpr vertex_vertex_range& operator=(const vertex_vertex_range&) = default;
    constexpr vertex_vertex_range& operator=(vertex_vertex_range&&) = default;

  public:
    constexpr iterator       begin() { return iterator(*g_, edges_begin(*g_, *u_)); }
    constexpr const_iterator begin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }
    constexpr const_iterator cbegin() const { return const_iterator(*g_, edges_begin(*g_, *u_)); }

    constexpr iterator       end() { return iterator(*g_, edges_end(*g_, *u_)); }
    constexpr const_iterator end() const { return const_iterator(*g_, edges_end(*g_, *u_)); }
    constexpr const_iterator cend() const { return const_iterator(*g_, edges_end(*g_, *u_)); }

    constexpr bool operator==(const vertex_vertex_range& rhs) const noexcept { return base_t::operator==(rhs); }
    constexpr bool operator!=(const vertex_vertex_range& rhs) const noexcept { return base_t::operator!=(rhs); }
  };

  template <typename T>
  using uncvref_t = remove_cv<remove_reference<T>>;

  template <typename T>
  using iter_difference_t = typename incrementable_traits<uncvref_t<T>>::difference_type;

  template <typename I>
  using iter_size_t = conditional_t<is_integral<iter_difference_t<I>>::value,
                                    make_unsigned<iter_difference_t<I>>,
                                    iter_difference_t<I>>;


} // namespace detail

template <typename I, typename S>
constexpr ranges::subrange<I, S> make_subrange2(I i, S s) {
  return {i, s};
}

template <typename I, typename S>
requires input_or_output_iterator<I>&& sentinel_for<S, I>            //
      constexpr ranges::subrange<I, S, ranges::subrange_kind::sized> //
      make_subrange2(I i, S s, detail::iter_size_t<I> n) {
  return {i, s, n};
}

template <typename R>
constexpr auto make_subrange2(R&& r)
      -> ranges::subrange<ranges::iterator_t<R>,
                          ranges::sentinel_t<R>,
                          (ranges::sized_range<R> || sized_sentinel_for<ranges::sentinel_t<R>, ranges::iterator_t<R>>)
                                ? ranges::subrange_kind::sized
                                : ranges::subrange_kind::unsized> //
{
  return {static_cast<R&&>(r)};
}

} // namespace std::graph

#endif // GRAPH_UTILITY_HPP
