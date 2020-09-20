//
//	Author: J. Phillip Ratzloff
//
#include <range/v3/algorithm/find_if.hpp>

#ifndef UNDIRECTED_ADJ_LIST_IMPL_HPP
#  define UNDIRECTED_ADJ_LIST_IMPL_HPP

namespace std::graph {

///-------------------------------------------------------------------------------------
/// ual_edge_list
///

// ual_edge_list::const_iterator
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::reference
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator*() const {
  return *edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::pointer
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator->() const {
  return edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator&
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator++() {
  advance();
  return *this;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator++(int) {
  const_iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator&
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator--() {
  retreat();
  return *this;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator--(int) {
  const_iterator tmp(*this);
  --*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::advance() {
  edge_list_inward_link_type&  inward_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
  edge_list_outward_link_type& outward_link = *edge_;
  if (inward_link.vertex_key_ == vertex_key_) {
    edge_ = inward_link.next_;
  } else {
    assert(outward_link.vertex_key_ == vertex_key_);
    edge_ = outward_link.next_;
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::retreat() {
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

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
bool ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator==(const_iterator const& rhs) const noexcept {
  return edge_ == rhs.edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
bool ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::operator!=(const_iterator const& rhs) const noexcept {
  return !(*this == rhs);
}

// ual_edge_list::iterator
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::reference
ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator*() const {
  return *this->edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::pointer
ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator->() const {
  return this->edge_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator& ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator++() {
  this->advance();
  return *this;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator
ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator++(int) {
  iterator tmp(*this);
  ++*this;
  return tmp;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator& ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator--() {
  this->retreat();
  return *this;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator
ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator--(int) {
  iterator tmp(*this);
  --*this;
  return tmp;
}

// ual_edge_list
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::size_type ual_edge_list<VV, EV, GV, IndexT, A>::size() const noexcept {
  return size_;
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
bool ual_edge_list<VV, EV, GV, IndexT, A>::empty() const noexcept {
  return size_ == 0;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::edge_type& ual_edge_list<VV, EV, GV, IndexT, A>::front() {
  return *head_;
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::edge_type const& ual_edge_list<VV, EV, GV, IndexT, A>::front() const {
  return *head_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::edge_type& ual_edge_list<VV, EV, GV, IndexT, A>::back() {
  return *tail_;
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::edge_type const& ual_edge_list<VV, EV, GV, IndexT, A>::back() const {
  return *tail_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, IndexT, A>::link_front(edge_type&                                        uv,
                                                      ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link) {
  using link_t = ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>;
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

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, IndexT, A>::link_back(edge_type&                                        uv,
                                                     ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link) {
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

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, IndexT, A>::unlink(edge_type&                                        uv,
                                                  ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link) {

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

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator
ual_edge_list<VV, EV, GV, IndexT, A>::begin(graph_type& g, vertex_type& u) noexcept {
  return iterator(g, u, head_);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::begin(graph_type const& g, vertex_type const& u) const noexcept {
  return const_iterator(g, u, head_);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::cbegin(graph_type const& g, vertex_type const& u) const noexcept {
  return const_iterator(g, u, head_);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator
ual_edge_list<VV, EV, GV, IndexT, A>::end(graph_type& g, vertex_type& u) noexcept {
  return iterator(g, u, nullptr);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::end(graph_type const& g, vertex_type const& u) const noexcept {
  return const_iterator(g, u, nullptr);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator
ual_edge_list<VV, EV, GV, IndexT, A>::cend(graph_type const& g, vertex_type const& u) const noexcept {
  return const_iterator(g, u, nullptr);
}


///-------------------------------------------------------------------------------------
/// ual_edge
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type& g, vertex_type& u, vertex_type& v) noexcept
      : base_type()
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().vertices.data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&                 g,
                                          vertex_type&                u,
                                          vertex_type&                v,
                                          edge_user_value_type const& val) noexcept
      : base_type(val)
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&            g,
                                          vertex_type&           u,
                                          vertex_type&           v,
                                          edge_user_value_type&& val) noexcept
      : base_type(move(val))
      , edge_list_inward_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_outward_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
  link_back(u, v);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type& g, vertex_iterator ui, vertex_iterator vi) noexcept
      : ual_edge(g, *ui, *vi) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&                 g,
                                          vertex_iterator             ui,
                                          vertex_iterator             vi,
                                          edge_user_value_type const& val) noexcept
      : ual_edge(g, *ui, *vi, val) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&            g,
                                          vertex_iterator        ui,
                                          vertex_iterator        vi,
                                          edge_user_value_type&& val) noexcept
      : ual_edge(g, *ui, *vi, move(val)) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::~ual_edge() noexcept {
  edge_list_outward_link_type& outward_link = *static_cast<edge_list_outward_link_type*>(this);
  assert(outward_link.prev() == nullptr && outward_link.next() == nullptr); // has edge been unlinked?

  edge_list_inward_link_type& inward_link = *static_cast<edge_list_inward_link_type*>(this);
  assert(inward_link.prev() == nullptr && inward_link.next() == nullptr); // has edge been unlinked?
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::link_front(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_front(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.link_front(*this, *static_cast<edge_list_outward_link_type*>(this));
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::link_back(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.link_back(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.link_back(*this, *static_cast<edge_list_outward_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::unlink(vertex_type& u, vertex_type& v) noexcept {
  u.edges_.unlink(*this, *static_cast<edge_list_inward_link_type*>(this));
  v.edges_.unlink(*this, *static_cast<edge_list_outward_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::inward_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + inward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::inward_vertex(graph_type const& g) const noexcept {
  return static_cast<edge_list_inward_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::inward_vertex_key(graph_type const& g) const noexcept {
  return static_cast<edge_list_inward_link_type const*>(this)->vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::outward_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::outward_vertex(graph_type const& g) const noexcept {
  return static_cast<edge_list_outward_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::outward_vertex_key(graph_type const& g) const noexcept {
  return static_cast<edge_list_outward_link_type const*>(this)->vertex_key();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::other_vertex(graph_type& g, vertex_key_type inward_or_outward_key) noexcept {
  return inward_or_outward_key != inward_vertex_key(g) ? inward_vertex(g) : outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::other_vertex(graph_type const& g,
                                              vertex_key_type   inward_or_outward_key) const noexcept {
  return inward_or_outward_key != inward_vertex_key(g) ? inward_vertex(g) : outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::other_vertex_key(graph_type const& g,
                                                  vertex_key_type   inward_or_outward_key) const noexcept {
  return inward_or_outward_key != inward_vertex_key(g) ? inward_vertex_key(g) : outward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::edge_key_type
ual_edge<VV, EV, GV, IndexT, A>::edge_key(graph_type const& g) const noexcept {
  return pair(inward_vertex_key(g), outward_vertex_key(g));
}


///-------------------------------------------------------------------------------------
/// ual_vertex
///
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_vertex<VV, EV, GV, IndexT, A>::ual_vertex(vertex_set& vertices, vertex_index index) {}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_vertex<VV, EV, GV, IndexT, A>::ual_vertex(vertex_set&                   vertices,
                                              vertex_index                  index,
                                              vertex_user_value_type const& val)
      : base_type(val) {}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_vertex<VV, EV, GV, IndexT, A>::ual_vertex(vertex_set&              vertices,
                                              vertex_index             index,
                                              vertex_user_value_type&& val) noexcept
      : base_type(move(val)) {}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_begin(graph_type& g) noexcept {
  return edges_.begin(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_begin(graph_type const& g) const noexcept {
  return edges_.begin(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_cbegin(graph_type const& g) const noexcept {
  return edges_.cbegin(g, *this);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_end(graph_type& g) noexcept {
  return edges_.end(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_end(graph_type const& g) const noexcept {
  return edges_.end(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edges_cend(graph_type const& g) const noexcept {
  return edges_.cend(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_key_type
ual_vertex<VV, EV, GV, IndexT, A>::vertex_key(graph_type const& g) const noexcept {
  return static_cast<vertex_key_type>(this - g.vertices().data());
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::edge_type&
ual_vertex<VV, EV, GV, IndexT, A>::edge_front(graph_type& g) noexcept {
  return edges_.front(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::edge_type const&
ual_vertex<VV, EV, GV, IndexT, A>::edge_front(graph_type const& g) const noexcept {
  return edges_.front(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::edge_type&
ual_vertex<VV, EV, GV, IndexT, A>::edge_back(graph_type& g) noexcept {
  return edges_.back();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::edge_type const&
ual_vertex<VV, EV, GV, IndexT, A>::edge_back(graph_type const& g) const noexcept {
  return edges_.back();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_size_type
ual_vertex<VV, EV, GV, IndexT, A>::edges_size() const noexcept {
  return edges_.size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_vertex<VV, EV, GV, IndexT, A>::erase_edge(graph_type& g, edge_type* uv) {
  vertex_type& u = *(g.vertices().data() + uv->inward_vertex_key(g));
  vertex_type& v = *(g.vertices().data() + uv->outward_vertex_key(g));
  uv->unlink(u, v);

  uv->~edge_type();
  g.edge_alloc_.deallocate(uv, 1);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::erase_edge(graph_type& g, vertex_edge_iterator uvi) {
  edge_type* uv = &*uvi;
  ++uvi;
  erase_edge(g, uv);
  return uvi;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::erase_edge(graph_type& g, vertex_edge_iterator first, vertex_edge_iterator last) {
  while (first != last)
    first = erase_edge(g, first);
  return first;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_vertex<VV, EV, GV, IndexT, A>::clear_edges(graph_type& g) {
  erase_edge(g, edges_begin(g), edges_end(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::create_edge(graph_type& g, vertex_type& v) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::create_edge(graph_type& g, vertex_type& v, edge_user_value_type&& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, move(val));
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::create_edge(graph_type& g, vertex_type& v, edge_user_value_type const& val) {
  edge_type* uv = g.edge_alloc_.allocate(1);
  new (uv) edge_type(g, *this, v, val);
  ++g.edges_size_;
  return vertex_edge_iterator(g, *this, uv);
}


///-------------------------------------------------------------------------------------
/// undirected_adjacency_list
///

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(allocator_type const& alloc) noexcept
      : vertices_(alloc), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(graph_user_value_type const& val,
                                                                            allocator_type const&        alloc)
      : vertices_(alloc), base_type(val), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(graph_user_value_type&& val,
                                                                            allocator_type const&   alloc)
      : vertices_(alloc), base_type(move(val)), edge_alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc, typename VRng, typename VValueFnc>
  requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> && vertex_range_extractor<VRng, VValueFnc>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(ERng const&      erng,
                                                                            VRng const&      vrng,
                                                                            EKeyFnc const&   ekey_fnc,
                                                                            EValueFnc const& evalue_fnc,
                                                                            VValueFnc const& vvalue_fnc,
                                                                            GV const&        gv,
                                                                            A const&         alloc)
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
  vertices_.reserve(max_vtx_key + 1);
  if constexpr (!same_as<decltype(vvalue_fnc(*ranges::begin(vrng))), void>) {
    for (auto& vtx : vrng)
      create_vertex(vvalue_fnc(vtx));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (erng.size() > 0) {
    //edges_.reserve(erng.size());
    edge_key_type   tu_key = ekey_fnc(*ranges::begin(erng)); // first edge
    vertex_key_type tkey   = tu_key.first;                   // last in-vertex key
    for (auto& edge_data : erng) {
      edge_key_type uv_key = ekey_fnc(edge_data);
      if (uv_key.first < tkey)
        throw_unordered_edges();

      edge_iterator uv;
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
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EValueFnc>
  requires edge_range_extractor<ERng, EKeyFnc, EValueFnc> 
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(ERng const&      erng, 
                                                                            EKeyFnc const&   ekey_fnc, 
                                                                            EValueFnc const& evalue_fnc, 
                                                                            GV const&        gv, 
                                                                            A const&         alloc)
      : undirected_adjacency_list(erng, vector<int>(), ekey_fnc, evalue_fnc, [](empty_value) 
{ return empty_value(); }, gv, alloc)
// clang-format on
{}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(
      initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist, A const& alloc)
      : base_type(), vertices_(alloc), edge_alloc_(alloc) {
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    auto const& [ukey, vkey, uv_val] = edge_data;
    max_vtx_key                      = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (ilist.size() > 0) {
    auto [tkey, uukey, tu_val] = *ranges::begin(ilist);
    for (auto& edge_data : ilist) {
      auto const& [ukey, vkey, uv_val] = edge_data;
      if (ukey < tkey)
        throw_unordered_edges();

      edge_iterator uv;
      uv   = create_edge(ukey, vkey, uv_val);
      tkey = ukey;
    }
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::undirected_adjacency_list(
      initializer_list<tuple<vertex_key_type, vertex_key_type>> const& ilist, A const& alloc)
      : base_type(), vertices_(alloc), edge_alloc_(alloc) {
  // Evaluate max vertex key needed
  vertex_key_type max_vtx_key = vertex_key_type();
  for (auto& edge_data : ilist) {
    auto const& [ukey, vkey] = edge_data;
    max_vtx_key              = max(max_vtx_key, max(ukey, vkey));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (ilist.size() > 0) {
    auto [tkey, uukey, tu_val] = *ranges::begin(ilist);
    for (auto& edge_data : ilist) {
      auto const& [ukey, vkey] = edge_data;
      if (ukey < tkey)
        throw_unordered_edges();

      edge_iterator uv;
      uv   = create_edge(ukey, vkey);
      tkey = ukey;
    }
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
undirected_adjacency_list<VV, EV, GV, IndexT, A>::~undirected_adjacency_list() {
  clear(); // assure edges are deleted using edge_alloc_
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_allocator_type
undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_allocator() const noexcept {
  return edge_alloc_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_set&
undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertices() {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_set const&
undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertices() const {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void undirected_adjacency_list<VV, EV, GV, IndexT, A>::reserve_vertices(vertex_size_type n) {
  vertices_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void undirected_adjacency_list<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void undirected_adjacency_list<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type              n,
                                                                       vertex_user_value_type const& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class VV2>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_vertex(VV2 const& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::begin() {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::begin() const {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::cbegin() const {
  return vertices_.cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::end() {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::end() const {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_vertex_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::cend() const {
  return vertices_.cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_size_type
undirected_adjacency_list<VV, EV, GV, IndexT, A>::edges_size() const noexcept {
  return edges_size_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type from_key, vertex_key_type to_key) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type        from_key,
                                                              vertex_key_type        to_key,
                                                              edge_user_value_type&& val) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v, move(val)));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type from_key,
                                                              vertex_key_type to_key,
                                                              EV2 const&      val) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v, val));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator u, vertex_iterator v) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator        u,
                                                              vertex_iterator        v,
                                                              edge_user_value_type&& val) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v, move(val)));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class EV2>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator u, vertex_iterator v, EV2 const& val) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v, val));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::const_edge_iterator
undirected_adjacency_list<VV, EV, GV, IndexT, A>::erase_edge(const_edge_iterator pos) {
  edge_type* uv = &*pos;
  ++pos;
  uv->~edge_type(); // unlinks from vertices
  edge_alloc_.deallocate(uv, 1);
  return pos;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void undirected_adjacency_list<VV, EV, GV, IndexT, A>::clear() {
  // make sure edges are deallocated from edge_alloc_
  for (vertex_type& u : vertices_)
    u.clear_edges(*this);
  vertices_.clear(); // now we can clear the vertices
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void undirected_adjacency_list<VV, EV, GV, IndexT, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw invalid_argument("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// undirected_adjacency_list graph API
///

//template <typename VV, typename EV, typename GV, typename IndexT, typename A>
//using vertices_size_t = typename undirected_adjacency_list<VV, EV, GV, IndexT, A>::vertex_size_type;

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return static_cast<vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>>(&u - g.vertices().data());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                           vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                     vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return ranges::make_subrange(u.edges_begin(g), u.edges_end(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return ranges::make_subrange(u.edges_begin(g), u.edges_end(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                     vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                      vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_cbegin(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                   vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                   vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                    vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_cend(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                           vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_cbegin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                            vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_cbegin(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_cend(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_cend(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return u.edges_size();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_ssize(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  using ssize_t = vertex_edge_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>;
  return static_cast<ssize_t>(u.edges_size());
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>&              g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> key)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&        g,
                           vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> key)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

//
// API edge functions
//


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                             edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.inward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto inward_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                             edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.inward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto inward_vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                                 edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.inward_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>&         g,
                              edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto outward_vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                              edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto outward_vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                                  edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex_key(g);
}

#  if 0
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g, edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                      edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                          edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.outward_vertex_key(g);
}
#  endif

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A>&                 g,
                      edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>&         uv,
                      vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& source)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex(g, vertex_key(g, source));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                      edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const&   uv,
                      vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& source)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex(g, vertex_key(g, source));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                          edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const&   uv,
                          vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& source)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex_key(g, vertex_key(g, source));
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                          edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv,
                          vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>  source_key)
      -> vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex_key(g, source_key);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&         g,
                        edge_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& uv)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return uv.edge_key(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_key(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                        vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u,
                        vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& v)
      -> edge_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return edge_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>(vertex_key(u), vertex_key(v));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, IndexT, A>&           g,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& u,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>& v)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>(
        g, to_iterator(g, u),
        ranges::find_if(edges(g, u), [&g, &u, &v](auto& uv) { return &*vertex(g, uv, u) == &v; }));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& u,
                         vertex_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& v)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>(
        g, to_iterator(g, u), ranges::find_if(edges(g, u), [&g, &v](auto& uv) { return &*vertex(g, uv) == &v; }));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, IndexT, A>&              g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> vkey)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return find_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(undirected_adjacency_list<VV, EV, GV, IndexT, A> const&        g,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> ukey,
                         vertex_key_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> vkey)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto erase_edge(undirected_adjacency_list<VV, EV, GV, IndexT, A>&                        g,
                          vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> uv)
      -> vertex_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> u = inward_vertex(g, *uv);
  return u->erase_edge(g, uv);
}


//
// API graph functions
//

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> vertex_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_size(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_ssize(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  using ssize_t = vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>;
  return static_cast<ssize_t>(g.vertices().size());
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_cbegin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_end(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_end(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_cend(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_vertices(undirected_adjacency_list<VV, EV, GV, IndexT, A>&               g,
                      vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> n) {
  g.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, IndexT, A>&               g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> n) {
  g.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(undirected_adjacency_list<VV, EV, GV, IndexT, A>&                       g,
                     vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>         n,
                     vertex_value_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> const& val) {
  g.resize(n, val);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> edge_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_edge_range_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_begin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_cbegin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_end(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_cend(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_edge_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges().cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> edge_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.edges_size();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_ssize(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> edge_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  using ssize_t = edge_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>>;
  return static_cast<ssize_t>(g.edges().size());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_edges(undirected_adjacency_list<VV, EV, GV, IndexT, A>&             g,
                   edge_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> n) {
  g.reserve_edges(n);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void clear(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g) {
  g.clear();
}


//
// API: common container functions
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto size(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_size_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return vertices_size(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto ssize(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_ssize_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return vertices_ssize(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, IndexT, A>& g)
      -> vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(undirected_adjacency_list<VV, EV, GV, IndexT, A> const& g)
      -> const_vertex_iterator_t<undirected_adjacency_list<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}

} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_IMPL_HPP
