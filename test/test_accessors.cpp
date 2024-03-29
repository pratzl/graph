#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <catch2/catch.hpp>
#include "graph/graph_utility.hpp"
#include "using_graph.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::list;
using std::forward_list;

using vol_key         = size_t;
using vol_edge_val    = double;
using vol_edge_type   = pair<vol_key, vol_edge_val>;
using vol_graph       = vector<list<vol_edge_type>>;
using vol_vertex_type = vol_graph::value_type;

template <typename T>
constexpr bool is_constant(const T&) {
  return true;
}
template <typename T>
constexpr bool is_constant(T&) {
  return false;
}

// create functions in std for ADL, to match location of vector
namespace std {
auto graph_value(vol_graph& g) { return 7; }                                  // a bogus value only for validation
auto vertex_value(vol_graph& g, vertex_iterator_t<vol_graph> u) { return 8; } // a bogus value only for validation
//auto edge_value(vol_graph&, list<vol_edge_type>::iterator uv) { return uv->second; }
} // namespace std

TEST_CASE("vol accessors", "[vol][accessors]") {
  using std::graph::vertices;
  using std::graph::edges;

  using std::graph::graph_value;
  using std::graph::vertex_key;
  using std::graph::vertex_value;

  // non-const
  {
    using G = vol_graph;
    G g{{{1, 1.1}, {2, 2.1}}, {{2, 2.2}}, {{0, 0.1}}};
    static_assert(std::graph::_graph_value_::_gph_has_ADL<G>);
    REQUIRE(graph_value(g) == 7);

    auto& vv = vertices(g);
    REQUIRE(!is_constant(vv));
    REQUIRE(size(vertices(g)) == 3);

    auto u = begin(vertices(g));
    REQUIRE(u == g.begin());
    REQUIRE(u->size() == 2);
    //REQUIRE(size(edges(g, u)) == 2);
    REQUIRE(vertex_key(g, u) == 0);
    //static_assert(std::_vertex_value_::_vtx_has_ADL<G, decltype(u)>);
    //REQUIRE(vertex_value(g, u) == 8);
  }

  // const
  {
    using G = const vol_graph;
    G     g;
    auto& vv = vertices(g);
    REQUIRE(is_constant(vv));
  }
}

using IntVec = std::vector<int>;

auto begin(IntVec& v) { return v.begin(); }

struct tbegin {
  IntVec iv    = {1, 2, 3};
  int    value = 0;
};
auto begin(tbegin& v) { return v.iv.begin(); }
auto graph_value(tbegin& v) {
  return v.value; // no fmt
}

TEST_CASE("cpo accessor", "[cpo][trace]") {
  IntVec iv{1, 2, 3};
  auto   it = std::ranges::begin(iv);

  tbegin tb;
  it         = std::ranges::begin(tb);
  auto value = std::graph::graph_value(tb);
  static_assert(std::graph::_graph_value_::_gph_has_ADL<tbegin>);

  int x = 0;
}
