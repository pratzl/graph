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
