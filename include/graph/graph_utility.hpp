// graph_utility.hpp - useful definitions for different graphs
#pragma once
#include "graph_fwd.hpp"

namespace std::graph {

struct adj_list_type {};   // adjacency list
struct adj_array_type {};  // adjacency array; contiguous edges
struct adj_matrix_type {}; // adjacency matrix; edges stored in VxV matrix

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
                          is_scalar<T>::value || is_array<T>::value || is_union<T>::value || is_reference<T>::value> {};

template <class T>
T& get_user_value(T& v) {
  return v;
}
template <class T>
T& get_user_value(graph_value<T>& v) {
  return v.value;
}
template <class T>
T const& get_user_value(T const& v) {
  return v;
}
template <class T>
T const& get_user_value(graph_value<T> const& v) {
  return v.value;
}

//
// graph, vertex & edge declarations
//
template <typename ADJ,
          typename GV = empty_value,
          typename VV = empty_value,
          typename EV = empty_value,
          typename A  = allocator<char>>
class graph;

template <typename ADJ, typename VV, typename EV>
class vertex;

template <typename ADJ, typename VV, typename EV>
class edge;

} // namespace std::graph
