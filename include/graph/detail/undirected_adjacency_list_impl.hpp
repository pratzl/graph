//
//	Author: J. Phillip Ratzloff
//

#ifndef UNDIRECTED_ADJ_LIST_IMPL_HPP
#define UNDIRECTED_ADJ_LIST_IMPL_HPP

#include <stdexcept>

namespace std::graph {

///-------------------------------------------------------------------------------------
/// ual_vertex_edge_list
///

// ual_vertex_edge_list::const_iterator
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::reference
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator*() const {
  return *edge_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::pointer
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator->() const {
  return edge_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator++() {
  advance();
  return *this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator++(int) {
  const_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator--() {
  retreat();
  return *this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator--(int) {
  const_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::advance() {
  vertex_edge_list_inward_link_type&  inward_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
  vertex_edge_list_outward_link_type& outward_link = *edge_;
  if (inward_link.vertex_key_ == vertex_key_) {
    edge_ = inward_link.next_;
  } else {
    assert(outward_link.vertex_key_ == vertex_key_);
    edge_ = outward_link.next_;
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::retreat() {
  if (edge_) {
    vertex_edge_list_inward_link_type&  inward_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
    vertex_edge_list_outward_link_type& outward_link = *edge_;
    if (inward_link.vertex_key_ == vertex_key_) {
      edge_ = inward_link.prev_;
    } else {
      assert(outward_link.vertex_key_ == vertex_key_);
      edge_ = outward_link.prev_;
    }
  } else {
    vertex_iterator u = graph_->find_vertex(vertex_key_);
    edge_             = &u->edge_back(*graph_);
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
bool ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator==(
      const const_iterator& rhs) const noexcept {
  return edge_ == rhs.edge_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
bool ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator::operator!=(
      const const_iterator& rhs) const noexcept {
  return !(*this == rhs);
}

// ual_vertex_edge_list::iterator
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::reference
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator*() const {
  return *this->edge_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::pointer
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator->() const {
  return this->edge_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator++() {
  this->advance();
  return *this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator++(int) {
  iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator--() {
  this->retreat();
  return *this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator::operator--(int) {
  iterator tmp(*this);
  --*this;
  return tmp;
}

// ual_vertex_edge_list
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::size_type
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::size() const noexcept {
  return size_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
bool ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::empty() const noexcept {
  return size_ == 0;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::front() {
  return *head_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
const typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::front() const {
  return *head_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::back() {
  return *tail_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
const typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::back() const {
  return *tail_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <typename ListT>
void ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::link_front(
      edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link) {
  using link_t = ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>;
  if (head_) {
    link_t& head_link = *static_cast<link_t*>(head_);
    uv_link.next_     = head_;
    head_link.prev_   = &uv;
    head_             = &uv;
  } else {
    head_ = tail_ = &uv;
  }
  ++size_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <typename ListT>
void ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::link_back(
      edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link) {
  if (tail_) {
    vertex_edge_list_inward_link_type&  tail_inward_link  = static_cast<vertex_edge_list_inward_link_type&>(*tail_);
    vertex_edge_list_outward_link_type& tail_outward_link = static_cast<vertex_edge_list_outward_link_type&>(*tail_);
    if (tail_inward_link.vertex_key_ == uv_link.vertex_key_) {
      uv_link.prev_          = tail_;
      tail_inward_link.next_ = &uv;
      tail_                  = &uv;
    } else {
      uv_link.prev_           = tail_;
      tail_outward_link.next_ = &uv;
      tail_                   = &uv;
    }
  } else {
    assert(!head_ && !tail_ && size_ == 0);
    head_ = tail_ = &uv;
  }
  ++size_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <typename ListT>
void ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::unlink(
      edge_type& uv, ual_vertex_edge_list_link<VV, EV, GV, KeyT, VContainer, Alloc, ListT>& uv_link) {

  if (uv_link.prev_) {
    vertex_edge_list_inward_link_type& prev_inward_link =
          static_cast<vertex_edge_list_inward_link_type&>(*uv_link.prev_);
    vertex_edge_list_outward_link_type& prev_outward_link =
          static_cast<vertex_edge_list_outward_link_type&>(*uv_link.prev_);
    if (prev_inward_link.vertex_key_ == uv_link.vertex_key_) {
      prev_inward_link.next_ = uv_link.next_;
    } else {
      assert(prev_outward_link.vertex_key_ == uv_link.vertex_key_);
      prev_outward_link.next_ = uv_link.next_;
    }
  }
  if (tail_ == &uv) {
    tail_ = uv_link.prev_;
  }

  if (uv_link.next_) {
    vertex_edge_list_inward_link_type& next_inward_link =
          static_cast<vertex_edge_list_inward_link_type&>(*uv_link.next_);
    vertex_edge_list_outward_link_type& next_outward_link =
          static_cast<vertex_edge_list_outward_link_type&>(*uv_link.next_);
    if (next_inward_link.vertex_key_ == uv_link.vertex_key_) {
      next_inward_link.prev_ = uv_link.prev_;
    } else {
      assert(next_outward_link.vertex_key_ == uv_link.vertex_key_);
      next_outward_link.prev_ = uv_link.prev_;
    }
  }
  if (head_ == &uv) {
    head_ = uv_link.next_;
  }

  uv_link.prev_ = uv_link.next_ = nullptr;
  --size_;

  if (size_ == 0)
    assert(head_ == nullptr && tail_ == nullptr);
  else if (size_ == 1)
    assert(head_ == tail_);
  else
    assert(head_ != tail_);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::begin(graph_type& g, vertex_key_type ukey) noexcept {
  return iterator(g, ukey, head_);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::begin(const graph_type& g,
                                                                 vertex_key_type   ukey) const noexcept {
  return const_iterator(g, ukey, head_);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::cbegin(const graph_type& g,
                                                                  vertex_key_type   ukey) const noexcept {
  return const_iterator(g, ukey, head_);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::end(graph_type& g, vertex_key_type ukey) noexcept {
  return iterator(g, ukey, nullptr);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::end(const graph_type& g,
                                                               vertex_key_type   ukey) const noexcept {
  return const_iterator(g, ukey, nullptr);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_iterator
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::cend(const graph_type& g,
                                                                vertex_key_type   ukey) const noexcept {
  return const_iterator(g, ukey, nullptr);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_range
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edges(graph_type& g, vertex_key_type ukey) noexcept {
  return {iterator(g, ukey, head_), iterator(g, ukey, nullptr), size_};
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_edge_range
ual_vertex_edge_list<VV, EV, GV, KeyT, VContainer, Alloc>::edges(const graph_type& g,
                                                                 vertex_key_type   ukey) const noexcept {
  return {const_iterator(g, ukey, head_), const_iterator(g, ukey, nullptr), size_};
}

///-------------------------------------------------------------------------------------
/// ual_edge
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type&     g,
                                                        vertex_key_type ukey,
                                                        vertex_key_type vkey) noexcept
      : base_type(), vertex_edge_list_inward_link_type(ukey), vertex_edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type&            g,
                                                        vertex_key_type        ukey,
                                                        vertex_key_type        vkey,
                                                        const edge_value_type& val) noexcept
      : base_type(val), vertex_edge_list_inward_link_type(ukey), vertex_edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type&       g,
                                                        vertex_key_type   ukey,
                                                        vertex_key_type   vkey,
                                                        edge_value_type&& val) noexcept
      : base_type(move(val)), vertex_edge_list_inward_link_type(ukey), vertex_edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type& g, vertex_iterator ui, vertex_iterator vi) noexcept
      : base_type()
      , vertex_edge_list_inward_link_type(vertex_key(g, ui))
      , vertex_edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type&            g,
                                                        vertex_iterator        ui,
                                                        vertex_iterator        vi,
                                                        const edge_value_type& val) noexcept
      : base_type(val)
      , vertex_edge_list_inward_link_type(vertex_key(g, ui))
      , vertex_edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::ual_edge(graph_type&       g,
                                                        vertex_iterator   ui,
                                                        vertex_iterator   vi,
                                                        edge_value_type&& val) noexcept
      : base_type(move(val))
      , vertex_edge_list_inward_link_type(vertex_key(g, ui))
      , vertex_edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::~ual_edge() noexcept {
  vertex_edge_list_outward_link_type& outward_link = *static_cast<vertex_edge_list_outward_link_type*>(this);
  assert(outward_link.prev() == nullptr && outward_link.next() == nullptr); // has edge been unlinked?

  vertex_edge_list_inward_link_type& inward_link = *static_cast<vertex_edge_list_inward_link_type*>(this);
  assert(inward_link.prev() == nullptr && inward_link.next() == nullptr); // has edge been unlinked?
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::link_front(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_front(*this, *static_cast<vertex_edge_list_inward_link_type*>(this));
  v.edges_.link_front(*this, *static_cast<vertex_edge_list_outward_link_type*>(this));
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::link_back(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_back(*this, *static_cast<vertex_edge_list_inward_link_type*>(this));
  v.edges_.link_back(*this, *static_cast<vertex_edge_list_outward_link_type*>(this));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::unlink(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.unlink(*this, *static_cast<vertex_edge_list_inward_link_type*>(this));
  v.edges_.unlink(*this, *static_cast<vertex_edge_list_outward_link_type*>(this));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::source_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + source_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::source_vertex(const graph_type& g) const noexcept {
  return static_cast<vertex_edge_list_inward_link_type const*>(this)->vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::source_vertex_key(const graph_type& g) const noexcept {
  return static_cast<vertex_edge_list_inward_link_type const*>(this)->vertex_key();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::target_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + target_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::target_vertex(const graph_type& g) const noexcept {
  return static_cast<vertex_edge_list_outward_link_type const*>(this)->vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::target_vertex_key(const graph_type& g) const noexcept {
  return static_cast<vertex_edge_list_outward_link_type const*>(this)->vertex_key();
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex(graph_type& g, const_vertex_iterator other) noexcept {
  return other != source_vertex(g) ? source_vertex(g) : target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex(const graph_type&     g,
                                                            const_vertex_iterator other) const noexcept {
  return other != source_vertex(g) ? source_vertex(g) : target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex(graph_type& g, vertex_key_type other_key) noexcept {
  return other_key != source_vertex_key(g) ? source_vertex(g) : target_vertex(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex(const graph_type& g,
                                                            vertex_key_type   other_key) const noexcept {
  return other_key != source_vertex_key(g) ? source_vertex(g) : target_vertex(g);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex_key(const graph_type&     g,
                                                                const_vertex_iterator other) const noexcept {
  return other != source_vertex(g) ? source_vertex_key(g) : target_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex_key(const graph_type& g,
                                                                vertex_key_type   other_key) const noexcept {
  return other_key != source_vertex_key(g) ? source_vertex_key(g) : target_vertex_key(g);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::edge_key_type
ual_edge<VV, EV, GV, KeyT, VContainer, Alloc>::edge_key(const graph_type& g) const noexcept {
  return unordered_pair(source_vertex_key(g), target_vertex_key(g));
}


///-------------------------------------------------------------------------------------
/// ual_vertex
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::ual_vertex(vertex_set& vertices, vertex_index index) {}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::ual_vertex(vertex_set&              vertices,
                                                            vertex_index             index,
                                                            const vertex_value_type& val)
      : base_type(val) {}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::ual_vertex(vertex_set&         vertices,
                                                            vertex_index        index,
                                                            vertex_value_type&& val) noexcept
      : base_type(move(val)) {}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_begin(graph_type& g, vertex_key_type ukey) noexcept {
  return edges_.begin(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_begin(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.begin(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_cbegin(const graph_type& g,
                                                              vertex_key_type   ukey) const noexcept {
  return edges_.cbegin(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::e_begin(graph_type const& g, vertex_key_type ukey) const {
  return const_cast<vertex_edge_list_type&>(edges_).begin(const_cast<graph_type&>(g), ukey);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_end(graph_type& g, vertex_key_type ukey) noexcept {
  return edges_.end(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_end(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.end(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_cend(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.cend(g, ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::e_end(graph_type const& g, vertex_key_type ukey) const {
  return const_cast<vertex_edge_list_type&>(edges_).end(const_cast<graph_type&>(g), ukey);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_range
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges(graph_type& g, vertex_key_type ukey) {
  return edges_.edges(g, ukey);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_edge_range
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges(const graph_type& g, vertex_key_type ukey) const {
  return edges_.edges(g, ukey);
}


#if 0
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key(const graph_type& g) const noexcept {
  return static_cast<vertex_key_type>(this - g.vertices().data());
}
#endif


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_front(graph_type& g) noexcept {
  return edges_.front(g, *this);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
const typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_front(const graph_type& g) const noexcept {
  return edges_.front(g, *this);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_back(graph_type& g) noexcept {
  return edges_.back();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
const typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_type&
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edge_back(const graph_type& g) const noexcept {
  return edges_.back();
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_size_type
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::edges_size() const {
  return edges_.size();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::erase_edge(graph_type& g, edge_type* uv) {
  vertex_type& u = g.vertices()[uv->source_vertex_key(g)];
  vertex_type& v = g.vertices()[uv->target_vertex_key(g)];
  uv->unlink(u, v);

  uv->~edge_type();
  g.edge_alloc_.deallocate(uv, 1);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::clear_edges(graph_type& g) {
  while (!edges_.empty()) {
    erase_edge(g, &edges_.front());
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::erase_edge(graph_type& g, vertex_edge_iterator uvi) {
  edge_type* uv = &*uvi;
  ++uvi;
  erase_edge(g, uv);
  return uvi;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::erase_edge(graph_type&          g,
                                                            vertex_edge_iterator first,
                                                            vertex_edge_iterator last) {
  while (first != last)
    first = erase_edge(g, first);
  return first;
}

#if 0
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(graph_type& g, vertex_type& v) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(graph_type& g, vertex_type& v, edge_value_type&& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, move(val));
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(graph_type& g, vertex_type& v, const edge_value_type& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, val);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}
#endif

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_vertex_size_type
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertices_size(const graph_type& g) const {
  return size(edges(g));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_vertex_range
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertices(graph_type& g, vertex_key_type ukey) {
  return {vertex_vertex_iterator(edges_.begin(g, ukey)), vertex_vertex_iterator(edges_.end(g, ukey)), edges_.size()};
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_vertex_range
ual_vertex<VV, EV, GV, KeyT, VContainer, Alloc>::vertices(const graph_type& g, vertex_key_type ukey) const {
  return {const_vertex_vertex_iterator(edges_.begin(g, ukey)), const_vertex_vertex_iterator(edges_.end(g, ukey)),
          edges_.size()};
}


///-------------------------------------------------------------------------------------
/// undirected_adjacency_list
///

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(const allocator_type& alloc)
      : vertices_(alloc), edge_alloc_(alloc) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(const graph_value_type& val,
                                                                                          const allocator_type&   alloc)
      : vertices_(alloc), base_type(val), edge_alloc_(alloc) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(graph_value_type&&    val,
                                                                                          const allocator_type& alloc)
      : vertices_(alloc), base_type(move(val)), edge_alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> &&  vertex_value_extractor<VRng, VValueFnc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(const ERng&      erng,
                                                                              const VRng&      vrng,
                                                                              const EKeyFnc&   ekey_fnc,
                                                                              const EValueFnc& evalue_fnc,
                                                                              const VValueFnc& vvalue_fnc,
                                                                              const GV&        gv,
                                                                              const Alloc&     alloc)
      : base_type(gv), vertices_(alloc), edge_alloc_(alloc)
// clang-format on
{
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = static_cast<vertex_key_type>(vrng.size() - 1);
  for (auto& e : erng) {
    edge_key_type edge_key = ekey_fnc(e);
    max_vtx_key            = max(max_vtx_key, max(edge_key.first, edge_key.second));
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
    edge_key_type   tu_key = ekey_fnc(*ranges::begin(erng)); // first edge
    vertex_key_type tkey   = tu_key.first;                   // last in-vertex key
    for (auto& edge_data : erng) {
      edge_key_type uv_key = ekey_fnc(edge_data);
      if (uv_key.first < tkey)
        throw_unordered_edges();

      vertex_edge_iterator uv;
      if constexpr (same_as<decltype(evalue_fnc(edge_data)), void>) {
        uv = create_edge(uv_key.first, uv_key.second);
      } else {
        uv = create_edge(uv_key.first, uv_key.second, evalue_fnc(edge_data));
      }
      tkey = uv_key.first;
    }
  }
}

// clang-format off
template <typename VV, typename EV, typename GV, integral KeyT, template <typename V, typename A> class VContainer, typename Alloc>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
  requires edge_value_extractor<ERng, EKeyFnc, EValueFnc> 
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(const ERng&      erng, 
                                                                              const EKeyFnc&   ekey_fnc, 
                                                                              const EValueFnc& evalue_fnc, 
                                                                              const GV&        gv, 
                                                                              const Alloc&     alloc)
      : undirected_adjacency_list(erng, vector<int>(), ekey_fnc, evalue_fnc, [](empty_value) 
{ return empty_value(); }, gv, alloc)
// clang-format on
{}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(
      const initializer_list<tuple<vertex_key_type, vertex_key_type, edge_value_type>>& ilist, const Alloc& alloc)
      : base_type(), vertices_(alloc), edge_alloc_(alloc) {
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey, uv_val] = edge_data;
    max_vtx_key                      = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (ilist.size() > 0) {
    auto [tkey, uukey, tu_val] = *ranges::begin(ilist);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey, uv_val] = edge_data;
      if (ukey < tkey)
        throw_unordered_edges();

      vertex_edge_iterator uv;
      uv   = create_edge(ukey, vkey, uv_val);
      tkey = ukey;
    }
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::undirected_adjacency_list(
      const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist, const Alloc& alloc)
      : base_type(), vertices_(alloc), edge_alloc_(alloc) {
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    const auto& [ukey, vkey] = edge_data;
    max_vtx_key              = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (ilist.size() > 0) {
    auto [tkey, uukey, tu_val] = *ranges::begin(ilist);
    for (auto& edge_data : ilist) {
      const auto& [ukey, vkey] = edge_data;
      if (ukey < tkey)
        throw_unordered_edges();

      vertex_edge_iterator uv;
      uv   = create_edge(ukey, vkey);
      tkey = ukey;
    }
  }
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::~undirected_adjacency_list() {
  clear(); // assure edges are deleted using edge_alloc_
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_allocator_type
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_allocator() const noexcept {
  return edge_alloc_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_set&
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertices() {
  return vertices_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr const typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_set&
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertices() const {
  return vertices_;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::reserve_vertices(vertex_size_type n) {
  detail::reserve(vertices_, n);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::resize_vertices(vertex_size_type         n,
                                                                                     const vertex_value_type& val) {
  vertices_.resize(n, val);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_vertex(vertex_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <class VV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_vertex(const VV2& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::find_vertex(vertex_key_type key) {
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
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::find_vertex(vertex_key_type key) const {
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
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::begin() {
  return vertices_.begin();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::begin() const {
  return vertices_.begin();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::end() {
  return vertices_.end();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::end() const {
  return vertices_.end();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::cend() const {
  return vertices_.cend();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_size_type
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::edges_size() const noexcept {
  return edges_size_;
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_key_type from_key,
                                                                            vertex_key_type to_key) {
  vertex_iterator ui = find_vertex(from_key);
  vertex_iterator vi = find_vertex(to_key);
  return create_edge(ui, vi);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_key_type   from_key,
                                                                            vertex_key_type   to_key,
                                                                            edge_value_type&& val) {
  vertex_iterator ui = vertices_.begin() + from_key;
  vertex_iterator vi = vertices_.begin() + to_key;
  return create_edge(ui, vi, move(val));
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_key_type from_key,
                                                                            vertex_key_type to_key,
                                                                            const EV2&      val) {
  vertex_iterator ui = vertices_.begin() + from_key;
  vertex_iterator vi = vertices_.begin() + to_key;
  return create_edge(ui, vi, val);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_iterator u, vertex_iterator v) {
  vertex_key_type ukey = u - vertices_.begin();
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v);
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_iterator   u,
                                                                            vertex_iterator   v,
                                                                            edge_value_type&& val) {
  vertex_key_type ukey = vertex_key(*this, u);
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v, move(val));
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::create_edge(vertex_iterator u,
                                                                            vertex_iterator v,
                                                                            const EV2&      val) {
  vertex_key_type ukey = vertex_key(*this, u);
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v, val);
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
typename undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::erase_edge(edge_iterator pos) {
  edge_type* uv = &*pos;
  ++pos;
  uv->~edge_type(); // unlinks from vertices
  edge_alloc_.deallocate(uv, 1);
  return pos;
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::clear() {
  // make sure edges are deallocated from edge_alloc_
  for (vertex_type& u : vertices_)
    u.clear_edges(*this);
  vertices_.clear(); // now we can clear the vertices
}


template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::swap(undirected_adjacency_list& rhs) {
  vertices_.swap(rhs.vertices_);
  swap(edges_size_, rhs.edges_size_);
  edge_alloc_.swap(rhs.edge_alloc_);
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
void undirected_adjacency_list<VV, EV, GV, KeyT, VContainer, Alloc>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw invalid_argument("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// ual_const_vertex_vertex_iterator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::ual_const_vertex_vertex_iterator(
      vertex_edge_iterator const& uv)
      : uv_(uv) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::graph_type&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::graph() noexcept {
  return uv_.graph();
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr const typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::graph_type&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::graph() const noexcept {
  return uv_.graph();
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::const_vertex_iterator
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex() const {
  return uv_->other_vertex(uv_.graph(), uv_.source_key());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_key_type
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex_key() const {
  return uv_->other_vertex_key(uv_.graph(), uv_.source_key());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::reference
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator*() const noexcept {
  return *uv_->other_vertex(uv_.graph(), uv_.source_key());
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::pointer
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator->() const noexcept {
  return &**this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator++() noexcept {
  ++uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator++(int) noexcept {
  ual_const_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator--(int) noexcept {
  ual_const_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr bool ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator==(
      const ual_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ == rhs.uv_;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr bool ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator!=(
      const ual_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator==(rhs);
}


///-------------------------------------------------------------------------------------
/// ual_vertex_vertex_iterator
///
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::ual_vertex_vertex_iterator(
      vertex_edge_iterator const& uv)
      : base_t(uv) {}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::vertex_iterator
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::other_vertex() {
  return uv_->other_vertex(uv_.graph(), uv_.source_key());
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::reference
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator*() const {
  return *const_cast<edge_type&>(*uv_).other_vertex(const_cast<graph_type&>(uv_.graph()), uv_.source_key());
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::pointer
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator->() const {
  return &**this;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>&
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator++() {
  ++uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator++(int) {
  ual_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>&
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator--(int) noexcept {
  ual_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr bool ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator==(
      const ual_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator==(rhs);
}
template <typename VV,
          typename EV,
          typename GV,
          integral KeyT,
          template <typename V, typename A>
          class VContainer,
          typename Alloc>
constexpr bool ual_vertex_vertex_iterator<VV, EV, GV, KeyT, VContainer, Alloc>::operator!=(
      const ual_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator!=(rhs);
}

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_IMPL_HPP
