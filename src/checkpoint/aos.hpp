//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2018
//


#ifndef __AOS_HPP
#define __AOS_HPP

#include <algorithm>
#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "adj.hpp"
#include "compressed.hpp"
#include "graph_base.hpp"
#include "vov.hpp"

template<directedness sym = undirected>
class sparse_aos : public graph_base {
private:
  typedef std::tuple<size_t, size_t> element;

public:
  sparse_aos(size_t d0, size_t d1) : graph_base(d0, d1) {}

  void clear() { storage.clear(); }
  void resize(size_t n) {
    assert(n >= 0);
    storage.resize(n);
  }
  void reserve(size_t n) {
    assert(n >= 0);
    storage.reserve(n);
  }

  void open_for_push_back() {}
  void close_for_push_back() {}

  void push_back(size_t i, size_t j) {
    assert(i < lim[0] && i >= 0);
    assert(j < lim[1] && j >= 0);

    storage.push_back(element(i, j));
  }

  template<int idx>
  void sort_by() {
    std::sort(storage.begin(), storage.end(),
              [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template<int idx>
  void stable_sort_by() {
    std::stable_sort(storage.begin(), storage.end(),
                     [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template<succession T = successor>
  void triangularize();

  template<succession T = successor>
  void symmetrize();

  template<int idx>
  void fill(compressed_sparse<idx, sym>& cs) {
    stable_sort_by<idx>();
    push_back_to(cs);
  }

  template<int idx>
  void fill(vov_sparse<idx, sym>& cs) {
    push_back_to(cs);
  }

  template<int idx>
  void fill(adj_sparse<idx, sym>& cs) {
    std::reverse(storage.begin(), storage.end());
    push_back_to(cs);
  }

  template<typename Mat>
  void fill(Mat& cs) {
    push_back_to(cs);
  }

#if 0
  template <int idx>
  void push_back_to(compressed_sparse<idx, sym>& cs) {
    cs.open_for_push_back();
    for (auto f : storage) {
      cs.push_back(std::get<0>(f), std::get<1>(f));
    }
    cs.close_for_push_back();
  }
#else
  template<typename Mat>
  void push_back_to(Mat& cs) {
    cs.open_for_push_back();
    for (auto f : storage) {
      cs.push_back(std::get<0>(f), std::get<1>(f));
    }
    cs.close_for_push_back();
  }
#endif

  void stream_header(std::ostream& outputFile) const;
  void stream_some_mm(std::ostream& outputFile, size_t n0, size_t n1) const {

    stream_header(outputFile);
    outputFile << "% Generated 17-Jul-2017" << std::endl;

    outputFile << n0 << " " << n1 << std::endl;

    // Write data
    for (size_t i = 0; i < storage.size(); ++i) {
      size_t j0 = std::get<0>(storage[i]);
      size_t j1 = std::get<1>(storage[i]);
      if (j0 < n0 && j1 < n1) {
        outputFile << j0 + 1 << " ";
        outputFile << j1 + 1 << " ";
        outputFile << 1 << std::endl;
      }
    }
  }

private:
  std::vector<element> storage;
};

template<>
template<>
void sparse_aos<undirected>::triangularize<successor>() {
  for (auto& f : storage) {
    if (std::get<0>(f) < std::get<1>(f)) {
      std::swap(std::get<0>(f), std::get<1>(f));
    }
  }
}

template<>
template<>
void sparse_aos<undirected>::symmetrize<successor>() {
  for (auto f : storage) {
    push_back(std::get<1>(f), std::get<0>(f));
  }
}

template<>
template<>
void sparse_aos<undirected>::symmetrize<predecessor>() {
  for (auto f : storage) {
    push_back(std::get<1>(f), std::get<0>(f));
  }
}

template<>
template<>
void sparse_aos<undirected>::triangularize<predecessor>() {
  for (auto& f : storage) {
    if (std::get<1>(f) < std::get<0>(f)) {
      std::swap(std::get<1>(f), std::get<0>(f));
    }
  }
}

template<>
void sparse_aos<undirected>::stream_header(std::ostream& outputFile) const {
  outputFile << "%%MatrixMarket matrix coordinate real unordered" << std::endl;
}

template<>
void sparse_aos<directed>::stream_header(std::ostream& outputFile) const {
  outputFile << "%%MatrixMarket matrix coordinate real general" << std::endl;
}

#endif    // __AOS_HPP
