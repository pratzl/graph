//
// This file is part of Standard Graph Library (SGL)
// (c) Pacific Northwest National Laboratory 2018
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#pragma once

#include <tuple>
#include <algorithm>

#include <util.hpp>
#include <plain_range.hpp>

//****************************************************************************
template <typename GraphT>
size_t triangle_count_v1(GraphT &A)
{
    size_t                   triangles = 0;
    counting_output_iterator counter(triangles);
    auto                     first     = A.begin();
    auto                     last      = A.end();

    for (auto G = first; first != last; ++first) {
        for (auto v = (*first).begin(); v != (*first).end(); ++v) {
            // Note that we begin the first set from v, skipping unnecessary comparisons
            std::set_intersection(v, (*first).end(), G[std::get<0>(*v)].begin(), G[std::get<0>(*v)].end(), counter,
                                  [](auto&& x, auto&& y) -> bool { return std::get<0>(x) < std::get<0>(y); });
        }
    }
    return triangles;
}


//****************************************************************************
// Should this work?
template <typename GraphT>
size_t triangle_count_v2(GraphT &A)
{
    size_t                   triangles = 0;
    counting_output_iterator counter(triangles);
    size_t                   i = 0;
    auto                     G = A.begin();
    for (auto&& node : A) {
        for (auto&& [u] : node) {
            // We can't skip unnecessary comparisons because we do not know the position of u
            std::set_intersection(G[i].begin(), G[i].end(), G[u].begin(), G[u].end(), counter,
                                  [](auto&& x, auto&& y) -> bool { return std::get<0>(x) < std::get<0>(y); });
        }
        ++i;
    }
    return triangles;
}

//****************************************************************************
template <typename GraphT>
size_t triangle_count_v3(GraphT &A)
{
    size_t                   triangles = 0;
    counting_output_iterator counter(triangles);

    auto G = A.begin();
    for (auto&& [v, u] : plain_edge_range(A)) {
        // Cannot skip unnecessary comparisons because we don't know the triangular structure and we don't know the positions of u and v in adjacency lists
        std::set_intersection(G[v].begin(), G[v].end(), G[u].begin(), G[u].end(), counter,
                              [](auto&& x, auto&& y) -> bool { return std::get<0>(x) < std::get<0>(y); });
    }
    return triangles;
}
