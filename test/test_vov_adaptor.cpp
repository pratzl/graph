#include "pch.h"
#include "graph/graph_fwd.hpp"
#include "graph/graph_utility.hpp"
//#include "common/test_header.hpp"

#include "edge_list.hpp"
#include "plain_range.hpp"
#include "edge_range.hpp"
#include "vovos.hpp"
#include "data_routes.hpp"

#include "util/print_types.hpp"

#include <range/v3/view/subrange.hpp>
#include <range/v3/algorithm/find_if.hpp>

#include "graph/range/dfs.hpp"

#define TEST_OPTION_OUTPUT (1)
#define TEST_OPTION_GEN (2)
#define TEST_OPTION_TEST (3)
#define TEST_OPTION TEST_OPTION_OUTPUT

using std::string;
using std::cout;
using std::endl;

namespace adaptor::vov {
using namespace std::graph;

class vov_vector;
class vov_edge;

template <typename... Attributes>
class vov_graph : public std::graph::empty_value {
public:
  using graph_type            = ::vov<Attributes...>;
  using graph_user_value_type = std::graph::empty_value;
  using base_t                = std::graph::empty_value;

  using vertex_set             = typename graph_type;
  using vertex_type            = typename vertex_set::value_type;
  using const_vertex_type      = const vertex_type;
  using iterator               = typename graph_type::iterator;
  using const_iterator         = typename graph_type::const_iterator;
  using vertex_iterator        = typename graph_type::iterator;
  using const_vertex_iterator  = typename graph_type::const_iterator;
  using vertex_index           = size_t;
  using vertex_user_value_type = std::graph::empty_value;
  using vertex_key_type        = vertex_index;
  using vertex_range           = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set*>(0)));
  using const_vertex_range     = decltype(::ranges::make_subrange(*reinterpret_cast<vertex_set const*>(0)));
  using vertex_size_type       = typename vertex_set::size_type;

  using edge_type       = std::tuple<size_t, Attributes...>; //typename graph_type::inner_container::value_type;
  using const_edge_type = const edge_type;
  //using edge_set             = void; //vector<edge_type, edge_allocator_type>;
  using edge_user_value_type = typename edge_type;

  //using edge_key_type   = pair<vertex_key_type, vertex_key_type>; // <from,to>

  using vertex_out_edge_range =
        decltype(::ranges::make_subrange(*reinterpret_cast<typename graph_type::inner_container*>(0)));
  using const_vertex_out_edge_range =
        decltype(::ranges::make_subrange(*reinterpret_cast<typename graph_type::inner_container const*>(0)));
  using vertex_out_edge_iterator       = typename vertex_out_edge_range::iterator;
  using const_vertex_out_edge_iterator = typename const_vertex_out_edge_range::iterator;
  using vertex_out_edge_size_type      = typename vertex_out_edge_range::size_type;

  using vertex_edge_range          = vertex_out_edge_range;
  using const_vertex_edge_range    = const_vertex_out_edge_range;
  using vertex_edge_iterator       = vertex_out_edge_iterator;
  using const_vertex_edge_iterator = const_vertex_out_edge_iterator;
  using vertex_edge_size_type      = vertex_out_edge_size_type;

  using edge_index          = size_t;
  using edge_iterator       = vertex_out_edge_iterator;
  using const_edge_iterator = const_vertex_out_edge_iterator;
  using edge_size_type      = vertex_out_edge_size_type;
  //using edge_allocator_type  = typename allocator_traits<A>::template rebind_alloc<edge_type>;

  using edge_range       = vertex_out_edge_range;
  using const_edge_range = const_vertex_out_edge_range;

public:
  vov_graph(graph_type& g) : graph_(g) {}

  vov_graph()                     = delete;
  vov_graph(vov_graph const&)     = delete;
  vov_graph(vov_graph&&) noexcept = default;
  ~vov_graph()                    = default;
  vov_graph& operator=(vov_graph const&) = delete;
  vov_graph& operator=(vov_graph&&) = default;

  vertex_iterator       begin() noexcept { return graph_.begin(); }
  const_vertex_iterator begin() const noexcept { return graph_.begin(); }
  const_vertex_iterator cbegin() const noexcept { return graph_.cbegin(); }

  vertex_iterator       end() noexcept { return graph_.end(); }
  const_vertex_iterator end() const noexcept { return graph_.end(); }
  const_vertex_iterator cend() const noexcept { return graph_.cend(); }

