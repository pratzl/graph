
#ifndef __COMPRESSED_HPP
#define __COMPRESSED_HPP

#include "aos.hpp"
#include "edge_list.hpp"
#include "graph_base.hpp"
#include "util.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#if defined(NEED_EXCLUSIVE_SCAN)
#  include "detail/numeric.hpp"
#endif
#include <tuple>
#include <vector>

namespace bgl17 {

template <directedness edge_directedness, typename... Attributes>
class edge_list;

template <typename... Attributes>
class indexed_struct_of_arrays {

public:
  indexed_struct_of_arrays(size_t N) : is_open(false), N_(N), indices_(N + 1) {}

public:
  class sub_view {
  public:
    sub_view(typename struct_of_arrays<Attributes...>::iterator _begin,
             typename struct_of_arrays<Attributes...>::iterator _end)
          : v_begin(_begin), v_end(_end) {}

    typedef typename struct_of_arrays<Attributes...>::iterator iterator;

    auto   begin() { return v_begin; }
    auto   end() { return v_end; }
    size_t size() { return v_end - v_begin; }

  private:
    typename struct_of_arrays<Attributes...>::iterator v_begin;
    typename struct_of_arrays<Attributes...>::iterator v_end;
  };

  typedef typename struct_of_arrays<Attributes...>::iterator inner_iterator;

public:
  class outer_iterator {
  public:
    typedef size_t   difference_type;
    typedef sub_view value_type;

  public:
    outer_iterator(std::vector<size_t>& _indices, struct_of_arrays<Attributes...>& _indexed, size_t _index)
          : index(_index), indices_(_indices), indexed_(_indexed) {}

    auto& operator=(const outer_iterator& b) {
      index    = b.index;
      indices_ = b.indices_;
      indexed_ = b.indexed_;
      return *this;
    }

    auto& operator++() {
      ++index;
      return *this;
    }
    auto& operator+=(size_t offset) {
      index += offset;
      return *this;
    }

    auto operator+(size_t offset) const { return outer_iterator(indices_, indexed_, index + offset); }
    auto operator-(size_t offset) const { return outer_iterator(indices_, indexed_, index - offset); }

    auto operator-(const outer_iterator& b) const { return index - b.index; }

    bool operator==(const outer_iterator& b) const { return index == b.index; }
    bool operator!=(const outer_iterator& b) const { return index != b.index; }
    bool operator<(const outer_iterator& b) const { return index < b.index; }

    auto operator*() { return sub_view(indexed_.begin() + indices_[index], indexed_.begin() + indices_[index + 1]); }

    template <typename... Attrs>
    auto& operator[](const std::tuple<size_t, Attrs...>& i) {
      return operator[](std::get<0>(i));
    }

    template <typename... Attrs>
    auto& operator[](const std::tuple<size_t, Attrs...>& i) const {
      return operator[](std::get<0>(i));
    }

    auto operator[](size_t i) {
      return sub_view(indexed_.begin() + indices_[index + i], indexed_.begin() + indices_[index + i + 1]);
    }
    auto& operator[](size_t i) const {
      return sub_view(indexed_.begin() + indices_[index + i], indexed_.begin() + indices_[index + i + 1]);
    }

    typedef std::random_access_iterator_tag iterator_category;
    //    typedef size_t                                     difference_type;
    //    typedef std::tuple<vertex_index_t, vertex_index_t> value_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    auto get_index() { return index; }

  private:
    size_t                           index;
    std::vector<size_t>&             indices_;
    struct_of_arrays<Attributes...>& indexed_;
  };

  typedef outer_iterator iterator;

  auto begin() { return outer_iterator(indices_, to_be_indexed_, 0); }
  auto end() { return outer_iterator(indices_, to_be_indexed_, N_); }
  auto begin() const { return outer_iterator(indices_, to_be_indexed_, 0); }
  auto end() const { return outer_iterator(indices_, to_be_indexed_, N_); }

  size_t size() const { return indices_.size() - 1; }

public:
  void open_for_push_back() {
    assert(to_be_indexed_.size() == 0);
    //If we decide to allow reopen for pushback, this will undo exclusive_scan
    /*if(to_be_indexed_.size() != 0) {
      std::adjacent_difference(indices_.begin()+1, indices_.end(), indices_.begin());
      indices_[N_]=0;
      }*/

    is_open = true;
  }

