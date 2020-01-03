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
  edge_list_out_link_type& out = *edge_;
  if (out.vertex_key_ == vertex_key_) {
    edge_ = out.next_;
  } else {
    edge_list_out_link_type& in = *edge_; // in.vertex_key_ == this->vertex_key_;
    edge_                       = in.next_;
  }
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
  edge_list_out_link_type& out = *this->edge_;
  if (out.vertex_key_ == this->vertex_key_) {
    this->edge_ = out.next_;
  } else {
    edge_list_out_link_type& in = *this->edge_; // in.vertex_key_ == this->vertex_key_;
    this->edge_                 = in.next_;
  }
  return *this;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge_list<VV, EV, GV, IndexT, A>::iterator
ual_edge_list<VV, EV, GV, IndexT, A>::iterator::operator++(int) {
  iterator tmp(*this);
  ++*this;
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
  using link_t = ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>;
  if (tail_) {
    link_t& tail_link = *static_cast<link_t*>(tail_);
    uv_link.prev_     = tail_;
    tail_link.next_   = &uv;
    tail_             = &uv;
  } else {
    head_ = tail_ = &uv;
  }
  ++size_;
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ListT>
void ual_edge_list<VV, EV, GV, IndexT, A>::unlink(edge_type&                                        uv,
                                                  ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>& uv_link) {
  using link_t = ual_edge_list_link<VV, EV, GV, IndexT, A, ListT>;
  if (head_ == tail_) { // &uv == head_ == tail_
    head_ = tail_ = nullptr;
  } else if (&uv == head_) {
    link_t& next_link = *static_cast<link_t*>(uv_link.next_);
    next_link.prev_   = nullptr;
    head_             = uv_link.next_;
  } else if (&uv == tail_) {
    link_t& prev_link = *static_cast<link_t*>(uv_link.prev_);
    prev_link.next_   = nullptr;
    tail_             = uv_link.prev;
  } else { // in the middle of the list
    link_t& prev_link = *static_cast<link_t*>(uv_link.prev_);
    link_t& next_link = *static_cast<link_t*>(uv_link.next_);
    prev_link.next_   = uv_link.next_;
    next_link.prev_   = uv_link.prev_;
  }
  --size_;
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
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(vertex_set const& vertices,
                                          vertex_iterator   in_vertex,
                                          vertex_iterator   out_vertex) noexcept
      : base_t()
      , edge_list_in_link_type(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {
  in_vertex->edges_.link_back(*this, *static_cast<edge_list_in_link_type*>(this));
  out_vertex->edges_.link_back(*this, *static_cast<edge_list_out_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(vertex_set const&           vertices,
                                          vertex_iterator             in_vertex,
                                          vertex_iterator             out_vertex,
                                          edge_user_value_type const& val) noexcept
      : base_t(val)
      , edge_list_in_link_type(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {
  in_vertex->edges_.link_back(*this, *static_cast<edge_list_in_link_type*>(this));
  out_vertex->edges_.link_back(*this, *static_cast<edge_list_out_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::ual_edge(vertex_set const&      vertices,
                                          vertex_iterator        in_vertex,
                                          vertex_iterator        out_vertex,
                                          edge_user_value_type&& val) noexcept
      : base_t(move(val))
      , edge_list_in_link_type(static_cast<vertex_key_type>(in_vertex - vertices.begin()))
      , edge_list_out_link_type(static_cast<vertex_key_type>(out_vertex - vertices.begin())) {
  in_vertex->edges_.link_back(*this, *static_cast<edge_list_in_link_type*>(this));
  out_vertex->edges_.link_back(*this, *static_cast<edge_list_out_link_type*>(this));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_edge<VV, EV, GV, IndexT, A>::~ual_edge() noexcept {
  in_vertex->edges_.unlink(*this, *static_cast<edge_list_in_link_type*>(this));
  out_vertex->edges_.unlink(*this, *static_cast<edge_list_out_link_type*>(this));
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + in_vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::in_vertex(graph_type const& g) const noexcept {
  return g.vertices().begin() + in_vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type ual_edge<VV, EV, GV, IndexT, A>::in_vertex_key() const
      noexcept {
  return static_cast<edge_list_in_link_type*>(this)->vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type& g) noexcept {
  return g.vertices().begin() + out_vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_edge<VV, EV, GV, IndexT, A>::out_vertex(graph_type const& g) const noexcept {
  return g.vertices().begin() + out_vertex_key();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_edge<VV, EV, GV, IndexT, A>::vertex_key_type ual_edge<VV, EV, GV, IndexT, A>::out_vertex_key() const
      noexcept {
  return static_cast<edge_list_out_link_type const*>(this)->vertex_key();
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
      : base_t(val) {}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_vertex<VV, EV, GV, IndexT, A>::ual_vertex(vertex_set&              vertices,
                                              vertex_index             index,
                                              vertex_user_value_type&& val) noexcept
      : base_t(move(val)) {}


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
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_size_type ual_vertex<VV, EV, GV, IndexT, A>::edge_size() const
      noexcept {
  return edges_.size();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_vertex<VV, EV, GV, IndexT, A>::vertex_edge_iterator
ual_vertex<VV, EV, GV, IndexT, A>::erase_edge(graph_type& g, vertex_edge_iterator uvi) {
  edge_type* uv = &*uvi;
  ++uvi;
  auto alloc = g.edge_allocator();
      uv->~edge_type();
  alloc.deallocate(uv, 1);
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


///-------------------------------------------------------------------------------------
/// ual_graph
///

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(allocator_type alloc) : vertices_(alloc), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(graph_user_value_type const& val, allocator_type alloc)
      : vertices_(alloc), base_t(val), edge_alloc_(alloc) {}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(graph_user_value_type&& val, allocator_type alloc) noexcept
      : vertices_(alloc), base_t(move(val)), edge_alloc_(alloc) {}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EPropFnc, typename VRng, typename VPropFnc>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(ERng const&     erng,
                                            VRng const&     vrng,
                                            EKeyFnc const&  ekey_fnc,
                                            EPropFnc const& eprop_fnc,
                                            VPropFnc const& vprop_fnc,
                                            A               alloc)
      : vertices_(alloc), edge_alloc_(alloc) {
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
  if (!erng.empty()) {
    //edges_.reserve(erng.size());
    edge_key_type   tu_key = ekey_fnc(*::ranges::begin(erng));
    vertex_iterator t      = to_iterator(*this, vertices_[tu_key.first]);
    for (auto& edge_data : erng) {
      edge_key_type edge_key = ekey_fnc(edge_data);
      //auto            edge_val = eprop_fnc(edge_data);
      vertex_iterator u = to_iterator(*this, vertices_[edge_key.first]);
      if (u < t)
        throw_unordered_edges();

      // assure begin edge is set for vertices w/o edges
      //t = finalize_out_edges(::ranges::make_subrange(t, u));

      edge_iterator uv;
      if constexpr (same_as<decltype(eprop_fnc(edge_data)), void>) {
        uv = create_edge(edge_key.first, edge_key.second);
      } else {
        uv = create_edge(edge_key.first, edge_key.second, eprop_fnc(edge_data));
      }
      //u->set_edge_begin(*this, uv);
    }

    // assure begin edge is set for remaining vertices w/o edges
    //finalize_out_edges(::ranges::make_subrange(t, vertices_.end()));
  }
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
ual_graph<VV, EV, GV, IndexT, A>::~ual_graph() {
  clear(); // assure edges are deleted using edge_alloc_
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
template <typename ERng, typename EKeyFnc, typename EPropFnc>
ual_graph<VV, EV, GV, IndexT, A>::ual_graph(ERng const&     erng,
                                            EKeyFnc const&  ekey_fnc,
                                            EPropFnc const& eprop_fnc,
                                            A               alloc)
      : ual_graph(
              erng, vector<int>(), ekey_fnc, eprop_fnc, [](empty_value) { return empty_value(); }, alloc) {}


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
  return vertices_.begin() + (vertices_.size() - 1);
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
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator ual_graph<VV, EV, GV, IndexT, A>::end() const
      noexcept {
  return vertices_.end();
}
template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::const_vertex_iterator
ual_graph<VV, EV, GV, IndexT, A>::cend() const noexcept {
  return vertices_.cend();
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr typename ual_graph<VV, EV, GV, IndexT, A>::edge_size_type ual_graph<VV, EV, GV, IndexT, A>::edges_size() const
      noexcept {
  return edges_size_;
}


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator
ual_graph<VV, EV, GV, IndexT, A>::create_edge(vertex_key_type const& from_key, vertex_key_type const& to_key) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);

  edge_type* uv = edge_alloc_.allocate(1);
  new (uv) edge_type(vertices_, from, to);
  ++edges_size_;
  return edge_iterator(*this, from, vertex_edge_iterator(*this, *from, uv));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator ual_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, edge_user_value_type&& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);

  edge_type* uv = edge_alloc_.allocate(1);
  new (uv) edge_type(vertices_, from, to, move(val));
  ++edges_size_;
  return edge_iterator(*this, from, vertex_edge_iterator(*this, *from, uv));
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
typename ual_graph<VV, EV, GV, IndexT, A>::edge_iterator ual_graph<VV, EV, GV, IndexT, A>::create_edge(
      vertex_key_type const& from_key, vertex_key_type const& to_key, edge_user_value_type const& val) {
  vertex_iterator from = find_vertex(from_key);
  vertex_iterator to   = find_vertex(to_key);

  edge_type* uv = edge_alloc_.allocate(1);
  new (uv) edge_type(vertices_, from, to, val);
  ++edges_size_;
  return edge_iterator(*this, from, vertex_edge_iterator(*this, *from, uv));
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
constexpr auto edges_size(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                          vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_size(g);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto edges_degree(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                            vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u) noexcept
      -> vertex_edge_size_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return u.edge_size(g);
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


template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A>&           g,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& u,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>>& v) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(edges(g, u), [&g, &v](auto uv) { return &*vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A> const&           g,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& u,
                         vertex_t<ual_graph<VV, EV, GV, IndexT, A>> const& v) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return ::ranges::find_if(edges(g, u), [&g, &v](auto uv) { return &*vertex(g, uv) == &v; });
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A>&                     g,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return find_edge(g, g.vertices()[ukey], g.vertices()[vkey]);
}

template <typename VV, typename EV, typename GV, typename IndexT, typename A>
constexpr auto find_edge(ual_graph<VV, EV, GV, IndexT, A> const&               g,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& ukey,
                         vertex_key_t<ual_graph<VV, EV, GV, IndexT, A>> const& vkey) noexcept
      -> const_vertex_edge_iterator_t<ual_graph<VV, EV, GV, IndexT, A>> {
  return find_edge(g, find_vertex(g, ukey), find_vertex(g, vkey));
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
