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

#include <tuple>
#include <vector>

#include <graph/algorithms/degree.hpp>
#include <graph/algorithms/utils.hpp>
#include <compressed.hpp>
#include <edge_list.hpp>
#include <spmv_range.hpp>

using namespace bgl17;

//****************************************************************************
template<typename GraphT, typename RealT = double>
void page_rank_range_for(GraphT& graph, std::vector<RealT>& page_rank, RealT damping_factor = 0.85, RealT threshold = 1.e-5,
                         size_t max_iters = std::numeric_limits<unsigned int>::max()) {
  /// @todo assert page_rank.size() == graph.size();

  RealT const scaled_teleport((1. - damping_factor) / (RealT)graph.size());

  // degree-normalize the rows of the graph and scale by damping factor
  std::vector<RealT> odegree(out_degree(graph));

  edge_list<directed, double> edges(0);
  edges.open_for_push_back();
  for (auto [i, j, v] : spmv_range(graph)) {
    edges.push_back(i, j, v * (damping_factor / odegree[i]));
    //v *= (damping_factor/out_degree[i]);
  }
  edges.close_for_push_back();
  adjacency<0, RealT> M(edges);

  // initialize page rank to 1/N
  std::vector<RealT> old_rank(graph.size(), 1. / (RealT)graph.size());

  for (size_t iter_num = 0; iter_num < max_iters; ++iter_num) {
    zeroize(page_rank);

    // compute new rank (transpose the graph): PR_i = M' * PR_i-1
    for (auto&& [i, j, v] : spmv_range(M)) {
      page_rank[j] += v * old_rank[i];
    }

    // add scaled teleport term: (1 - damping_factor)/N
    for (auto& rank_val : page_rank) {
      rank_val += scaled_teleport;
    }

    // Test for convergence, compute squared error
    RealT squared_error(0.);
    for (size_t idx = 0; idx < page_rank.size(); ++idx) {
      RealT tmp = page_rank[idx] - old_rank[idx];
      squared_error += tmp * tmp;
    }

    //std::cout << "Iteration " << iter_num << ": sq_err = "
    //          << squared_error/((RealT)graph.size()) << std::endl;

    if (squared_error / ((RealT)graph.size()) < threshold) break;

    old_rank.swap(page_rank);
  }
}
