//
// Author: Phil Ratzloff
//

#include "../graph.hpp"
#include <vector>

#ifndef GRAPH_COMPONENTS_HPP
#  define GRAPH_COMPONENTS_HPP

namespace std::graph {

template <typename G, integral CompT = uint32_t>
struct component {
  CompT                component_number;
  vertex_iterator_t<G> vertex;

  component(CompT comp, vertex_iterator_t<G> vi) : component_number(comp), vertex(vi) {}

  component()                 = default;
  component(component const&) = default;
  component(component&&)      = default;
  component& operator=(component const&) = default;
  component& operator=(component&&) = default;
  ~component()                      = default;
};


//---------------------------------------------------------------------------------------
// Connected Components (for undirected graphs)
//
template <undirected_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> class con_comp_fn {
public:
  using graph_type            = G;
  using allocator_type        = A;
  using component_number_type = CompT;
  using component_type        = component<graph_type, component_number_type>;

protected:
  using stack_elem  = vertex_iterator_t<G>;
  using stack_alloc = typename allocator_traits<A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

  using visited_alloc = typename allocator_traits<A>::template rebind_alloc<bool>;
  using visited_type  = vector<bool, visited_alloc>;

public:
  con_comp_fn(graph_type& g, A alloc) : graph_(g), alloc_(alloc), stack_(alloc), visited_(alloc) {}

  graph_type&       graph() { return graph_; }
  graph_type const& graph() const { return graph_; }
  allocator_type    allocator() const { return alloc_; }

  void operator()(vertex_iterator_t<G> start, OutIter result_iter) { eval_cc(start, result_iter); }

  void operator()(vertex_range_t<G> rng, OutIter result_iter) {
    for (vertex_iterator_t<G> u = begin(rng); u != end(rng); ++u)
      if (!visited_[vertex_key(graph_, *u)])
        eval_cc(u, result_iter);
  }

protected:
  void eval_cc(vertex_iterator_t<G> seed, OutIter result_iter) {
    stack_.push(seed);
    visited_[vertex_key(graph_, *seed)] = true;
    *result_iter                        = component_type(curr_comp_, seed);

    while (!stack_.empty()) {
      vertex_iterator_t<G> u = stack_.top();
      stack_.pop();
      for (vertex_iterator_t<G> v = begin(graph_, *u); v != end(graph_, *u); ++u) {
        vertex_key_t<G> v_key = vertex_key(graph_, *v, *u);
        if (!visited_[v_key]) {
          stack_.push(v);
          visited_[v_key] = true;
          *result_iter    = component_type(curr_comp_, v);
        }
      }
    }
    ++curr_comp_;
  }

protected:
  graph_type&           graph_;
  allocator_type        alloc_;
  stack_type            stack_;
  visited_type          visited_;
  component_number_type curr_comp_ = 0;
};

template <undirected_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> void
connected_components(G& g, vertex_iterator_t<G> start, OutIter result_iter, A alloc = A()) {
  con_comp_fn cc(g, alloc);
  cc(start, result_iter);
}

template <undirected_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> void
connected_components(G& g, vertex_range_t<G> rng, OutIter result_iter, A alloc = A()) {
  con_comp_fn cc(g, alloc);
  cc(rng, result_iter);
}

//---------------------------------------------------------------------------------------
// Strongly Connected Components (Tarjen's algorithm for directed graphs)
//
template <directed_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> class tarjen_scc_fn {
public:
  using graph_type            = G;
  using allocator_type        = A;
  using component_number_type = CompT;
  using component_type        = component<graph_type, component_number_type>;

public:
  tarjen_scc_fn(graph_type& g, allocator_type alloc = A())
        : graph_(g)
        , alloc_(alloc)
        , stack_(alloc)
        , visited_(vertices_size(g), alloc)
        , in_stack_(vertices_size(g), alloc) {}

  void operator()(vertex_iterator_t<G> start, OutIter result_iter) { eval_scc(start, result_iter); }

  void operator()(vertex_range_t<G> rng, OutIter result_iter) {
    for (vertex_iterator_t<G> it = begin(rng); it != end(rng); ++it)
      eval_scc(it, result_iter);
  }

protected:
  struct visit_elem {
    CompT discovered = undiscovered();
    CompT low        = undiscovered();

    visit_elem()                  = default;
    visit_elem(visit_elem const&) = default;
    visit_elem& operator=(visit_elem const&) = default;
  };
  using visited_alloc = typename allocator_traits<A>::template rebind_alloc<visit_elem>;
  using visited_type  = vector<visit_elem, visited_alloc>;

  using stack_elem  = vertex_iterator_t<G>;
  using stack_alloc = typename allocator_traits<A>::template rebind_alloc<stack_elem>;
  using stack_type  = stack<stack_elem, deque<stack_elem, stack_alloc>>;

  using in_stack_alloc = typename allocator_traits<A>::template rebind_alloc<bool>;
  using in_stack_type  = vector<bool, in_stack_alloc>;

protected:
  constexpr static CompT undiscovered() { return numeric_limits<CompT>::max(); }

  void eval_scc(vertex_iterator_t<G> u, OutIter result_iter) {
    component_number_type const u_key = static_cast<component_number_type>(vertex_key(graph_, *u));
    if (visited_[u_key].discovered != undiscovered())
      return;

    visited_[u_key].discovered = visited_[u_key].low = next_seq_++;
    stack_.push(u);
    in_stack_[u_key] = true;

    for (edge_t<G>& uv : edges(graph_, *u)) {
      vertex_key_t<G> v_key = vertex_key(graph_, uv, *u);
      if (visited_[v_key].discovered == undiscovered()) {
        eval_scc(vertex(graph_, uv, *u), result_iter);
        visited_[v_key].low = std::min(visited_[v_key].low, visited_[u_key].low);
      } else if (in_stack_[v_key])
        visited_[v_key].low = std::min(visited_[v_key].low, visited_[u_key].discovered);
    }

    if (visited_[u_key].low == visited_[u_key].discovered) {
      using result_type = component<G, CompT>;
      while (stack_.top() != u) {
        vertex_iterator_t<G> v     = stack_.top();
        vertex_key_t<G>      v_key = vertex_key(graph_, *v);
        in_stack_[v_key]           = false;
        *result_iter               = result_type(curr_comp_, v);
        stack_.pop();
      }
      vertex_iterator_t<G> v     = stack_.top();
      vertex_key_t<G>      v_key = vertex_key(graph_, *v);
      in_stack_[v_key]           = false;
      *result_iter               = result_type(curr_comp_, v);
      stack_.pop();
      ++curr_comp_;
    }
  }

protected:
  graph_type&           graph_;
  allocator_type        alloc_;
  stack_type            stack_;
  visited_type          visited_;
  in_stack_type         in_stack_;
  component_number_type next_seq_  = 0; // next discovery sequence
  component_number_type curr_comp_ = 0;
};

template <directed_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> void
strongly_connected_components(G& g, vertex_iterator_t<G> start, OutIter result_iter, A alloc = A()) {
  tarjen_scc_fn<G, OutIter, CompT, A> scc(g, alloc);
  scc(start, result_iter);
}

template <directed_graph G, typename OutIter, integral CompT = uint32_t, typename A = allocator<char>>
requires output_iterator<OutIter, component<G, CompT>> void
strongly_connected_components(G& g, vertex_range_t<G> rng, OutIter result_iter, A alloc = A()) {
  tarjen_scc_fn<G, OutIter, CompT, A> scc(g, alloc);
  scc(rng, result_iter);
}


} // namespace std::graph

#endif // GRAPH_COMPONENTS_HPP
