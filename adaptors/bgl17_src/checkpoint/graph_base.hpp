//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#include <cstddef>

#ifndef __GRAPH_BASE_HPP
#define __GRAPH_BASE_HPP

#include <cstddef>

enum directedness { undirected, directed };

class graph_base {
public:
  graph_base(size_t d0, size_t d1) : lim{d0, d1}, is_open(false) {}

protected:
  size_t lim[2];     // ordinal limits
  bool   is_open;    // can we mutate graph
};

template<directedness sym>
class sparse_aos;

template<int idx, directedness sym, typename... Attributes>
class compressed_sparse;

template<int idx, directedness sym>
class vov_sparse;

template<int idx, directedness sym>
class adj_sparse;

enum succession { successor, predecessor };

#endif    // __GRAPH_BASE_HPP
