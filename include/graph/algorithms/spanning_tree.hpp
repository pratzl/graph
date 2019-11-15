//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Kevin Deweese
//

#pragma once

#include <tuple>
#include <algorithm>

#include <util.hpp>
#include <disjoint_set.hpp>

//****************************************************************************
template<typename EdgeListT>
EdgeListT kruskal(EdgeListT &E)
{
  return kruskal(E,[](auto t1, auto t2) { return std::get<2>(t1) < std::get<2>(t2) ;});
}
//****************************************************************************
template <typename EdgeListT, typename Compare>
EdgeListT kruskal(EdgeListT &E, Compare comp)
{
  size_t n_vtx = E.size();
  EdgeListT T(n_vtx);
  std::sort(E.begin(), E.end(), comp);
  
  std::vector<std::pair<vertex_index_t,size_t> > subsets(n_vtx);
  for (size_t i=0; i < n_vtx; ++i) {
    subsets[i].first = i;
    subsets[i].second = 0;
  }
  
  for (auto y : E) {
    auto u = std::get<0>(y);
    auto v = std::get<1>(y);
    if(disjoint_union_find(subsets,u,v))
      T.push_back(y);
  }
  
  return T;
}



