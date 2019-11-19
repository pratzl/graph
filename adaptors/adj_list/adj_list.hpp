//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//

#ifndef SIMPLE_ADJ_LIST_HPP
#define SIMPLE_ADJ_LIST_HPP

#include <cassert>
#include <forward_list>
#include <vector>
#include <tuple>

namespace simple {

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

//
// Core graph types
//

template <class VV = empty_value, class EV = empty_value, class GV = empty_value, class A = std::allocator<char>>
struct adjaceny_list;

template <class VV, class EV>
class vertex;

template <class VV, class EV>
class edge;




#if 0
template <typename VV

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

private:
  std::vector<std::forward_list<std::tuple<Attributes...>>> storage_;
};


template <typename... Attributes>
class vov : public vector_of_vector_of_structs<size_t, Attributes...> {
public:
  vov(size_t N) : vector_of_vector_of_structs<size_t, Attributes...>(N) {}
};
#endif //0

} // namespace simple


#endif // SIMPLE_ADJ_LIST_HPP
