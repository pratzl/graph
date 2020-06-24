//
//	Author: J. Phillip Ratzloff
//
#include <range/v3/algorithm/find_if.hpp>

#ifndef COMPRESSED_ADJ_ARRAY_IMPL_HPP
#  define COMPRESSED_ADJ_ARRAY_IMPL_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// daa_edge
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_edge<VV, EV, GV, IndexT, A>::daa_edge(vertex_set const& vertices,
                                          vertex_iterator   in_vertex,
                                          vertex_iterator   out_vertex)
      : base_type()
      , in_vertex_(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , out_vertex_(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_edge<VV, EV, GV, IndexT, A>::daa_edge(vertex_set const&           vertices,
                                          vertex_iterator             in_vertex,
                                          vertex_iterator             out_vertex,
                                          edge_user_value_type const& val)
      : base_type(val)
      , in_vertex_(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , out_vertex_(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_edge<VV, EV, GV, IndexT, A>::daa_edge(vertex_set const&      vertices,
                                          vertex_iterator        in_vertex,
                                          vertex_iterator        out_vertex,
                                          edge_user_value_type&& val)
      : base_type(move(val))
      , in_vertex_(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , out_vertex_(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::vertex_iterator daa_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type& g) {
  return g.vertices().begin() + in_vertex_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type const& g) const {
  return g.vertices().begin() + in_vertex_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::vertex_key_type
daa_edge<VV, EV, GV, IndexT, A>::in_vertex_key(graph_type const&) const {
  return in_vertex_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::vertex_iterator daa_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type& g) {
  return g.vertices().begin() + out_vertex_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type const& g) const {
  return g.vertices().begin() + out_vertex_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_edge<VV, EV, GV, IndexT, A>::vertex_key_type
daa_edge<VV, EV, GV, IndexT, A>::out_vertex_key(graph_type const&) const {
  return out_vertex_;
}


///-------------------------------------------------------------------------------------
/// daa_vertex
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_vertex<VV, EV, GV, IndexT, A>::daa_vertex(vertex_set& vertices, vertex_index index) {}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_vertex<VV, EV, GV, IndexT, A>::daa_vertex(vertex_set&                   vertices,
                                              vertex_index                  index,
                                              vertex_user_value_type const& val)
      : base_type(val) {}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_vertex<VV, EV, GV, IndexT, A>::daa_vertex(vertex_set& vertices, vertex_index index, vertex_user_value_type&& val)
      : base_type(move(val)) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_vertex<VV, EV, GV, IndexT, A>::set_edge_begin(graph_type& g, edge_iterator uv) {
  if (first_edge_ == numeric_limits<edge_index>::max()) // not set yet?
    first_edge_ = static_cast<edge_index>(uv - g.edges().begin());
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_begin(graph_type& g) {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_begin(graph_type const& g) const {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_cbegin(graph_type const& g) const {
  return g.edges().begin() + first_edge_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_end(graph_type& g) {
  if (this < &g.vertices().back())
    return (this + 1)->edge_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_end(graph_type const& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->edge_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
daa_vertex<VV, EV, GV, IndexT, A>::edge_cend(graph_type const& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->edge_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_vertex<VV, EV, GV, IndexT, A>::edge_size_type
daa_vertex<VV, EV, GV, IndexT, A>::edge_size(graph_type const& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->first_edge_ - first_edge_;
  else
    return g.edges().size() - first_edge_;
}


///-------------------------------------------------------------------------------------
/// daa_graph
///

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(allocator_type alloc) : vertices_(alloc), edges_(alloc), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(graph_user_value_type const& val, allocator_type alloc)
      : vertices_(alloc), edges_(alloc), base_type(val), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(graph_user_value_type&& val, allocator_type alloc)
      : vertices_(alloc), edges_(alloc), base_type(move(val)), alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires daa_edge_data_c<ERng, EKeyFnc, EValueFnc> && daa_vertex_data_c<VRng, VValueFnc>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(ERng const&     erng,
                                            VRng const&     vrng,
                                            EKeyFnc const&  ekey_fnc,
                                            EValueFnc const& evalue_fnc,
                                            VValueFnc const& vvalue_fnc,
                                            GV const&       gv,
                                            A               alloc)
      : base_type(gv), vertices_(alloc), edges_(alloc), alloc_(alloc)
// clang-format on
{
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = static_cast<vertex_key_type>(vrng.size() - 1);
  for (auto& e : erng) {
    edge_key_type const& edge_key = ekey_fnc(e);
    max_vtx_key                   = max(max_vtx_key, max(edge_key.first, edge_key.second));
  }

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if constexpr (!same_as<decltype(vvalue_fnc(*::ranges::begin(vrng))), void>) {
    for (auto& vtx : vrng)
      create_vertex(vvalue_fnc(vtx));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (erng.size() > 0) {
    edges_.reserve(erng.size());
    edge_key_type   tu_key = ekey_fnc(*::ranges::begin(erng));
    vertex_iterator t      = to_iterator(*this, vertices_[tu_key.first]);
    for (auto& edge_data : erng) {
      edge_key_type   uv_key = ekey_fnc(edge_data);
      vertex_iterator u      = to_iterator(*this, vertices_[uv_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_out_edges(::ranges::make_subrange(t, u));

      edge_iterator uv;
      if constexpr (same_as<decltype(evalue_fnc(edge_data)), void>) {
        uv = create_edge(uv_key.first, uv_key.second);
      } else {
        uv = create_edge(uv_key.first, uv_key.second, evalue_fnc(edge_data));
      }
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_out_edges(::ranges::make_subrange(t, vertices_.end()));
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
requires daa_edge_data_c<ERng, EKeyFnc, EValueFnc> //
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(
      ERng const& erng, EKeyFnc const& ekey_fnc, EValueFnc const& evalue_fnc, GV const& gv, A alloc)
      : daa_graph(
              erng,
              vector<vertex_key_type>(),
              ekey_fnc,
              evalue_fnc,
              [](vertex_key_type const&) { return empty_value(); },
              gv,
              alloc) {}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(
      initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist, A alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    auto const& [ukey, vkey, uv_val] = edge_data;
    max_vtx_key                      = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    edges_.reserve(ilist.size());
    auto const& [tkey, uukey, tu_val] = *::ranges::begin(ilist);

    vertex_iterator t = to_iterator(*this, vertices_[tkey]);
    for (auto& edge_data : ilist) {
      auto const& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = to_iterator(*this, vertices_[ukey]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_out_edges(::ranges::make_subrange(t, u));

      edge_iterator uv;
      uv = create_edge(ukey, vkey, uv_val);
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_out_edges(::ranges::make_subrange(t, vertices_.end()));
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
daa_graph<VV, EV, GV, IndexT, A>::daa_graph(initializer_list<tuple<vertex_key_type, vertex_key_type>> const& ilist,
                                            A                                                                alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    auto const& [ukey, vkey] = edge_data;
    max_vtx_key              = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    edges_.reserve(ilist.size());
    auto const& [tkey, uukey] = *::ranges::begin(ilist);

    vertex_iterator t = to_iterator(*this, vertices_[tkey]);
    for (auto& edge_data : ilist) {
      auto const& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = to_iterator(*this, vertices_[ukey]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_out_edges(::ranges::make_subrange(t, u));

      edge_iterator uv;
      uv = create_edge(ukey, vkey);
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_out_edges(::ranges::make_subrange(t, vertices_.end()));
  }
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::finalize_out_edges(vertex_range vr) {
  for (auto& u : vr)
    u.set_edge_begin(*this, edges_.end());
  return ::ranges::end(vr);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::vertex_set& daa_graph<VV, EV, GV, IndexT, A>::vertices() {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::vertex_set const&
daa_graph<VV, EV, GV, IndexT, A>::vertices() const {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::reserve_vertices(vertex_size_type n) {
  vertices_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type n, vertex_user_value_type const& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator daa_graph<VV, EV, GV, IndexT, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class VV2>
typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::create_vertex(VV2 const& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type const& key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type const& key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator daa_graph<VV, EV, GV, IndexT, A>::begin() {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::begin() const {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::vertex_iterator daa_graph<VV, EV, GV, IndexT, A>::end() {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::end() const {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
daa_graph<VV, EV, GV, IndexT, A>::cend() const {
  return vertices_.cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::edge_set& daa_graph<VV, EV, GV, IndexT, A>::edges() {
  return edges_;
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename daa_graph<VV, EV, GV, IndexT, A>::edge_set const& daa_graph<VV, EV, GV, IndexT, A>::edges() const {
  return edges_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::reserve_edges(edge_size_type n) {
  edges_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::edge_iterator
daa_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type const& from_key, vertex_key_type const& to_key) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to));
  return edges_.begin() + (edges_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename daa_graph<VV, EV, GV, IndexT, A>::edge_iterator daa_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, edge_user_value_type&& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, move(val)));
  return edges_.begin() + static_cast<vertex_key_type>(edges_.size() - 1);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class EV2>
typename daa_graph<VV, EV, GV, IndexT, A>::edge_iterator daa_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, EV2 const& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, val));
  return edges_.begin() + (edges_.size() - 1);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::clear() {
  vertices_.clear();
  edges_.clear();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void daa_graph<VV, EV, GV, IndexT, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw domain_error("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// daa_graph graph API
///

//template <typename VV, typename EV, typename GV, typename IndexT, typename A>
//using vertices_size_t = typename daa_graph<VV, EV, GV, IndexT, A>::vertex_size_type;

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return static_cast<vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>>>(&u - g.vertices().data());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u)
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_edges(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_out_edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return ::ranges::make_subrange(u.edge_begin(g), v->edge_begin(g));
  } else {
    return ::ranges::make_subrange(u.edge_begin(g), g.edges().end());
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_edges(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_out_edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return ::ranges::make_subrange(u.edge_begin(g), v->edge_begin(g));
  } else {
    return ::ranges::make_subrange(u.edge_begin(g), g.edges().end());
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_begin(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_begin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_cbegin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cbegin(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_end(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_end(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                       vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_cend(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                        vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cend(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_size(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                        vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_out_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_size(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_degree(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_out_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_size(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_edges(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_edges(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_begin(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_begin(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                      vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_cbegin(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_end(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                   vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_end(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                    vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_cend(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_begin(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_begin(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cbegin(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_cbegin(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_end(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                        vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_end(g, u);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cend(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return out_cend(g, u);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return static_cast<vertex_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>>>(u.edge_end(g) - u.edge_begin(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_degree(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                            vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return static_cast<vertex_edge_size_t<daa_graph<VV, EV, GV, IndexT, A>>>(u.edge_end(g) - u.edge_begin(g));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(daa_graph<VV, EV, GV, IndexT, A>&                     g,
                           vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& key) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(daa_graph<VV, EV, GV, IndexT, A> const&               g,
                           vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& key) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

//
// API edge functions
//


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex(daa_graph<VV, EV, GV, IndexT, A>& g, edge_t<daa_graph<VV, EV, GV, IndexT, A>>& uv) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex(daa_graph<VV, EV, GV, IndexT, A> const&         g,
                         edge_t<daa_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex_key(daa_graph<VV, EV, GV, IndexT, A> const&         g,
                             edge_t<daa_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex(daa_graph<VV, EV, GV, IndexT, A>& g, edge_t<daa_graph<VV, EV, GV, IndexT, A>>& uv) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex(daa_graph<VV, EV, GV, IndexT, A> const&         g,
                          edge_t<daa_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex_key(daa_graph<VV, EV, GV, IndexT, A> const&         g,
                              edge_t<daa_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(daa_graph<VV, EV, GV, IndexT, A>&           g,
                      edge_t<daa_graph<VV, EV, GV, IndexT, A>>&   uv,
                      vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& source) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                      edge_t<daa_graph<VV, EV, GV, IndexT, A>> const&   uv,
                      vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& source) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                          edge_t<daa_graph<VV, EV, GV, IndexT, A>> const&   uv,
                          vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& source) noexcept
      -> vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(daa_graph<VV, EV, GV, IndexT, A> const&               g,
                          edge_t<daa_graph<VV, EV, GV, IndexT, A>> const&       uv,
                          vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& source_key) noexcept
      -> vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex_key(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_out_edge(daa_graph<VV, EV, GV, IndexT, A>&           g,
                             vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u,
                             vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& v) noexcept
      -> vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_out_edge(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                             vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u,
                             vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& v) noexcept
      -> const_vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_out_edge(daa_graph<VV, EV, GV, IndexT, A>&                     g,
                             vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                             vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_out_edge(daa_graph<VV, EV, GV, IndexT, A> const&               g,
                             vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                             vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> const_vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex_edge(daa_graph<VV, EV, GV, IndexT, A>&           g,
                                vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u,
                                vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& v) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex_edge(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                                vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u,
                                vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& v) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex_edge(daa_graph<VV, EV, GV, IndexT, A>&                     g,
                                vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                                vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, ukey, vkey);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex_edge(daa_graph<VV, EV, GV, IndexT, A> const&               g,
                                vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                                vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> const_vertex_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, ukey, vkey);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(daa_graph<VV, EV, GV, IndexT, A>&           g,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& u,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>>& v) noexcept
      -> vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(daa_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& u,
                         vertex_t<daa_graph<VV, EV, GV, IndexT, A>> const& v) noexcept
      -> const_vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(out_edges(g, u), [&g, &v](auto uv) { return &*out_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(daa_graph<VV, EV, GV, IndexT, A>&                     g,
                         vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(daa_graph<VV, EV, GV, IndexT, A> const&               g,
                         vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<daa_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> const_vertex_out_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return find_out_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}


//
// API graph functions
//

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_size(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_begin(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_begin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_cbegin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_end(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_end(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_cend(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_vertices(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_size_t<daa_graph<VV, EV, GV, IndexT, A>> n) {
  g.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(daa_graph<VV, EV, GV, IndexT, A>& g, vertex_size_t<daa_graph<VV, EV, GV, IndexT, A>> n) {
  g.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(daa_graph<VV, EV, GV, IndexT, A>&                       g,
                     vertex_size_t<daa_graph<VV, EV, GV, IndexT, A>>         n,
                     vertex_value_t<daa_graph<VV, EV, GV, IndexT, A>> const& val) {
  g.resize(n, val);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept -> edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_range_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cbegin(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(daa_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cend(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(daa_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_edges(daa_graph<VV, EV, GV, IndexT, A>& g, edge_size_t<daa_graph<VV, EV, GV, IndexT, A>> n) {
  g.reserve_edges(n);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void clear(daa_graph<VV, EV, GV, IndexT, A>& g) {
  g.clear();
}


} // namespace std::graph

#endif // COMPRESSED_ADJ_ARRAY_IMPL_HPP
