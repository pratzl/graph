
#ifndef COMPRESSED_ADJ_ARRAY_IMPL_HPP
#define COMPRESSED_ADJ_ARRAY_IMPL_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// caa_edge
///
template <typename VV, typename EV, typename GV, typename A>
caa_edge<VV, EV, GV, A>::caa_edge(vertex_set const& vertices, vertex_iterator out_vertex, vertex_iterator in_vertex)
      : out_vertex_(out_vertex - vertices.begin()) {}
template <typename VV, typename EV, typename GV, typename A>
caa_edge<VV, EV, GV, A>::caa_edge(vertex_set const&           vertices,
                                  vertex_iterator             out_vertex,
                                  vertex_iterator             in_vertex,
                                  edge_user_value_type const& val)
      : out_vertex_(out_vertex - vertices.begin()), base_t(val) {}

//template <typename VV, typename EV, typename GV, typename A>
//caa_edge<VV, EV, GV, A>::caa_edge(vertex_set const& vertices, vertex_iterator out_vertex, vertex_iterator in_vertex, edge_user_value_type&& val)
//      : out_vertex_(out_vertex - vertices.begin()), base_t(move(val)) {}

template <typename VV, typename EV, typename GV, typename A>
typename caa_edge<VV, EV, GV, A>::vertex_type& caa_edge<VV, EV, GV, A>::out_vertex(graph_type& g) {
  return *(g.vertices().begin() + out_vertex_);
}

template <typename VV, typename EV, typename GV, typename A>
typename caa_edge<VV, EV, GV, A>::vertex_type const& caa_edge<VV, EV, GV, A>::out_vertex(graph_type const& g) const {
  return *(g.vertices().begin() + out_vertex_);
}


///-------------------------------------------------------------------------------------
/// caa_vertex
///
template <typename VV, typename EV, typename GV, typename A>
caa_vertex<VV, EV, GV, A>::caa_vertex(vertex_set& vertices, vertex_index index) {}
template <typename VV, typename EV, typename GV, typename A>
caa_vertex<VV, EV, GV, A>::caa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type const& val)
      : base_t(val) {}
template <typename VV, typename EV, typename GV, typename A>
caa_vertex<VV, EV, GV, A>::caa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&& val)
      : base_t(move(val)) {}

template <typename VV, typename EV, typename GV, typename A>
void caa_vertex<VV, EV, GV, A>::set_edge_begin(graph_type& g, edge_iterator uv) {
  if (first_edge_ == numeric_limits<edge_index>::max()) // not set yet?
    first_edge_ = uv - g.edges().begin();
}


