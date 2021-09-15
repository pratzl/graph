#include <concepts>

#ifndef GRAPH_ACCESS_HPP
#  define GRAPH_ACCESS_HPP

namespace std::graph {
namespace _detail {
  //template <typename T>
  //inline constexpr bool _borrowed_range = enable_borrowed_range<uncvref_t<T>>;

  //template <typename T>
  //inline constexpr bool _borrowed_range<T&> = true;

  template <typename T>
  constexpr decay_t<T> _decay_copy(T&& _t) noexcept(is_nothrow_convertible_v<T, decay_t<T>>) {
    return forward<T>(_t);
  }
} // namespace _detail


namespace _vertices_ {
  template <typename T>
  void vertices(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_member = requires(G&& g) {
    { forward<G>(g).vertices() } -> ranges::forward_range;
  };
  template <typename G>
  concept _gph_has_ADL = requires(G&& g) {
    is_reference_v<G>;
    { vertices(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G>
  concept _gph_has_rng = ranges::forward_range<G>;

  template <typename G, typename VI>
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { u->vertices(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { vertices(forward<G>(g), u) } -> ranges::forward_range;
  };

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G> || _gph_has_rng<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G&>().vertices()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(vertices(declval<G&>())));
      else if constexpr (_gph_has_rng<G>)
        return noexcept(_detail::_decay_copy(declval<G&>()));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertices(declval<G&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertices(declval<G&>(), declval<VI>())));
    }

  public:
    // g.vertices(), vertices(g), g
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G> || _gph_has_rng<G>
    constexpr auto& operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_member<G>)
        return forward<G>(g).vertices();
      else if constexpr (_gph_has_ADL<G>)
        return vertices(forward<G>(g));
      else if constexpr (_gph_has_rng<G>)
        return g;
    }

    // u->vertices(g), vertices(g,u)
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return u->vertices(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertices(forward<G>(g), u);
    }
  };
} // namespace _vertices_

inline namespace _cpo_ {
  inline constexpr _vertices_::fn vertices{};
}


namespace _vertex_key_ {
  template <typename T>
  void vertex_key(std::initializer_list<T>) = delete;

  template <typename G, typename VI>
  concept _vtx_has_member = forward_iterator<VI> && requires(G&& g, VI u) {
    {u->vertex_key(forward<G>(g))};
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = forward_iterator<VI> && requires(G&& g, VI u) {
    {vertex_key(forward<G>(g), u)};
  };
  template <typename G, typename VI>
  concept _vtx_is_rng = random_access_iterator<VI>;

  struct fn {
  private:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_is_rng<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertex_key(declval<G&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertex_key(declval<G&>(), declval<VI>())));
      else if constexpr (_vtx_is_rng<G, VI>)
        return noexcept(declval<VI>() - declval<VI>());
    }

  public:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_is_rng<G, VI>
    constexpr auto operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return u->vertex_key(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertex_key(forward<G>(g), u);
      else if constexpr (_vtx_is_rng<G, VI>)
        return static_cast<size_t>(u - ranges::begin(vertices(g))); // default impl if not defined
    }
  };
} // namespace _vertex_key_

inline namespace _cpo_ {
  inline constexpr _vertex_key_::fn vertex_key{};
}

namespace _vertex_value_ {
  template <typename T>
  void vertex_value(std::initializer_list<T>) = delete;

  template <typename G, typename VI>
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    {u->vertex_value(forward<G>(g))};
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    forward_iterator<VI>;
    {vertex_value(forward<G>(g), u)};
  };

  struct fn {
  private:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertex_value(declval<G&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertex_value(declval<G&>(), declval<VI>())));
    }

  public:
    // u->vertex_value(g), vertex_value(g,u)
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return u->vertex_value(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertex_value(forward<G>(g), u);
    }
  };
} // namespace _vertex_value_

inline namespace _cpo_ {
  inline constexpr _vertex_value_::fn vertex_value{};
}

namespace _find_vertex_ {
  template <typename T>
  void find_vertex(std::initializer_list<T>) = delete;

  template <typename G, typename VK>
  concept _vtx_has_member = requires(G&& g, VK ukey) {
    { g.find_vertex(ukey) } -> forward_iterator;
  };
  template <typename G, typename VK>
  concept _vtx_has_ADL = requires(G&& g, VK ukey) {
    { find_vertex(forward<G>(g), ukey) } -> forward_iterator;
  };
  template <typename G, typename VK>
  concept _vtx_has_rng = true; //integral<VK> && ranges::random_access_range<vertex_range_t<G>>;

