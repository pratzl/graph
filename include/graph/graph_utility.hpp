#include "graph_fwd.hpp"

#ifndef GRAPH_UTILITY_HPP
#  define GRAPH_UTILITY_HPP

namespace std::graph {

//--------------------------------------------------------------------------------------
// graph_value<> - wraps scaler, union & reference user values for graph, vertex & edge
//
template <class T>
struct graph_value {
  graph_value()                   = default;
  graph_value(graph_value const&) = default;
  graph_value& operator=(graph_value const&) = default;
  graph_value(graph_value&& v) : value(move(v.value)) {}
  graph_value(T const& v) : value(v) {}
  graph_value(T&& v) : value(move(v)) {}

  T value = T();
};

template <class T>
struct graph_value_needs_wrap
      : integral_constant<bool,
                          is_scalar<T>::value || is_array<T>::value ||
                                is_union<T>::value || is_reference<T>::value> {};

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


} // namespace std::graph

#endif // GRAPH_UTILITY_HPP
