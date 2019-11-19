//
// This file is part of the Graph Standard Library (aka BGL17 aka NWGraph)
// (c) Pacific Northwest National Laboratory
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#pragma once

#include <cassert>
#include <queue>
#include <vector>
#include <tuple>

#include <util.hpp>   // for vertex_index_t
#include <bfs_edge_range.hpp>

//****************************************************************************
/// @todo cannot yet specify const graph.
template <typename DistanceT, typename GraphT>
std::vector<DistanceT> dijkstra(GraphT &graph, vertex_index_t source)
{
    size_t N(graph.end() - graph.begin());
    assert(source < N);

    std::vector<DistanceT> distance(N, 0xDEADBEEF);
    distance[source] = 0;

    using weight_t = DistanceT;
    using weighted_vertex = std::tuple<vertex_index_t, weight_t>;

    std::priority_queue<weighted_vertex,
                        std::vector<weighted_vertex>,
                        std::greater<weighted_vertex>> Q;

    for (auto&& [u, v, w] : bfs_edge_range2(graph, Q)) {
        if (distance[u] + w < distance[v]) {
            distance[v] = distance[u] + w;
            Q.push({v, distance[v]});
        }
    }

    return distance;
}
