#include <vector>
#include <list>
#include <type_traits>

// do both parameters have non-const, or both const, values?
template <typename T1, typename T2>
constexpr bool is_same_const(T1&& t1, T2&& t2) {
  return std::is_const_v<T1> == std::is_const_v<T2>;
}

// Define base classes for a simple graph used for testing.
namespace simple_ns {

struct simple_edge_base {
  using edge_key_type   = std::pair<size_t, size_t>; // src_key,tgt_key
  using edge_value_type = double;

  edge_key_type   the_key   = {};
  edge_value_type the_value = {};

  simple_edge_base(const edge_key_type& k, edge_value_type const& val = edge_value_type{})
        : the_key(k), the_value(val) {}
  simple_edge_base()                        = default;
  simple_edge_base(simple_edge_base const&) = default;
  ~simple_edge_base()                       = default;
  simple_edge_base& operator=(simple_edge_base const&) = default;
};

template <typename E>
struct simple_vertex_base {
  using vertex_key_type   = size_t;
  using vertex_value_type = double;
  using edge_range        = std::list<E>;

  vertex_key_type   the_key   = {}; // this vertex's key (should be same as it's index in the_vertices)
  vertex_value_type the_value = {}; // this vertex's value
  edge_range        the_edges;      // incidence edges

  simple_vertex_base(vertex_key_type key, vertex_value_type val, const edge_range& init)
        : the_key(key), the_value(val), the_edges(init) {}
  simple_vertex_base()                          = default;
  simple_vertex_base(simple_vertex_base const&) = default;
  ~simple_vertex_base()                         = default;
  simple_vertex_base& operator=(simple_vertex_base const&) = default;
};

template <typename V, typename E>
struct simple_graph_base {
  using edge_range       = typename simple_vertex_base<E>::edge_range;
  using vertex_type      = V;
  using vertex_key_type  = typename V::vertex_key_type;
  using vertex_range     = std::vector<V>;
  using graph_value_type = int;

  graph_value_type the_value = {};
  vertex_range     the_vertices;

  simple_graph_base() { init(); };
  simple_graph_base(simple_graph_base const&) = default;
  ~simple_graph_base()                        = default;
  simple_graph_base& operator=(simple_graph_base const&) = default;

  void init() {
    the_value = 7;
    the_vertices.push_back(vertex_type({0, 10.0, edge_range{{{0, 1}, 1.1}, {{0, 2}, 2.1}}}));
    the_vertices.push_back(vertex_type({1, 11.0, edge_range{{{1, 2}, 2.2}}}));
    the_vertices.push_back(vertex_type({2, 12.0, edge_range{{{2, 0}, 0.1}}}));
  }
};
} // namespace simple_ns
