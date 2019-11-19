//
// This file is part of the Graph Standard Library
// (c) Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <queue>

#include "aos.hpp"
#include "compressed.hpp"
#include "mmio.hpp"
#include "util.hpp"

typedef compressed_sparse<0> csr_graph;

typedef size_t vertex_index_t;

template<typename Graph>
class edge_ranges {
public:
  edge_ranges(Graph& g) : the_graph_(g) {}

  class edge_range_iterator {
  private:
    edge_ranges<Graph>&            the_range_;
    typename Graph::outer_iterator first, last;
    typename Graph::outer_iterator G;
    typename Graph::inner_iterator u_begin, u_end;

  public:
    edge_range_iterator(edge_ranges<Graph>& range)
        : the_range_(range), first(the_range_.the_graph_.begin()), last(the_range_.the_graph_.end()), G(first),
          u_begin((*first).begin()), u_end((*first).end()) {}

    edge_range_iterator& operator++() {
      ++u_begin;
      while (u_begin == u_end) {
        if (++first == last) break;
        u_begin = (*first).begin();
        u_end   = (*first).end();
      }

      return *this;
    }

    auto operator*() {
      return std::tuple<typename Graph::inner_container_ref, typename Graph::inner_container_ref>(*first, G[*u_begin]);
    }

    class end_sentinel_type {
    public:
      end_sentinel_type() {}
    };

    auto operator==(const end_sentinel_type&) const { return first == last; }
    bool operator!=(const end_sentinel_type&) const { return (first != last); }
  };

  typedef edge_range_iterator iterator;

  auto begin() { return edge_range_iterator(*this); }
  auto end() { return typename edge_range_iterator::end_sentinel_type(); }

private:
  Graph& the_graph_;
};

int main(int argc, char* argv[]) {

  size_t num_threads = 0;
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " adj.mmio [ num_threads ]" << std::endl;
    return -1;
  }

  // sparse matvec
  // template<typename RandomAccessIteratorMat, typename RandomAccessIteratorVec>
  // void matvec(RandomAccessIteratorMat first, RandomAccessIteratorMat last,
  // 	    RandomAccessIteratorVec left_hand_side, RandomAccessIteratorVec right_hand_side) {

  //   for (/*auto A = first*/; first != last; ++first) {
  //     auto tmp = *left_hand_side;
  //     for (auto v = (*first).begin(); v != (*first).end(); ++v) {
  //       tmp += std::get<1>(*v) * right_hand_side[std::get<0>(*v)];
  //     }
  //     *left_hand_side = tmp;
  //     ++left_hand_side;
  //   }
  // }

  if (argc >= 3) num_threads = std::stol(argv[2]);

  sparse_aos<undirected> aos_a = read_mm<undirected>(argv[1]);
  aos_a.triangularize<predecessor>();
  aos_a.sort_by<1>();
  aos_a.stable_sort_by<0>();

  csr_graph A(aos_a);

  size_t                   triangles = 0;
  counting_output_iterator counter(triangles);

  for (auto y : edge_ranges(A)) {
    auto u = std::get<0>(y);
    auto v = std::get<1>(y);
    std::set_intersection(u.begin(), u.end(), v.begin(), v.end(), counter);
  }
  std::cout << triangles << " triangles" << std::endl;

  return 0;
}