#if 0
  edge_iterator       edge_begin(graph_type&);
  const_edge_iterator edge_begin(graph_type const&) const;
  const_edge_iterator edge_cbegin(graph_type const&) const;

  edge_iterator       edge_end(graph_type&);
  const_edge_iterator edge_end(graph_type const&) const;
  const_edge_iterator edge_cend(graph_type const&) const;
#endif

  constexpr graph_type&       graph() noexcept { return graph_; }
  constexpr graph_type const& graph() const noexcept { return graph_; }

  constexpr graph_type&       vertices() noexcept { return graph_; }
  constexpr graph_type const& vertices() const noexcept { return graph_; }

  edge_size_type edges_size() const {
    edge_size_type n = 0;
    for (auto& u : vertices())
      for (auto& uv : u)
        ++n;
    return n;
  }

private:
  graph_type& graph_;
};


// Uniform API: Common functions (accepts graph, vertex and edge)
template <typename... Attributes>
constexpr auto value(::std::graph::edge_t<vov_graph<Attributes...>>& uv)
      -> ::std::graph::edge_value_t<vov_graph<Attributes...>>& {
  return uv; // edge properties
}

template <typename... Attributes>
constexpr auto vertices(vov_graph<Attributes...>& g) noexcept -> vertex_range_t<vov_graph<Attributes...>> {
  return g.vertices();
}
template <typename... Attributes>
constexpr auto vertices(vov_graph<Attributes...> const& g) noexcept -> vertex_range_t<vov_graph<Attributes...>> {
  return g.vertices();
}

template <typename... Attributes>
constexpr auto vertices_size(vov_graph<Attributes...> const& g) noexcept -> vertex_size_t<vov_graph<Attributes...>> {
  return g.graph().size();
}

template <typename... Attributes>
constexpr auto out_vertex(vov_graph<Attributes...>& g, edge_t<vov_graph<Attributes...>>& uv) noexcept
      -> vertex_iterator_t<vov_graph<Attributes...>> {
  return g.graph().begin() + vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto out_vertex(vov_graph<Attributes...> const& g, edge_t<vov_graph<Attributes...>> const& uv) noexcept
      -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return g.graph().begin() + vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto out_vertex_key(vov_graph<Attributes...> const& g, edge_t<vov_graph<Attributes...>> const& uv) noexcept
      -> vertex_key_t<vov_graph<Attributes...>> {
  return get<0>(uv);
}

template <typename... Attributes>
constexpr auto vertex(vov_graph<Attributes...>&           g,
                      edge_t<vov_graph<Attributes...>>&   uv,
                      vertex_t<vov_graph<Attributes...>>& source) noexcept
      -> vertex_iterator_t<vov_graph<Attributes...>> {
  return out_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex(vov_graph<Attributes...> const&           g,
                      edge_t<vov_graph<Attributes...>> const&   uv,
                      vertex_t<vov_graph<Attributes...>> const& source) noexcept
      -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return out_vertex(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(vov_graph<Attributes...> const& g, edge_t<vov_graph<Attributes...>> const& uv) noexcept
      -> vertex_key_t<vov_graph<Attributes...>> {
  return out_vertex_key(g, uv);
}
template <typename... Attributes>
constexpr auto vertex_key(vov_graph<Attributes...> const&           g,
                          edge_t<vov_graph<Attributes...>> const&   uv,
                          vertex_t<vov_graph<Attributes...>> const& source) noexcept
      -> vertex_key_t<vov_graph<Attributes...>> {
  return out_vertex_key(g, uv);
}

template <typename... Attributes>
constexpr auto vertex_key(vov_graph<Attributes...> const&               g,
                          edge_t<vov_graph<Attributes...>> const&       uv,
                          vertex_key_t<vov_graph<Attributes...>> const& source_key) noexcept
      -> vertex_key_t<vov_graph<Attributes...>> {
  return out_vertex_key(g, uv);
}


template <typename... Attributes>
constexpr auto vertex_key(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> vertex_key_t<vov_graph<Attributes...>> {
  return &u - g.vertices().data();
}

template <typename... Attributes>
constexpr auto begin(vov_graph<Attributes...>& g) noexcept -> vertex_iterator_t<vov_graph<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto begin(vov_graph<Attributes...> const& g) noexcept -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return g.begin();
}
template <typename... Attributes>
constexpr auto cbegin(vov_graph<Attributes...> const& g) noexcept -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return g.cbegin();
}

template <typename... Attributes>
constexpr auto end(vov_graph<Attributes...>& g) noexcept -> vertex_iterator_t<vov_graph<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto end(vov_graph<Attributes...> const& g) noexcept -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return g.end();
}
template <typename... Attributes>
constexpr auto cend(vov_graph<Attributes...> const& g) noexcept -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return g.end();
}