template <typename VV, typename EV, typename GV, typename A>
typename caa_vertex<VV, EV, GV, A>::vertex_edge_iterator caa_vertex<VV, EV, GV, A>::edge_begin(graph_type& g) {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, typename A>
typename caa_vertex<VV, EV, GV, A>::const_vertex_edge_iterator
caa_vertex<VV, EV, GV, A>::edge_begin(graph_type const& g) const {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, typename A>
typename caa_vertex<VV, EV, GV, A>::const_vertex_edge_iterator
caa_vertex<VV, EV, GV, A>::edge_cbegin(graph_type const& g) const {
  return g.edges().begin() + first_edge_;
}


///-------------------------------------------------------------------------------------
/// caa_graph
///

template <typename VV, typename EV, typename GV, typename A>
caa_graph<VV, EV, GV, A>::caa_graph(allocator_type alloc) : vertices_(alloc), edges_(alloc), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename A>
caa_graph<VV, EV, GV, A>::caa_graph(graph_user_value_type const& val, allocator_type alloc)
      : vertices_(alloc), edges_(alloc), base_t(val), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename A>
caa_graph<VV, EV, GV, A>::caa_graph(graph_user_value_type&& val, allocator_type alloc)
      : vertices_(alloc), edges_(alloc), base_t(move(val)), alloc_(alloc) {}


template <typename VV, typename EV, typename GV, typename A>
template <template <class, class> class VCont, template <class, class> class ECont, class VV2, class A2, class A3>
caa_graph<VV, EV, GV, A>::caa_graph(VCont<VV2, A2> const&           vcont,
                                    ECont<edge_key_type, A3> const& econt,
                                    allocator_type                  alloc)
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vcont.size();
  for (edge_key_type const& edge_key : econt)
    max_vtx_key = max(max_vtx_key, max(edge_key.first, edge_key.second));

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if (!vcont.empty()) {
    for (vertex_user_value_type const& vtx_val : vcont)
      create_vertex(vtx_val);
  }

  // add edges
  if (!econt.empty()) {
    edges_.reserve(econt.size());
    vertex_iterator t = to_iterator(*this, vertices_[econt.begin()->first]);
    for (edge_key_type const& edge_key : econt) {
      vertex_iterator u = to_iterator(*this, vertices_[edge_key.first]);
      if (u < t) {
        assert(false); // container must be sorted by edge_key.first
        throw domain_error("edges not ordered");
      }

      // assure begin edge is set for vertices w/o edges
      for (; t < u; ++t)
        t->set_edge_begin(*this, edges_.end());

      edge_iterator uv = create_edge(edge_key.first, edge_key.second);
      u->set_edge_begin(*this, uv);
    }
  }
}

template <typename VV, typename EV, typename GV, typename A>
template <template <class, class> class VCont,
          template <class, class>
          class ECont,
          class VV2,
          class EV2,
          class A2,
          class A3>
caa_graph<VV, EV, GV, A>::caa_graph(VCont<VV2, A2> const&                      vcont,
                                    ECont<pair<edge_key_type, EV2>, A3> const& econt,
                                    allocator_type                             alloc)
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vcont.size();
  for (auto& [edge_key, edge_val] : econt)
    max_vtx_key = max(max_vtx_key, max(edge_key.first, edge_key.second));

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if (!vcont.empty()) {
    for (vertex_user_value_type const& vtx_val : vcont)
      create_vertex(vtx_val);
  }

  // add edges
  if (!econt.empty()) {
    edges_.reserve(econt.size());
    vertex_iterator t = to_iterator(*this, vertices_[econt.begin()->first.first]);
    for (auto& [edge_key, edge_val] : econt) {
      vertex_iterator u = to_iterator(*this, vertices_[edge_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      for (; t < u; ++t)
        t->set_edge_begin(*this, edges_.end());

      edge_iterator uv = create_edge(edge_key.first, edge_key.second);
      u->set_edge_begin(*this, uv);
    }
  }
}

template <typename VV, typename EV, typename GV, typename A>
template <template <class, class> class VCont,
          template <class, class, class>
          class ECont,
          class VV2,
          class Cmp,
          class A2,
          class A3>
caa_graph<VV, EV, GV, A>::caa_graph(VCont<VV2, A2> const&                vcont,
                                    ECont<edge_key_type, Cmp, A3> const& econt,
                                    allocator_type                       alloc)
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vcont.size();
  for (auto& edge_key : econt)
    max_vtx_key = max(max_vtx_key, max(edge_key.first, edge_key.second));

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if (!vcont.empty()) {
    for (vertex_user_value_type const& vtx_val : vcont)
      create_vertex(vtx_val);
  }

  // add edges
  if (!econt.empty()) {
    edges_.reserve(econt.size());
    vertex_iterator t = to_iterator(*this, vertices_[econt.begin()->first.first]);
    for (auto& edge_key : econt) {
      vertex_iterator u = to_iterator(*this, vertices_[edge_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      for (; t < u; ++t)
        t->set_edge_begin(*this, edges_.end());

      edge_iterator uv = create_edge(edge_key.first, edge_key.second);
      u->set_edge_begin(*this, uv);
    }
  }
}

template <typename VV, typename EV, typename GV, typename A>
template <template <class, class> class VCont,
          template <class, class, class, class>
          class ECont,
          class VV2,
          class EV2,
          class Cmp,
          class A2,
          class A3>
caa_graph<VV, EV, GV, A>::caa_graph(VCont<VV2, A2> const&                     vcont,
                                    ECont<edge_key_type, EV2, Cmp, A3> const& econt,
                                    allocator_type                            alloc)
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vcont.size();
  for (auto& [edge_key, edge_val] : econt)
    max_vtx_key = max(max_vtx_key, max(edge_key.first, edge_key.second));

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if (!vcont.empty()) {
    for (vertex_user_value_type const& vtx_val : vcont)
      create_vertex(vtx_val);
  }

  // add edges
  if (!econt.empty()) {
    edges_.reserve(econt.size());
    vertex_iterator t = to_iterator(*this, vertices_[econt.begin()->first.first]);
    for (auto& [edge_key, edge_val] : econt) {
      vertex_iterator u = to_iterator(*this, vertices_[edge_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      for (; t < u; ++t)
        t->set_edge_begin(*this, edges_.end());

      edge_iterator uv = create_edge(edge_key.first, edge_key.second);
      u->set_edge_begin(*this, uv);
    }
  }
}

template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw domain_error("edges not ordered");
}


template <typename VV, typename EV, typename GV, typename A>
constexpr typename caa_graph<VV, EV, GV, A>::vertex_set& caa_graph<VV, EV, GV, A>::vertices() {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename A>
constexpr typename caa_graph<VV, EV, GV, A>::vertex_set const& caa_graph<VV, EV, GV, A>::vertices() const {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::reserve_vertices(vertex_size_type n) {
  vertices_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::resize_vertices(vertex_size_type n, vertex_user_value_type const& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::vertex_iterator caa_graph<VV, EV, GV, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::vertex_iterator
caa_graph<VV, EV, GV, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename A>
template <class VV2>
typename caa_graph<VV, EV, GV, A>::vertex_iterator caa_graph<VV, EV, GV, A>::create_vertex(VV2 const& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), val));
  return vertices_.begin() + (vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::vertex_iterator caa_graph<VV, EV, GV, A>::find_vertex(vertex_key_type const& key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::const_vertex_iterator
caa_graph<VV, EV, GV, A>::find_vertex(vertex_key_type const& key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, typename A>
constexpr typename caa_graph<VV, EV, GV, A>::edge_set& caa_graph<VV, EV, GV, A>::edges() {
  return edges_;
}
template <typename VV, typename EV, typename GV, typename A>
constexpr typename caa_graph<VV, EV, GV, A>::edge_set const& caa_graph<VV, EV, GV, A>::edges() const {
  return edges_;
}

template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::reserve_edges(edge_size_type n) {
  edges_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::edge_iterator caa_graph<VV, EV, GV, A>::create_edge(vertex_key_type const& from_key,
                                                                                       vertex_key_type const& to_key) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, to, from));
  return edges_.begin() + (edges_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename A>
typename caa_graph<VV, EV, GV, A>::edge_iterator caa_graph<VV, EV, GV, A>::create_edge(vertex_key_type const& from_key,
                                                                                       vertex_key_type const& to_key,
                                                                                       edge_user_value_type&& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, to, from, move(val)));
  return edges_.begin() + (edges_.size() - 1);
}

template <typename VV, typename EV, typename GV, typename A>
template <class EV2>
typename caa_graph<VV, EV, GV, A>::edge_iterator
caa_graph<VV, EV, GV, A>::create_edge(vertex_key_type const& from_key, vertex_key_type const& to_key, EV2 const& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, to, from, val));
  return edges_.begin() + (edges_.size() - 1);
}

template <typename VV, typename EV, typename GV, typename A>
void caa_graph<VV, EV, GV, A>::clear() {
  vertices_.clear();
  edges_.clear();
}


///-------------------------------------------------------------------------------------
/// caa_graph graph API
///

//template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//using vertices_size_t = typename G<VV, EV, GV, A>::vertex_size_type;

//
// API vertex functions
//
template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.edges()); } // vector & array
constexpr auto vertex_key(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u) noexcept
      -> vertex_key_t<G<VV, EV, GV, A>> {
  return &u - g.vertices().data();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto to_iterator(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u) -> vertex_iterator_t<G<VV, EV, GV, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto to_iterator(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u)
      -> const_vertex_iterator_t<G<VV, EV, GV, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.edges()); } // vector & array
constexpr auto out_edges(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u) noexcept
      -> out_edge_range_t<G<VV, EV, GV, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return ::ranges::make_subrange(u.edge_begin(g), v->edge_begin(g));
  } else {
    return ::ranges::make_subrange(u.edge_begin(g), g.edges().end());
  }
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.edges()); } // vector & array
constexpr auto out_edges(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u) noexcept
      -> const_out_edge_range_t<G<VV, EV, GV, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return ::ranges::make_subrange(u.edge_begin(g), v->edge_begin(g));
  } else {
    return ::ranges::make_subrange(u.edge_begin(g), g.edges().end());
  }
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.edges()); } // vector & array
constexpr auto out_size(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u) noexcept -> out_edge_size_t<G> {
  if (&u < &g.edges().back()) {
    auto& v = *(&u + 1);
    return v.edge_begin(g) - u.edge_begin(g);
  } else {
    return g.edges().end() - u.edge_begin(g);
  }
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto out_degree(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u) noexcept
      -> out_edge_size_t<G> {
  return out_size(g, u);
}

//template <graph_c G> constexpr void clear_out_edges(G& g, vertex_t<G>& u); // n/a

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto edges(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u) noexcept
      -> out_edge_range_t<G<VV, EV, GV, A>> {
  return out_edges(g, u);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto edges(G<VV, EV, GV, A> const& g, vertex_t<G<VV, EV, GV, A>> const& u) noexcept
      -> const_out_edge_range_t<G<VV, EV, GV, A>> {
  return out_edges(g, u);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_vertex(G<VV, EV, GV, A>& g, vertex_key_t<G<VV, EV, GV, A>> const& key) noexcept
      -> vertex_iterator_t<G<VV, EV, GV, A>> {
  return g.find_vertex(key);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_vertex(G<VV, EV, GV, A> const& g, vertex_key_t<G<VV, EV, GV, A>> const& key) noexcept
      -> const_vertex_iterator_t<G<VV, EV, GV, A>> {
  return g.find_vertex(key);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_vertex(G<VV, EV, GV, A>& g) -> pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_vertex(), true);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_vertex(G<VV, EV, GV, A>& g, vertex_value_t<G<VV, EV, GV, A>> const& val)
      -> pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_vertex(val), true);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_vertex(G<VV, EV, GV, A>& g, vertex_value_t<G<VV, EV, GV, A>>&& val)
      -> pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_vertex(move(val)), true);
}

//
// API edge functions
//
template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto out_vertex(G<VV, EV, GV, A>& g, edge_t<G<VV, EV, GV, A>>& uv) noexcept -> vertex_t<G<VV, EV, GV, A>>& {
  return uv.out_vertex(g);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto out_vertex(G<VV, EV, GV, A> const& g, edge_t<G<VV, EV, GV, A>> const& uv) noexcept
      -> vertex_t<G<VV, EV, GV, A>> const& {
  return uv.out_vertex(g);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto vertex(G<VV, EV, GV, A>& g, edge_t<G<VV, EV, GV, A>>& uv) noexcept -> vertex_t<G<VV, EV, GV, A>>& {
  return uv.out_vertex(g);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto vertex(G<VV, EV, GV, A> const& g, edge_t<G<VV, EV, GV, A>> const& uv) noexcept
      -> vertex_t<G<VV, EV, GV, A>> const& {
  return uv.out_vertex(g);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_out_edge(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u, vertex_t<G<VV, EV, GV, A>>& v) noexcept
      -> out_edge_iterator_t<G<VV, EV, GV, A>> {
  return ::ranges::find_if(out_range(g, u), [&g, &v](auto uv) { return &out_vertex(g, uv) == &v; });
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_out_edge(G<VV, EV, GV, A> const&           g,
                             vertex_t<G<VV, EV, GV, A>> const& u,
                             vertex_t<G<VV, EV, GV, A>> const& v) noexcept
      -> const_out_edge_iterator_t<G<VV, EV, GV, A>> {
  return ::ranges::find_if(out_range(g, u), [&g, &v](auto uv) { return &out_vertex(g, uv) == &v; });
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_out_edge(G<VV, EV, GV, A>&                     g,
                             vertex_key_t<G<VV, EV, GV, A>> const& ukey,
                             vertex_key_t<G<VV, EV, GV, A>> const& vkey) noexcept
      -> out_edge_iterator_t<G<VV, EV, GV, A>> {
  return find_out_edge(g, vertex(g, ukey), vertex(g, vkey));
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_out_edge(G<VV, EV, GV, A> const&               g,
                             vertex_key_t<G<VV, EV, GV, A>> const& ukey,
                             vertex_key_t<G<VV, EV, GV, A>> const& vkey) noexcept
      -> const_out_edge_iterator_t<G<VV, EV, GV, A>> {
  return find_out_edge(g, vertex(g, ukey), vertex(g, vkey));
}


template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_edge(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u, vertex_t<G<VV, EV, GV, A>>& v) noexcept
      -> out_edge_iterator_t<G<VV, EV, GV, A>> {
  return ::ranges::find_if(out_range(g, u), [&g, &v](auto uv) { return &out_vertex(g, uv) == &v; });
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_edge(G<VV, EV, GV, A> const&           g,
                         vertex_t<G<VV, EV, GV, A>> const& u,
                         vertex_t<G<VV, EV, GV, A>> const& v) noexcept -> const_out_edge_iterator_t<G<VV, EV, GV, A>> {
  return ::ranges::find_if(out_range(g, u), [&g, &v](auto uv) { return &out_vertex(g, uv) == &v; });
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_edge(G<VV, EV, GV, A>&                     g,
                         vertex_key_t<G<VV, EV, GV, A>> const& ukey,
                         vertex_key_t<G<VV, EV, GV, A>> const& vkey) noexcept -> out_edge_iterator_t<G<VV, EV, GV, A>> {
  return find_out_edge(g, vertex(g, ukey), vertex(g, vkey));
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto find_edge(G<VV, EV, GV, A> const&               g,
                         vertex_key_t<G<VV, EV, GV, A>> const& ukey,
                         vertex_key_t<G<VV, EV, GV, A>> const& vkey) noexcept
      -> const_out_edge_iterator_t<G<VV, EV, GV, A>> {
  return find_out_edge(g, vertex(g, ukey), vertex(g, vkey));
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_edge(G<VV, EV, GV, A>& g, vertex_t<G<VV, EV, GV, A>>& u, vertex_t<G<VV, EV, GV, A>>& v)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return create_edge(vertex_key(g, u), vertex_key(g, v));
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_edge(G<VV, EV, GV, A>&               g,
                           vertex_t<G<VV, EV, GV, A>>&     u,
                           vertex_t<G<VV, EV, GV, A>>&     v,
                           edge_value_t<G<VV, EV, GV, A>>& val)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return create_edge(vertex_key(g, u), vertex_key(g, v), val);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_edge(G<VV, EV, GV, A>&                g,
                           vertex_t<G<VV, EV, GV, A>>&      u,
                           vertex_t<G<VV, EV, GV, A>>&      v,
                           edge_value_t<G<VV, EV, GV, A>>&& val)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return create_edge(vertex_key(g, u), vertex_key(g, v), move(val));
}


template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto
create_edge(G<VV, EV, GV, A>& g, vertex_key_t<G<VV, EV, GV, A>>& ukey, vertex_key_t<G<VV, EV, GV, A>>& vkey)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_edge(ukey, vkey), true);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_edge(G<VV, EV, GV, A>&               g,
                           vertex_key_t<G<VV, EV, GV, A>>& ukey,
                           vertex_key_t<G<VV, EV, GV, A>>& vkey,
                           edge_value_t<G<VV, EV, GV, A>>& val)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_edge(ukey, vkey, val), true);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
//requires(G&& g) { contiguous_range(g.vertices()); } // vector & array
constexpr auto create_edge(G<VV, EV, GV, A>&                g,
                           vertex_key_t<G<VV, EV, GV, A>>&  ukey,
                           vertex_key_t<G<VV, EV, GV, A>>&  vkey,
                           edge_value_t<G<VV, EV, GV, A>>&& val)
      -> pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool> {
  return pair<vertex_edge_iterator_t<G<VV, EV, GV, A>>, bool>(g.create_edge(ukey, vkey, move(val)), true);
}


//
// API graph functions
//

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
constexpr auto vertices(G<VV, EV, GV, A>& g) noexcept -> vertex_range_t<G<VV, EV, GV, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
constexpr auto vertices(G<VV, EV, GV, A> const& g) noexcept -> const_vertex_range_t<G<VV, EV, GV, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename A>
constexpr auto vertices_size(caa_graph<VV, EV, GV, A> const& g) noexcept -> vertex_size_t<caa_graph<VV, EV, GV, A>> {
  return g.vertices().size();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
void reserve_vertices(G<VV, EV, GV, A>& g, vertex_size_t<G<VV, EV, GV, A>> n) {
  g.reserve(n);
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
void resize_vertices(G<VV, EV, GV, A>& g, vertex_size_t<G<VV, EV, GV, A>> n) {
  g.resize(n);
}
template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
void resize_vertices(G<VV, EV, GV, A>&                       g,
                     vertex_size_t<G<VV, EV, GV, A>>         n,
                     vertex_value_t<G<VV, EV, GV, A>> const& val) {
  g.resize(n, val);
}


template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
constexpr auto edges(G<VV, EV, GV, A>& g) noexcept -> edge_range_t<G<VV, EV, GV, A>> {
  return g.edges();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
constexpr auto edges(G<VV, EV, GV, A> const& g) noexcept -> const_edge_range_t<G<VV, EV, GV, A>> {
  return g.edges();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
constexpr auto edges_size(G<VV, EV, GV, A> const& g) noexcept -> edge_size_t<G<VV, EV, GV, A>> {
  return g.edges().size();
}

template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
void reserve_edges(G<VV, EV, GV, A>& g, edge_size_t<G<VV, EV, GV, A>> n) {
  g.reserve_edges(n);
}


template <template <typename, typename, typename, typename> class G, typename VV, typename EV, typename GV, typename A>
void clear(G<VV, EV, GV, A>& g) {
  g.clear();
}


} // namespace std::graph

#endif // COMPRESSED_ADJ_ARRAY_IMPL_HPP
