//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef __AOLOS_HPP
#define __AOLOS_HPP

#include <cassert>
#include <forward_list>
#include <tuple>
#include <vector>

namespace bgl17 {
template <typename... Attributes>
class array_of_list_of_structs {

public:
  array_of_list_of_structs(size_t N) : storage_(N) {}

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

private:
  std::vector<std::forward_list<std::tuple<Attributes...>>> storage_;
};

template <typename... Attributes>
class adj_list : public array_of_list_of_structs<size_t, Attributes...> {
public:
  adj_list(size_t N) : array_of_list_of_structs<size_t, Attributes...>(N) {}
};

} // namespace bgl17

#endif    // __AOLOS_HPP
