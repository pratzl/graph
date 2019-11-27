//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __EDGE_LIST_HPP
#define __EDGE_LIST_HPP

#include "aos.hpp"
#include "compressed.hpp"
#include "graph_base.hpp"
#include "plain_range.hpp"

namespace bgl17 {

template <int idx, directedness sym, typename... Attributes>
class compressed_sparse;

template <int idx, typename... Attributes>
class adjacency;

template <int idx, succession cessor, typename... Attributes>
class packed;

template <directedness edge_directedness = undirected, typename... Attributes>
class edge_list
      : public graph_base
      , public array_of_structs<size_t, size_t, Attributes...> {

  using element = std::tuple<size_t, size_t, Attributes...>;
  using base    = array_of_structs<size_t, size_t, Attributes...>;

public:
  edge_list(size_t N) : graph_base(N, N) {}

  void open_for_push_back() {}
  void close_for_push_back() {
    size_t min = 0xFFFFFFFFFFFFFFFFULL, max = 0;
    for (size_t k = 0; k < base::storage_.size(); ++k) {
      size_t i = std::get<0>(base::storage_[k]);
      size_t j = std::get<1>(base::storage_[k]);
      min      = std::min(i, std::min(j, min));
      max      = std::max(i, std::max(j, max));
    }
    if (min != 0) {
      for (size_t k = 0; k < base::storage_.size(); ++k) {
        std::get<0>(base::storage_[k]) -= min;
        std::get<1>(base::storage_[k]) -= min;
      }
      max -= min;
    }
    lim[0] = lim[1] = max + 1;
  }

  void push_back(size_t i, size_t j, Attributes... attrs) { base::push_back(i, j, attrs...); }
  void push_back(element& elem) { base::push_back(elem); }

  template <int idx>
  void sort_by() {
    std::sort(base::storage_.begin(), base::storage_.end(),
              [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx>
  void stable_sort_by() {
    std::stable_sort(base::storage_.begin(), base::storage_.end(),
                     [](const element& a, const element& b) -> bool { return (std::get<idx>(a) < std::get<idx>(b)); });
  }

  template <int idx, directedness sym>
  void fill(compressed_sparse<idx, sym, Attributes...>& cs) {
    stable_sort_by<idx>();
    cs.open_for_push_back();

    for (auto& elt : base::storage_) {
      std::apply([&](size_t i, size_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
    }

    cs.close_for_push_back();
  }

  template <int idx, succession cessor>
  void fill(packed<idx, cessor, Attributes...>& cs) {
    triangularize<idx, cessor>();
    sort_by<(idx + 1) % 2>();
    stable_sort_by<idx>();
    cs.open_for_push_back();

    for (auto& elt : base::storage_) {
      std::apply([&](size_t i, size_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
    }
    cs.close_for_push_back();
  }

  template <int idx>
  void fill(adjacency<idx, Attributes...>& cs) {
    if constexpr (edge_directedness == directed) {
      stable_sort_by<idx>();
      cs.open_for_push_back();

      for (auto& elt : base::storage_) {
        std::apply([&](size_t i, size_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
      }
      cs.close_for_push_back();

    } else {
      edge_list<edge_directedness, Attributes...> Tmp(0);
      Tmp.reserve(2 * base::storage_.size());
      Tmp.open_for_push_back();
      for (auto& elt : base::storage_) {
        std::apply([&](size_t i, size_t j, Attributes... attrs) { Tmp.push_back(i, j, attrs...); }, elt);
        std::apply([&](size_t i, size_t j, Attributes... attrs) { Tmp.push_back(j, i, attrs...); }, elt);
      }
      Tmp.close_for_push_back();

      Tmp.stable_sort_by<idx>();
      cs.open_for_push_back();

      for (auto& elt : Tmp.storage_) {
        std::apply([&](size_t i, size_t j, Attributes... attrs) { cs.push_back(i, j, attrs...); }, elt);
      }
      cs.close_for_push_back();
    }
  }

  size_t size() { return lim[0]; }

  template <int idx, succession cessor = predecessor>
  void triangularize() {
    if constexpr ((idx == 0 && cessor == predecessor) || (idx == 1 && cessor == successor)) {
      for (auto& f : base::storage_) {
        if (std::get<0>(f) < std::get<1>(f)) {
          std::swap(std::get<0>(f), std::get<1>(f));
        }
      }
    } else if constexpr ((idx == 0 && cessor == successor) || (idx == 1 && cessor == predecessor)) {
      for (auto& f : base::storage_) {
        if (std::get<1>(f) < std::get<0>(f)) {
          std::swap(std::get<1>(f), std::get<0>(f));
        }
      }
    }
  }
};

} // namespace bgl17

#endif    // __EDGE_LIST_HPP
