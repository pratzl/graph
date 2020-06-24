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
  edge_list_in_link_type&  in_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
  edge_list_out_link_type& out_link = *edge_;
  if (in_link.vertex_key_ == vertex_key_) {
    edge_ = in_link.next_;
  } else {
    assert(out_link.vertex_key_ == vertex_key_);
    edge_ = out_link.next_;
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge_list<VV, EV, GV, IndexT, A>::const_iterator::retreat() {
  if (edge_) {
    edge_list_in_link_type&  in_link  = *edge_; // in.vertex_key_ == this->vertex_key_;
    edge_list_out_link_type& out_link = *edge_;
    if (in_link.vertex_key_ == vertex_key_) {
      edge_ = in_link.prev_;
    } else {
      assert(out_link.vertex_key_ == vertex_key_);
      edge_ = out_link.prev_;
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
    edge_list_in_link_type&  tail_in_link  = static_cast<edge_list_in_link_type&>(*tail_);
    edge_list_out_link_type& tail_out_link = static_cast<edge_list_out_link_type&>(*tail_);
    if (tail_in_link.vertex_key_ == uv_link.vertex_key_) {
      uv_link.prev_      = tail_;
      tail_in_link.next_ = &uv;
      tail_              = &uv;
    } else {
      uv_link.prev_       = tail_;
      tail_out_link.next_ = &uv;
      tail_               = &uv;
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
    edge_list_in_link_type&  prev_in_link  = static_cast<edge_list_in_link_type&>(*uv_link.prev_);
    edge_list_out_link_type& prev_out_link = static_cast<edge_list_out_link_type&>(*uv_link.prev_);
    if (prev_in_link.vertex_key_ == uv_link.vertex_key_) {
      prev_in_link.next_ = uv_link.next_;
    } else {
      assert(prev_out_link.vertex_key_ == uv_link.vertex_key_);
      prev_out_link.next_ = uv_link.next_;
    }
  }
  if (tail_ == &uv) {
    tail_ = uv_link.prev_;
  }

  if (uv_link.next_) {
    edge_list_in_link_type&  next_in_link  = static_cast<edge_list_in_link_type&>(*uv_link.next_);
    edge_list_out_link_type& next_out_link = static_cast<edge_list_out_link_type&>(*uv_link.next_);
    if (next_in_link.vertex_key_ == uv_link.vertex_key_) {
      next_in_link.prev_ = uv_link.prev_;
    } else {
      assert(next_out_link.vertex_key_ == uv_link.vertex_key_);
      next_out_link.prev_ = uv_link.prev_;
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
      , edge_list_in_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(&v - g.vertices().vertices.data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&                 g,
                                          vertex_type&                u,
                                          vertex_type&                v,
                                          edge_user_value_type const& val) noexcept
      : base_type(val)
      , edge_list_in_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
  link_back(u, v);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(graph_type&            g,
                                          vertex_type&           u,
                                          vertex_type&           v,
                                          edge_user_value_type&& val) noexcept
      : base_type(move(val))
      , edge_list_in_link_type(static_cast<vertex_key_type>(&u - g.vertices().data()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(&v - g.vertices().data())) {
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
  edge_list_out_link_type& out_link = *static_cast<edge_list_out_link_type*>(this);
  assert(out_link.prev() == nullptr && out_link.next() == nullptr); // has edge been unlinked?

  edge_list_in_link_type& in_link = *static_cast<edge_list_in_link_type*>(this);
  assert(in_link.prev() == nullptr && in_link.next() == nullptr); // has edge been unlinked?
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::link_front(vertex_type& u, vertex_type& v) {
  u.edges_.link_front(*this, *static_cast<edge_list_in_link_type*>(this));
  v.edges_.link_front(*this, *static_cast<edge_list_out_link_type*>(this));
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::link_back(vertex_type& u, vertex_type& v) {
  u.edges_.link_back(*this, *static_cast<edge_list_in_link_type*>(this));
  v.edges_.link_back(*this, *static_cast<edge_list_out_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_edge<VV, EV, GV, IndexT, A>::unlink(vertex_type& u, vertex_type& v) {
  u.edges_.unlink(*this, *static_cast<edge_list_in_link_type*>(this));
  v.edges_.unlink(*this, *static_cast<edge_list_out_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + in_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type const& g) const noexcept {
  return static_cast<edge_list_in_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::in_vertex_key(graph_type const& g) const noexcept {
  return static_cast<edge_list_in_link_type const*>(this)->vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + out_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type const& g) const noexcept {
  return static_cast<edge_list_out_link_type const*>(this)->vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::out_vertex_key(graph_type const& g) const noexcept {
  return static_cast<edge_list_out_link_type const*>(this)->vertex_key();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::other_vertex(graph_type& g, vertex_key_type const in_or_out_key) noexcept {
  return in_or_out_key != in_vertex_key(g) ? in_vertex(g) : out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::other_vertex(graph_type const& g, vertex_key_type const in_or_out_key) const noexcept {
  return in_or_out_key != in_vertex_key(g) ? in_vertex(g) : out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type
ual_edge<VV, EV, GV, IndexT, A>::other_vertex_key(graph_type const&     g,
                                                  vertex_key_type const in_or_out_key) const noexcept {
  return in_or_out_key != in_vertex_key(g) ? in_vertex_key(g) : out_vertex_key(g);
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
ual_vertex<VV, EV, GV, IndexT, A>::edge_begin(graph_type& g) noexcept {
  return edges_.begin(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edge_begin(graph_type const& g) const noexcept {
  return edges_.begin(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edge_cbegin(graph_type const& g) const noexcept {
  return edges_.cbegin(g, *this);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edge_end(graph_type& g) noexcept {
  return edges_.end(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edge_end(graph_type const& g) const noexcept {
  return edges_.end(g, *this);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::const_vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::edge_cend(graph_type const& g) const noexcept {
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
ual_vertex<VV, EV, GV, IndexT, A>::edge_size() const noexcept {
  return edges_.size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_vertex<VV, EV, GV, IndexT, A>::erase_edge(graph_type& g, edge_type* uv) {
  vertex_type& u = *(g.vertices().data() + uv->in_vertex_key(g));
  vertex_type& v = *(g.vertices().data() + uv->out_vertex_key(g));
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
  erase_edge(g, edge_begin(g), edge_end(g));
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
/// ual_graph
///

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(allocator_type alloc) : vertices_(alloc), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(graph_user_value_type const& val, allocator_type alloc)
      : vertices_(alloc), base_type(val), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(graph_user_value_type&& val, allocator_type alloc) noexcept
      : vertices_(alloc), base_type(move(val)), edge_alloc_(alloc) {}


// clang-format off
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EPropFnc, typename VRng, typename VPropFnc>
  requires ual_edge_data_c<ERng, EKeyFnc, EPropFnc> && ual_vertex_data_c<VRng, VPropFnc>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(ERng const&     erng,
                                            VRng const&     vrng,
                                            EKeyFnc const&  ekey_fnc,
                                            EPropFnc const& eprop_fnc,
                                            VPropFnc const& vprop_fnc,
                                            GV const&       gv,
                                            A               alloc)
      : base_type(gv), vertices_(alloc), edge_alloc_(alloc)
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
  if constexpr (!same_as<decltype(vprop_fnc(*::ranges::begin(vrng))), void>) {
    for (auto& vtx : vrng)
      create_vertex(vprop_fnc(vtx));
  }
  vertices_.resize(max_vtx_key + 1); // assure expected vertices exist

  // add edges
  if (erng.size() > 0) {
    //edges_.reserve(erng.size());
    edge_key_type   tu_key = ekey_fnc(*::ranges::begin(erng)); // first edge
    vertex_key_type tkey   = tu_key.first;                     // last in-vertex key
    for (auto& edge_data : erng) {
      edge_key_type uv_key = ekey_fnc(edge_data);
      if (uv_key.first < tkey)
        throw_unordered_edges();

      edge_iterator uv;
      if constexpr (same_as<decltype(eprop_fnc(edge_data)), void>) {
        uv = create_edge(uv_key.first, uv_key.second);
      } else {
        uv = create_edge(uv_key.first, uv_key.second, eprop_fnc(edge_data));
      }
      tkey = uv_key.first;
    }
  }
}

// clang-format off
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EPropFnc>
  requires ual_edge_data_c<ERng, EKeyFnc, EPropFnc> 
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(ERng const& erng, EKeyFnc const& ekey_fnc, EPropFnc const& eprop_fnc, GV const& gv, A alloc)
      : ual_graph(erng, vector<int>(), ekey_fnc, eprop_fnc, [](empty_value) { return empty_value(); }, gv, alloc)
// clang-format on
{}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(
      initializer_list<tuple<vertex_key_type, vertex_key_type, edge_user_value_type>> const& ilist, A alloc)
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
    auto [tkey, uukey, tu_val] = *::ranges::begin(ilist);
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
ual_graph<VV, EV, GV, IndexT, A>::~ual_graph() {
  clear(); // assure edges are deleted using edge_alloc_
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::edge_allocator_type
ual_graph<VV, EV, GV, IndexT, A>::edge_allocator() const noexcept {
  return edge_alloc_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::vertex_set& ual_graph<VV, EV, GV, IndexT, A>::vertices() noexcept {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::vertex_set const&
ual_graph<VV, EV, GV, IndexT, A>::vertices() const noexcept {
  return vertices_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_graph<VV, EV, GV, IndexT, A>::reserve_vertices(vertex_size_type n) {
  vertices_.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_graph<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type n) {
  vertices_.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_graph<VV, EV, GV, IndexT, A>::resize_vertices(vertex_size_type n, vertex_user_value_type const& val) {
  vertices_.resize(n, val);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator ual_graph<VV, EV, GV, IndexT, A>::create_vertex() {
  vertices_.push_back(vertex_type(vertices_, vertices_.size()));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_vertex(vertex_user_value_type&& val) {
  vertices_.push_back(vertex_type(vertices_, vertices_.size(), move(val)));
  return vertices_.begin() + (vertices_.size() - 1);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class VV2>
typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_vertex(VV2 const& val) {
  vertices_.push_back(vertex_type(vertices_, static_cast<vertex_key_type>(vertices_.size()), val));
  return vertices_.begin() + static_cast<vertex_key_type>(vertices_.size() - 1);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type const& key) {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::find_vertex(vertex_key_type const& key) const {
  if (key < vertices_.size())
    return vertices_.begin() + key;
  else
    return vertices_.end();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::begin() noexcept {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::begin() const noexcept {
  return vertices_.begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::cbegin() const noexcept {
  return vertices_.cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::vertex_iterator ual_graph<VV, EV, GV, IndexT, A>::end() noexcept {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::end() const noexcept {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::cend() const noexcept {
  return vertices_.cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::edge_size_type
ual_graph<VV, EV, GV, IndexT, A>::edges_size() const noexcept {
  return edges_size_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type const& from_key, vertex_key_type const& to_key) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator ual_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, edge_user_value_type&& val) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v, move(val)));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class EV2>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator ual_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, EV2 const& val) {
  vertex_type& u = vertices_[from_key];
  vertex_type& v = vertices_[to_key];
  return edge_iterator(*this, to_iterator(*this, u), u.create_edge(*this, v, val));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator u, vertex_iterator v) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator u, vertex_iterator v, edge_user_value_type&& val) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v, move(val)));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <class EV2>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_iterator u, vertex_iterator v, EV2 const& val) {
  return edge_iterator(*this, to_iterator(*this, *u), u->create_edge(*this, *v, val));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::const_edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::erase_edge(const_edge_iterator pos) {
  edge_type* uv = &*pos;
  ++pos;
  uv->~edge_type(); // unlinks from vertices
  edge_alloc_.deallocate(uv, 1);
  return pos;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_graph<VV, EV, GV, IndexT, A>::clear() {
  // make sure edges are deallocated from edge_alloc_
  for (vertex_type& u : vertices_)
    u.clear_edges(*this);
  vertices_.clear(); // now we can clear the vertices
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void ual_graph<VV, EV, GV, IndexT, A>::throw_unordered_edges() const {
  assert(false); // container must be sorted by edge_key.first
  throw invalid_argument("edges not ordered");
}

///-------------------------------------------------------------------------------------
/// ual_graph graph API
///

//template <typename VV, typename EV, typename GV, typename IndexT, typename A>
//using vertices_size_t = typename ual_graph<VV, EV, GV, IndexT, A>::vertex_size_type;

//
// API vertex functions
//
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return static_cast<vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>>>(&u - g.vertices().data());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u)
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto to_iterator(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u)
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  auto const key = vertex_key(g, u);
  if (key < numeric_limits<decltype(key)>::max())
    return g.vertices().begin() + key;
  else
    return g.vertices().end();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(u.edge_begin(g), u.edge_end(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(u.edge_begin(g), u.edge_end(g));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                     vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                      vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cbegin(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                   vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                    vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cend(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_begin(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cbegin(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                           vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cbegin(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                        vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_end(g);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cend(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_cend(g);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_degree(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                            vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_size();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(ual_graph<VV, EV, GV, IndexT, A>&                     g,
                           vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& key) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_vertex(ual_graph<VV, EV, GV, IndexT, A> const&               g,
                           vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& key) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.find_vertex(key);
}

//
// API edge functions
//


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex(ual_graph<VV, EV, GV, IndexT, A>& g, edge_t<ual_graph<VV, EV, GV, IndexT, A>>& uv) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                         edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto in_vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                             edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.in_vertex_key(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex(ual_graph<VV, EV, GV, IndexT, A>& g, edge_t<ual_graph<VV, EV, GV, IndexT, A>>& uv) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                          edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto out_vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                              edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex_key(g);
}

#  if 0
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(ual_graph<VV, EV, GV, IndexT, A>& g, edge_t<ual_graph<VV, EV, GV, IndexT, A>>& uv) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                      edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&         g,
                          edge_t<ual_graph<VV, EV, GV, IndexT, A>> const& uv) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.out_vertex_key(g);
}
#  endif

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(ual_graph<VV, EV, GV, IndexT, A>&                 g,
                      edge_t<ual_graph<VV, EV, GV, IndexT, A>>&         uv,
                      vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& source) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex(g, vertex_key(g, source));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                      edge_t<ual_graph<VV, EV, GV, IndexT, A>> const&   uv,
                      vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& source) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex(g, vertex_key(g, source));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                          edge_t<ual_graph<VV, EV, GV, IndexT, A>> const&   uv,
                          vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& source) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex_key(g, vertex_key(g, source));
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_key(ual_graph<VV, EV, GV, IndexT, A> const&               g,
                          edge_t<ual_graph<VV, EV, GV, IndexT, A>> const&       uv,
                          vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& source_key) noexcept
      -> vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return uv.other_vertex_key(g, source_key);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A>&           g,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& v) noexcept
      -> edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>>(
        g, to_iterator(g, u),
        ::ranges::find_if(edges(g, u), [&g, &u, &v](auto& uv) { return &*vertex(g, uv, u) == &v; }));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& v) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>>(
        g, to_iterator(g, u), ::ranges::find_if(edges(g, u), [&g, &v](auto& uv) { return &*vertex(g, uv) == &v; }));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A>&                     g,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return find_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A> const&               g,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto erase_edge(ual_graph<VV, EV, GV, IndexT, A>&                        g,
                          vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> uv)
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> u = in_vertex(g, *uv);
  return u->erase_edge(g, uv);
}


//
// API graph functions
//

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::make_subrange(g.vertices());
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertices_size(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> vertex_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto begin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cbegin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto end(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto cend(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_begin(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_begin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_cbegin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_end(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_end(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto vertex_cend(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_vertex_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.vertices().cend();
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_vertices(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_size_t<ual_graph<VV, EV, GV, IndexT, A>> n) {
  g.reserve(n);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(ual_graph<VV, EV, GV, IndexT, A>& g, vertex_size_t<ual_graph<VV, EV, GV, IndexT, A>> n) {
  g.resize(n);
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void resize_vertices(ual_graph<VV, EV, GV, IndexT, A>&                       g,
                     vertex_size_t<ual_graph<VV, EV, GV, IndexT, A>>         n,
                     vertex_value_t<ual_graph<VV, EV, GV, IndexT, A>> const& val) {
  g.resize(n, val);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept -> edge_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_range_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_begin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().begin();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cbegin(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().cbegin();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(ual_graph<VV, EV, GV, IndexT, A>& g) noexcept
      -> edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_end(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edge_cend(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> const_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges().cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_size(ual_graph<VV, EV, GV, IndexT, A> const& g) noexcept
      -> edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return g.edges_size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void reserve_edges(ual_graph<VV, EV, GV, IndexT, A>& g, edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> n) {
  g.reserve_edges(n);
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
void clear(ual_graph<VV, EV, GV, IndexT, A>& g) {
  g.clear();
}


} // namespace std::graph

#endif // UNDIRECTED_ADJ_LIST_IMPL_HPP
