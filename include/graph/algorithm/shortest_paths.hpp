//
// Author: Phil Ratzloff
//
// inspired by dijkstra & bfs_edge_range2 from BGL17
//
// Shortest Paths and Shortest Distances algorithms using Dijkstra & Bellman-Ford algorithms
// for a single source vertex.
//
// The algorithms are designed to work with both directed & undirected graphs
// by using general functions such as vertex() and edges() instead of outward_vertex()
// and outward_edges().
//
// Dijktra's shortest path algorithm runs in O(|E| + |V|log|V|) time and supports non-negative
// edge weights. Signed weight types are allowed to accomodate real-world situations, and it
// is the user's responsibility to assure only non-negative weights exist. Outputting leaf-only
// distances/paths adds an additional O(|V| + |E|).
//
// Bellman-Ford shortest path algorithm runs in O(|V| * |E|) and support negative edge weights.
// It is slower than Dijktra's algorithm but is more versatile because it supports negative
// edge weights. Outputting leaf-only distances/paths adds an additional O(|V| + |E|). Detecting
// negative edge cycles adds an addtional O(|E|).
//
// Both algorithms support shortest-distance and shortest-path varients, where shortest-path
// requires memory allocation of a vector of vertex iterators for each value returned to the
// output iterator passed.
//
// NOTES
//  A vertex range variant isn't included because it's assumes that it would be useful for
//  contiguous vertices in a range. Rather, it would be better to support a collection of vertex
//  iterators to allow for disjoint vertices.
//
//  The output code for distances and paths is the same between Dijkstra & Bellmand-Ford.
//  There's an opportunity to share code by creating a common base class with their implementation
//  and the member variables. However, the amount of code shared isn't large and the extra
//  complexity doesn't seem to warrent that at this time.
//
// TODO
//  1.  Figure how to define DistanceT from DistFnc so user isn't required to specify it.
//      Then move DistFnc parameter later in template.
//  2.  Add output_iterator concept check for parameter
//  3.  Add edges(g) requirement concept to djikstra
//
// ISSUES / QUESTIONS
//  1.  Should bellman_ford always check for negative edge cycles? (e.g. remove
//      detect_neg_edge_cycles parameter?)
//

#include <queue>
#include <vector>
#include "../graph.hpp"
#include "range/v3/algorithm/reverse.hpp"

#define SHORTEST_RANGE

#ifndef GRAPH_SHORTEST_PATHS_HPP
#  define GRAPH_SHORTEST_PATHS_HPP

namespace std::graph {

// forward declarations

template <typename G, typename A = allocator<vertex_iterator_t<G>>>
using vertex_path_t = vector<vertex_iterator_t<G>, A>;

template <typename G, typename A = allocator<edge_iterator_t<G>>>
using vertex_path_range = decltype(ranges::make_subrange(declval<vertex_path_t<G, A>&>()));

template <typename G, typename A = allocator<edge_iterator_t<G>>>
using edge_path_t = vector<edge_iterator_t<G>, A>;

template <typename G, typename A>
using edge_path_range = decltype(ranges::make_subrange(declval<edge_path_t<G, A>&>()));


template <typename G, arithmetic DistanceT, typename A = allocator<vertex_iterator_t<G>>>
struct shortest_path2 {
  DistanceT               distance;
  vertex_path_range<G, A> vertex_path;
  edge_path_range<G, A>   path;
};


//! The return value of the shortest distance functions
template <forward_iterator VertexIteratorT, arithmetic DistanceT>
struct shortest_distance {
  VertexIteratorT first;                  // source vertex
  VertexIteratorT last;                   // last vertex in path
  DistanceT       distance = DistanceT(); // sum of the path's edge distances in the path
};

//! The return value of the shortest path functions
template <forward_iterator VertexIteratorT, arithmetic DistanceT, typename A = allocator<VertexIteratorT>>
struct shortest_path {
  vector<VertexIteratorT, A> path;                   // vertices that make up the path
  DistanceT                  distance = DistanceT(); // sum of the path's edge distances in the path

