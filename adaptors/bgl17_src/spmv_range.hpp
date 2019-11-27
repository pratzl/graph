//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#pragma once

namespace bgl17 {

template <typename Graph>
class spmv_range {
public:
  spmv_range(Graph& g) : the_graph_(g) {}
  spmv_range(const spmv_range& b) : the_graph_(b.the_graph_) {}
  auto& operator=(const spmv_range& b) {
    the_graph_ = b.the_graph_;
    return *this;
  }

  class v_range_iterator {
  private:
    spmv_range<Graph>&             the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    v_range_iterator(spmv_range<Graph>& range, size_t offset = 0)
          : the_range_{range}, first{the_range_.the_graph_.begin() + offset}, last{the_range_.the_graph_.end()} {
      if (first != last) {
        u_begin = (*first).begin();
        u_end   = (*first).end();
      }
    }

    v_range_iterator(const v_range_iterator& b)
          : the_range_(b.the_range_), first(b.first), last(b.last), u_begin(b.u_begin), u_end(b.u_end) {}

    v_range_iterator& operator++() {
      ++u_begin;
      if (u_begin == u_end) {
        if (++first != last) {
          u_begin =
                (*first)
                      .begin(); // FIXME:  This was commented out -- why?  Might not need it for CSR -- but in general, we do
          u_end = (*first).end();
        }
      }

      return *this;
    }

    // auto operator*() { return std::tuple<vertex_index_t, vertex_index_t&&>(last - first, std::get<0>(*u_begin)); }

    auto operator*() {
      return std::tuple<typename std::iterator_traits<typename Graph::outer_iterator>::difference_type, vertex_index_t,
                        double&>(first - the_range_.the_graph_.begin(), std::get<0>(*u_begin), std::get<1>(*u_begin));
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

} // namespace bgl17