template <typename... Attributes>
constexpr auto find_vertex(vov_graph<Attributes...>& g, vertex_key_t<vov_graph<Attributes...>> const& key) noexcept
      -> vertex_iterator_t<vov_graph<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}
template <typename... Attributes>
constexpr auto find_vertex(vov_graph<Attributes...> const&               g,
                           vertex_key_t<vov_graph<Attributes...>> const& key) noexcept
      -> const_vertex_iterator_t<vov_graph<Attributes...>> {
  return key < vertices_size(g) ? (g.graph().begin() + key) : g.graph.end();
}

template <typename... Attributes>
void reserve_vertices(vov_graph<Attributes...>& g, vertex_size_t<vov_graph<Attributes...>> n) {
  g.graph_().reserve(n);
}

template <typename... Attributes>
void resize_vertices(vov_graph<Attributes...>& g, vertex_size_t<vov_graph<Attributes...>> n) {
  g.graph_().resize();
}

template <typename... Attributes>
constexpr auto edges(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_out_edge_range_t<vov_graph<Attributes...>> {
  return out_edges(g, u);
}
template <typename... Attributes>
constexpr auto edges(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_range_t<vov_graph<Attributes...>> {
  return out_edges(g, u);
}

template <typename... Attributes>
constexpr auto edges_size(vov_graph<Attributes...> const& g) noexcept -> edge_size_t<vov_graph<Attributes...>> {
  return g.edges_size();
}

template <typename... Attributes>
constexpr auto edges_size(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> vertex_edge_size_t<vov_graph<Attributes...>> {
  return u.size();
}

template <typename... Attributes>
constexpr auto find_edge(vov_graph<Attributes...>&           g,
                         vertex_t<vov_graph<Attributes...>>& u,
                         vertex_t<vov_graph<Attributes...>>& v) noexcept -> edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, u, v);
}
template <typename... Attributes>
constexpr auto find_edge(vov_graph<Attributes...> const&           g,
                         vertex_t<vov_graph<Attributes...>> const& u,
                         vertex_t<vov_graph<Attributes...>> const& v) noexcept
      -> const_edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, u, v);
}

template <typename... Attributes>
constexpr auto find_edge(vov_graph<Attributes...>&               g,
                         vertex_key_t<vov_graph<Attributes...>>& ukey,
                         vertex_key_t<vov_graph<Attributes...>>& vkey) noexcept
      -> edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_edge(vov_graph<Attributes...> const&               g,
                         vertex_key_t<vov_graph<Attributes...>> const& ukey,
                         vertex_key_t<vov_graph<Attributes...>> const& vkey) noexcept
      -> const_edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

