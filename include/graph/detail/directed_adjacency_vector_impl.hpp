//
//	Author: J. Phillip Ratzloff
//
#include <ranges>
#include <stdexcept>

#ifndef DIRECTED_ADJ_ARRAY_IMPL_HPP
#  define DIRECTED_ADJ_ARRAY_IMPL_HPP

namespace std {

///-------------------------------------------------------------------------------------
/// dav_edge
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_edge(vertex_key_type source_vertex_key,
                                                                    vertex_key_type target_vertex_key)
      : base_type(), source_vertex_(source_vertex_key), target_vertex_(target_vertex_key) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_edge(vertex_key_type        source_vertex_key,
                                                                    vertex_key_type        target_vertex_key,
                                                                    const edge_value_type& val)
      : base_type(val), source_vertex_(source_vertex_key), target_vertex_(target_vertex_key) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_edge(vertex_key_type   source_vertex_key,
                                                                    vertex_key_type   target_vertex_key,
                                                                    edge_value_type&& val)
      : base_type(move(val)), source_vertex_(source_vertex_key), target_vertex_(target_vertex_key) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::source_vertex(graph_type& g) {
  return g.vertices().begin() + source_vertex_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::source_vertex(const graph_type& g) const {
  return g.vertices().begin() + source_vertex_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_key_type
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::source_vertex_key() const noexcept {
  return source_vertex_;
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex(graph_type& g) {
  return g.vertices().begin() + target_vertex_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex(const graph_type& g) const {
  return g.vertices().begin() + target_vertex_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_key_type
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex_key() const noexcept {
  return target_vertex_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_key_type
dav_edge<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_key() const noexcept {
  return edge_key_type(source_vertex_, target_vertex_);
}


///-------------------------------------------------------------------------------------
/// dav_vertex
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_vertex(vertex_set& vertices, vertex_index_type index) {
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_vertex(vertex_set&              vertices,
                                                                        vertex_index_type        index,
                                                                        const vertex_value_type& val)
      : base_type(val) {}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_vertex(vertex_set&         vertices,
                                                                        vertex_index_type   index,
                                                                        vertex_value_type&& val)
      : base_type(move(val)) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::set_edge_begin(graph_type& g, edge_iterator uv) {
  if (first_edge_ == numeric_limits<edge_index_type>::max()) // not set yet?
    first_edge_ = static_cast<edge_index_type>(uv - g.edges().begin());
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::set_edge_begin(edge_index_type edge_index) {
  if (first_edge_ == numeric_limits<edge_index_type>::max()) // not set yet?
    first_edge_ = edge_index;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_index_type
dav_vertex<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_begin_index() const {
  return first_edge_;
}

///-------------------------------------------------------------------------------------
/// dav_const_vertex_vertex_iterator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::
      dav_const_vertex_vertex_iterator(graph_type& g, vertex_edge_iterator uv)
      : g_(&g), uv_(uv) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr
      typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
      dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex(
            graph_type const& g) const {
  return uv_->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_key_type
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex_key() const {
  return uv_->target_vertex_key();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::reference
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator*() const noexcept {
  return *uv_->target_vertex(*g_);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::pointer
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator->() const noexcept {
  return &**this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator++() noexcept {
  ++uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator++(int) noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator+=(
      const difference_type distance) noexcept {
  uv_ += distance;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator+(
      const difference_type distance) const noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  return tmp += distance;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator--(int) noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator-=(
      const difference_type distance) noexcept {
  uv_ -= distance;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator-(
      const difference_type distance) const noexcept {
  dav_const_vertex_vertex_iterator tmp(*this);
  return tmp -= distance;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator==(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ == rhs.uv_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator!=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator==(rhs);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator>(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ > rhs.uv_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator<=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator>(rhs);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator<(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ < rhs.uv_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator>=(
      const dav_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator<(rhs);
}


///-------------------------------------------------------------------------------------
/// dav_vertex_vertex_iterator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::dav_vertex_vertex_iterator(
      graph_type& g, vertex_edge_iterator uv)
      : base_t(g, uv) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::target_vertex(graph_type& g) {
  return uv_->target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::reference
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator*() const {
  return *const_cast<edge_type&>(*uv_).target_vertex(const_cast<graph_type&>(*g_));
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::pointer
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator->() const {
  return &**this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator++() {
  ++uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator++(int) {
  dav_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator+=(
      const difference_type distance) noexcept {
  uv_ += distance;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator+(
      const difference_type distance) const noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  return tmp += distance;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator--(int) noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>&
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator-=(
      const difference_type distance) noexcept {
  uv_ -= distance;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator-(
      const difference_type distance) const noexcept {
  dav_vertex_vertex_iterator tmp(*this);
  return tmp -= distance;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::reference
dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator[](
      const difference_type distance) const noexcept {
  return *uv_[distance].target_vertex(*g_);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator==(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator==(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator!=(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator!=(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator>(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator>(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator<=(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator<=(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator<(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator<(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr bool dav_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::operator>=(
      const dav_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator>=(rhs);
}


///-------------------------------------------------------------------------------------
/// directed_adjacency_vector
///

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      const allocator_type& alloc)
      : vertices_(alloc), edges_(alloc), alloc_(alloc) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      const graph_value_type& val, const allocator_type& alloc)
      : vertices_(alloc), edges_(alloc), base_type(val), alloc_(alloc) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      graph_value_type&& val, const allocator_type& alloc)
      : vertices_(alloc), edges_(alloc), base_type(move(val)), alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, template <typename E, typename A> class EContainer, typename Alloc>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> &&  vertex_value_extractor<VRng, VValueFnc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(const ERng&     erng,
                                            const VRng&     vrng,
                                            const EKeyFnc&  ekey_fnc,
                                            const EValueFnc& evalue_fnc,
                                            const VValueFnc& vvalue_fnc,
                                            const GV&       gv,
                                            const Alloc&    alloc)
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
  detail::reserve(vertices_, max_vtx_key + 1);
  if constexpr (!same_as<decltype(vvalue_fnc(*ranges::begin(vrng))), void>) {
    for (auto& vtx : vrng)
      create_vertex(vvalue_fnc(vtx));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (erng.size() > 0) {
    detail::reserve(edges_, erng.size());
    edge_key_type   tu_key = ekey_fnc(*ranges::begin(erng));
    vertex_iterator t      = find_vertex(tu_key.first);
    for (auto& edge_data : erng) {
      edge_key_type   uv_key = ekey_fnc(edge_data);
      vertex_iterator u      = find_vertex(uv_key.first);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(detail::make_subrange(t, u));

      edge_iterator uv;
      if constexpr (same_as<decltype(evalue_fnc(edge_data)), void>) {
        edges_.emplace_back(edge_type(uv_key.first, uv_key.second));
      } else {
        edges_.emplace_back(edge_type(uv_key.first, uv_key.second, evalue_fnc(edge_data)));
      }
      u->set_edge_begin(static_cast<edge_index_type>(edges_.size() - 1));
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(detail::make_subrange(t, vertices_.end()));
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> //
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      const ERng& erng, const EKeyFnc& ekey_fnc, const EValueFnc& evalue_fnc, const GV& gv, const Alloc& alloc)
      : directed_adjacency_vector(
              erng,
              vector<vertex_key_type>(),
              ekey_fnc,
              evalue_fnc,
              [](const vertex_key_type&) { return empty_value(); },
              gv,
              alloc) {}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      const initializer_list<tuple<vertex_key_type, vertex_key_type, edge_value_type>>& ilist, const Alloc& alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey, uv_val] = edge_data;
    max_vtx_key                      = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    detail::reserve(edges_, ilist.size());
    const auto& [tkey, uukey, tu_val] = *ranges::begin(ilist);

    vertex_iterator t = find_vertex(tkey);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = find_vertex(ukey);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(detail::make_subrange(t, u));

      edges_.emplace_back(edge_type(ukey, vkey, uv_val));
      u->set_edge_begin(static_cast<edge_index_type>(edges_.size() - 1));
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(detail::make_subrange(t, vertices_.end()));
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::directed_adjacency_vector(
      const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist, const Alloc& alloc)
      : base_type(), vertices_(alloc), edges_(alloc), alloc_(alloc) {

  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey] = edge_data;
    max_vtx_key              = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  if (ilist.size() > 0) {
    detail::reserve(edges_, ilist.size());
    const auto& [tkey, uukey] = *ranges::begin(ilist);

    vertex_iterator t = find_vertex(tkey);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey, uv_val] = edge_data;
      vertex_iterator u                = find_vertex(ukey);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      t = finalize_outward_edges(detail::make_subrange(t, u));

      edge_iterator uv;
      uv = create_edge(ukey, vkey);
      u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    finalize_outward_edges(detail::make_subrange(t, vertices_.end()));
  }
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::finalize_outward_edges(vertex_subrange vr) {
  for (auto& u : vr)
    u.set_edge_begin(*this, edges_.end());
  return ranges::end(vr);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_set&
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertices() {
  return vertices_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr const typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_set&
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertices() const {
  return vertices_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::reserve_vertices(vertex_size_type n) {
  detail::reserve(vertices_, n);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::resize_vertices(
      vertex_size_type n, const vertex_value_type& val) {
  vertices_.resize(n, val);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_vertex(vertex_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
template <class VV2>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_vertex(const VV2& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::find_vertex(vertex_key_type key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::find_vertex(vertex_key_type key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_edge_range
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::outward_edges(vertex_iterator u) {
  if (u == vertices_.end())
    return {edges_.end(), edges_.end(), 0};
  else if (u != --vertices_.end()) {
    auto v = u + 1;
    return {edges_.begin() + u->edge_begin_index(), edges_.begin() + v->edge_begin_index(),
            v->edge_begin_index() - u->edge_begin_index()};
  } else
    return {edges_.begin() + u->edge_begin_index(), edges_.end(), edges_.size() - u->edge_begin_index()};
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_edge_range
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::outward_edges(
      const_vertex_iterator u) const {
  if (u == vertices_.end())
    return {edges_.end(), edges_.end(), 0};
  else if (u != --vertices_.end()) {
    auto v = u + 1;
    return {edges_.begin() + u->edge_begin_index(), edges_.begin() + v->edge_begin_index(),
            v->edge_begin_index() - u->edge_begin_index()};
  } else
    return {edges_.begin() + u->edge_begin_index(), edges_.end(), edges_.size() - u->edge_begin_index()};
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr
      typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_outward_vertex_range
      directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::outward_vertices(vertex_iterator u) {
  if (u == vertices_.end())
    return {vertex_outward_vertex_iterator(*this, edges_.end()), vertex_outward_vertex_iterator(*this, edges_.end()),
            0};
  else if (u != --vertices_.end()) {
    auto v = u + 1;
    return {vertex_outward_vertex_iterator(*this, edges_.begin() + u->edge_begin_index()),
            vertex_outward_vertex_iterator(*this, edges_.begin() + v->edge_begin_index()),
            v->edge_begin_index() - u->edge_begin_index()};
  } else
    return {vertex_outward_vertex_iterator(*this, edges_.begin() + u->edge_begin_index()),
            vertex_outward_vertex_iterator(*this, edges_.end()), edges_.size() - u->edge_begin_index()};
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::
      const_vertex_outward_vertex_range
      directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::outward_vertices(
            const_vertex_iterator u) const {
  if (u == vertices_.end())
    return {const_vertex_outward_vertex_iterator(*this, edges_.end()),
            const_vertex_outward_vertex_iterator(*this, edges_.end())};
  else if (u != --vertices_.end())
    return {const_vertex_outward_vertex_iterator(*this, edges_.begin() + u->edge_begin_index()),
            const_vertex_outward_vertex_iterator(*this, edges_.begin() + (u + 1)->edge_begin_index())};
  else
    return {const_vertex_outward_vertex_iterator(*this, edges_.begin() + u->edge_begin_index()),
            const_vertex_outward_vertex_iterator(*this, edges_.end())};
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::allocator_type
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::allocator() const {
  return alloc_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::begin() {
  return vertices_.begin();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::begin() const {
  return vertices_.begin();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::end() {
  return vertices_.end();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::end() const {
  return vertices_.end();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::const_vertex_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::cend() const {
  return vertices_.cend();
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_set&
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edges() {
  return edges_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
constexpr const typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_set&
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edges() const {
  return edges_;
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::reserve_edges(edge_size_type n) {
  detail::reserve(edges_, n);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_edge(vertex_key_type from_key,
                                                                                        vertex_key_type to_key) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to));
  return edges_.begin() + (edges_.size() - 1);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_edge(vertex_key_type   from_key,
                                                                                        vertex_key_type   to_key,
                                                                                        edge_value_type&& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, move(val)));
  return edges_.begin() + static_cast<vertex_key_type>(edges_.size() - 1);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
template <class EV2>
typename directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::edge_iterator
directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::create_edge(vertex_key_type from_key,
                                                                                        vertex_key_type to_key,
                                                                                        const EV2&      val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);
  edges_.emplace_back(edge_type(vertices_, from, to, val));
  return edges_.begin() + (static_cast<ptrdiff_t>(edges_.size()) - 1);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::clear() {
  vertices_.clear();
  edges_.clear();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::swap(directed_adjacency_vector& rhs) {
  vertices_.swap(rhs.vertices_);
  edges_.swap(rhs.edges_);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          template <typename E, typename A>
          class EContainer,
          typename Alloc>
void directed_adjacency_vector<VV, EV, GV, KeyT, VContainer, EContainer, Alloc>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw domain_error("edges not ordered");
}


} // namespace std

#endif // DIRECTED_ADJ_ARRAY_IMPL_HPP
