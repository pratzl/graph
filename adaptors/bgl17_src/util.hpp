//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cstddef>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <numeric>
#include <tuple>

class counting_output_iterator : public std::iterator<std::random_access_iterator_tag, size_t> {
public:
  counting_output_iterator(size_t& count) : count{count} {}
  void                      operator++() {}
  void                      operator++(int) {}
  counting_output_iterator& operator*() { return *this; }
  counting_output_iterator& operator[](size_t) { return *this; }

  template<typename T>
  void operator=(T) {
    count++;
  }
  size_t get_count() { return count; }

private:
  size_t& count;
};

typedef size_t vertex_index_t;
const auto null_vertex = std::numeric_limits<vertex_index_t>::max();

template <typename> struct is_tuple: std::false_type {};

template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};


template<typename InputIterator, typename RandomAccessIterator, typename = std::enable_if<is_tuple<typename InputIterator::value_type>::value>>
void histogram(InputIterator first, InputIterator last, RandomAccessIterator o_first, RandomAccessIterator o_last, size_t idx = 0) {
  std::fill(o_first, o_last, 0);
  std::for_each(first, last, [&] (auto &i) { o_first[std::get<idx>(i)]++ ; } );
};


template<typename InputIterator, typename RandomAccessIterator>
void histogram(InputIterator first, InputIterator last, RandomAccessIterator o_first, RandomAccessIterator o_last) {
  std::fill(o_first, o_last, 0);
  std::for_each(first, last, [&] (auto &i) { o_first[i]++ ; } );
};

template <typename T>
constexpr typename std::underlying_type<T>::type idx(T value) 
{
    return static_cast<typename std::underlying_type<T>::type>(value);
}

template<typename OuterIter>
auto get_source(OuterIter &outer) {
  return outer.get_index();
};

template<typename InnerIter>
vertex_index_t get_target(InnerIter &inner) {
  return std::get<0>(*inner);
};

template<size_t Idx, typename Iterator>
auto property(Iterator &inner) {
  return std::get<Idx>(*inner);
}

template<size_t Idx, typename Iterator>
auto property_ptr(Iterator &inner) {
  return &std::get<Idx>(*inner);
}

#endif    // __UTIL_HPP
