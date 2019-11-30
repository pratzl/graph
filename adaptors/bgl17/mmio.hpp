//
// This file is part of BGL17 (aka NWGraph aka GraphPack aka the Graph Standard Library)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#ifndef __MMIO_HPP
#define __MMIO_HPP

#include <iostream>
#include <sstream>

namespace bgl17 {
#include "edge_list.hpp"
#include "graph_base.hpp"

inline void
mm_fill(std::istream& inputStream, edge_list<directed>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {
  A.reserve((file_symmetry ? 2 : 1) * nNonzeros);
  A.open_for_push_back();
  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;

    std::getline(inputStream, buffer);
    std::stringstream(buffer) >> d0 >> d1;

    A.push_back(d0, d1);

    if (file_symmetry && (d0 != d1)) {
      A.push_back(d1, d0);
    }
  }
  A.close_for_push_back();
}

template <typename T>
void mm_fill(std::istream& inputStream, edge_list<directed, T>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {

  A.reserve((file_symmetry ? 2 : 1) * nNonzeros);
  A.open_for_push_back();
  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;
    T           v(1.0);

    std::getline(inputStream, buffer);
    if (pattern) {
      std::stringstream(buffer) >> d0 >> d1;
    } else {
      std::stringstream(buffer) >> d0 >> d1 >> v;
    }

    A.push_back(d0, d1, v);

    if (file_symmetry && (d0 != d1)) {
      A.push_back(d1, d0, v);
    }
  }
  A.close_for_push_back();
}

inline void
mm_fill(std::istream& inputStream, edge_list<undirected>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {
  assert(file_symmetry);
  A.reserve(nNonzeros);

  A.open_for_push_back();

  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;

    std::getline(inputStream, buffer);
    std::stringstream(buffer) >> d0 >> d1;
    A.push_back(d0, d1);
  }

  A.close_for_push_back();
}

template <typename T>
void mm_fill(
      std::istream& inputStream, edge_list<undirected, T>& A, size_t nNonzeros, bool file_symmetry, bool pattern) {
  assert(file_symmetry);
  A.reserve(nNonzeros);
  A.open_for_push_back();
  for (size_t i = 0; i < nNonzeros; ++i) {
    std::string buffer;
    size_t      d0, d1;
    T           v(1.0);
    std::getline(inputStream, buffer);
    if (pattern) {
      std::stringstream(buffer) >> d0 >> d1;
    } else {
      std::stringstream(buffer) >> d0 >> d1 >> v;
    }
    A.push_back(d0, d1, v);
  }
  A.close_for_push_back();
}

template <directedness sym, typename... Attributes>
edge_list<sym, Attributes...> read_mm(std::istream& inputStream) {
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
  } else if (header[4] == "general") {
    file_symmetry = false;
  } else {
    std::cerr << "Bad format (symmetry): " << header[4] << std::endl;
    throw;
  }

  while (std::getline(inputStream, string_input)) {
    if (string_input[0] != '%')
      break;
  }
  size_t n0, n1, nNonzeros;
  std::stringstream(string_input) >> n0 >> n1 >> nNonzeros;

  assert(n0 == n1);

  edge_list<sym, Attributes...> A(n0);
  mm_fill(inputStream, A, nNonzeros, file_symmetry, (header[3] == "pattern"));

  return A;
}

template <directedness sym, typename... Attributes>
edge_list<sym, Attributes...> read_mm(const std::string& filename) {
  std::ifstream inputFile(filename);

  edge_list<sym, Attributes...> A = read_mm<sym, Attributes...>(inputFile);

  return A;
}

template <size_t w_idx, directedness sym, typename... Attributes>
void aos_stream(std::ofstream&                outputStream,
                edge_list<sym, Attributes...> A,
                const std::string&            file_symmetry,
                std::string&                  w_type) {
  outputStream << "%%MatrixMarket matrix coordinate " << w_type << " " << file_symmetry << "\n%%\n";

  outputStream << A.size() << " " << A.size() << " ";
  if (file_symmetry == "general" && sym == directedness::undirected)
    outputStream << 2 * (A.end() - A.begin()) << std::endl;
  else
    outputStream << A.end() - A.begin() << std::endl;

  for (auto& element : A.storage_) {
    outputStream << std::get<0>(element) + 1 << " " << std::get<1>(element) + 1 << " ";
    if (w_idx != 0) {
      outputStream << std::get<w_idx>(element);
    }
    outputStream << std::endl;
    if (file_symmetry == "general" && sym == directedness::undirected) {
      outputStream << std::get<1>(element) + 1 << " " << std::get<0>(element) + 1 << " ";
      if (w_idx != 0)
        outputStream << std::get<w_idx>(element);

      outputStream << std::endl;
    }
  }
}

template <size_t w_idx = 0, typename idxtype = void, directedness sym, typename... Attributes>
void write_mm(const std::string&            filename,
              edge_list<sym, Attributes...> A,
              const std::string&            file_symmetry = "general") {
  if (file_symmetry == "symmetric" && sym == directedness::directed) {
    std::cerr << "cannot save directed matrix as symmetric matrix market" << std::endl;
  }

  std::string w_type = "pattern";
  if (std::numeric_limits<idxtype>::is_integer)
    w_type = "integer";
  else if (std::is_floating_point<idxtype>::value)
    w_type = "real";

  std::ofstream outputStream(filename);
  aos_stream<w_idx>(outputStream, A, file_symmetry, w_type);
}

template <size_t w_idx, int idx, typename... Attributes>
void adjacency_stream(std::ofstream&                outputStream,
                      adjacency<idx, Attributes...> A,
                      const std::string&            file_symmetry,
                      std::string&                  w_type) {
  outputStream << "%%MatrixMarket matrix coordinate " << w_type << " " << file_symmetry << "\n%%\n";

  outputStream << A.size() << " " << A.size() << " "
               << std::accumulate(A.begin(), A.end(), 0, [&](int a, auto b) { return a + (int)(b.end() - b.begin()); })
               << std::endl;

  for (auto first = A.begin(); first != A.end(); ++first) {
    for (auto v = (*first).begin(); v != (*first).end(); ++v) {
      outputStream << first - A.begin() + (1 - idx) << " " << std::get<0>(*v) + (1 - idx);
      if (w_idx != 0)
        outputStream << " " << std::get<w_idx>(*v);
      outputStream << std::endl;
    }
  }
}

template <size_t w_idx = 0, typename idxtype = void, int idx, typename... Attributes>
void write_mm(const std::string&            filename,
              adjacency<idx, Attributes...> A,
              const std::string&            file_symmetry = "general") {
  /*if (file_symmetry == "symmetric" && sym == directedness::directed) {
    std::cerr << "cannot save directed matrix as symmetric matrix market" << std::endl;
  }*/

  std::string w_type = "pattern";
  if (std::numeric_limits<idxtype>::is_integer)
    w_type = "integer";
  else if (std::is_floating_point<idxtype>::value)
    w_type = "real";

  std::ofstream outputStream(filename);
  adjacency_stream<w_idx>(outputStream, A, file_symmetry, w_type);
}

} // namespace bgl17

#endif    // __MMIO_HPP
