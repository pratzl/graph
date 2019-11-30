//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __COMPRESSED_HPP
#define __COMPRESSED_HPP

#include "edge_list.hpp"
#include "graph_base.hpp"
#include <iterator>

template<int idx, directedness sym = undirected, typename... Attributes>
class compressed_sparse : public graph_base {

public: typedef std::vector<size_t>::iterator inner_iterator;

public:
  compressed_sparse(size_t d0, size_t d1) : graph_base(d0, d1), ptrs(lim[0] + 1) {}
  compressed_sparse(edge_list<sym, Attributes...>& A) : graph_base(A), ptrs(lim[0] + 1) { A.fill(*this); }

  class inner_container_ref {
  public:
    typedef std::vector<size_t>::iterator inner_iterator;

    typedef inner_iterator iterator;

    inner_container_ref(std::vector<size_t>::iterator _begin, std::vector<size_t>::iterator _end)
        : inner_begin(_begin), inner_end(_end) {}
    inner_iterator begin() { return inner_begin; }
    inner_iterator end() { return inner_end; }

  private:
    size_t         cursor;
    inner_iterator inner_begin, inner_end;
  };

  // The graph is concpetually a container of container
  // outer iterates over the outer -- the spine stores ranges which can in turn be iterated over
  class outer_iterator {
  public:
    typedef size_t              difference_type;
    typedef inner_container_ref value_type;

    outer_iterator(std::vector<size_t>& _ptrs, std::tuple<std::vector<size_t>, std::vector<Attributes>...>& storage, size_t _init)
      : ptrs(_ptrs), attributes(storage), position(_init) {}

    outer_iterator      operator+(size_t offset) const { return outer_iterator(ptrs, attributes, position + offset); }
    outer_iterator      operator-(size_t offset) const { return outer_iterator(ptrs, attributes, position - offset); }

    inner_container_ref operator*() {
      return inner_container_ref(attributes.begin() + ptrs[position], attributes.begin() + ptrs[position + 1]);
    }

    inner_container_ref operator[](size_t index) {
      return inner_container_ref(attributes.begin() + ptrs[position + index], attributes.begin() + ptrs[position + index + 1]);
    }

    outer_iterator operator++() {
      ++position;
      return *this;
    }
    outer_iterator operator+=(size_t offset) {
      position += offset;
      return *this;
    }

    bool operator==(const outer_iterator& b) const { return position == b.position; }
    bool operator!=(const outer_iterator& b) const { return position != b.position; }
    bool operator<(const outer_iterator& b) const { return position < b.position; }

    difference_type operator-(const outer_iterator& b) const { return position - b.position; }

  private:
    std::vector<size_t>&                   ptrs;
    std::tuple<std::vector<size_t>, std::vector<Attributes>...>& attributes;
    size_t                                 position;
  };

public:
  outer_iterator begin() { return outer_iterator(ptrs, storage, 0); }
  outer_iterator end()   { return outer_iterator(ptrs, storage, lim[0]); }

  // outer_iterator begin() { return outer_iterator(ptrs, indices, 0); }
  // outer_iterator end() { return outer_iterator(ptrs, indices, lim[0]); }


public:    // bookkeeping for getting data in and out
  void open_for_push_back() { is_open = true; }

  void close_for_push_back() {
    for (size_t i = 0; i < lim[0]; ++i) {
      ptrs[i + 1] += ptrs[i];
    }
    for (size_t i = lim[0]; i > 0; --i) {
      ptrs[i] = ptrs[i - 1];
    }
    ptrs[0] = 0;

    is_open = false;
  }

  template<size_t... Is>
  void push_back_helper(std::tuple<Attributes...> attrs, std::integer_sequence<size_t, Is...>) {
    (std::get<Is>(storage).push_back(std::get<Is>(attrs)), ...);
  }

  // There is probably a way to do this with apply but I'm not smart enough to see it
  void push_back(size_t i, size_t j, std::tuple<Attributes...> attrs) {
    ++ptrs[i];
    push_back_helper(attrs, std::make_integer_sequence<size_t, sizeof...(Attributes)>());
  }

  void push_back(size_t i, size_t j, Attributes... attrs) { push_back(i, {j, attrs...}); }

private:
  // compressed format -- array of indices into vectors for begin and end of each set of neighbors
  std::vector<size_t> ptrs;

  // tuple of vectors
  std::tuple<std::vector<size_t>, std::vector<Attributes>...> storage;
};

#endif    // __COMPRESSED_HPP