  shortest_path()                     = default;
  shortest_path(const shortest_path&) = default;
  shortest_path(shortest_path&&)      = default;
  shortest_path& operator=(const shortest_path&) = default;
  shortest_path& operator=(shortest_path&&) = default;
  shortest_path(A alloc) : path(alloc) {}
};

//! Internal implementation of the Dijstra algorithm.
//!
template <typename G, typename DistFnc, typename DistanceT, typename A = allocator<DistanceT>>
class dijkstra_fn {
  enum colors : int8_t {
    white, // undiscovered
    grey,  // discovered (in-process)
    black  // visited; all edges have been visited
  };

  struct vertex_dist { // --> template<G,DistanceT> path_detail; move outside function
    vertex_key_t<G> vtx_key  = numeric_limits<vertex_key_t<G>>::max();
    DistanceT       distance = numeric_limits<DistanceT>::max(); // distance from source
    bool            operator<(const vertex_dist& rhs) const {
      return distance > rhs.distance; // > so top has lowest distance in priority_queue
    }
  };
  using vertex_dist_cont = vector<vertex_dist>;

public:
  using graph_t        = G;
  using distance_fnc_t = DistFnc;
  using allocator_t    = A;

  dijkstra_fn(
        graph_t&    g,
        DistFnc     distance_fnc = [](edge_value_t<G>&) -> size_t { return 1; },
        allocator_t alloc        = allocator_t())
        : g_(g), distance_fnc_(distance_fnc), alloc_(alloc) {}

  //template <ranges::output_iterator<shortest_distance<vertex_iterator_t<G>, DistanceT>> OutIter>
  template <typename OutIter>
  void shortest_distances(vertex_iterator_t<G> source, OutIter result_iter, bool const leaves_only) {
    // find the paths
    vertex_dist_cont distances(vertices_size(g_), alloc_);
    vector<bool>     leaf(vertices_size(g_), alloc_);
    find_paths(source, distances, leaves_only, leaf);

    // output distances to the output iterator
    //using path                 = shortest_distance<vertex_iterator_t<G>, DistanceT>;
    using key_t                = vertex_key_t<G>;
    vertex_iterator_t<G> first = begin(vertices(g_));
    for (key_t vkey = 0; vkey < static_cast<key_t>(distances.size()); ++vkey) {
      if (!leaves_only || (leaves_only && leaf[vkey])) {
        auto& [prev_key, dist] = distances[vkey];
        *result_iter           = {source, first + vkey, dist};
      }
    }
  }

