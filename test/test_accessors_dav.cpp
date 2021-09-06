#include "graph/directed_adjacency_vector.hpp"
#include "graph/graph_utility.hpp"
#include "using_graph.hpp"
//#include "data_routes.hpp"
#include <iostream>
#include <tuple>
#include <catch2/catch.hpp>
//#include <range/v3/action/sort.hpp>

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_TEST

//using namespace std::graph;
//using namespace std::ranges;
using std::vector;
using std::string;
using std::string_view;
using std::numeric_limits;
using std::pair;
using std::tuple;
using std::get;
using std::cout;
using std::endl;
using std::is_same;


using namespace std::graph;

struct dbl_value {
  double value = 0.0;

  constexpr dbl_value()       = default;
  dbl_value(const dbl_value&) = default;
  dbl_value& operator=(const dbl_value&) = default;
  dbl_value(const double& w) : value(w) {}
};


using Graph = std::graph::containers::directed_adjacency_vector<dbl_value, dbl_value, dbl_value, uint32_t>;

// do both parameters have non-const, or both const, values?
template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

TEMPLATE_TEST_CASE("dav accessors", "[dav][accessors]", (Graph), (const Graph)) {
  static_assert(std::is_same_v<TestType, Graph> || std::is_same_v<TestType, const Graph>);
  using G = TestType;

  vector<double> the_vtx_vals = {10.0, 11.0, 12.0};

  using edge_data_type                = tuple<size_t, size_t, double>;
  using edge_key_type                 = edge_key_t<G, vertex_edge_iterator_t<G>>;
  vector<edge_data_type> the_edg_vals = {{0, 1, 1.1}, {0, 2, 2.1}, {1, 2, 2.2}, {2, 0, 0.1}}; // {ukey, vkey, val}

  auto ekey_fnc = [](const edge_data_type& data) -> edge_key_type { return edge_key_type(get<0>(data), get<1>(data)); };
  auto eval_fnc = [](const edge_data_type& data) -> dbl_value { return dbl_value(get<2>(data)); };
  auto vval_fnc = [](const double& vtxval) -> dbl_value { return dbl_value(vtxval); };

  G g(the_edg_vals, the_vtx_vals, ekey_fnc, eval_fnc, vval_fnc);

  //
  // vertex range
  //
  SECTION("vertices(g)") {
    //static_assert(_vertices_::_gph_has_ADL<G>);
    auto& vv = vertices(g);
    REQUIRE(is_same_const(g, vv));
    REQUIRE(std::ranges::random_access_range<decltype(vv)>);
    REQUIRE(size(vertices(g)) == 3);
    auto u = begin(vv);
    //REQUIRE(u->size() == 2);
    REQUIRE(size(edges(g, u)) == 2);
  }

  //
  // value types
  //

  // graph values
  SECTION("graph_value(g)") {
    //REQUIRE(graph_value(g) == 7);
  }

  // vertex values
  SECTION("vertex_key(g,u)") {
    REQUIRE(vertex_key(g, ++begin(vertices(g))) == 1); // no fmt
    //REQUIRE(vertex_value(g, begin(vertices(g))) == 8); // no fmt
  }
}
