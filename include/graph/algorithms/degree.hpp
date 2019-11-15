//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Scott McMillan
//

#pragma once

#include <spmv_range.hpp>
#include <vector>

//****************************************************************************
template<typename GraphT, typename RealT = double>
std::vector<RealT> in_degree(GraphT& graph)    //const &graph)
{
  std::vector<RealT> in_degree(graph.size());

  for (auto [i, j, v] : spmv_range(graph)) {
    in_degree[j] += v;
  }

  return in_degree;
}

//****************************************************************************
template<typename GraphT, typename RealT = double>
std::vector<RealT> out_degree(GraphT& graph)    //const &graph)
{
  std::vector<RealT> out_degree(graph.size());

  for (auto [i, j, v] : spmv_range(graph)) {
    out_degree[i] += v;
  }

  return out_degree;
}