  struct fn {
  private:
    template <typename G, typename VK>
    requires _vtx_has_member<G, VK> || _vtx_has_ADL<G, VK> || _vtx_has_rng<G, VK>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VK>)
        return noexcept(_detail::_decay_copy(declval<G&>().find_vertex(declval<VK>())));
      else if constexpr (_vtx_has_ADL<G, VK>)
        return noexcept(_detail::_decay_copy(find_vertex(declval<G&>(), declval<VK>())));
      else if constexpr (_vtx_has_rng<G, VK>)
        return noexcept(ranges::begin(vertices(declval<G&>())) + declval<VK>());
    }

  public:
    // u->find_vertex(g), find_vertex(g,u)
    template <typename G, typename VK>
    requires _vtx_has_member<G, VK> || _vtx_has_ADL<G, VK> || _vtx_has_rng<G, VK>
    constexpr auto operator()(G&& g, VK ukey) const noexcept(_vtx_fnc_except<G, VK>()) {
      if constexpr (_vtx_has_member<G, VK>)
        return g.find_vertex(ukey);
      else if constexpr (_vtx_has_ADL<G, VK>)
        return find_vertex(forward<G>(g), ukey);
      else if constexpr (_vtx_has_rng<G, VK>)
        if (static_cast<ranges::range_size_t<vertex_range_t<G>>>(ukey) < ranges::size(vertices(g)))
          return ranges::begin(vertices(g)) + static_cast<ranges::range_difference_t<vertex_range_t<G>>>(ukey);
        else
          return ranges::end(vertices(g));
    }
  };
} // namespace _find_vertex_

inline namespace _cpo_ {
  inline constexpr _find_vertex_::fn find_vertex{};
}

namespace _edges_ {
  template <typename T>
  void edges(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_member = requires(G&& g) {
    { g.edges() } -> ranges::forward_range;
  };
  template <typename G>
  concept _gph_has_ADL = requires(G&& g) {
    is_reference_v<G>;
    { edges(forward<G>(g)) } -> ranges::forward_range;
  };

  template <typename G, typename VI>
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { u->edges(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { edges(forward<G>(g), u) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_rng = ranges::forward_range<decltype(*declval<VI>())>;

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G&>().edges()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(edges(declval<G&>())));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_has_rng<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->edges(declval<G&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(edges(declval<G&>(), declval<VI>())));
      else if constexpr (_vtx_has_rng<G, VI>)
        return noexcept(_detail::_decay_copy(*declval<VI>()));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_has_rng<G, VI> //
    using _vtx_retval = conditional_t<
          _vtx_has_member<G, VI>,
          decltype(declval<VI>()->edges(declval<G&>())),
          conditional_t<_vtx_has_ADL<G, VI>, decltype(edges(declval<G&>(), declval<VI>())), decltype(*declval<VI>())>>;

  public:
    // edges(g), g->edges()
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    constexpr auto& operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_member<G>)
        return forward<G>(g).edges();
      else if constexpr (_gph_has_ADL<G>)
        return edges(forward<G>(g));
    }

    // edges(g,u), u->edges(g), *u
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_has_rng<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) /*-> _vtx_retval<G, VI>*/ {
      if constexpr (_vtx_has_member<G, VI>)
        return u->edges(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return edges(forward<G>(g), u);
      else if constexpr (_vtx_has_rng<G, VI>)
        return *u;
    }
  };
} // namespace _edges_

inline namespace _cpo_ {
  inline constexpr _edges_::fn edges{};
}