// Directed API (outgoing): Vertex functions
template <typename... Attributes>
constexpr auto out_edges(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_out_edge_range_t<vov_graph<Attributes...>> {
  return u;
}
template <typename... Attributes>
constexpr auto out_edges(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_range_t<vov_graph<Attributes...>> {
  return u;
}

template <typename... Attributes>
constexpr auto begin(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto begin(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto cbegin(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto end(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto end(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto cend(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}


template <typename... Attributes>
constexpr auto out_begin(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto out_begin(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}
template <typename... Attributes>
constexpr auto out_cbegin(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.begin();
}

template <typename... Attributes>
constexpr auto out_end(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) noexcept
      -> vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto out_end(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}
template <typename... Attributes>
constexpr auto out_cend(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return u.end();
}

template <typename... Attributes>
constexpr auto out_size(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> vertex_out_edge_size_t<vov_graph<Attributes...>> {
  return u.size();
}
template <typename... Attributes>
constexpr auto out_degree(vov_graph<Attributes...> const& g, vertex_t<vov_graph<Attributes...>> const& u) noexcept
      -> vertex_out_edge_size_t<vov_graph<Attributes...>> {
  return u.size();
}

template <typename... Attributes>
constexpr auto find_out_edge(vov_graph<Attributes...>&           g,
                             vertex_t<vov_graph<Attributes...>>& u,
                             vertex_t<vov_graph<Attributes...>>& v) noexcept
      -> vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}
template <typename... Attributes>
constexpr auto find_out_edge(vov_graph<Attributes...> const&           g,
                             vertex_t<vov_graph<Attributes...>> const& u,
                             vertex_t<vov_graph<Attributes...>> const& v) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename... Attributes>
constexpr auto find_out_edge(vov_graph<Attributes...>&                     g,
                             vertex_key_t<vov_graph<Attributes...>> const& ukey,
                             vertex_key_t<vov_graph<Attributes...>> const& vkey) noexcept
      -> vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}
template <typename... Attributes>
constexpr auto find_out_edge(vov_graph<Attributes...> const&               g,
                             vertex_key_t<vov_graph<Attributes...>> const& ukey,
                             vertex_key_t<vov_graph<Attributes...>> const& vkey) noexcept
      -> const_vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  return find_out_edge(g, vertices(g)[ukey], vertices(g)[vkey]);
}

template <typename... Attributes>
constexpr void erase_edges(vov_graph<Attributes...>& g, vertex_out_edge_range_t<vov_graph<Attributes...>> uv_rng) {
  erase(uv_rng);
}

template <typename... Attributes>
constexpr void clear_out_edges(vov_graph<Attributes...>& g, vertex_t<vov_graph<Attributes...>>& u) {
  u.clear();
}

// Directed API (outgoing): Edge functions
template <typename... Attributes>
constexpr auto erase_edge(vov_graph<Attributes...>& g, vertex_out_edge_iterator_t<vov_graph<Attributes...>> uv)
      -> vertex_out_edge_iterator_t<vov_graph<Attributes...>> {
  using G        = vov_graph<Attributes...>;
  vertex_t<G>& u = in_vertex(g, uv);
  u.erase(uv);
}

using vov_germany_t      = ::vov<double>;
using vov_germany_wrap_t = vov_graph<double>;

vov_germany_t get_germany_routes() {
  using directed_graph_t = typename GraphXlate<caa_routes_edge_mapper>::target_graph_t;
  directed_graph_t caa_routes_graph(
        germany_routes_directed_graph.create_graph()); // translates string labels to indexes

  vov_germany_t routes(vertices_size(caa_routes_graph));
  for (edge_t<directed_graph_t>& uv : edges(caa_routes_graph)) {
    vertex_key_t<directed_graph_t> u_key = in_vertex_key(caa_routes_graph, uv);
    vertex_key_t<directed_graph_t> v_key = out_vertex_key(caa_routes_graph, uv);
    routes.push_back(u_key, v_key, uv.weight);
  }
  return routes;
}

} // namespace adaptor::vov


TEST_CASE(vov_graph_test, vov) {
  //using namespace adaptor::vov;
  using vov_germany_t = adaptor::vov::vov_germany_t;
  using Graph         = adaptor::vov::vov_germany_wrap_t;

  vov_germany_t vov_germany = adaptor::vov::get_germany_routes();
  Graph         g           = Graph(vov_germany);

  EXPECT_EQ(10, vertices_size(g));
  EXPECT_EQ(11, edges_size(g));

  using lhs  = std::forward_list<std::tuple<size_t, double>>;
  using rhs  = typename vov_germany_t::inner_container;
  using et   = std::graph::edge_t<Graph>;
  using evt  = std::graph::edge_value_t<Graph>;
  using evt2 = typename vov_germany_t::inner_container::value_type;
  static_assert(std::is_same_v<lhs, rhs>, "not same");
  static_assert(std::is_same_v<et, std::tuple<size_t, double>>, "edge types not same");
  static_assert(std::is_same_v<evt, std::tuple<size_t, double>>, "edge value types not same");
  static_assert(std::is_same_v<evt2, std::tuple<size_t, double>>, "edge value types2 not same");

  for (std::graph::vertex_t<Graph>& u : vertices(g)) {
    cout << "city[" << vertex_key(g, u) << "]\n";
    for (std::graph::edge_t<Graph>& uv : edges(g, u)) {
      //adaptor::vov::value(uv);    // compiler error in msvc (bug?)
      cout << "  --> city[" << out_vertex_key(g, uv) << "] " << get<1>(uv) << "km\n";
    }
  }
}

TEST_CASE(vov_graph_test, DFSVertex) {
  using std::graph::dfs_vertex_range;
  using vov_germany_t = adaptor::vov::vov_germany_t;
  using Graph         = adaptor::vov::vov_germany_wrap_t;

  vov_germany_t vov_germany = adaptor::vov::get_germany_routes();
  Graph         g           = Graph(vov_germany);

  // Linker errors in MSVC
  // begin(g, vertex_t<Graph>&) isn't found/generated (linker error)
  // end(g, vertex_t<Graph>&) isn't found/generated (linker error)
  // end(g) isn't found/generated (linker error)

#if 0
#  if TEST_OPTION == TEST_OPTION_OUTPUT
  dfs_vertex_range dfs_vtx_rng(g, begin(g) + 2); // "Frankfürt"
  for (dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin(); u != dfs_vtx_rng.end(); ++u)
    cout << string(u.depth() * 2, ' ') << vertex_key(g,*u) << endl;

  /* Output: seed = Frankfürt
  Frankfürt
    Mannheim
      Karlsruhe
        Augsburg
          München
    Würzburg
      Erfurt
      Nürnberg
        Stuttgart
    Kassel
  */

  // a flat list when using range syntax (depth n/a on vertex)
  cout << endl;
  for (std::graph::vertex_t<Graph>& u : dfs_vertex_range(g, begin(g) + 2)) // Frankfürt
    cout << vertex_key(g,u) << endl;
    /* Output: seed = Frankfürt
    Frankfürt
    Mannheim
    Karlsruhe
    Augsburg
    München
    Würzburg
    Erfurt
    Nürnberg
    Stuttgart
    Kassel
  */
#elif TEST_OPTION == TEST_OPTION_GEN
#elif TEST_OPTION == TEST_OPTION_TEST
  dfs_vertex_range                  dfs_vtx_rng(g, find_city(g, "Frankfürt"));
  dfs_vertex_range<Graph>::iterator u = dfs_vtx_rng.begin();
  EXPECT_EQ("Frankfürt", u->name);
  EXPECT_EQ(1, u.depth());
  EXPECT_EQ("Mannheim", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Karlsruhe", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Augsburg", (++u)->name);
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ("München", (++u)->name);
  EXPECT_EQ(5, u.depth());
  EXPECT_EQ("Würzburg", (++u)->name);
  EXPECT_EQ(2, u.depth());
  EXPECT_EQ("Erfurt", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Nürnberg", (++u)->name);
  EXPECT_EQ(3, u.depth());
  EXPECT_EQ("Stuttgart", (++u)->name);
  EXPECT_EQ(4, u.depth());
  EXPECT_EQ("Kassel", (++u)->name);
  EXPECT_EQ(2, u.depth());
#endif
#endif
}
