//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cstddef>
#include <iterator>

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

#endif    // __UTIL_HPP
