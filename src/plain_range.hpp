//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __PLAIN_RANGE_HPP
#define __PLAIN_RANGE_HPP

#include "util.hpp"
#include <queue>
#include <vector>
#include <tuple>

template<typename Graph>
class plain_range {
public:
  plain_range(Graph& g) : the_graph_(g) {}

  class v_range_iterator {
  private:
    plain_range<Graph>&            the_range_;
    typename Graph::outer_iterator first;

  public:
    v_range_iterator(plain_range<Graph>& range, size_t offset = 0)
        : the_range_(range), first(the_range_.the_graph_.begin() + offset) {}

    v_range_iterator& operator++() {
      ++first;

      return *this;
    }

    auto operator*() { return std::tuple<vertex_index_t>(first - the_range_.the_graph_.begin()); }

    auto operator==(const v_range_iterator& b) const { return first == b.first; }
    bool operator!=(const v_range_iterator& b) const { return first != b.first; }
  };

  typedef v_range_iterator iterator;

  auto begin() { return v_range_iterator(*this); }
  auto end() { return v_range_iterator(*this, the_graph_.size()); }

private:
  Graph& the_graph_;
};

template<typename Graph>
class plain_edge_range {
public:
  plain_edge_range(Graph& g) : the_graph_(g) {}
  plain_edge_range(const plain_edge_range& b) : the_graph_(b.the_graph_) {}
  auto& operator=(const plain_edge_range& b) {
    the_graph_ = b.the_graph_;
    return *this;
  }

  class v_range_iterator {
  private:
    plain_edge_range<Graph>&       the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    v_range_iterator(plain_edge_range<Graph>& range, size_t offset = 0)
        : the_range_(range), first(the_range_.the_graph_.begin() + offset), last(the_range_.the_graph_.end())
      {
          if (first != last) {
              u_begin = (*first).begin();
              u_end = (*first).end();
          }
      }

    v_range_iterator(const v_range_iterator& b)
        : the_range_(b.the_range_), first(b.first), last(b.last), u_begin(b.u_begin), u_end(b.u_end) {}

    v_range_iterator& operator++() {
      ++u_begin;
      while (u_begin == u_end) {
        if (++first == last) break;
        u_begin = (*first).begin();
        u_end   = (*first).end();
      }

      return *this;
    }

    // auto operator*() { return std::tuple<vertex_index_t, vertex_index_t&&>(last - first, std::get<0>(*u_begin)); }

    auto operator*() {
      return std::tuple<vertex_index_t, vertex_index_t>(first - the_range_.the_graph_.begin(), std::get<0>(*u_begin));
    }

    auto operator==(const v_range_iterator& b) const { return first == b.first; }
    auto operator!=(const v_range_iterator& b) const { return first != b.first; }

    auto operator<(const v_range_iterator& b) const { return first < b.first; }
    auto operator-(const v_range_iterator& b) const { return first - b.first; }
    auto operator+(unsigned long step) const { return v_range_iterator(the_range_, step); }

    auto& operator=(const v_range_iterator& b) {
      the_range_ = b.the_range_;
      first      = b.first;
      last       = b.last;
      u_begin    = b.u_begin;
      u_end      = b.u_end;
      return *this;
    }

    //    typedef std::random_access_iterator_tag            iterator_category;
    typedef std::forward_iterator_tag                  iterator_category;
    typedef size_t                                     difference_type;
    typedef std::tuple<vertex_index_t, vertex_index_t> value_type;
    typedef value_type*                                pointer;
    typedef value_type&                                reference;
  };

  typedef v_range_iterator iterator;

  auto begin() { return v_range_iterator(*this); }
  auto end() { return v_range_iterator(*this, the_graph_.size()); }

private:
  Graph& the_graph_;
};

template<typename Graph>
class plain_degree_range {
public:
  plain_degree_range(Graph& g) : the_graph_(g) {}

  class degree_range_iterator {
  private:
    plain_degree_range<Graph>&            the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    degree_range_iterator(plain_degree_range<Graph>& range, size_t offset = 0)
      : the_range_(range), first(the_range_.the_graph_.begin() + offset), last(the_range_.the_graph_.end())
      {
          if (first != last) {
              u_begin = (*first).begin();
              u_end = (*first).end();
          }
      }

    degree_range_iterator(const degree_range_iterator& b)
        : the_range_(b.the_range_), first(b.first), last(b.last), u_begin(b.u_begin), u_end(b.u_end) {}

    degree_range_iterator& operator++() {
      ++first; 
      return *this;
    }

    // auto operator*() { return std::tuple<vertex_index_t, vertex_index_t&&>(last - first, std::get<0>(*u_begin)); }

    auto operator*() { return std::tuple<vertex_index_t, size_t>(first - the_range_.the_graph_.begin(), (*first).size());}

    auto operator==(const degree_range_iterator& b) const { return first == b.first; }
    bool operator!=(const degree_range_iterator& b) const { return first != b.first; }
    };

  typedef degree_range_iterator iterator;

  auto begin() { return degree_range_iterator(*this); }
  auto end()   { return degree_range_iterator(*this, the_graph_.size()); }

private:
  Graph& the_graph_;
};


#endif    // __PLAIN_RANGE_HPP
