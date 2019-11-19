//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine (c) 2017
//

#ifndef __MMIO_HPP
#define __MMIO_HPP

#include <iostream>
#include <sstream>

#include "aos.hpp"
#include "graph_base.hpp"

template<directedness sym>
void mm_fill(std::istream& inputStream, sparse_aos<sym>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {}

template<>
void mm_fill<directed>(std::istream& inputStream, sparse_aos<directed>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {
  A.reserve((file_symmetry ? 2 : 1) * nNonzeros);
  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;
    std::getline(inputStream, buffer);
    if (pattern) {
      std::stringstream(buffer) >> d0 >> d1;
    } else {
      double v;
      std::stringstream(buffer) >> d0 >> d1 >> v;
    }
    A.push_back(d0 - 1, d1 - 1);    // Correct for one-based
    if (file_symmetry && (d0 != d1)) {
      A.push_back(d1 - 1, d0 - 1);    // Correct for one-based
    }
  }
}

template<>
void mm_fill<undirected>(std::istream& inputStream, sparse_aos<undirected>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {
  assert(file_symmetry);
  A.reserve(nNonzeros);
  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;
    std::getline(inputStream, buffer);
    if (pattern) {
      std::stringstream(buffer) >> d0 >> d1;
    } else {
      double v;
      std::stringstream(buffer) >> d0 >> d1 >> v;
    }
    A.push_back(d0 - 1, d1 - 1);    // Correct for one-based
  }
}

template<directedness sym>
sparse_aos<sym> read_mm(std::istream& inputStream) {
  std::string              string_input;
  bool                     file_symmetry = false;
  std::vector<std::string> header(5);

  // %%MatrixMarket matrix coordinate integer symmetric
  std::getline(inputStream, string_input);
  std::stringstream h(string_input);
  for (auto& s : header)
    h >> s;

  if (header[0] != "%%MatrixMarket") {
    std::cerr << "Unsupported format" << std::endl;
    throw;
  }
  if (header[4] == "symmetric") {
    file_symmetry = true;
  } else if (header[4] == "unsymmetric") {
    file_symmetry = false;
  } else {
    std::cerr << "Bad format (symmetry): " << header[4] << std::endl;
    throw;
  }

  while (std::getline(inputStream, string_input)) {
    if (string_input[0] != '%') break;
  }
  size_t n0, n1, nNonzeros;
  std::stringstream(string_input) >> n0 >> n1 >> nNonzeros;

  sparse_aos<sym> A(n0, n1);
  mm_fill<sym>(inputStream, A, nNonzeros, file_symmetry, (header[3] == "pattern"));

  return A;
}

template<directedness sym>
sparse_aos<sym> read_mm(const std::string& filename) {
  std::ifstream inputFile;
  inputFile.open(filename);
  sparse_aos<sym> A = read_mm<sym>(inputFile);
  inputFile.close();

  return A;
}
#endif    // __MMIO_HPP
