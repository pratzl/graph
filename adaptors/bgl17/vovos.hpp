//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef __VOVOS_HPP
#define __VOVOS_HPP

#include <cassert>
#include <forward_list>
#include <vector>
#include <tuple>
#include <graph/graph_utility.hpp>

namespace bgl17 {

#if 0
template <typename EV = std::graph::empty_value>
class vector_of_vector_of_structs {

public:
  vector_of_vector_of_structs(size_t N) : storage_(N) {}

  using inner_iterator      = typename std::forward_list<EV>::iterator;
  using inner_container_ref = typename std::forward_list<EV>&;
  using outer_iterator      = typename std::vector<std::forward_list<EV>>::iterator;

  auto begin() { return storage_.begin(); }
  auto end() { return storage_.end(); }
  auto begin() const { return storage_.begin(); }
  auto end() const { return storage_.end(); }

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, EV&& attr) { storage_[i].emplace_front(attr); }

  auto size() const { return storage_.size(); }

public:
  using graph_value_t = std::graph::empty_value;

  using vertices_type = std::vector<std::forward_list<EV>>;

  using vertex_type    = std::forward_list<EV>;
  using vertex_key_t   = typename vertices_type::size_type;
  using vertex_value_t = typename std::graph::empty_value;
  //using vertex_range_t =
  using vertex_iterator = typename vertices_type::iterator;
  using vertex_sentinal = typename vertices_type::iterator;
  using vertex_size     = typename vertex_type::size_type;

  using edge_type       = EV;
  using edge_user_value = EV;
  //using edge_range = ...
  using edge_iterator  = typename vertex_type::iterator;
  using edge_sentinal  = typename vertex_type::iterator;
  using edge_size_type = typename vertex_type::size_type;

  using out_edge_iterator  = typename vertex_type::iterator;
  using out_edge_sentinal  = typename vertex_type::iterator;
  using out_edge_size_type = typename vertex_type::size_type;

  // no in_edge types

private:
  std::vector<std::forward_list<EV>> storage_;
};


template <typename EV>
class vov : public vector_of_vector_of_structs<size_t, EV> {
public:
  vov(size_t N) : vector_of_vector_of_structs<size_t, Attributes...>(N) {}
};
#else
template <typename... Attributes>
class vector_of_vector_of_structs {

public:
  vector_of_vector_of_structs(size_t N) : storage_(N) {}

  using inner_iterator      = typename std::forward_list<std::tuple<Attributes...>>::iterator;
  using inner_container_ref = typename std::forward_list<std::tuple<Attributes...>>&;
  using outer_iterator      = typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator;

  auto begin() { return storage_.begin(); }
  auto end() { return storage_.end(); }
  auto begin() const { return storage_.begin(); }
  auto end() const { return storage_.end(); }

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) { storage_[i].emplace_front(attrs...); }

  auto size() const { return storage_.size(); }

public:
  using graph_value_t = std::graph::empty_value;

  using vertices_type = std::vector<std::forward_list<std::tuple<Attributes...>>>;

  using vertex_type    = std::forward_list<std::tuple<Attributes...>>;
  using vertex_key_t   = typename vertices_type::size_type;
  using vertex_value_t = typename std::graph::empty_value;
  //using vertex_range_t =
  using vertex_iterator = typename vertices_type::iterator;
  using vertex_sentinal = typename vertices_type::iterator;
  using vertex_size     = typename vertex_type::size_type;

  using edge_type       = std::tuple<Attributes...>;
  using edge_user_value = std::tuple<Attributes...>;
  //using edge_range = ...
  using edge_iterator  = typename vertex_type::iterator;
  using edge_sentinal  = typename vertex_type::iterator;
  using edge_size_type = typename vertex_type::size_type;

  using out_edge_iterator  = typename vertex_type::iterator;
  using out_edge_sentinal  = typename vertex_type::iterator;
  using out_edge_size_type = typename vertex_type::size_type;

  // no in_edge types

private:
  std::vector<std::forward_list<std::tuple<Attributes...>>> storage_;
};


template <typename... Attributes>
class vov : public vector_of_vector_of_structs<size_t, Attributes...> {
public:
  vov(size_t N) : vector_of_vector_of_structs<size_t, Attributes...>(N) {}
};
#endif


} // namespace bgl17

#endif // __VOVOS_HPP
