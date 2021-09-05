#pragma once

#include "graph/directed_adjacency_vector.hpp"
#ifdef CPO
#  include "graph/undirected_adjacency_list.hpp"
#endif

#include <range/v3/algorithm/lower_bound.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/range.hpp>

#include <string>
#include <vector>


//---------------------------------------------------------------------------------------
// forward declarations
//
template <typename Mapper>
class GraphXlate;

struct route;
using routes_t     = std::vector<route>;
using route_city_t = std::vector<std::string>;

struct daa_routes_edge_mapper;
struct ual_routes_edge_mapper;

struct daa_data_edge_mapper;
struct ual_data_edge_mapper;

//---------------------------------------------------------------------------------------
// data
//
extern GraphXlate<daa_routes_edge_mapper> germany_routes_directed_graph;
extern GraphXlate<daa_data_edge_mapper>   dollar_directed_graph;
extern GraphXlate<daa_data_edge_mapper>   candle_directed_graph;

#ifdef CPO
extern GraphXlate<ual_routes_edge_mapper> germany_routes_undirected_graph;
extern GraphXlate<ual_data_edge_mapper>   dollar_undirected_graph;
extern GraphXlate<ual_data_edge_mapper>   candle_undirected_graph;
#endif

//---------------------------------------------------------------------------------------
// route data structures
//
struct route {
  std::string from;
  std::string to;
  int         km = 0;

  route(const std::string& from_city, const std::string& to_city, int kilometers)
        : from(from_city), to(to_city), km(kilometers) {}
  route()             = default;
  route(const route&) = default;
  route(route&&)      = default;
  route& operator=(const route&) = default;
  route& operator=(route&&) = default;
  ~route()                  = default;
};

struct daa_routes_edge_mapper {
  using target_graph_t      = std::graph::containers::directed_adjacency_vector<std::graph::containers::name_value,
                                                                           std::graph::containers::weight_value>;
  using source_edge_range_t = routes_t;
  using source_edge_t       = source_edge_range_t::value_type;

  using vertex_label_t = std::string;

  vertex_label_t inward_label(const source_edge_t& uv) const { return uv.from; }
  vertex_label_t outward_label(const source_edge_t& uv) const { return uv.to; }

  auto edge_value(const source_edge_t& uv) const { return uv.km; }
};

#ifdef CPO
struct ual_routes_edge_mapper {
  using target_graph_t      = std::graph::undirected_adjacency_list<std::graph::name_value, std::graph::weight_value>;
  using source_edge_range_t = routes_t;
  using source_edge_t       = source_edge_range_t::value_type;

  using vertex_label_t = std::string;

  const vertex_label_t& inward_label(const source_edge_t& uv) const { return uv.from; }
  const vertex_label_t& outward_label(const source_edge_t& uv) const { return uv.to; }

  auto edge_value(const source_edge_t& uv) const { return uv.km; }
};
#endif


// a weight value with double type
struct dbl_weight_value {
  double weight = 0;

  dbl_weight_value(const double& w) : weight(w) {}
  dbl_weight_value()                        = default;
  dbl_weight_value(const dbl_weight_value&) = default;
  dbl_weight_value& operator=(const dbl_weight_value&) = default;
  ~dbl_weight_value()                                  = default;
};


//---------------------------------------------------------------------------------------
// edge_data data structures
//
struct data_edge {
  std::string from;
  std::string to;
  double      weight = 0.0;
  data_edge(const std::string& from_vertex, const std::string& to_vertex, double wght)
        : from(from_vertex), to(to_vertex), weight(wght) {}
  data_edge()                 = default;
  data_edge(const data_edge&) = default;
  data_edge& operator=(const data_edge&) = default;
  ~data_edge()                           = default;
};
using data_edges_t    = std::vector<data_edge>;
using vertex_labels_t = std::vector<std::string>;

struct daa_data_edge_mapper {
  using target_graph_t =
        std::graph::containers::directed_adjacency_vector<std::graph::containers::name_value, dbl_weight_value>;
  using source_edge_range_t = data_edges_t;
  using source_edge_t       = data_edges_t::value_type;

  using vertex_label_t = std::string;

  vertex_label_t inward_label(const source_edge_t& uv) const { return uv.from; }
  vertex_label_t outward_label(const source_edge_t& uv) const { return uv.to; }

  auto edge_value(const source_edge_t& uv) const { return uv.weight; }
};

#ifdef CPO
struct ual_data_edge_mapper {
  using target_graph_t = std::graph::containers::undirected_adjacency_list<std::graph::name_value, dbl_weight_value>;
  using source_edge_range_t = data_edges_t;
  using source_edge_t       = data_edges_t::value_type;

