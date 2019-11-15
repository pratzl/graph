//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Jesun Sahariar Firoz
//

/*Implements DAG-adapter based MIS algorithm */
#pragma once

#include "plain_range.hpp"
#include "dag_range.hpp"

// #define PRINT_DEBUG 1

template<typename Graph>
void dag_based_mis(Graph A, std::vector<bool>& mis) {
  size_t N = A.size();
#ifdef PRINT_DEBUG
  std::cout << "size: " << N << std::endl; 
#endif

  std::vector<size_t> degrees(N, 0);
  // std::vector<size_t> colors(N, std::numeric_limits<std::uint32_t>::max());
  
  std::vector<std::vector<size_t>> predecessor_list(N);
  std::vector<std::vector<size_t>> successor_list(N);
  std::vector<size_t> degree_list(N, 0);
  /*Need a "Degree range"*/
  for (auto&& [v, deg] : plain_degree_range(A)) {
#ifdef PRINT_DEBUG
    std::cout << "v: " << v + 1 << " deg: " << deg << std::endl;
#endif    
    degrees[v] = deg;
  }
  
   /*plain edge range will be used to calculate predecessor/successors*/
   /*plain_edge_range returns a tuple every time iterated through*/

  for (auto&& [v, u] : plain_edge_range(A)) {
    if (degrees[v] > degrees[u]) {
      successor_list[v].push_back(u);
      predecessor_list[u].push_back(v);
    } else if (degrees[v] < degrees[u]) {
      predecessor_list[v].push_back(u);
      successor_list[u].push_back(v);
    } else {
      if (v > u) {
	predecessor_list[v].push_back(u);
	successor_list[u].push_back(v);
      } else {
	successor_list[v].push_back(u);
	predecessor_list[u].push_back(v);
      }
    }
  }

  for (uint64_t vtx = 0; vtx < N; vtx++) {
    /*Set the color of the roots to be zero.*/
    if (predecessor_list[vtx].size() == 0) {
      mis[vtx] = true;
#ifdef PRINT_DEBUG
      std::cout << vtx + 1 << " is in mis: " << mis[vtx] << std::endl;
#endif
    }
  }

  for (auto&& [v, u, ready_to_process]: 
	 dag_range(A, predecessor_list, successor_list)) {
#ifdef PRINT_DEBUG
    std::cout << "V: " << v + 1 << " U:" << u + 1 << std::endl;
#endif
    if (mis[v] == true) mis[u] = false;
   
  }

  return; 
}
