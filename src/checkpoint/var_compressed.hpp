//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __VAR_COMPRESSED_HPP
#define __VAR_COMPRESSED_HPP

#include "aos.hpp"
#include "graph_base.hpp"
#include <iterator>

template<int idx, directedness sym = undirected, typename... Attributes>
class compressed_sparse : public graph_base {

public:
  typedef std::vector<size_t>::iterator inner_iterator;

  class inner_container_ref {
  public:
    typedef inner_iterator iterator;

    inner_container_ref(std::vector<size_t>::iterator _begin, std::vector<size_t>::iterator _end)
        : inner_begin(_begin), inner_end(_end) {}
    inner_iterator begin() { return inner_begin; }
    inner_iterator end() { return inner_end; }

  private:
    inner_iterator inner_begin, inner_end;
  };

  class outer_iterator {
  public:
    typedef size_t              difference_type;
    typedef inner_container_ref value_type;

    outer_iterator(std::vector<size_t>& _ptrs, std::vector<size_t>& _indices, size_t _init)
        : ptrs(_ptrs), indices(_indices), position(_init) {}

    outer_iterator      operator+(size_t offset) const { return outer_iterator(ptrs, indices, position + offset); }
    outer_iterator      operator-(size_t offset) const { return outer_iterator(ptrs, indices, position - offset); }
    inner_container_ref operator*() {
      return inner_container_ref(indices.begin() + ptrs[position], indices.begin() + ptrs[position + 1]);
    }
    inner_container_ref operator[](size_t index) {
      return inner_container_ref(indices.begin() + ptrs[position + index], indices.begin() + ptrs[position + index + 1]);
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
    std::vector<size_t>& ptrs;
    std::vector<size_t>& indices;
    size_t               position;
  };

  outer_iterator begin() { return outer_iterator(ptrs, indices, 0); }
  outer_iterator end() { return outer_iterator(ptrs, indices, lim[0]); }

public:
  compressed_sparse(size_t d0, size_t d1) : graph_base(d0, d1), ptrs(lim[0] + 1) {}
  compressed_sparse(sparse_aos<sym>& A) : graph_base(A), ptrs(lim[0] + 1) { A.fill(*this); }

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
    indices.push_back(j);
    push_back_helper(attrs, std::make_integer_sequence<size_t, sizeof...(Attributes)>());
  }

  void push_back(size_t i, size_t j, Attributes... attrs) { push_back(i, j, {attrs...}); }

  // void push_back(size_t i, size_t j);

private:
  std::vector<size_t>                    ptrs;
  std::vector<size_t>                    indices;
  std::tuple<std::vector<Attributes>...> storage;
};

#if 0

template<>
void compressed_sparse<0, directed>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  ++ptrs[i];
  indices.push_back(j);
}

template<>
void compressed_sparse<0, undirected>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  ++ptrs[i];
  indices.push_back(j);
}

template<>
void compressed_sparse<1, directed>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  ++ptrs[j];
  indices.push_back(i);
}

template<>
void compressed_sparse<1, undirected>::push_back(size_t i, size_t j) {
  assert(is_open);
  assert(i < lim[0] && i >= 0);
  assert(j < lim[1] && j >= 0);

  ++ptrs[j];
  indices.push_back(i);
}
#endif

typedef compressed_sparse<0>             csr_graph;
typedef compressed_sparse<1>             csc_graph;
typedef compressed_sparse<0, undirected> unordered_csr_graph;
typedef compressed_sparse<1, undirected> unordered_csc_graph;
typedef compressed_sparse<0, directed>   ordered_csr_graph;
typedef compressed_sparse<1, directed>   ordered_csc_graph;

#endif    // __COMPRESSED_HPP
