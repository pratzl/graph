#include "pch.h"
#include "data_routes.hpp"
#include <ostream>
#include <range/v3/action/unique.hpp>

using std::vector;
using std::string;

routes_t const germany_routes{
      {"Frankfürt", "Mannheim", 85},  {"Frankfürt", "Würzburg", 217},
      {"Frankfürt", "Kassel", 173},   {"Mannheim", "Karlsruhe", 80},
      {"Karlsruhe", "Augsburg", 250}, {"Augsburg", "München", 84},
      {"Würzburg", "Erfurt", 186},    {"Würzburg", "Nürnberg", 103},
      {"Nürnberg", "Stuttgart", 183}, {"Nürnberg", "München", 167},
      {"Kassel", "München", 502}};

vector<string> germany_cities = unique_cities(germany_routes);

std::vector<std::string> unique_cities(routes_t const& routes) {
  vector<string> cities;
  cities.reserve(routes.size() * 2);
  for (route const& r : routes) {
    cities.push_back(r.from);
    cities.push_back(r.to);
  }
  return move(cities) | ranges::actions::sort | ranges::actions::unique;
}
