//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __UTIL_PAR_HPP
#define __UTIL_PAR_HPP

#include <atomic>
#include <cstddef>
#include <iterator>
#include <mutex>

std::mutex mtx;

class par_counting_output_iterator : public std::iterator<std::random_access_iterator_tag, size_t> {
public:
  par_counting_output_iterator(size_t& count) : count{count} {}
  void                          operator++() {}
  void                          operator++(int) {}
  par_counting_output_iterator& operator*() { return *this; }
  par_counting_output_iterator& operator[](size_t) { return *this; }

  template<typename T>
  void operator=(T) {
    std::lock_guard<std::mutex> lock(mtx);
    count++;
  }
  size_t get_count() { return count; }

private:
  size_t& count;
};

class atomic_counting_output_iterator : public std::iterator<std::random_access_iterator_tag, size_t> {
public:
  atomic_counting_output_iterator(std::atomic<size_t>& count) : count{count} {}
  void                             operator++() {}
  void                             operator++(int) {}
  atomic_counting_output_iterator& operator*() { return *this; }
  atomic_counting_output_iterator& operator[](size_t) { return *this; }

  template<typename T>
  void operator=(T) {
    count++;
  }
  size_t get_count() { return count; }

private:
  std::atomic<size_t>& count;
};

typedef size_t vertex_index_t;

#endif    // __UTIL_HPP