  void close_for_push_back() {
    if (to_be_indexed_.size() == 0)
      return;

    indices_[N_] = indices_[N_ - 1];
    std::exclusive_scan(indices_.begin(), indices_.end() - 1, indices_.begin(), static_cast<size_t>(0));
    indices_[N_] += indices_[N_ - 1];
    assert(indices_[N_] == to_be_indexed_.size());

    is_open = false;
  }

  void push_back(size_t i, const Attributes&... attrs) {
    ++indices_[i];
    to_be_indexed_.push_back(attrs...);
  }

  void push_at(size_t i, const Attributes&... attrs) {
    size_t j          = indices_[i]++;
    to_be_indexed_[j] = std::tuple<Attributes...>(attrs...);
  }

  void stream(const std::string& msg = "") {
    auto first = begin();
    auto last  = end();

    std::cout << msg;
    for (auto G = first; first != last; ++first) {
      for (auto v = (*first).begin(); v != (*first).end(); ++v) {
        std::cout << "( " << first - G << ", " << std::get<0>(*v) << " )" << std::endl;
      }
    }
  }

private:
  bool   is_open;
  size_t N_;

  std::vector<size_t>             indices_;
  struct_of_arrays<Attributes...> to_be_indexed_;
};


/**
 * @brief 
 * 
 * @todo compressed_sparse should be deprecated -- there is no such thing as directedness
 * 
 * @file compressed.hpp
 * @author Andrew Lumsdaine
 * @date 2018-08-22
 */

template <int idx, directedness sym = undirected, typename... Attributes>
class compressed_sparse : public indexed_struct_of_arrays<size_t, Attributes...> {
public:
  compressed_sparse(size_t N) : indexed_struct_of_arrays<size_t, Attributes...>(N) {}
  compressed_sparse(edge_list<sym, Attributes...>& A) : indexed_struct_of_arrays<size_t, Attributes...>(A.size()) {
    A.fill(*this);
  }

  //  size_t size() const { return indexed_struct_of_arrays<size_t, Attributes...>::size(); }
};

template <int idx, typename... Attributes>
class adjacency : public indexed_struct_of_arrays<size_t, Attributes...> {
public:
  adjacency(size_t N) : indexed_struct_of_arrays<size_t, Attributes...>(N) {}
  adjacency(edge_list<directed, Attributes...>& A) : indexed_struct_of_arrays<size_t, Attributes...>(A.size()) {
    A.fill(*this);
  }
  adjacency(edge_list<undirected, Attributes...>& A) : indexed_struct_of_arrays<size_t, Attributes...>(A.size()) {
    A.fill(*this);
  }

  //  size_t size() const { return indexed_struct_of_arrays<size_t, Attributes...>::size(); }

#if 0
  auto transpose() {
    if ((idx != 0) || (idx != 1)) {
      throw;
    }
    size_t t_idx = (idx+1) % 2;
    adjacency<t_idx, Attributes...> A_transpose(N);
    auto A_column_nos = std::get<0>(to_be_indexed_);
    auto A_row_indices = indices_;

    auto A_t_col_indices = A_transpose.indices_;
    
    for (size_t i = 0; size_t < A_columns.size(); ++i) {
      size_t col = A_column_nos[i];
      A_t_col_indices[col]++;
    }
    A_transpose.close_for_push_back();
    A_transpose.resize(to_be_indexed_.size());

    auto A_t_row_nos = std::get<0>(A_transpose.to_be_indexed_);

    for (size_t i = 0; i < N_; ++i) {
      for (size_t j = indices_[i]; j < indices_[i+1]; ++j) {
	size_t A_col = A_column_nos[j];
	size_t A_tra = A_t_col_indices_[A_col]++;
	A_transpose.to_be_indexed_[A_tra] = to_be_indexed_[j];
	A_t_row_nos[A_tra] = i;
      }
    }
    return A_transpose;
  }
#endif
};

template <int idx, succession cessor, typename... Attributes>
class packed : public indexed_struct_of_arrays<size_t, Attributes...> {
public:
  packed(size_t N) : indexed_struct_of_arrays<size_t, Attributes...>(N) {}
  packed(edge_list<undirected, Attributes...>& A) : indexed_struct_of_arrays<size_t, Attributes...>(A.size()) {
    A.fill(*this);
  }

  //  size_t size() const { return indexed_struct_of_arrays<size_t, Attributes...>::size(); }
};

} // namespace bgl17

#endif // __COMPRESSED_HPP
