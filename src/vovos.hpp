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

template<typename... Attributes>
class vector_of_vector_of_structs {

public:
  vector_of_vector_of_structs(size_t N) : storage_(N) { }

  using inner_iterator = typename std::forward_list<std::tuple<Attributes...>>::iterator;
  using inner_container_ref = typename std::forward_list<std::tuple<Attributes...>>&;
  using outer_iterator = typename std::vector<std::forward_list<std::tuple<Attributes...>>>::iterator;

  auto begin() { return storage_.begin(); }
  auto end()   { return storage_.end(); }
  auto begin() const { return storage_.begin(); }
  auto end()   const { return storage_.end(); }

  void open_for_push_back() {}
  void close_for_push_back() {}
  void push_back(size_t i, Attributes... attrs) {
    storage_[i].emplace_front(attrs...);
  }

  auto size() const { return storage_.size(); }

private:
  std::vector<std::forward_list<std::tuple<Attributes...>>> storage_;
};


template<typename... Attributes>
class vov : public vector_of_vector_of_structs<size_t, Attributes...>{
public:
  vov(size_t N) : vector_of_vector_of_structs<size_t, Attributes...>(N) {}
};


#endif    // __VOVOS_HPP
