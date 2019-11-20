// adjlist_fwd_vl.hpp - concrete data structure for adjacency list
#pragma once
#include "graph.hpp"
#include <vector>
#include <list>

namespace std::graph {

//
// forward-only adjacency list graph
// vertices: vector
// edges   : list (out edges)
// (incomplete definition; experimental only)
//
template <typename GV = empty_value, typename VV = empty_value, typename EV = empty_value>
using adjacency_list = graph<adj_list_type, GV, VV, EV>;

template <typename VV, typename EV>
class out_edge_set<adj_list_type, VV, EV> {
public:
  using ADJ         = adj_list_type;
  using this_t      = out_edge_set<ADJ, VV, EV>;
  using vertex_type = vertex<ADJ, VV, EV>;
  using vertex_desc = size_t;
  using edge_type   = edge<ADJ, VV, EV>;
  using edges_type  = list<edge_type>;

  using iterator       = typename edges_type::iterator;
  using const_iterator = typename edges_type::const_iterator;

  size_t size() const { return edges_.size(); }

  iterator       begin() { return edges_.begin(); }
  const_iterator begin() const { return edges_.begin(); }
  const_iterator cbegin() const { return edges_.begin(); }

  iterator       end() { return edges_.end(); }
  const_iterator end() const { return edges_.end(); }
  const_iterator cend() const { return edges_.end(); }

  //void add();


private:
  edges_type edges_;
};


template <typename GV, typename VV, typename EV, typename A>
class graph<adj_list_type, GV, VV, EV, A>
      : public conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV> {
  // types
public:
  using ADJ              = adj_list_type;
  using this_t           = graph<ADJ, GV, VV, EV, A>;
  using base_t           = conditional_t<graph_value_needs_wrap<GV>::value, graph_value<GV>, GV>;
  using graph_user_value = GV;
  using allocator_type   = A;

  using vertex_type = vertex<ADJ, VV, EV>;
  using vertex_set  = vector<vertex_type>;
  using vertex_desc = size_t;
  //using vertex_key_type = ...;
  using vertex_user_value = VV;
  //using vertex_range = ...;
  //using vertex_iterator = ...;
  // using vertex_sentinal = ...;
  // using vertex_size_t = ...;

  using edge_type = edge<ADJ, VV, EV>;

  // Construction/Destruction
public:
  // Properties
public:
  // Member Variables
private:
  vertex_set vertices_;
};


template <typename VV, typename EV>
class vertex<adj_list_type, VV, EV> : public conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV> {
  // types
public:
  using ADJ    = adj_list_type;
  using this_t = vertex<ADJ, VV, EV>;
  using base_t = conditional_t<graph_value_needs_wrap<VV>::value, graph_value<VV>, VV>;

  using vertex_desc = size_t;

  using edge_type               = edge<ADJ, VV, EV>;
  using out_edge_list_type      = list<edge_type>;
  using out_edge_iterator       = typename out_edge_list_type::iterator;
  using const_out_edge_iterator = typename out_edge_list_type::const_iterator;
  using in_edge_list_type       = void;

  // Construction/Destruction
public:
  // Properties
public:
  // Member Variables
private:
  out_edge_list_type out_edges_;
};


template <typename VV, typename EV>
class edge<adj_list_type, VV, EV> : public conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV> {
  // types
public:
  using ADJ    = adj_list_type;
  using this_t = edge<ADJ, VV, EV>;
  using base_t = conditional_t<graph_value_needs_wrap<EV>::value, graph_value<EV>, EV>;

  using vertex_user_value = VV;
  using vertex_desc       = size_t;

  using edge_user_value = EV;

  // Construction/Destruction
public:
  edge()            = default;
  edge(edge const&) = default;
  edge(edge&&)      = default;
  edge(vertex_desc in_vertex, vertex_desc out_vertex);
  edge(vertex_desc in_vertex, vertex_desc out_vertex, edge_user_value const&);
  edge(vertex_desc in_vertex, vertex_desc out_vertex, edge_user_value&&);

  edge& operator=(edge const&) = default;
  edge& operator=(edge&&) = default;

  // Properties
public:
  vertex_desc out_vertex() const;
  vertex_desc in_vertex() const;

  // Member Variables
private:
  vertex_desc out_vertex_ = numeric_limits<vertex_desc>::max();
  vertex_desc in_vertex_  = numeric_limits<vertex_desc>::max();
};


} // namespace std::graph
