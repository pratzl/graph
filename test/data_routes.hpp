#pragma once

#include <vector>
#include <string>
#include "graph/graph_fwd.hpp"
#include <range/v3/algorithm/find.hpp>
#include <range/v3/action/sort.hpp>

// forward declarations
struct route;
using routes_t     = std::vector<route>;
using route_city_t = std::vector<std::string>;

// data
extern routes_t const germany_routes;

// data structures
struct route {
  std::string from;
  std::string to;
  int         km = 0;

  route(std::string const& from_city, std::string const& to_city, int kilometers)
        : from(from_city), to(to_city), km(kilometers) {}
};


std::vector<std::string> unique_cities(routes_t const& routes);

template <typename G>
std::vector<typename G::edge_value_type> to_edge_values(routes_t const& routes, std::vector<std::string> const& cities) {
  using std::graph::vertex_key_t;
  using std::graph::edge_value_t;
  std::vector<G::edge_value_type> edge_values;
  edge_values.reserve(routes.size());
  for (route const& r : routes) {
    vertex_key_t<G> from =
          static_cast<vertex_key_t<G>>(::ranges::find(cities, r.from) - cities.begin());
    vertex_key_t<G> to =
          static_cast<vertex_key_t<G>>(::ranges::find(cities, r.to) - cities.begin());
    assert(from < cities.size());
    assert(to < cities.size());
    edge_values.push_back({{from, to}, r.km});
  }
  auto cmp = [](typename G::edge_value_type const& lhs,
                typename G::edge_value_type const& rhs) {
    return lhs.first.first < rhs.first.first;
  };
  ::ranges::sort(edge_values, cmp);
  return edge_values;
}

