//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __ADJ_HPP
#define __ADJ_HPP

#include "graph_base.hpp"
#include <cassert>
#include <forward_list>
#include <vector>

template<int idx, directedness sym = undirected>
class adj_sparse : public graph_base {

public:
  typedef std::forward_list<size_t>::iterator              inner_iterator;
  typedef std::forward_list<size_t>&                       inner_container_ref;
  typedef std::vector<std::forward_list<size_t>>::iterator outer_iterator;

  outer_iterator begin() { return storage.begin(); }
  outer_iterator end() { return storage.end(); }

public:
  adj_sparse(size_t d0, size_t d1) : graph_base(d0, d1), storage(lim[0]) {}
  adj_sparse(sparse_aos<sym>& A) : graph_base(A), storage(lim[0]) { A.fill(*this); }

  void open_for_push_back() { is_open = true; }
  void close_for_push_back() { is_open = false; }
  void push_back(size_t i, size_t j);

private:
  std::vector<std::forward_list<size_t>> storage;
};

template<>
void adj_sparse<0>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  storage[i].push_front(j);
}

template<>
void adj_sparse<1>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  storage[j].push_front(i);
}

#endif    // __ADJ_HPP
