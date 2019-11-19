#ifndef SGL_ABSTRACT_TEST
#define SGL_ABSTRACT_TEST

#include "test_header.hpp"
#include "compressed.hpp"
#include "mmio.hpp"

typedef compressed_sparse<0, directed> directed_csr_graph_t;
class test_util {
public:

  // Constructs a directed graph with the mmio file
  // pointed by the file path DATA_FILE
  directed_csr_graph_t generate_directed_graph() {
    auto aos_a = read_mm<directed>(DATA_FILE);
    compressed_sparse<0, directed> A(aos_a);
    return A;
  }

  // Constructs directed graph with the given
  // mmio file path
  directed_csr_graph_t generate_directed_graph(const std::string mmiofile) {
    auto aos_a = read_mm<directed>(mmiofile);
    compressed_sparse<0, directed> A(aos_a);
    return A;
  }

  // Constructs a directed graph with the mmio file
  // pointed by the file path DATA_FILE
  auto generate_directed_aos() {
    return read_mm<directed>(DATA_FILE);
  }

  // Constructs directed graph with the given
  // mmio file path
  auto generate_directed_aos(const std::string mmiofile) {
    return read_mm<directed>(mmiofile);
  }
};
#endif
