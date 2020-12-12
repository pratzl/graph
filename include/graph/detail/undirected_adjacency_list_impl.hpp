//
//	Author: J. Phillip Ratzloff
//

#ifndef UNDIRECTED_ADJ_LIST_IMPL_HPP
#define UNDIRECTED_ADJ_LIST_IMPL_HPP

#include <stdexcept>

namespace std::graph {

///-------------------------------------------------------------------------------------
/// ual_edge_list
///

// ual_edge_list::const_iterator
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::reference
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator*() const {
  return *edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::pointer
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator->() const {
  return edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator&
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator++() {
  advance();
  return *this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator++(int) {
  const_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator&
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator--() {
  retreat();
  return *this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator--(int) {
  const_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::advance() {
  edge_list_inward_link_type&  inward_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
  edge_list_outward_link_type& outward_link = *edge_;
  if (inward_link.vertex_key_ == vertex_key_) {
    edge_ = inward_link.next_;
  } else {
    assert(outward_link.vertex_key_ == vertex_key_);
    edge_ = outward_link.next_;
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::retreat() {
  if (edge_) {
    edge_list_inward_link_type&  inward_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
    edge_list_outward_link_type& outward_link = *edge_;
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
bool ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator==(const const_iterator& rhs) const noexcept {
  return edge_ == rhs.edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
bool ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator::operator!=(const const_iterator& rhs) const noexcept {
  return !(*this == rhs);
}

// ual_edge_list::iterator
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::reference ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator*() const {
  return *this->edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::pointer ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator->() const {
  return this->edge_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator& ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator++() {
  this->advance();
  return *this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator++(int) {
  iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator& ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator--() {
  this->retreat();
  return *this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator ual_edge_list<VV, EV, GV, KeyT, A>::iterator::operator--(int) {
  iterator tmp(*this);
  --*this;
  return tmp;
}

// ual_edge_list
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::size_type ual_edge_list<VV, EV, GV, KeyT, A>::size() const noexcept {
  return size_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
bool ual_edge_list<VV, EV, GV, KeyT, A>::empty() const noexcept {
  return size_ == 0;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::edge_type& ual_edge_list<VV, EV, GV, KeyT, A>::front() {
  return *head_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
const typename ual_edge_list<VV, EV, GV, KeyT, A>::edge_type& ual_edge_list<VV, EV, GV, KeyT, A>::front() const {
  return *head_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::edge_type& ual_edge_list<VV, EV, GV, KeyT, A>::back() {
  return *tail_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
const typename ual_edge_list<VV, EV, GV, KeyT, A>::edge_type& ual_edge_list<VV, EV, GV, KeyT, A>::back() const {
  return *tail_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, KeyT, A>::link_front(edge_type&                                      uv,
                                                    ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link) {
  using link_t = ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>;
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, KeyT, A>::link_back(edge_type&                                      uv,
                                                   ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link) {
  if (tail_) {
    edge_list_inward_link_type&  tail_inward_link  = static_cast<edge_list_inward_link_type&>(*tail_);
    edge_list_outward_link_type& tail_outward_link = static_cast<edge_list_outward_link_type&>(*tail_);
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, KeyT, A>::unlink(edge_type&                                      uv,
                                                ual_edge_list_link<VV, EV, GV, KeyT, A, ListT>& uv_link) {

  if (uv_link.prev_) {
    edge_list_inward_link_type&  prev_inward_link  = static_cast<edge_list_inward_link_type&>(*uv_link.prev_);
    edge_list_outward_link_type& prev_outward_link = static_cast<edge_list_outward_link_type&>(*uv_link.prev_);
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
    edge_list_inward_link_type&  next_inward_link  = static_cast<edge_list_inward_link_type&>(*uv_link.next_);
    edge_list_outward_link_type& next_outward_link = static_cast<edge_list_outward_link_type&>(*uv_link.next_);
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator
ual_edge_list<VV, EV, GV, KeyT, A>::begin(graph_type& g, vertex_key_type ukey) noexcept {
  return iterator(g, ukey, head_);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::begin(const graph_type& g, vertex_key_type ukey) const noexcept {
  return const_iterator(g, ukey, head_);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::cbegin(const graph_type& g, vertex_key_type ukey) const noexcept {
  return const_iterator(g, ukey, head_);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::iterator
ual_edge_list<VV, EV, GV, KeyT, A>::end(graph_type& g, vertex_key_type ukey) noexcept {
  return iterator(g, ukey, nullptr);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::end(const graph_type& g, vertex_key_type ukey) const noexcept {
  return const_iterator(g, ukey, nullptr);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge_list<VV, EV, GV, KeyT, A>::const_iterator
ual_edge_list<VV, EV, GV, KeyT, A>::cend(const graph_type& g, vertex_key_type ukey) const noexcept {
  return const_iterator(g, ukey, nullptr);
}


///-------------------------------------------------------------------------------------
/// ual_edge
///
#if 1
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type& g, vertex_key_type ukey, vertex_key_type vkey) noexcept
      : base_type(), edge_list_inward_link_type(ukey), edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&                 g,
                                        vertex_key_type             ukey,
                                        vertex_key_type             vkey,
                                        const edge_user_value_type& val) noexcept
      : base_type(val), edge_list_inward_link_type(ukey), edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&            g,
                                        vertex_key_type        ukey,
                                        vertex_key_type        vkey,
                                        edge_user_value_type&& val) noexcept
      : base_type(move(val)), edge_list_inward_link_type(ukey), edge_list_outward_link_type(vkey) {
  link_back(g.vertices()[ukey], g.vertices()[vkey]);
}
#else
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type& g, vertex_type& u, vertex_type& v) noexcept
      : base_type()
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().vertices.data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&                 g,
                                        vertex_type&                u,
                                        vertex_type&                v,
                                        const edge_user_value_type& val) noexcept
      : base_type(val)
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&            g,
                                        vertex_type&           u,
                                        vertex_type&           v,
                                        edge_user_value_type&& val) noexcept
      : base_type(move(val))
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
  link_back(u, v);
}
#endif

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type& g, vertex_iterator ui, vertex_iterator vi) noexcept
      : base_type(), edge_list_inward_link_type(vertex_key(g, ui)), edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&                 g,
                                        vertex_iterator             ui,
                                        vertex_iterator             vi,
                                        const edge_user_value_type& val) noexcept
      : base_type(val), edge_list_inward_link_type(vertex_key(g, ui)), edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::ual_edge(graph_type&            g,
                                        vertex_iterator        ui,
                                        vertex_iterator        vi,
                                        edge_user_value_type&& val) noexcept
      : base_type(move(val))
      , edge_list_inward_link_type(vertex_key(g, ui))
      , edge_list_outward_link_type(vertex_key(g, vi)) {
  link_back(*ui, *vi);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_edge<VV, EV, GV, KeyT, A>::~ual_edge() noexcept {
  edge_list_outward_link_type& outward_link = *static_cast<edge_list_outward_link_type*>(this);
  assert(outward_link.prev() == nullptr && outward_link.next() == nullptr); // has edge been unlinked?

  edge_list_inward_link_type& inward_link = *static_cast<edge_list_inward_link_type*>(this);
  assert(inward_link.prev() == nullptr && inward_link.next() == nullptr); // has edge been unlinked?
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_edge<VV, EV, GV, KeyT, A>::link_front(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_front(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.link_front(*this, *static_cast<edge_list_outward_link_type*>(this));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_edge<VV, EV, GV, KeyT, A>::link_back(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_back(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.link_back(*this, *static_cast<edge_list_outward_link_type*>(this));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_edge<VV, EV, GV, KeyT, A>::unlink(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.unlink(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.unlink(*this, *static_cast<edge_list_outward_link_type*>(this));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::inward_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + inward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::inward_vertex(const graph_type& g) const noexcept {
  return static_cast<edge_list_inward_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, A>::inward_vertex_key(const graph_type& g) const noexcept {
  return static_cast<edge_list_inward_link_type const*>(this)->vertex_key();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::outward_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::outward_vertex(const graph_type& g) const noexcept {
  return static_cast<edge_list_outward_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, A>::outward_vertex_key(const graph_type& g) const noexcept {
  return static_cast<edge_list_outward_link_type const*>(this)->vertex_key();
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::other_vertex(graph_type& g, const_vertex_iterator other) noexcept {
  return other != inward_vertex(g) ? inward_vertex(g) : outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::other_vertex(const graph_type& g, const_vertex_iterator other) const noexcept {
  return other != inward_vertex(g) ? inward_vertex(g) : outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::other_vertex(graph_type& g, vertex_key_type other_key) noexcept {
  return other_key != inward_vertex_key(g) ? inward_vertex(g) : outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, KeyT, A>::other_vertex(const graph_type& g, vertex_key_type other_key) const noexcept {
  return other_key != inward_vertex_key(g) ? inward_vertex(g) : outward_vertex(g);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, A>::other_vertex_key(const graph_type& g, const_vertex_iterator other) const noexcept {
  return other != inward_vertex(g) ? inward_vertex_key(g) : outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::vertex_key_type
ual_edge<VV, EV, GV, KeyT, A>::other_vertex_key(const graph_type& g, vertex_key_type other_key) const noexcept {
  return other_key != inward_vertex_key(g) ? inward_vertex_key(g) : outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_edge<VV, EV, GV, KeyT, A>::edge_key_type
ual_edge<VV, EV, GV, KeyT, A>::edge_key(const graph_type& g) const noexcept {
  return unordered_pair(inward_vertex_key(g), outward_vertex_key(g));
}


///-------------------------------------------------------------------------------------
/// ual_vertex
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_vertex<VV, EV, GV, KeyT, A>::ual_vertex(vertex_set& vertices, vertex_index index) {}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_vertex<VV, EV, GV, KeyT, A>::ual_vertex(vertex_set& vertices, vertex_index index, const vertex_user_value_type& val)
      : base_type(val) {}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
ual_vertex<VV, EV, GV, KeyT, A>::ual_vertex(vertex_set&              vertices,
                                            vertex_index             index,
                                            vertex_user_value_type&& val) noexcept
      : base_type(move(val)) {}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_begin(graph_type& g, vertex_key_type ukey) noexcept {
  return edges_.begin(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_begin(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.begin(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_cbegin(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.cbegin(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::e_begin(graph_type const& g, vertex_key_type ukey) const {
  return const_cast<edge_list_type&>(edges_).begin(const_cast<graph_type&>(g), ukey);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_end(graph_type& g, vertex_key_type ukey) noexcept {
  return edges_.end(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_end(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.end(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::edges_cend(const graph_type& g, vertex_key_type ukey) const noexcept {
  return edges_.cend(g, ukey);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::e_end(graph_type const& g, vertex_key_type ukey) const {
  return const_cast<edge_list_type&>(edges_).end(const_cast<graph_type&>(g), ukey);
}

#if 0
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_key_type
ual_vertex<VV, EV, GV, KeyT, A>::vertex_key(const graph_type& g) const noexcept {
  return static_cast<vertex_key_type>(this - g.vertices().data());
}
#endif


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::edge_type&
ual_vertex<VV, EV, GV, KeyT, A>::edge_front(graph_type& g) noexcept {
  return edges_.front(g, *this);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
const typename ual_vertex<VV, EV, GV, KeyT, A>::edge_type&
ual_vertex<VV, EV, GV, KeyT, A>::edge_front(const graph_type& g) const noexcept {
  return edges_.front(g, *this);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::edge_type&
ual_vertex<VV, EV, GV, KeyT, A>::edge_back(graph_type& g) noexcept {
  return edges_.back();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
const typename ual_vertex<VV, EV, GV, KeyT, A>::edge_type&
ual_vertex<VV, EV, GV, KeyT, A>::edge_back(const graph_type& g) const noexcept {
  return edges_.back();
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_size_type ual_vertex<VV, EV, GV, KeyT, A>::edges_size() const {
  return edges_.size();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_vertex<VV, EV, GV, KeyT, A>::erase_edge(graph_type& g, edge_type* uv) {
  vertex_type& u = g.vertices()[uv->inward_vertex_key(g)];
  vertex_type& v = g.vertices()[uv->outward_vertex_key(g)];
  uv->unlink(u, v);

  uv->~edge_type();
  g.edge_alloc_.deallocate(uv, 1);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void ual_vertex<VV, EV, GV, KeyT, A>::clear_edges(graph_type& g) {
  while (!edges_.empty()) {
    erase_edge(g, &edges_.front());
  }
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::erase_edge(graph_type& g, vertex_edge_iterator uvi) {
  edge_type* uv = &*uvi;
  ++uvi;
  erase_edge(g, uv);
  return uvi;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::erase_edge(graph_type& g, vertex_edge_iterator first, vertex_edge_iterator last) {
  while (first != last)
    first = erase_edge(g, first);
  return first;
}

#if 0
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::create_edge(graph_type& g, vertex_type& v) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::create_edge(graph_type& g, vertex_type& v, edge_user_value_type&& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, move(val));
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, KeyT, A>::create_edge(graph_type& g, vertex_type& v, const edge_user_value_type& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, val);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}
#endif

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_begin(graph_type& g, vertex_key_type ukey) {
  return vertex_vertex_iterator(edges_begin(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_begin(const graph_type& g, vertex_key_type ukey) const {
  return const_vertex_vertex_iterator(e_begin(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_cbegin(const graph_type& g, vertex_key_type ukey) const {
  return const_vertex_vertex_iterator(e_begin(g, ukey));
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_end(graph_type& g, vertex_key_type ukey) {
  return vertex_vertex_iterator(edges_end(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_end(const graph_type& g, vertex_key_type ukey) const {
  return const_vertex_vertex_iterator(e_end(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::const_vertex_vertex_iterator
ual_vertex<VV, EV, GV, KeyT, A>::vertices_cend(const graph_type& g, vertex_key_type ukey) const {
  return const_vertex_vertex_iterator(e_end(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename ual_vertex<VV, EV, GV, KeyT, A>::vertex_vertex_size_type
ual_vertex<VV, EV, GV, KeyT, A>::vertices_size(const graph_type& g) const {
  return edges_size(g);
}


///-------------------------------------------------------------------------------------
/// undirected_adjacency_list
///

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(const allocator_type& alloc) noexcept
      : vertices_(alloc), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(const graph_user_value_type& val,
                                                                          const allocator_type&        alloc)
      : vertices_(alloc), base_type(val), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(graph_user_value_type&& val,
                                                                          const allocator_type&   alloc)
      : vertices_(alloc), base_type(move(val)), edge_alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> && vertex_range_extractor<VRng, VValueFnc>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(const ERng&      erng,
                                                                            const VRng&      vrng,
                                                                            const EKeyFnc&   ekey_fnc,
                                                                            const EValueFnc& evalue_fnc,
                                                                            const VValueFnc& vvalue_fnc,
                                                                            const GV&        gv,
                                                                            const A&         alloc)
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
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
  requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> 
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(const ERng&      erng, 
                                                                            const EKeyFnc&   ekey_fnc, 
                                                                            const EValueFnc& evalue_fnc, 
                                                                            const GV&        gv, 
                                                                            const A&         alloc)
      : undirected_adjacency_list(erng, vector<int>(), ekey_fnc, evalue_fnc, [](empty_value) 
{ return empty_value(); }, gv, alloc)
// clang-format on
{}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(
      const initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>>& ilist, const A& alloc)
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::undirected_adjacency_list(
      const initializer_list<tuple<vertex_key_type, vertex_key_type>>& ilist, const A& alloc)
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

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
undirected_adjacency_list<VV, EV, GV, KeyT, A>::~undirected_adjacency_list() {
  clear(); // assure edges are deleted using edge_alloc_
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::edge_allocator_type
undirected_adjacency_list<VV, EV, GV, KeyT, A>::edge_allocator() const noexcept {
  return edge_alloc_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_set&
undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertices() {
  return vertices_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr const typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_set&
undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertices() const {
  return vertices_;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::reserve_vertices(vertex_size_type n) {
  detail::reserve(vertices_, n);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::resize_vertices(vertex_size_type              n,
                                                                     const vertex_user_value_type& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <class VV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_vertex(const VV2& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::find_vertex(vertex_key_type key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::find_vertex(vertex_key_type key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::begin() {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::begin() const {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::end() {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::end() const {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::cend() const {
  return vertices_.cend();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::edge_size_type
undirected_adjacency_list<VV, EV, GV, KeyT, A>::edges_size() const noexcept {
  return edges_size_;
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type from_key, vertex_key_type to_key) {
  vertex_iterator ui = find_vertex(from_key);
  vertex_iterator vi = find_vertex(to_key);
  return create_edge(ui, vi);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type        from_key,
                                                            vertex_key_type        to_key,
                                                            edge_user_value_type&& val) {
  vertex_iterator ui = vertices_.begin() + from_key;
  vertex_iterator vi = vertices_.begin() + to_key;
  return create_edge(ui, vi, move(val));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_key_type from_key,
                                                            vertex_key_type to_key,
                                                            const EV2&      val) {
  vertex_iterator ui = vertices_.begin() + from_key;
  vertex_iterator vi = vertices_.begin() + to_key;
  return create_edge(ui, vi, val);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_iterator u, vertex_iterator v) {
  vertex_key_type ukey = u - vertices_.begin();
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v);
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_iterator        u,
                                                            vertex_iterator        v,
                                                            edge_user_value_type&& val) {
  vertex_key_type ukey = vertex_key(*this, u);
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v, move(val));
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::vertex_edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::create_edge(vertex_iterator u, vertex_iterator v, const EV2& val) {
  vertex_key_type ukey = vertex_key(*this, u);
  edge_type*      uv   = edge_alloc_.allocate(1);
  new (uv) edge_type(*this, u, v, val);
  ++edges_size_;
  return vertex_edge_iterator(*this, ukey, uv);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
typename undirected_adjacency_list<VV, EV, GV, KeyT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, KeyT, A>::erase_edge(edge_iterator pos) {
  edge_type* uv = &*pos;
  ++pos;
  uv->~edge_type(); // unlinks from vertices
  edge_alloc_.deallocate(uv, 1);
  return pos;
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::clear() {
  // make sure edges are deallocated from edge_alloc_
  for (vertex_type& u : vertices_)
    u.clear_edges(*this);
  vertices_.clear(); // now we can clear the vertices
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::swap(undirected_adjacency_list& rhs) {
  vertices_.swap(rhs.vertices_);
  swap(edges_size_, rhs.edges_size_);
  edge_alloc_.swap(rhs.edge_alloc_);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void undirected_adjacency_list<VV, EV, GV, KeyT, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw invalid_argument("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// ual_const_vertex_vertex_iterator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::ual_const_vertex_vertex_iterator(
      vertex_edge_iterator const& uv)
      : uv_(uv) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::graph_type&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::graph() noexcept {
  return uv_.graph();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr const typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::graph_type&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::graph() const noexcept {
  return uv_.graph();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::const_vertex_iterator
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::other_vertex() const {
  return uv_->other_vertex(uv_.graph(), uv_.source_key());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::vertex_key_type
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::other_vertex_key() const {
  return uv_->other_vertex_key(uv_.graph(), uv_.source_key());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::reference
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator*() const noexcept {
  return *uv_->other_vertex(uv_.graph(), uv_.source_key());
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::pointer
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator->() const noexcept {
  return &**this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++() noexcept {
  ++uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++(int) noexcept {
  ual_const_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--(int) noexcept {
  ual_const_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator==(
      const ual_const_vertex_vertex_iterator& rhs) const noexcept {
  return uv_ == rhs.uv_;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool ual_const_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator!=(
      const ual_const_vertex_vertex_iterator& rhs) const noexcept {
  return !operator==(rhs);
}


///-------------------------------------------------------------------------------------
/// ual_vertex_vertex_iterator
///
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::ual_vertex_vertex_iterator(vertex_edge_iterator const& uv)
      : base_t(uv) {}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::vertex_iterator
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::other_vertex() {
  return uv_->other_vertex(uv_.graph(), uv_.source_key());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::reference
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator*() const {
  return *const_cast<edge_type&>(*uv_).other_vertex(const_cast<graph_type&>(uv_.graph()), uv_.source_key());
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr typename ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::pointer
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator->() const {
  return &**this;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++() {
  ++uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator++(int) {
  ual_vertex_vertex_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>&
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--() noexcept {
  --uv_;
  return *this;
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator--(int) noexcept {
  ual_vertex_vertex_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator==(const ual_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator==(rhs);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr bool
ual_vertex_vertex_iterator<VV, EV, GV, KeyT, A>::operator!=(const ual_vertex_vertex_iterator& rhs) const noexcept {
  return base_t::operator!=(rhs);
}

///-------------------------------------------------------------------------------------
/// undirected_adjacency_list graph API
///

// Helper functions

//
// Uniform API: Common functions (accepts graph, vertex and edge)
//

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return static_cast<vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>>(u - g.vertices().begin());
}

// (clear, create_vertex, erase_vertex not supported because the graph is immutable)

//
// Uniform API: Edge functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&         g,
                        const edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& uv)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv.edge_key(g);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edge_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edge_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>(vertex_key(g, u), vertex_key(g, v));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                         g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>         uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>   uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                      edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>    source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                      const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>          source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>   uv,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                          const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>          source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source_key);
}

// (create_edge & erase_edge)

//
// Uniform API: Graph-Vertex range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return make_subrange2(g.vertices());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.vertices().size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_begin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cbegin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_end(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_cend(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.vertices().cend();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.find_vertex(key);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void reserve_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&               g,
                      vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> n) {
  g.reserve_vertices(n);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&               g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> n) {
  g.resize(n);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                       g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>         n,
                     const vertex_value_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& val) {
  g.resize(n, val);
}


//
// Uniform API: Graph-Edge range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cbegin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().cbegin();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cend(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges().cend();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return g.edges_size();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) noexcept
      -> edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(g.edges().size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g);
  for (auto uv = e.begin(); uv != e.end(); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return e.end();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}


//
// Uniform API: Vertex-Edge range functions
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                         g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>  uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>      source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>           source_key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                      const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                      vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>                 source_key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>      source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>                 source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->other_vertex_key(g, source_key);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                     vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto ukey = vertex_key(g, u);
  return make_subrange2(u->edges_begin(g, ukey), u->edges_end(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                     const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto ukey = vertex_key(g, u);
  return make_subrange2(u->edges_begin(g, ukey), u->edges_end(g, ukey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_begin(g, vertex_key(g, u));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_begin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_begin(g, vertex_key(g, u));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cbegin(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                            const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_cbegin(g, vertex_key(g, u));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_end(g, vertex_key(g, u));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_end(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                         const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_end(g, vertex_key(g, u));
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_cend(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_cend(g, vertex_key(g, u));
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                          const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_edge_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u->edges_size();
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto edges_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u) noexcept
      -> vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  using ssize_t = vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>;
  return static_cast<ssize_t>(u->edges_size());
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u,
                                vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> v)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                    g,
                                const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& u,
                                const vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>& v)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  auto e = edges(g, u);
  for (auto uv = begin(e); uv != end(e); ++uv)
    if (vertex(g, uv, u) == v)
      return uv;
  return end(e);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&              g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto find_vertex_edge(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&        g,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> ukey,
                                vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> vkey)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return find_vertex_edge(find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto erase_edge(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  edge_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>>* uv_ptr = &*uv;
  ++uv;
  delete uv_ptr;
  return uv;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto erase_edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                     g,
                           vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv_rng)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {

  vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv = ranges::begin(uv_rng);
  while (uv != ranges::end(uv_rng))
    uv = erase_edge(g, uv);

  return uv;
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr void clear_edges(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                           vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u) {
  u->clear_edges(g);
}


//
// Uniform API: Vertex-Vertex range functions
//
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                          g,
                          const_vertex_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return u.other_vertex_key();
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return {u->vertices_begin(g, vertex_key(g, u)), u->vertices_end(g, vertex_key(g, u))};
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                        const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> const_vertex_vertex_range_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return {u->vertices_begin(g, vertex_key(g, u)), u->vertices_end(g, vertex_key(g, u))};
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_size(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                             const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_size_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edges_size(g, u);
}
template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto vertices_ssize(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                   g,
                              const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> u)
      -> vertex_vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return edges_ssize(g, u);
}


//
// API edge functions
//

//
// Directed API (inward & outward)
//

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                              edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                              const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                              vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                              const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                                  const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto outward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                                  const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->outward_vertex_key(g);
}


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                             edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                             const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                             vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                             const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                 g,
                                 const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, integral KeyT, typename A>
constexpr auto inward_vertex_key(const undirected_adjacency_list<VV, EV, GV, KeyT, A>&                        g,
                                 const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, KeyT, A>> {
  return uv->inward_vertex_key(g);
}


//
// Directed API (outward): Vertex-Edge functions
//

// todo: outward_vertices


#if 0
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g, vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                      const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(const undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                          const_vertex_edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv->outward_vertex_key(g);
}
#endif


//
// API graph functions
//


template <typename VV, typename EV, typename GV, integral KeyT, typename A>
void clear(undirected_adjacency_list<VV, EV, GV, KeyT, A>& g) {
  g.clear();
}


//
// API: common container functions
//

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_IMPL_HPP
