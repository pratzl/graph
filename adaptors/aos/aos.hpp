//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

/**
 * @brief 
 * 
 * @todo Add emplace functions
 * 
 * @file aos.hpp
 * @author your name
 * @date 2018-08-21
 */

#ifndef __AOS_HPP
#define __AOS_HPP

#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

// Bare bones array of structs (vector of tuples)
template<typename... Attributes>
class array_of_structs {
public:
  void clear() { storage_.clear(); }
  void resize(size_t n) { storage_.resize(n); }
  void reserve(size_t n) { storage_.reserve(n); }

  void push_back(const Attributes&... attrs) { storage_.push_back({attrs...}); }

  void push_back(const std::tuple<Attributes&...>& attrs) { storage_.push_back(attrs); }

  void push_back(const std::tuple<Attributes...>& attrs) { storage_.push_back(attrs); }

  auto begin() { return storage_.begin(); }
  auto end() { return storage_.end(); }

  template<size_t... Is>
  void print_helper(std::ostream& output_stream, std::tuple<Attributes...> attrs, std::index_sequence<Is...>) {
    output_stream << "( ";
    (..., (output_stream << (0 == Is ? "" : ", ") << std::get<Is>(attrs)));
    output_stream << " )" << std::endl;
  }

  void stream(std::ostream& output_stream, const std::string& msg = "") {
    output_stream << msg;
    for (auto& element : storage_) {
      print_helper(output_stream, element, std::make_index_sequence<sizeof...(Attributes)>());
    }
  }

  void stream(const std::string& msg = "") { stream(std::cout, msg); }

  size_t size() const { return size(); }

public:
  using storage_type = std::vector<std::tuple<Attributes...>>;
  using reference = typename std::iterator_traits<typename storage_type::iterator>::reference;
  storage_type storage_;
};

// Bare bones struct of arrays (tuple of vectors)
template<typename... Attributes>
class struct_of_arrays {

public:
  template<typename... RandomAccessIterators>
  class inner_iterator {

  public:
    inner_iterator() = default;
    inner_iterator(const RandomAccessIterators... iters, size_t _init = 0) : start({iters...}), cursor(_init) {}
    inner_iterator(const std::tuple<RandomAccessIterators...> iters, size_t _init = 0) : start(iters), cursor(_init) {}

  private:
    template<size_t... Is>
    constexpr auto star_helper(std::index_sequence<Is...>) {
      return std::forward_as_tuple(*(std::get<Is>(start) + cursor)...);
    }

    template<size_t... Is>
    constexpr auto bracket_helper(size_t i, std::index_sequence<Is...>) {
      return std::forward_as_tuple(std::get<Is>(start)[i + cursor]...);
    }

  public:
    auto& operator++() {
      ++cursor;
      return *this;
    }
    auto operator+(size_t offset) const { return inner_iterator(start, cursor + offset); }
    auto operator-(size_t offset) const { return inner_iterator(start, cursor - offset); }

    auto operator-(const inner_iterator& b) const { return cursor - b.cursor; }

    auto operator==(const inner_iterator& b) const { return cursor == b.cursor; }
    auto operator!=(const inner_iterator& b) const { return cursor != b.cursor; }

    using value_type        = typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::value_type...>;
    using reference         = typename std::tuple<typename std::iterator_traits<RandomAccessIterators>::reference...>;
    // For difference type we use first iterator's difference type as it would
    // not work to use a tuple of distances.
    using difference_type   = typename std::iterator_traits<std::tuple_element_t<0, std::tuple<RandomAccessIterators...>>>::difference_type;
    using pointer           = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    auto operator*() { return star_helper(std::make_integer_sequence<size_t, sizeof...(RandomAccessIterators)>()); }
    auto operator[](size_t i) { return bracket_helper(i, std::make_integer_sequence<size_t, sizeof...(RandomAccessIterators)>()); }

  private:
    std::tuple<RandomAccessIterators...> start;
    size_t                               cursor;
  };

  typedef std::tuple<Attributes...> value_type;

  template<size_t... Is>
  auto begin_helper(std::index_sequence<Is...>) {
    return inner_iterator<typename std::vector<Attributes>::iterator...>(std::get<Is>(storage_).begin()...);
  }

  template<size_t... Is>
  auto end_helper(std::index_sequence<Is...>) {
    return inner_iterator(std::get<Is>(storage_).end()...);
  }

  auto begin() { return begin_helper(std::make_integer_sequence<size_t, sizeof...(Attributes)>()); }
  auto end() { return end_helper(std::make_integer_sequence<size_t, sizeof...(Attributes)>()); }

  template<size_t... Is>
  void push_back_helper(std::tuple<Attributes...> attrs, std::integer_sequence<size_t, Is...>) {
    (std::get<Is>(storage_).push_back(std::get<Is>(attrs)), ...);
  }

  void push_back(std::tuple<Attributes...> attrs) {
    push_back_helper(attrs, std::make_integer_sequence<size_t, sizeof...(Attributes)>());
  }

  void push_back(Attributes... attrs) { push_back(std::tuple<Attributes...>(attrs...)); }

  void clear() {
    std::apply([&](auto&... vs) { (vs.clear(), ...); }, storage_);
  }
  void resize(size_t N) {
    std::apply([&](auto&... vs) { (vs.resize(N), ...); }, storage_);
  }
  void reserve(size_t N) {
    std::apply([&](auto&... vs) { (vs.reserve(N), ...); }, storage_);
  }

  size_t size() const { return std::get<0>(storage_).size(); }

public:
  typedef inner_iterator<typename std::vector<Attributes>::iterator...> iterator;

public:
  using storage_type = std::tuple<std::vector<Attributes>...>;
  storage_type storage_;
};

#endif    // __AOS_HPP