  template <typename OutIter>
  void shortest_paths(vertex_iterator_t<G> source, OutIter result_iter, bool const leaves_only) {
    // find the paths
    vertex_dist_cont distances(vertices_size(g_), alloc_);
    vector<bool>     leaf(vertices_size(g_), alloc_);
    find_paths(source, distances, leaves_only, leaf);

    // output paths to the output iterator
    using path_t = shortest_path<vertex_iterator_t<G>, DistanceT>;
    using key_t  = vertex_key_t<G>;
    path_t               spath(alloc_);
    key_t const          source_key = static_cast<vertex_key_t<G>>(source - begin(g_));
    vertex_iterator_t<G> first      = begin(vertices(g_));
    for (key_t vkey = 0; vkey < static_cast<key_t>(distances.size()); ++vkey) {
      if (!leaves_only || (leaves_only && leaf[vkey])) {
        spath.distance = distances[vkey].distance;
        for (key_t ukey = vkey; ukey != source_key; ukey = distances[ukey].vtx_key) {
          spath.path.push_back(first + ukey);
        }
        spath.path.push_back(source);

        ranges::reverse(spath.path);
        *result_iter = spath;
        spath.path.clear();
      }
    }
  }

protected:
  //! Find the shortest paths. Caller can determine how to transform the results to a higher level.
  //!
  //! @param source   The source vertex to start the search at
  //! @param distance The distance vertex which holds vertex_dist values, where the vtx_key holds
  //!                 the key for the previous vertex. It is assumed to be pre-initialized to be
  //!                 the number of vertices in the graph.
  //! @param leaf     A bool vertex where leaf[k]==true indicates distance[k] is the last vertex
  //!                 in a path. It is assumed to be pre-initialized to be the number of vertices
  //!                 in the graph with value of false.
  void
  find_paths(vertex_iterator_t<G> source, vertex_dist_cont& distances, bool const leaves_only, vector<bool>& leaf) {
    vertex_key_t<G> const source_key = vertex_key(g_, *source);
    distances[source_key]            = {source_key, 0};

    struct q_vertex_dist { // --> template<G,DistanceT> path_detail; move outside function
      vertex_key_t<G> vtx_key    = numeric_limits<vertex_key_t<G>>::max();
      DistanceT       distance   = numeric_limits<DistanceT>::max(); // distance from source
      vertex_key_t<G> parent_key = numeric_limits<vertex_key_t<G>>::max();
      bool            operator<(const q_vertex_dist& rhs) const {
        return distance > rhs.distance; // > so top has lowest distance in priority_queue
      }
    };
    priority_queue<q_vertex_dist, vector<q_vertex_dist>> q(alloc_);
    vector<bool>                                         in_q(vertices_size(g_), alloc_);

    q.push({source_key, 0});
    in_q[source_key] = true;
    while (!q.empty()) {
      // next vertex
      auto ukey       = q.top().vtx_key;
      auto parent_key = q.top().parent_key;
      q.pop();
      in_q[ukey] = false;

      // thru u's edges
      for (edge_t<G>& uv : edges(g_, *find_vertex(g_, ukey))) {
        DistanceT       v_dist = distances[ukey].distance + distance_fnc_(uv);
        vertex_key_t<G> vkey   = vertex_key(g_, uv, ukey);
        if (vkey != parent_key)
          leaf[ukey] = false;

        // new shorter distance to v?
        if (v_dist < distances[vkey].distance) {
          distances[vkey] = {ukey, v_dist}; // {prev,dist}
          leaf[vkey]      = true;
          if (!in_q[vkey]) {
            q.push({vkey, v_dist, ukey}); // {vtx,dist}
            in_q[vkey] = true;
          }
        }
      }
    }

    // Identify the leaves, if needed (only needed for undirected graphs)
    if (leaves_only) {
      // identify all vertices that are reachable by source
      size_t reached = 0;
      for (vertex_key_t<G> vkey = 0; vkey < distances.size(); ++vkey) {
        if (distances[vkey].vtx_key != numeric_limits<vertex_key_t<G>>::max()) {
          leaf[vkey] = true;
          ++reached;
        }
      }
      // turn off leaf for vertices that are previous to other vertices
      if (reached > 1) {
        for (edge_t<G>& uv : edges(g_))
          if (outward_vertex_key(g_, uv) != numeric_limits<vertex_key_t<G>>::max())
            leaf[inward_vertex_key(g_, uv)] = false;
      }
    }
  }

protected:
  graph_t&       g_;
  distance_fnc_t distance_fnc_;
  allocator_t    alloc_;
};


//! Internal implementation of the Bellman-Ford algorithm.
//!
template <typename G, typename DistFnc, typename DistanceT, typename A = allocator<DistanceT>>
class bellman_ford_fn {
  struct vertex_dist { // --> template<G,DistanceT> path_detail; move outside function
    vertex_key_t<G> vtx_key  = numeric_limits<vertex_key_t<G>>::max();
    DistanceT       distance = numeric_limits<DistanceT>::max();
    bool            operator<(const vertex_dist& rhs) const {
      return distance > rhs.distance; // > so top has lowest distance in priority_queue
    }
  };
  using vertex_dist_cont = vector<vertex_dist>;

public:
  using graph_t        = G;
  using distance_fnc_t = DistFnc;
  using allocator_t    = A;

  bellman_ford_fn(
        graph_t&    g,
        DistFnc     distance_fnc = [](edge_value_t<G>&) -> size_t { return 1; },
        allocator_t alloc        = allocator_t())
        : g_(g), distance_fnc_(distance_fnc), alloc_(alloc) {}

