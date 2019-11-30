//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __DIMACS_IO_HPP
#define __DIMACS_IO_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

template<typename Graph>
Graph read_dimacs(std::string path) {
  std::string  buffer;
  std::fstream dimacs_file(path);
  std::getline(dimacs_file, buffer);
  size_t num_vertices, num_edges;
  std::stringstream(buffer) >> num_vertices >> num_edges;

  Graph A(num_vertices, num_vertices);

  size_t vertex = 0;
  size_t edge   = 0;
  A.open_for_push_back();
  while (getline(dimacs_file, buffer)) {
    std::stringstream str(buffer);
    while (str) {
      size_t neighbor = 0;
      str >> neighbor;
      if (neighbor > vertex) {
        A.push_back(vertex, neighbor - 1);
        ++edge;
      }
    }
    ++vertex;
  }
  A.close_for_push_back();

  size_t check_edges = (edge - num_vertices) / 2;
  if ((vertex != num_vertices) || (edge != num_edges)) {
    std::cerr << vertex << " " << num_vertices << std::endl;
    std::cerr << edge << " " << num_edges << std::endl;
    throw;
  }

  return A;
}

#endif    // __DIMAVS_IO_HPP