  using vertex_label_t = std::string;

  const vertex_label_t& inward_label(const source_edge_t& uv) const { return uv.from; }
  const vertex_label_t& outward_label(const source_edge_t& uv) const { return uv.to; }

  auto edge_value(const source_edge_t& uv) const { return uv.weight; }
};
#endif

#if 0
struct vov_data_edge_mapper {
  using target_graph_t      = vov<double>;
  using source_edge_range_t = data_edges_t;
  using source_edge_t       = data_edges_t::value_type;

  using vertex_label_t = std::string;
  using vertex_index_t = size_t;

  const vertex_label_t& inward_label(const source_edge_t& uv) const { return uv.from; }
  const vertex_label_t& outward_label(const source_edge_t& uv) const { return uv.to; }

  const vertex_index_t& inward_index(const source_edge_t& uv) const { return uv.from; }
  const vertex_index_t& outward_index(const source_edge_t& uv) const { return uv.to; }

  std::edge_value_t<target_graph_t> edge_value(const source_edge_t& uv) const { return uv.weight; }
};
#endif

//---------------------------------------------------------------------------------------
// Translates raw edge data into intermediate forms that can easily be used for
// creating a directed_adjacency_vector or undirected_adjacency_list.
template <typename Mapper>
class GraphXlate {
public:
  using target_graph_t      = typename Mapper::target_graph_t;
  using source_edge_range_t = typename Mapper::source_edge_range_t;
  using source_edge_t       = typename Mapper::source_edge_t;
  using vertex_label_t      = typename Mapper::vertex_label_t;
  using vertex_labels_t     = std::vector<vertex_label_t>;
  using vertex_index_t      = std::graph::vertex_key_t<target_graph_t>;
  using vertex_value_t      = std::graph::vertex_value_t<target_graph_t>;
  using target_edge_t       = std::pair<typename target_graph_t::edge_key_type,
                                  typename target_graph_t::edge_value_type>; // pair{ pair{u_label, v_label}, uv_value }
  using target_edges_t      = std::vector<target_edge_t>;

public:
  GraphXlate(const Mapper& mapper, const source_edge_range_t& edges) : mapper_(mapper) {
    vertex_labels_ = std::move(unique_vertex_labels(edges)); // get list of unique vertex labels from edges (in,out)
    edge_values_   = to_edge_values(edges);                  // convert to new edge list with {{u_idx,v_idx} uv_value}
  }

  target_graph_t create_graph() const {
    return target_graph_t(
          edge_values_, vertex_labels_, [](const target_edge_t& uv) { return uv.first; },
          [](const target_edge_t& er) { return er.second; },
          [](const vertex_label_t& label) -> const vertex_label_t& { return label; });
  }

  const target_edges_t&  edge_values() const { return edge_values_; } // exposed mainly for sanity test
  const vertex_labels_t& vertex_values() const { return vertex_labels_; }

protected:
  vertex_index_t find_vertex_index(const vertex_label_t& label) const {
    return static_cast<vertex_index_t>(::ranges::lower_bound(vertex_labels_, label) - ranges::begin(vertex_labels_));
  }

  // extract unique vertex lables from edges (in, out values)
  // result returned in ordered vector
  template <ranges::forward_range EdgeRng>
  vertex_labels_t unique_vertex_labels(const EdgeRng& uv_rng) const {
    vertex_labels_t vertex_labels;
    for (const auto& uv : uv_rng) {
      vertex_labels.push_back(mapper_.inward_label(uv));
      vertex_labels.push_back(mapper_.outward_label(uv));
    }
    return move(vertex_labels) | ::ranges::actions::sort | ::ranges::actions::unique;
  }

  // extract the vertex indexes (in & out) & value to be used in the target graph's edges
  target_edges_t to_edge_values(const source_edge_range_t& edges) {
    target_edges_t edge_values;
    edge_values.reserve(edges.size());
    for (const source_edge_t& r : edges) {
      vertex_index_t from = find_vertex_index(r.from);
      vertex_index_t to   = find_vertex_index(r.to);
      assert(from < vertex_labels_.size());
      assert(to < vertex_labels_.size());
      edge_values.push_back({{from, to}, mapper_.edge_value(r)}); // pair{ pair{u_label, v_label}, uv_value }
    }
    auto cmp = [](const target_edge_t& lhs, const target_edge_t& rhs) { return lhs.first.first < rhs.first.first; };
    ::ranges::actions::sort(edge_values, cmp);
    return edge_values;
  }

private:
  Mapper          mapper_;
  vertex_labels_t vertex_labels_;
  target_edges_t  edge_values_;
};
