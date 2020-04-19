#pragma once
#include "graph/compressed_adj_array.hpp"
#include "graph/undirected_adj_list.hpp"

#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/lower_bound.hpp>
#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/range.hpp>

#include <string>
#include <vector>

namespace data {

template <typename Mapper>
class GraphXlate;

// a weight value with double type
struct dbl_weight_value {
  double weight = 0;

  dbl_weight_value()                        = default;
  dbl_weight_value(dbl_weight_value const&) = default;
  dbl_weight_value& operator=(dbl_weight_value const&) = default;
  dbl_weight_value(double const& w) : weight(w) {}
};

struct data_edge;
struct caa_data_edge_mapper;
struct ual_data_edge_mapper;
extern GraphXlate<caa_data_edge_mapper> dollar_directed_graph;
extern GraphXlate<ual_data_edge_mapper> dollar_undirected_graph;

extern GraphXlate<caa_data_edge_mapper> candle_directed_graph;
extern GraphXlate<ual_data_edge_mapper> candle_undirected_graph;


struct data_edge {
  std::string from;
  std::string to;
  double      weight = 0.0;
  data_edge(std::string const& from_vertex, std::string const& to_vertex, double wght)
        : from(from_vertex), to(to_vertex), weight(wght) {}
  data_edge()                 = default;
  data_edge(data_edge const&) = default;
  data_edge& operator=(data_edge const&) = default;
  ~data_edge()                           = default;
};
using data_edges_t    = std::vector<data_edge>;
using vertex_labels_t = std::vector<std::string>;

struct caa_data_edge_mapper {
  using target_graph_t        = std::graph::compressed_adjacency_array<std::graph::name_value, dbl_weight_value>;
  using source_edge_range_t   = data_edges_t;
  using source_edge_t         = data_edges_t::value_type;

  using vertex_label_t = std::string;

  vertex_label_t in_label(source_edge_t const& uv) const { return uv.from;  }
  vertex_label_t out_label(source_edge_t const& uv) const { return uv.to; }

  std::graph::edge_value_t<target_graph_t> edge_value(source_edge_t const& uv) const { return uv.weight; }
};

struct ual_data_edge_mapper {
  using target_graph_t      = std::graph::undirected_adjacency_list<std::graph::name_value, dbl_weight_value>;
  using source_edge_range_t = data_edges_t;
  using source_edge_t       = data_edges_t::value_type;

  using vertex_label_t = std::string;

  vertex_label_t const& in_label(source_edge_t const& uv) const { return uv.from; }
  vertex_label_t const& out_label(source_edge_t const& uv) const { return uv.to; }

  std::graph::edge_value_t<target_graph_t> edge_value(source_edge_t const& uv) const { return uv.weight; }
};


// Translates raw edge data into intermediate forms that can easily be used for
// creating a compressed_adjacency_array or undirected_adjacency_list.
template <typename Mapper>
class GraphXlate {
public:
  using target_graph_t      = typename Mapper::target_graph_t;
  using source_edge_range_t = typename Mapper::source_edge_range_t;
  using source_edge_t       = typename Mapper::source_edge_t;
  using vertex_label_t      = typename Mapper::vertex_label_t;
  using vertex_labels_t     = std::vector<vertex_label_t>;
  using vertex_index_t      = std::graph::vertex_key_t<target_graph_t>;
  using target_edge_t       = typename target_graph_t::edge_value_type; // pair{ pair{u_label, v_label}, uv_value }
  using target_edges_t      = std::vector<target_edge_t>;

public:
  GraphXlate(Mapper const& mapper, source_edge_range_t const& edges) : mapper_(mapper) {
    vertex_labels_ = std::move(unique_vertex_labels(edges)); // get list of unique vertex labels from edges (in,out)
    edge_values_   = to_edge_values(edges);                  // convert to new edge list with {{u_idx,v_idx} uv_value}
  }

  target_graph_t create_graph() const {
    return target_graph_t(
          edge_values_, [](target_edge_t const& uv) { return uv.first; },
          [](target_edge_t const& er) { return er.second; });
  }

protected:
  vertex_index_t find_vertex_index(vertex_label_t const& label) const {
    return static_cast<vertex_index_t>(::ranges::lower_bound(vertex_labels_, label) - ::ranges::begin(vertex_labels_));
  }

  // extract unique vertex lables from edges (in, out values)
  // result returned in ordered vector
  template <::ranges::forward_range EdgeRng>
  vertex_labels_t unique_vertex_labels(EdgeRng const& uv_rng) const {
    vertex_labels_t vertex_labels;
    for (auto const& uv : uv_rng) {
      vertex_labels.push_back(mapper_.in_label(uv));
      vertex_labels.push_back(mapper_.out_label(uv));
    }
    return move(vertex_labels) | ranges::actions::sort | ranges::actions::unique;
  }

  // extract the vertex indexes (in & out) & value to be used in the target graph's edges
  target_edges_t to_edge_values(source_edge_range_t const& edges) {
    using std::graph::vertex_key_t;
    using std::graph::edge_value_t;
    target_edges_t edge_values;
    edge_values.reserve(edges.size());
    for (source_edge_t const& r : edges) {
      vertex_key_t<target_graph_t> from = find_vertex_index(r.from);
      vertex_key_t<target_graph_t> to   = find_vertex_index(r.to);
      assert(from < vertex_labels_.size());
      assert(to < vertex_labels_.size());
      edge_values.push_back({{from, to}, r.weight}); // pair{ pair{u_label, v_label}, uv_value }
    }
    auto cmp = [](typename target_graph_t::edge_value_type const& lhs,
                  typename target_graph_t::edge_value_type const& rhs) { return lhs.first.first < rhs.first.first; };
    ::ranges::sort(edge_values, cmp);
    return edge_values;
  }

private:
  Mapper          mapper_;
  vertex_labels_t vertex_labels_;
  target_edges_t  edge_values_;
};


} // namespace data
