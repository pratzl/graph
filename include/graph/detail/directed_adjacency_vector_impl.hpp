//
//	Author: J. Phillip Ratzloff
//
#include <ranges>
#include <stdexcept>

#ifndef DIRECTED_ADJ_ARRAY_IMPL_HPP
#  define DIRECTED_ADJ_ARRAY_IMPL_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// dav_edge
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_edge<VV, EV, GV, KeyT, A>::dav_edge(const vertex_set& vertices,
                                        vertex_iterator   inward_vertex,
                                        vertex_iterator   outward_vertex)
      : base_type()
      , inward_vertex_(static_cast<vertex_key_type>(inward_vertex - vertices.begin()))
      , outward_vertex_(static_cast<vertex_key_type>(outward_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_edge<VV, EV, GV, KeyT, A>::dav_edge(const vertex_set&           vertices,
                                        vertex_iterator             inward_vertex,
                                        vertex_iterator             outward_vertex,
                                        const edge_user_value_type& val)
      : base_type(val)
      , inward_vertex_(static_cast<vertex_key_type>(inward_vertex - vertices.begin()))
      , outward_vertex_(static_cast<vertex_key_type>(outward_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_edge<VV, EV, GV, KeyT, A>::dav_edge(const vertex_set&      vertices,
                                        vertex_iterator        inward_vertex,
                                        vertex_iterator        outward_vertex,
                                        edge_user_value_type&& val)
      : base_type(move(val))
      , inward_vertex_(static_cast<vertex_key_type>(inward_vertex - vertices.begin()))
      , outward_vertex_(static_cast<vertex_key_type>(outward_vertex - vertices.begin())) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::vertex_iterator dav_edge<VV, EV, GV, KeyT, A>::inward_vertex(graph_type& g) {
  return g.vertices().begin() + inward_vertex_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
dav_edge<VV, EV, GV, KeyT, A>::inward_vertex(const graph_type& g) const {
  return g.vertices().begin() + inward_vertex_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::vertex_key_type
dav_edge<VV, EV, GV, KeyT, A>::inward_vertex_key(const graph_type&) const {
  return inward_vertex_;
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::vertex_iterator dav_edge<VV, EV, GV, KeyT, A>::outward_vertex(graph_type& g) {
  return g.vertices().begin() + outward_vertex_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
dav_edge<VV, EV, GV, KeyT, A>::outward_vertex(const graph_type& g) const {
  return g.vertices().begin() + outward_vertex_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_edge<VV, EV, GV, KeyT, A>::vertex_key_type
dav_edge<VV, EV, GV, KeyT, A>::outward_vertex_key(const graph_type&) const {
  return outward_vertex_;
}


///-------------------------------------------------------------------------------------
/// dav_vertex
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_vertex<VV, EV, GV, KeyT, A>::dav_vertex(vertex_set& vertices, vertex_index_type index) {}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_vertex<VV, EV, GV, KeyT, A>::dav_vertex(vertex_set&                   vertices,
                                            vertex_index_type             index,
                                            const vertex_user_value_type& val)
      : base_type(val) {}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
dav_vertex<VV, EV, GV, KeyT, A>::dav_vertex(vertex_set& vertices, vertex_index_type index, vertex_user_value_type&& val)
      : base_type(move(val)) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void dav_vertex<VV, EV, GV, KeyT, A>::set_edge_begin(graph_type& g, edge_iterator uv) {
  if (first_edge_ == numeric_limits<edge_index_type>::max()) // not set yet?
    first_edge_ = static_cast<edge_index_type>(uv - g.edges().begin());
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_begin(graph_type& g) {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_begin(const graph_type& g) const {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_cbegin(const graph_type& g) const {
  return g.edges().begin() + first_edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::e_begin(const graph_type& g) const {
  return vertex_edge_iterator(const_cast<graph_type&>(g).edges().begin() + first_edge_);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_end(graph_type& g) {
  if (this < &g.vertices().back())
    return (this + 1)->edges_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_end(const graph_type& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->edges_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::edges_cend(const graph_type& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->edges_begin(g);
  else
    return g.edges().end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
dav_vertex<VV, EV, GV, KeyT, A>::e_end(const graph_type& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->e_begin(g);
  else
    return const_cast<graph_type&>(g).edges().end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::edge_size_type
dav_vertex<VV, EV, GV, KeyT, A>::edges_size(const graph_type& g) const {
  if (this < &g.vertices().back())
    return (this + 1)->first_edge_ - first_edge_;
  else
    return g.edges().size() - first_edge_;
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_begin(graph_type& g) {
  return vertex_vertex_iterator(g, edges_begin(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_begin(const graph_type& g) const {
  return const_vertex_vertex_iterator(const_cast<graph_type&>(g), e_begin(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_cbegin(const graph_type& g) const {
  return const_vertex_vertex_iterator(const_cast<graph_type&>(g), e_begin(g));
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_end(graph_type& g) {
  return vertex_vertex_iterator(g, edges_end(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_end(const graph_type& g) const {
  return const_vertex_vertex_iterator(const_cast<graph_type&>(g), e_end(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
dav_vertex<VV, EV, GV, KeyT, A>::vertices_cend(const graph_type& g) const {
  return const_vertex_vertex_iterator(const_cast<graph_type&>(g), e_end(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_size_type
dav_vertex<VV, EV, GV, KeyT, A>::vertices_size(const graph_type& g) const {
  return edges_size(g);
}

///-------------------------------------------------------------------------------------
/// dav_const_vertex_vertex_iterator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::dav_const_vertex_vertex_iterator(
      graph_type& g, vertex_edge_iterator uv)
      : g_(&g), uv_(uv) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::reference
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator*() const noexcept {
  return *uv_->outward_vertex(*g_);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::pointer
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator->() const noexcept {
  return &**this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++() noexcept {
  ++uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++(int) noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator+=(const difference_type distance) noexcept {
  uv_ += distance;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator+(const difference_type distance) const noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  return tmp += distance;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--(int) noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator-=(const difference_type distance) noexcept {
  uv_ -= distance;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator-(const difference_type distance) const noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  return tmp -= distance;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator==(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ == rhs.uv_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator!=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator==(rhs);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator>(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ > rhs.uv_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator<=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator>(rhs);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator<(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ < rhs.uv_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator>=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator<(rhs);
}


///-------------------------------------------------------------------------------------
/// dav_vertex_vertex_iterator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::dav_vertex_vertex_iterator(graph_type&          g,
                                                                                      vertex_edge_iterator uv)
      : base_t(g, uv) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::reference
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator*() const {
  return *const_cast<edge_type&>(*uv_).outward_vertex(const_cast<graph_type&>(*g_));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::pointer
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator->() const {
  return &**this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++() {
  ++uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++(int) {
  dav_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator+=(const difference_type distance) noexcept {
  uv_ += distance;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator+(const difference_type distance) const noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  return tmp += distance;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--(int) noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator-=(const difference_type distance) noexcept {
  uv_ -= distance;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator-(const difference_type distance) const noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  return tmp -= distance;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::reference
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator[](const difference_type distance) const noexcept {
  return *uv_[distance].outward_vertex(*g_);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator==(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator==(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator!=(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator!=(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator>(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator>(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator<=(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator<=(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator<(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator<(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator>=(const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator>=(rhs);
}


///-------------------------------------------------------------------------------------
/// directed_adjacency_vector
///

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(const allocator_type& alloc) noexcept
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(const graph_user_value_type& val,
                                                                          const allocator_type&        alloc)
      : vertices_(alloc), edges_(alloc), base_type(val), alloc_(alloc) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(graph_user_value_type&& val,
                                                                          const allocator_type&   alloc)
      : vertices_(alloc), edges_(alloc), base_type(move(val)), alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> && vertex_range_extractor<VRng, VValueFnc>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(const ERng&     erng,
                                            const VRng&     vrng,
                                            const EKeyFnc&  ekey_fnc,
                                            const EValueFnc& evalue_fnc,
                                            const VValueFnc& vvalue_fnc,
                                            const GV&       gv,
                                            const A&        alloc)
      : base_type(gv), vertices_(alloc), edges_(alloc), alloc_(alloc)
// clang-format on
{
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = static_cast<vertex_key_type>(vrng.size() - 1);
  for (auto& e : erng) {
    const edge_key_type& edge_key = ekey_fnc(e);
    max_vtx_key                   = max(max_vtx_key, max(edge_key.first, edge_key.second));
  }

  // add vertices
  vertices_.reserve(max_vtx_key + 1);
  if constexpr (!same_as<decltype(vvalue_fnc(*ranges::begin(vrng))), void>) {
    for (auto& vtx : vrng)
      create_vertex(vvalue_fnc(vtx));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (erng.size() > 0) {
    edges_.reserve(erng.size());
    edge_key_type   tu_key = ekey_fnc(*ranges::begin(erng));
    vertex_iterator t      = to_iterator(*this, vertices_[tu_key.first]);
    for (auto& edge_data : erng) {
      edge_key_type   uv_key = ekey_fnc(edge_data);
      vertex_iterator u      = to_iterator(*this, vertices_[uv_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(make_subrange2(t, u));

      edge_iterator uv;
      if constexpr (same_as<decltype(evalue_fnc(edge_data)), void>) {
        uv = create_edge(uv_key.first, uv_key.second);
      } else {
        uv = create_edge(uv_key.first, uv_key.second, evalue_fnc(edge_data));
      }
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(make_subrange2(t, vertices_.end()));
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> //
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(
      const ERng& erng, const EKeyFnc& ekey_fnc, const EValueFnc& evalue_fnc, const GV& gv, const A& alloc)
      : directed_adjacency_vector(
              erng,
              vector<vertex_key_type>(),
              ekey_fnc,
              evalue_fnc,
              [](const vertex_key_type&) { return empty_value(); },
              gv,
              alloc) {}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(
      const initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>>& ilist, const A& alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey, uv_val] = edge_data;
    max_vtx_key                      = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    edges_.reserve(ilist.size());
    const auto& [tkey, uukey, tu_val] = *ranges::begin(ilist);

    vertex_iterator t = to_iterator(*this, vertices_[tkey]);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = to_iterator(*this, vertices_[ukey]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(make_subrange2(t, u));

      edge_iterator uv;
      uv = create_edge(ukey, vkey, uv_val);
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(make_subrange2(t, vertices_.end()));
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
directed_adjacency_vector<VV, EV, GV, KeyT, A>::directed_adjacency_vector(
      const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist, const A& alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey] = edge_data;
    max_vtx_key              = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    edges_.reserve(ilist.size());
    const auto& [tkey, uukey] = *ranges::begin(ilist);

    vertex_iterator t = to_iterator(*this, vertices_[tkey]);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = to_iterator(*this, vertices_[ukey]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(make_subrange2(t, u));

      edge_iterator uv;
      uv = create_edge(ukey, vkey);
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(make_subrange2(t, vertices_.end()));
  }
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::finalize_outward_edges(vertex_range vr) {
  for (auto& u : vr)
    u.set_edge_begin(*this, edges_.end());
  return ranges::end(vr);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_set&
directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertices() {
  return vertices_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr const typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_set&
directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertices() const {
  return vertices_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::reserve_vertices(vertex_size_type n) {
  vertices_.reserve(n);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::resize_vertices(vertex_size_type              n,
                                                                     const vertex_user_value_type& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <class VV2>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_vertex(const VV2& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::find_vertex(vertex_key_type key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::find_vertex(vertex_key_type key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::begin() {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::begin() const {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::end() {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::end() const {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::cend() const {
  return vertices_.cend();
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::edge_set&
directed_adjacency_vector<VV, EV, GV, KeyT, A>::edges() {
  return edges_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr const typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::edge_set&
directed_adjacency_vector<VV, EV, GV, KeyT, A>::edges() const {
  return edges_;
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::reserve_edges(edge_size_type n) {
  edges_.reserve(n);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type from_key, vertex_key_type to_key) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to));
  return edges_.begin() + (edges_.size() - 1);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type        from_key,
                                                            vertex_key_type        to_key,
                                                            edge_user_value_type&& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, move(val)));
  return edges_.begin() + static_cast<vertex_key_type>(edges_.size() - 1);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <class EV2>
typename directed_adjacency_vector<VV, EV, GV, KeyT, A>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type from_key,
                                                            vertex_key_type to_key,
                                                            const EV2&      val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, val));
  return edges_.begin() + (static_cast<ptrdiff_t>(edges_.size()) - 1);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::clear() {
  vertices_.clear();
  edges_.clear();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::swap(directed_adjacency_vector& rhs) {
  vertices_.swap(rhs.vertices_);
  edges_.swap(rhs.edges_);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void directed_adjacency_vector<VV, EV, GV, KeyT, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw domain_error("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// directed_adjacency_vector graph API
///

// Helper Functions
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto to_iterator(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto to_iterator(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

//
// Uniform API: Graph functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void clear(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) {
  g.clear();
}

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                          const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return static_cast<vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>>(&u - g.vertices().data());
}

// (clear, create_vertex, erase_vertex not supported because the graph is immutable)

//
// Uniform API: Edge functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                        const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.edge_key(uv);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                        const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                        const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>(vertex_key(u), vertex_key(u));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&        g,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                        vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return edge_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>(ukey, vkey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                      edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>&   uv,
                      vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& source)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                      const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>&   uv,
                      const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& source)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                          const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>&   uv,
                          const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& source)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                          const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv,
                          vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>  source_key)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex_key(g);
}

// (create_edge & erase_edge not supported because the graph is immutable)


//
// Uniform API: Graph-Vertex range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.vertices().size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, A>&              g,
                           vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> key)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&        g,
                           vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> key)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}

// (reserve_vertices & resize_vertices not supported because the graph is immutable)

//
// Uniform API: Graph-Edge range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().cend();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.edges().size();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  using ssize_t = edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.edges().size());
}

// (erase_edge & reserve_edges not supported because the graph is immutable)

//
// Uniform API: Vertex-Edge range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                     vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_edges(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                     const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_edges(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                     vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_begin(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                     const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_begin(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                      const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_cbegin(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                   vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_end(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                   const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_end(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                    const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_cend(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_begin(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_begin(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                            const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_cbegin(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                         vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_end(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                         const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_end(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                          const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return outward_cend(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                          const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u) noexcept
      -> vertex_edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return static_cast<vertex_edge_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>>(u.edges_end(g) -
                                                                                         u.edges_begin(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u) noexcept
      -> vertex_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return static_cast<vertex_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>>(u.edges_end(g) -
                                                                                          u.edges_begin(g));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                         vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                         vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                         const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                         const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> const_vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&              g,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&        g,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> const_vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

// (create_edge, erase_edge and clear_edges aren't supported because the graph is immutable)


//
// Uniform API: Vertex-Vertex range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                        vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return {u.vertices_begin(g), u.vertices_end(g)};
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                        const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_vertex_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return {u.vertices_begin(g), u.vertices_end(g)};
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return edges_size(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                              const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return edges_ssize(g, u);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                              vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_begin(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                              const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_begin(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                               const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_cbegin(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                            vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_end(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                            const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_end(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.vertices_cend(g);
}


//
// Directed API (inward & outward)
//

//
// Directed API (outward): Vertex functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                              edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                              const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                                  const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.outward_vertex_key(g);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                             edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                             const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex_key(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&         g,
                                 const edge_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& uv)
      -> vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return uv.inward_vertex_key(g);
}


//
// Directed API (outward): Vertex-Edge functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_edges(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_outward_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return make_subrange2(u.edges_begin(g), v->edges_begin(g));
  } else {
    return make_subrange2(u.edges_begin(g), g.edges().end());
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_edges(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_outward_edge_range_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  if (&u < &g.vertices().back()) {
    auto v = to_iterator(g, u) + 1;
    return make_subrange2(u.edges_begin(g), v->edges_begin(g));
  } else {
    return make_subrange2(u.edges_begin(g), g.edges().end());
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                              const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_cbegin(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_end(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                           const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                            const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_cend(g);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                            const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u) noexcept
      -> vertex_outward_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return u.edges_size(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                             const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u) noexcept
      -> vertex_outward_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_outward_edge_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(u.edges_size(g));
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_outward_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                                 vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                                 vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_outward_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                                 const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                                 const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> const_vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return ranges::find_if(outward_edges(g, u), [&g, &v](auto uv) { return &*outward_vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_outward_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&              g,
                                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_outward_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&        g,
                                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                                 vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> const_vertex_outward_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                                vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                                vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, u, v);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&           g,
                                const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& u,
                                const vertex_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>>& v)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, u, v);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(directed_adjacency_vector<VV, EV, GV, KeyT, A>&              g,
                                vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, ukey, vkey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const directed_adjacency_vector<VV, EV, GV, KeyT, A>&        g,
                                vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> vkey)
      -> const_vertex_edge_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return find_outward_edge(g, ukey, vkey);
}


//
// API: common container functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto size(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return vertices_size(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto ssize(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_ssize_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return vertices_ssize(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto begin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cbegin(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto end(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto cend(const directed_adjacency_vector<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<directed_adjacency_vector<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}


//
// API graph functions
//


} // namespace std::graph

#endif // DIRECTED_ADJ_ARRAY_IMPL_HPP
