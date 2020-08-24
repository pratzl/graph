#include "graph/directed_adjacency_array.hpp"
#include <catch2/catch.hpp>

struct rl_weight_value {
  float weight = 0;

  rl_weight_value()                       = default;
  rl_weight_value(rl_weight_value const&) = default;
  rl_weight_value& operator=(rl_weight_value const&) = default;
  rl_weight_value(float const& w) : weight(w) {}
};

using rl_graph = std::graph::directed_adjacency_array<rl_weight_value, rl_weight_value>;

TEST_CASE("rl definition", "[rl][definition]") {
  //using G = rl_graph;
}