namespace _edge_key_ {
  template <typename T>
  void edge_key(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = forward_iterator<EI> && requires(G&& g, EI uv) {
    {uv->edge_key(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = forward_iterator<EI> && requires(G&& g, EI uv) {
    {edge_key(forward<G>(g), uv)};
  };
  template <typename G, typename EI>
  concept _edg_has_src = forward_iterator<EI> && requires(G&& g, EI uv) {
    {source_key(g, uv)};
    {target_key(g, uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_src<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->edge_key(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(edge_key(declval<G&>(), declval<EI>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(
              pair(source_key(declval<G&>(), declval<EI>()), target_key(declval<G&>(), declval<EI>()))));
    }

  public:
    // uv->edge_key(g), edge_key(g,uv), (uv - begin(vertices(g)))
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_src<G, EI>
    constexpr auto operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->edge_key(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return edge_key(forward<G>(g), uv);
      else if constexpr (_edg_has_src<G, EI>)
        return pair(source_key(g, uv), target_key(g, uv));
    }
  };
} // namespace _edge_key_

inline namespace _cpo_ {
  inline constexpr _edge_key_::fn edge_key{};
}


namespace _edge_value_ {
  template <typename T>
  void edge_value(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {uv->edge_value(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {edge_value(forward<G>(g), uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->edge_value(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(edge_value(declval<G&>(), declval<EI>())));
    }

  public:
    // uv->edge_value(g), edge_value(g,uv)
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    constexpr auto& operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->edge_value(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return edge_value(forward<G>(g), uv);
    }
  };
} // namespace _edge_value_

inline namespace _cpo_ {
  inline constexpr _edge_value_::fn edge_value{};
}


namespace _source_key_ {
  template <typename T>
  void source_key(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = forward_iterator<EI> && requires(G&& g, EI uv) {
    {uv->source_key(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = forward_iterator<EI> && requires(G&& g, EI uv) {
    {source_key(forward<G>(g), uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->source_key(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(source_key(declval<G&>(), declval<EI>())));
    }

  public:
    // uv->source_key(g), source_key(g,uv)
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    constexpr auto operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->source_key(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return source_key(forward<G>(g), uv);
    }
  };
} // namespace _source_key_

inline namespace _cpo_ {
  inline constexpr _source_key_::fn source_key{};
}


namespace _source_ {
  template <typename T>
  void source(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {uv->source(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {source(forward<G>(g), uv)};
  };
  template <typename G, typename EI>
  concept _edg_has_skey = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {source_key(forward<G>(g), uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_skey<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->source(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(source(declval<G&>(), declval<EI>())));
      else if constexpr (_edg_has_skey<G, EI>)
        return noexcept(
              _detail::_decay_copy(begin(vertices(declval<G&>())) + source_key(declval<G&>(), declval<EI>())));
    }

  public:
    // uv->source(g), source(g,uv)
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_skey<G, EI>
    constexpr auto operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->source(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return source(forward<G>(g), uv);
      else if constexpr (_edg_has_skey<G, EI>)
        return begin(vertices(g)) + source_key(g, uv);
    }
  };
} // namespace _source_

inline namespace _cpo_ {
  inline constexpr _source_::fn source{};
}


namespace _target_key_ {
  template <typename T>
  void target_key(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = forward_iterator<EI> && requires(G&& g, EI uv) {
    {uv->target_key(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = forward_iterator<EI> && requires(G&& g, EI uv) {
    {target_key(forward<G>(g), uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->target_key(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(target_key(declval<G&>(), declval<EI>())));
    }

  public:
    // uv->target_key(g), target_key(g,uv)
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI>
    constexpr auto operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->target_key(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return target_key(forward<G>(g), uv);
    }
  };
} // namespace _target_key_

inline namespace _cpo_ {
  inline constexpr _target_key_::fn target_key{};
}


namespace _target_ {
  template <typename T>
  void target(std::initializer_list<T>) = delete;

  template <typename G, typename EI>
  concept _edg_has_member = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {uv->target(forward<G>(g))};
  };
  template <typename G, typename EI>
  concept _edg_has_ADL = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {target(forward<G>(g), uv)};
  };
  template <typename G, typename EI>
  concept _edg_has_tkey = requires(G&& g, EI uv) {
    forward_iterator<EI>;
    {target_key(g, uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_tkey<G, EI>
    static consteval bool _edg_fnc_except() {
      if constexpr (_edg_has_member<G, EI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->target(declval<G&>())));
      else if constexpr (_edg_has_ADL<G, EI>)
        return noexcept(_detail::_decay_copy(target(declval<G&>(), declval<EI>())));
      else if constexpr (_edg_has_tkey<G, EI>)
        return noexcept(
              _detail::_decay_copy(begin(vertices(declval<G&>())) + target_key(declval<G&>(), declval<EI>())));
    }

  public:
    // uv->target(g), target(g,uv)
    template <typename G, typename EI>
    requires _edg_has_member<G, EI> || _edg_has_ADL<G, EI> || _edg_has_tkey<G, EI>
    constexpr auto operator()(G&& g, EI uv) const noexcept(_edg_fnc_except<G, EI>()) {
      if constexpr (_edg_has_member<G, EI>)
        return uv->target(forward<G>(g));
      else if constexpr (_edg_has_ADL<G, EI>)
        return target(forward<G>(g), uv);
      else if constexpr (_edg_has_tkey<G, EI>)
        return begin(vertices(g)) + target_key(g, uv);
    }
  };
} // namespace _target_

inline namespace _cpo_ {
  inline constexpr _target_::fn target{};
}


namespace _graph_value_ {
  template <typename T>
  void graph_value(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_member = requires(G& g) {
    {g.graph_value()};
  };
  template <typename G>
  concept _gph_has_ADL = requires(G& g) {
    {graph_value(g)};
  };

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G&>().graph_value()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(graph_value(declval<G&>())));
    }

  public:
    // g.graph_value(), graph_value(g)
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    constexpr auto& operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_member<G>)
        return g.graph_value();
      else if constexpr (_gph_has_ADL<G>)
        return graph_value(g);
    }
  };
} // namespace _graph_value_

inline namespace _cpo_ {
  inline constexpr _graph_value_::fn graph_value{};
}


namespace _other_vertex_key_ {
  template <typename T>
  void other_vertex_key(std::initializer_list<T>) = delete;

  template <typename G, typename EI, typename VK>
  concept _edg_vkey_has_member = forward_iterator<EI> && requires(G&& g, EI uv, VK src_key) {
    {uv->other_vertex_key(forward<G>(g), src_key)};
  };
  template <typename G, typename EI, typename VK>
  concept _edg_vkey_has_ADL = forward_iterator<EI> && requires(G&& g, EI uv, VK src_key) {
    {other_vertex_key(forward<G>(g), uv, src_key)};
  };
  template <typename G, typename EI, typename VK>
  concept _edg_vkey_has_tgt = forward_iterator<EI> && requires(G&& g, EI uv, VK src_key) {
    {source_key(g, uv)};
    {target_key(g, uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI, typename VK>
    requires _edg_vkey_has_member<G, EI, VK> || _edg_vkey_has_ADL<G, EI, VK> || _edg_vkey_has_tgt<G, EI, VK>
    static consteval bool _edg_vkey_fnc_except() {
      if constexpr (_edg_vkey_has_member<G, EI, VK>)
        return noexcept(_detail::_decay_copy(declval<EI>()->other_vertex_key(declval<G&>(), declval<VK>())));
      else if constexpr (_edg_vkey_has_ADL<G, EI, VK>)
        return noexcept(_detail::_decay_copy(other_vertex_key(declval<G&>(), declval<EI>(), declval<VK>())));
      else if (_edg_vkey_has_tgt<G, EI, VK>)
        return noexcept(_detail::_decay_copy(declval<VK>() != target_key(declval<G&>(), declval<EI>())
                                                   ? target_key(declval<G&>(), declval<EI>())
                                                   : declval<VK>()));
    }

  public:
    template <typename G, typename EI, typename VK>
    requires _edg_vkey_has_member<G, EI, VK> || _edg_vkey_has_ADL<G, EI, VK> || _edg_vkey_has_tgt<G, EI, VK>
    constexpr auto operator()(G&& g, EI uv, VK src_key) const noexcept(_edg_vkey_fnc_except<G, EI, VK>()) {
      if constexpr (_edg_vkey_has_member<G, EI, VK>)
        return uv->other_vertex_key(forward<G>(g), src_key);
      else if constexpr (_edg_vkey_has_ADL<G, EI, VK>)
        return other_vertex_key(forward<G>(g), uv, src_key);
      else if constexpr (_edg_vkey_has_tgt<G, EI, VK>)
        return src_key == source_key(g, uv) ? target_key(g, uv) : source_key(g, uv);
    }
  };
} // namespace _other_vertex_key_

inline namespace _cpo_ {
  inline constexpr _other_vertex_key_::fn other_vertex_key{};
}

#  if 0
namespace _other_vertex_ {
  template <typename T>
  void other_vertex(std::initializer_list<T>) = delete;

  template <typename G, typename EI, typename VI>
  concept _edg_vtx_has_member = forward_iterator<EI> && forward_iterator<VI> && requires(G&& g, EI uv, VI src) {
    {uv->other_vertex(forward<G>(g), src)};
  };
  template <typename G, typename EI, typename VI>
  concept _edg_vtx_has_ADL = forward_iterator<EI> && forward_iterator<VI> && requires(G&& g, EI uv, VI src) {
    {other_vertex(forward<G>(g), uv, src)};
  };
  template <typename G, typename EI, typename VI>
  concept _edg_vtx_has_tgt = forward_iterator<EI> && forward_iterator<VI> && requires(G&& g, EI uv, VI src) {
    {source(g, uv)};
    {target(g, uv)};
  };

  struct fn {
  private:
    template <typename G, typename EI, typename VI>
    requires _edg_vtx_has_member<G, EI, VI> || _edg_vtx_has_ADL<G, EI, VI> || _edg_vtx_has_tgt<G, EI, VI>
    static consteval bool _edg_vtx_fnc_except() {
      if constexpr (_edg_vtx_has_member<G, EI, VI>)
        return noexcept(_detail::_decay_copy(declval<EI>()->other_vertex(declval<G&>(), declval<VI>())));
      else if constexpr (_edg_vtx_has_ADL<G, EI, VI>)
        return noexcept(_detail::_decay_copy(other_vertex(declval<G&>(), declval<EI>(), declval<VI>())));
      else if constexpr (_edg_vtx_has_tgt<G, EI, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>() != target(declval<G&>(), declval<EI>())
                                                   ? target(declval<G&>(), declval<EI>())
                                                   : declval<VI>()));
    }

  public:
    template <typename G, typename EI, typename VI>
    requires _edg_vtx_has_member<G, EI, VI> || _edg_vtx_has_ADL<G, EI, VI> || _edg_vtx_has_tgt<G, EI, VI>
    constexpr auto operator()(G&& g, EI uv, VI src) const noexcept(_edg_vtx_fnc_except<G, EI, VI>()) {
      if constexpr (_edg_vtx_has_member<G, EI, VI>)
        return uv->other_vertex(forward<G>(g), src);
      else if constexpr (_edg_vtx_has_ADL<G, EI, VI>)
        return other_vertex(forward<G>(g), uv, src);
      else if constexpr (_edg_vtx_has_tgt<G, EI, VI>)
        return src == source(g, uv) ? target(g, uv) : source(g, uv);
    }
  };
} // namespace _other_vertex_

inline namespace _cpo_ {
  inline constexpr _other_vertex_::fn other_vertex{};
}
#  endif


#  if 0
namespace _find_vertex_edge_ {
  template <typename T>
  void find_vertex_edge(std::initializer_list<T>) = delete;

  template <typename G, typename VI>
  concept _vtx_has_member = forward_iterator<VI> && requires(G&& g, VI u, VI v) {
    {u->find_vertex_edge(forward<G>(g), v)};
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = forward_iterator<VI> && requires(G&& g, VI u, VI v) {
    {find_vertex_edge(forward<G>(g), u, v)};
  };
  template <typename G, typename VI>
  concept _vtx_vi_default = forward_iterator<VI> && requires(G&& g, VI u, VI v) {
    true;
  };

  template <typename G, typename VI, typename VK>
  concept _vtx_key_has_member = forward_iterator<VI> && !forward_iterator<VK> && requires(G && g, VI u, VK vk) {
    {u->find_vertex_edge(forward<G>(g), vk)};
  };
  template <typename G, typename VI, typename VK>
  concept _vtx_key_has_ADL = forward_iterator<VI> && !forward_iterator<VK> && requires(G && g, VI u, VK vk) {
    {find_vertex_edge(forward<G>(g), u, vk)};
  };
  template <typename G, typename VI, typename VK>
  concept _vtx_key_default = forward_iterator<VI> && !forward_iterator<VK> && requires(G && g, VI u, VK vk) {
    true;
  };


  struct fn {
  private:
    template <typename G, typename VI>
    static constexpr auto find_other_vertex(G&& g, VI u, VI look_for) noexcept {
      auto src = u;
      auto uv  = begin(edges(g, u));
      for (; uv != end(edges(g, uv)); ++uv) {
        if (other_vertex(g, uv, src) == look_for)
          return uv;
        src = other_vertex(g, uv, src); // advance to target of uv (new src)
      }
      return uv; // end
    }

    template <typename G, typename VI, typename VK>
    static constexpr auto find_other_key(G&& g, VI u, VK look_for) noexcept {
      auto src = u;
      auto uv  = begin(edges(g, u));
      for (; uv != end(edges(g, uv)); ++uv) {
        if (other_vertex_key(g, uv, src) == look_for)
          return uv;
        src = other_vertex_key(g, uv, src); // advance to target of uv (new src)
      }
      return uv; // end
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_vi_default<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->find_vertex_edge(declval<G&>(), declval<VI>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(find_vertex_edge(declval<G&>(), declval<VI>(), declval<VI>())));
      else if constexpr (_vtx_vi_default<G, VI>)
        return noexcept(_detail::_decay_copy(find_other_vertex(
              begin(edges(declval<G&>(), declval<G&>())), end(edges(declval<G&>(), declval<VI>())), declval<VI>())));
    }

    template <typename G, typename VI, typename VK>
    requires _vtx_key_has_member<G, VI, VK> || _vtx_key_has_ADL<G, VI, VK> || _vtx_key_default<G, VI, VK>
    static consteval bool _vtx_key_fnc_except() {
      if constexpr (_vtx_has_member<G, VI, VK>)
        return noexcept(_detail::_decay_copy(declval<VI>()->find_vertex_edge(declval<G&>(), declval<VK>())));
      else if constexpr (_vtx_key_has_ADL<G, VI, VK>)
        return noexcept(_detail::_decay_copy(find_vertex_edge(declval<G&>(), declval<VI>(), declval<VK>())));
      else if constexpr (_vtx_key_default<G, VI, VK>)
        return noexcept(_detail::_decay_copy(find_other_vertex(
              begin(edges(declval<G&>(), declval<G&>())), end(edges(declval<G&>(), declval<VI>())), declval<VK>())));
    }

  public:
    template <typename G, forward_iterator VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_vi_default<G, VI>
    constexpr auto operator()(G&& g, VI u, VI look_for) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return u->find_vertex_edge(forward<G>(g), look_for);
      else if constexpr (_vtx_has_ADL<G, VI>)
        return find_vertex_edge(forward<G>(g), u, look_for);
      else if constexpr (_vtx_vi_default<G, VI>)
        return find_other_vertex(begin(edges(g, u)), end(edges(g, u)), look_for);
    }

    template <typename G, forward_iterator VI, typename VK>
    requires _vtx_key_has_member<G, VI, VK> || _vtx_key_has_ADL<G, VI, VK> || _vtx_key_default<G, VI, VK>
    constexpr auto operator()(G&& g, VI u, VK look_for) const noexcept(_vtx_key_fnc_except<G, VI, VK>()) {
      if constexpr (_vtx_key_has_member<G, VI, VK>)
        return u->find_vertex_edge(forward<G>(g), look_for);
      else if constexpr (_vtx_key_has_ADL<G, VI, VK>)
        return find_vertex_edge(forward<G>(g), u, look_for);
      else if constexpr (_vtx_key_default<G, VI, VK>)
        return find_other_key(begin(edges(g, u)), end(edges(g, u)), look_for);
    }
  };
} // namespace _find_vertex_edge_

inline namespace _cpo_ {
  inline constexpr _other_vertex_::fn find_vertex_edge{};
}
#  endif //0


namespace _degree_ {
  template <typename T>
  void degree(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_member = requires(G& g) {
    {g.degree()};
  };
  template <typename G>
  concept _gph_has_ADL = requires(G& g) {
    {degree(g)};
  };
  template <typename G>
  concept _gph_has_sz = requires(G& g) {
    {ranges::size(g)};
  };

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G> || _gph_has_sz<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G&>().degree()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(degree(declval<G&>())));
      else if constexpr (_gph_has_sz<G>)
        return noexcept(_detail::_decay_copy(ranges::size(declval<G&>())));
    }

  public:
    // g.degree(), degree(g)
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    constexpr auto& operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_member<G>)
        return g.degree();
      else if constexpr (_gph_has_ADL<G>)
        return degree(g);
      else if constexpr (_gph_has_sz<G>)
        return ranges::size(g);
    }
  };
} // namespace _degree_

inline namespace _cpo_ {
  inline constexpr _degree_::fn degree{};
}


} // namespace std::graph

#endif //GRAPH_ACCESS_HPP
