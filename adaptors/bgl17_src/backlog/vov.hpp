//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __VOV_HPP
#define __VOV_HPP

#include "graph_base.hpp"
#include <iterator>

template<int idx, directedness sym = undirected>
class vov_sparse : public graph_base {

public:
  typedef std::vector<size_t>::iterator              inner_iterator;
  typedef std::vector<size_t>&                       inner_container_ref;
  typedef std::vector<std::vector<size_t>>::iterator outer_iterator;

  outer_iterator begin() { return storage.begin(); }
  outer_iterator end() { return storage.end(); }

public:
  vov_sparse(size_t d0, size_t d1) : graph_base(d0, d1), storage(lim[0]) {}
  vov_sparse(sparse_aos<sym>& A) : graph_base(A), storage(lim[0]) { A.fill(*this); }

  void open_for_push_back() { is_open = true; }
  void close_for_push_back() { is_open = false; }
  void push_back(size_t i, size_t j);

private:
  std::vector<std::vector<size_t>> storage;
};

template<>
void vov_sparse<0>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  storage[i].push_back(j);
}

template<>
void vov_sparse<1>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  storage[j].push_back(i);
}

#endif    // __VOV_HPP
