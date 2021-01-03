#include "graph/directed_adjacency_vector.hpp"
#include <catch2/catch.hpp>

struct rl_weight_value {
  float weight = 0;

  rl_weight_value()                       = default;
  rl_weight_value(const rl_weight_value&) = default;
  rl_weight_value& operator=(const rl_weight_value&) = default;
  rl_weight_value(const float& w) : weight(w) {}
};

using rl_graph = std::directed_adjacency_vector<rl_weight_value, rl_weight_value>;

TEST_CASE("rl definition", "[rl][definition]") {
  //using G = rl_graph;
  //static_assert(std::is_same_v<std::edge_key_t<G>,
  //                             std::ordered_pair<std::vertex_key_t<G>, std::vertex_key_t<G>>>);
}