  //template <ranges::output_iterator<shortest_distance<vertex_iterator_t<G>, DistanceT>> OutIter>
  template <typename OutIter>
  bool shortest_distances(vertex_iterator_t<G> source,
                          OutIter              result_iter,
                          bool const           leaves_only,
                          bool const           detect_neg_edge_cycles) {
    // find the paths
    vertex_dist_cont distance(vertices_size(g_), alloc_);
    vector<bool>     leaf(vertices_size(g_), alloc_);
    bool             neg_edge_cycles = find_paths(source, distance, leaf, leaves_only, detect_neg_edge_cycles);
    if (neg_edge_cycles)
      return true;

    // output distances to the output iterator
    //using path                 = shortest_distance<vertex_iterator_t<G>, DistanceT>;
    using key_t                = vertex_key_t<G>;
    vertex_iterator_t<G> first = begin(vertices(g_));
    for (key_t vkey = 0; vkey < static_cast<key_t>(distance.size()); ++vkey) {
      if (!leaves_only || (leaves_only && leaf[vkey])) {
        auto& [prev_key, dist] = distance[vkey];
        *result_iter           = {source, first + vkey, dist};
      }
    }
    return false;
  }

  template <typename OutIter>
  bool shortest_paths(vertex_iterator_t<G> source,
                      OutIter              result_iter,
                      bool const           leaves_only,
                      bool const           detect_neg_edge_cycles) {
    // find the paths
    vertex_dist_cont distances(vertices_size(g_), alloc_);
    vector<bool>     leaf(vertices_size(g_), alloc_);
    bool             neg_edge_cycles = find_paths(source, distances, leaf, leaves_only, detect_neg_edge_cycles);
    if (neg_edge_cycles)
      return true;

    // output paths to the output iterator
    using path_t = shortest_path<vertex_iterator_t<G>, DistanceT>;
    using key_t  = vertex_key_t<G>;
    path_t               spath(alloc_);
    key_t const          source_key = static_cast<vertex_key_t<G>>(source - begin(g_));
    vertex_iterator_t<G> first      = begin(vertices(g_));
    for (key_t vkey = 0; vkey < static_cast<key_t>(distances.size()); ++vkey) {
      if (!leaves_only || (leaves_only && leaf[vkey])) {
        spath.distance = distances[vkey].distance;
        for (key_t ukey = vkey; ukey != source_key; ukey = distances[ukey].vtx_key) {
          spath.path.push_back(first + ukey);
        }
        spath.path.push_back(source);

        ranges::reverse(spath.path);
        *result_iter = spath;
        spath.path.clear();
      }
    }
    return false;
  }

protected:
  bool find_paths(vertex_iterator_t<G> source,
                  vertex_dist_cont&    distances,
                  vector<bool>&        leaf,
                  bool const           leaves_only,
                  bool const           detect_neg_edge_cycles) {
    vertex_key_t<G> const source_key = static_cast<vertex_key_t<G>>(source - begin(g_));
    distances[source_key]            = {source_key, 0};

    // find the shortest paths
    bool changed = true; // allows exiting early once results are stable
    for (size_t i = 1; changed && i < vertices_size(g_); ++i) {
      changed = false;
      for (edge_t<G>& uv : edges(g_)) {
        vertex_key_t<G> ukey = inward_vertex_key(g_, uv);
        if (distances[ukey].vtx_key == numeric_limits<vertex_key_t<G>>::max())
          continue; // ukey not connected to source [yet]

        vertex_key_t<G> vkey   = outward_vertex_key(g_, uv);
        DistanceT       v_dist = distances[ukey].distance + distance_fnc_(uv);

        if (v_dist < distances[vkey].distance) {
          distances[vkey] = {ukey, v_dist};
          changed         = true;
        }
      }
    }

    // Identify the leaves, if needed
    if (leaves_only) {
      // identify all vertices that are reachable by source
      size_t reached = 0;
      for (vertex_key_t<G> vkey = 0; vkey < distances.size(); ++vkey) {
        if (distances[vkey].vtx_key != numeric_limits<vertex_key_t<G>>::max()) {
          leaf[vkey] = true;
          ++reached;
        }
      }
      // turn off leaf for vertices that are previous to other vertices
      if (reached > 1) {
        for (edge_t<G>& uv : edges(g_))
          if (outward_vertex_key(g_, uv) != numeric_limits<vertex_key_t<G>>::max())
            leaf[inward_vertex_key(g_, uv)] = false;
      }
    }

    // Detect negative edge cycles, if desired
    bool neg_edge_cycles = false;
    if (detect_neg_edge_cycles) {
      for (edge_t<G>& uv : edges(g_)) {
        vertex_key_t<G> ukey = inward_vertex_key(g_, uv);
        if (distances[ukey].vtx_key == numeric_limits<vertex_key_t<G>>::max())
          continue; // ukey not connected to source

        vertex_key_t<G> vkey = outward_vertex_key(g_, uv);
        if (distances[ukey].distance + distance_fnc_(uv) < distances[vkey].distance) {
          neg_edge_cycles = true;
          break;
        }
      }
    }

    return neg_edge_cycles;
  }

protected:
  graph_t&       g_;
  distance_fnc_t distance_fnc_;
  allocator_t    alloc_;
};


//! Find the shortest distances to vertices reachable from the source vertex.
//!
//! @param g           The graph
//! @param source      The single source vertex to start the search.
//! @param result_iter The output iterator that results are written to. The iterator
//!                    must accept a type of shortest_distance<vertex_iterator_t<G>, DistanceT>.
//! @param leaves_only When false, all vertices are written to the output iterator.
//!                    When true, only vertices that are the end of a path are written
//!                    to the output iterator.
//! @param distance_fnc
//!                    The weight function object used to determine the distance between
//!                    vertices on an edge. The default is to return a value of 1.
//! @param alloc       The allocator to use for internal containers.
//
template <typename G, typename OutIter, typename DistFnc, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>> void dijkstra_shortest_distances(
      G&                   g,
      vertex_iterator_t<G> source,
      OutIter              result_iter,
      bool const           leaves_only  = true,
      DistFnc              distance_fnc = [](edge_value_t<G>&) -> size_t { return 1; },
      A                    alloc        = A()) {

  using distance_t = decltype(distance_fnc(*ranges::begin(edges(g, *begin(g)))));
  dijkstra_fn<G, DistFnc, distance_t, A> fn(g, distance_fnc, alloc);
  fn.shortest_distances(source, result_iter, leaves_only);
}


//! Find the shortest paths to vertices reachable from the source vertex.
//!
//! @param g           The graph
//! @param source      The single source vertex to start the search.
//! @param result_iter The output iterator that results are written to. The iterator
//!                    must accept a type of shortest_path<vertex_iterator_t<G>, DistanceT>.
//! @param leaves_only When false, the shortest path is written to the output iterator for each
//!                    vertex reachable from source.
//!                    When true, only paths to final vertices at the end of a path are written
//!                    to the output iterator.
//! @param distance_fnc
//!                    The weight function object used to determine the distance between
//!                    vertices on an edge. The default is to return a value of 1.
//! @param alloc       The allocator to use for internal containers.
//
template <typename G, typename OutIter, typename DistFnc, typename A = allocator<char>>
//requires (edge_t<G>& uv) { output_iterator<OutIter, typename OutIter::value_type> && Distant && DistFnc(uv) -> arithmetic; }
requires integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>> void dijkstra_shortest_paths(
      G&                   g,
      vertex_iterator_t<G> source,
      OutIter              result_iter,
      bool const           leaves_only  = true,
      DistFnc              distance_fnc = [](edge_value_t<G>&) -> size_t { return 1; },
      A                    alloc        = A()) {
  using distance_t = decltype(distance_fnc(*ranges::begin(edges(g, *begin(g)))));
  dijkstra_fn<G, DistFnc, distance_t, A> fn(g, distance_fnc, alloc);
  fn.shortest_paths(source, result_iter, leaves_only);
}


//! Find the shortest distances to vertices reachable from the source vertex.
//!
//! @param g           The graph
//! @param source      The single source vertex to start the search.
//! @param result_iter The output iterator that results are written to. The iterator
//!                    must accept a type of shortest_distance<vertex_iterator_t<G>, DistanceT>.
//!                    Nothing will be output to this if detect_neg_edge_cycles is true and
//!                    a negative edge cycle exists.
//! @param leaves_only When false, all vertices are written to the output iterator.
//!                    When true, only vertices that are the end of a path are written
//!                    to the output iterator with an additional cost of O(|V|+|E|).
//! @param detect_neg_edge_cycles
//!                    Detects if a negateve edge cycle exists. When true, an additional O(|E|)
//!                    pass is made and true/false is returned to identify if a negateve edge
//!                    cycle exists.
//! @param distance_fnc
//!                    The weight function object used to determine the distance between
//!                    vertices on an edge. The default is to return a value of 1.
//! @param alloc       The allocator to use for internal containers.
//! @return            true if a negateve edge cycle exists, which is only detected if
//!                    detect_neg_edge_cycles is true.
//
template <typename G, typename OutIter, typename DistFnc, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>> bool
                                     bellman_ford_shortest_distances(
                                           G&                   g,
                                           vertex_iterator_t<G> source,
                                           OutIter              result_iter,
                                           bool const           leaves_only            = true,
                                           bool const           detect_neg_edge_cycles = true,
                                           DistFnc              distance_fnc = [](edge_value_t<G>&) -> size_t { return 1; },
                                           A                    alloc        = A()) {

  using distance_t = decltype(distance_fnc(*ranges::begin(edges(g, *begin(g)))));
  bellman_ford_fn<G, DistFnc, distance_t, A> fn(g, distance_fnc, alloc);
  return fn.shortest_distances(source, result_iter, leaves_only, detect_neg_edge_cycles);
}


//! Find the shortest paths to vertices reachable from the source vertex.
//!
//! @param g           The graph
//! @param source      The single source vertex to start the search.
//! @param result_iter The output iterator that results are written to. The iterator
//!                    must accept a type of shortest_path<vertex_iterator_t<G>, DistanceT>.
//!                    Nothing will be output to this if detect_neg_edge_cycles is true and
//!                    a negative edge cycle exists.
//! @param leaves_only When false, the shortest path is written to the output iterator for each
//!                    vertex reachable from source.
//!                    When true, only paths to final vertices at the end of a path are written
//!                    to the output iterator with an additional cost of O(|V|+|E|).
//! @param detect_neg_edge_cycles
//!                    Detects if a negateve edge cycle exists. When true, an additional O(|E|)
//!                    pass is made and true/false is returned to identify if a negateve edge
//!                    cycle exists.
//! @param distance_fnc
//!                    The weight function object used to determine the distance between
//!                    vertices on an edge. The default is to return a value of 1.
//! @param alloc       The allocator to use for internal containers.
//! @return            true if a negateve edge cycle exists, which is only detected if
//!                    detect_neg_edge_cycles is true.
//!
template <typename G, typename OutIter, typename DistFnc, typename A = allocator<char>>
requires integral<vertex_key_t<G>>&& ranges::random_access_range<vertex_range_t<G>>
      //requires output_iterator<OutIter, typename OutIter::value_type>
      bool bellman_ford_shortest_paths(
            G&                   g,
            vertex_iterator_t<G> source,
            OutIter              result_iter,
            bool const           leaves_only            = true,
            bool const           detect_neg_edge_cycles = true,
            DistFnc              distance_fnc           = [](edge_value_t<G>&) -> size_t { return 1; },
            A                    alloc                  = A()) {
  //static_assert(is_same<invoke_result<DistFnc(edge_value_t<G>&)>, DistanceT>::value);
  using distance_t = decltype(distance_fnc(*ranges::begin(edges(g, *begin(g)))));
  bellman_ford_fn<G, DistFnc, distance_t, A> fn(g, distance_fnc, alloc);
  return fn.shortest_paths(source, result_iter, leaves_only, detect_neg_edge_cycles);
}


} // namespace std::graph

#endif //GRAPH_SHORTEST_PATHS_HPP
