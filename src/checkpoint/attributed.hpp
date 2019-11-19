//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __ATTRIBUTED_HPP
#define __ATTRIBUTED_HPP

#include "graph_base.hpp"
#include <functional>
#include <iterator>
#include <tuple>

// using csr1 = attributed_sparse<compressed_sparse, double>;
// using csr2 = attributed_sparse<compressed_sparse, double, double>;
// using csr3 = attributed_sparse<attributed_sparse, double>;

template<typename Graph, typename ValueType>
class attributed_sparse : public Graph {

public:
#if 0
  class inner_iterator {
  public:
    typedef size_t              difference_type;
    typedef std::tuple<size_t, double> value_type;

    inner_iterator(std::vector<size_t>::iterator _indices, std::vector<double>::iterator _values, size_t _init)
      : indices(_indices), values(_values), position(_init) {}


    value_type operator*() {
      return std::tuple<size_t, double> (indices[position], values[position]);
    }
    value_type operator[](size_t index) {
      return std::tuple<size_t, double> (indices[position + index], values[position + index]);
    }

    inner_iterator operator++() {
      ++position;
      return *this;
    }
    inner_iterator operator+=(size_t offset) {
      position += offset;
      return *this;
    }

    bool operator==(const inner_iterator& b) { return position == b.position; }
    bool operator!=(const inner_iterator& b) { return position != b.position; }
    bool operator<(const inner_iterator& b) { return position < b.position; }

    difference_type operator-(const inner_iterator& b) const { return position - b.position; }
  private:
    std::vector<size_t>::iterator indices;
    std::vector<double>::iterator values;
    size_t               position;

  };

  class inner_container_ref {
  public:
    typedef inner_iterator iterator;

    inner_container_ref(std::vector<size_t>::iterator _index_begin, std::vector<size_t>::iterator _index_end, 
			std::vector<double>::iterator _value_begin, std::vector<double>::iterator _value_end) 
      : inner_index_begin(_index_begin), inner_index_end(_index_end), inner_value_begin(_value_begin), inner_value_end(_value_end) {}
    inner_iterator begin() { return inner_iterator(inner_index_begin, inner_value_begin, 0); }
    inner_iterator end()   { return inner_iterator(inner_index_end, inner_value_end, inner_index_end-inner_index_begin);   }

  private:
    std::vector<size_t>::iterator inner_index_begin, inner_index_end;
    std::vector<double>::iterator inner_value_begin, inner_value_end;
  };

#endif
  class outer_iterator : Graph::outer_iterator {
  public:
    typedef size_t difference_type;
    //    typedef inner_container_ref value_type;

    outer_iterator(typename Graph::outer_iterator go, std::vector<double>& _values) : Graph::outer_iterator(go), values(_values) {}

    outer_iterator operator+(size_t offset) const { return outer_iterator(Graph::operator+(offset)); }
    outer_iterator operator-(size_t offset) const { return outer_iterator(Graph::operator+(offset)); }

#if 0
    inner_container_ref operator*() {
      return inner_container_ref(indices.begin() + ptrs[position], indices.begin() + ptrs[position + 1],
				 values.begin() + ptrs[position], values.begin() + ptrs[position + 1]);
    }
    inner_container_ref operator[](size_t index) {
      return inner_container_ref(indices.begin() + ptrs[position + index], indices.begin() + ptrs[position + index + 1],
				 values.begin() + ptrs[position + index], values.begin() + ptrs[position + index + 1]);
    }
#endif

  private:
    std::vector<double>& values;
  };

  outer_iterator begin() { return outer_iterator(Graph::begin(), values); }    // construct with Graph::begin and values
  outer_iterator end() { return outer_iterator(Graph::end(), values); }

public:
  attributed_sparse(size_t d0, size_t d1) : Graph(d0, d1), values(Graph::lim[0]) {}

  void push_back(size_t i, size_t j, ValueType val) {
    assert(Graph::is_open);

    values[i].push_back(val);
    Graph::push_back(i, j);
  }

private:
  std::vector<ValueType> values;
};

#endif    // __ATTRIBUTED_HPP
