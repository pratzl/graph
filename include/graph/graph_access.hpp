#include <concepts>

#ifndef GRAPH_ACCESS_HPP
#  define GRAPH_ACCESS_HPP

namespace std {
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


namespace _graph_value_ {
  template <typename T>
  void graph_value(std::initializer_list<T>) = delete;

  template <typename G, typename VI>
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { forward<VI>(u)->graph_value(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { graph_value(forward<G>(g), forward<VI>(u)) } -> ranges::forward_range;
  };

  struct fn {
  private:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->graph_value(declval<G&&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(graph_value(declval<G&&>(), declval<VI>())));
    }

  public:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return forward<G>(u)->graph_value(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return graph_value(forward<G>(g), forward<G>(u));
    }
  };
} // namespace _graph_value_

inline namespace _cpo_ {
  inline constexpr _graph_value_::fn graph_value{};
}


namespace _vertices_ {
  template <typename T>
  void vertices(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_rng = ranges::forward_range<G>;
  template <typename G>
  concept _gph_has_member = requires(G&& g) {
    { forward<G>(g).vertices() } -> ranges::forward_range;
  };
  template <typename G>
  concept _gph_has_ADL = requires(G&& g) {
    is_reference_v<G>;
    { vertices(forward<G>(g)) } -> ranges::forward_range;
  };

  template <typename G, typename VI>
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { forward<VI>(u)->vertices(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { vertices(forward<G>(g), forward<VI>(u)) } -> ranges::forward_range;
  };

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_rng<G> || _gph_has_ADL<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_rng<G>)
        return noexcept(_detail::_decay_copy(declval<G&&>()));
      else if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G&&>().vertices()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(vertices(declval<G&&>())));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertices(declval<G&&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertices(declval<G&&>(), declval<VI>())));
    }

  public:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_rng<G> || _gph_has_ADL<G>
    constexpr auto& operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_rng<G>)
        return forward<G>(g);
      else if constexpr (_gph_has_member<G>)
        return forward<G>(g).vertices();
      else if constexpr (_gph_has_ADL<G>)
        return vertices(forward<G>(g));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return forward<G>(u)->vertices(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertices(forward<G>(g), forward<G>(u));
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
  concept _vtx_has_member = requires(G&& g, VI u) {
    forward_iterator<VI>;
    { forward<VI>(u)->vertex_key(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { vertex_key(forward<G>(g), forward<VI>(u)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_is_rnd = requires(G&& g, VI u) {
    is_reference_v<G>;
    random_access_iterator<VI>;
    convertible_to<VI, decltype(vertices(begin(g)))>;
  };

  struct fn {
  private:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_is_rnd<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertex_key(declval<G&&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertex_key(declval<G&&>(), declval<VI>())));
      else if constexpr (_vtx_is_rnd<G, VI>)
        return noexcept(declval<VI>() - declval<VI>());
    }

  public:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI> || _vtx_is_rnd<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return forward<G>(u)->vertex_key(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertex_key(forward<G>(g), forward<G>(u));
      else if constexpr (_vtx_is_rnd<G, VI>)
        return u - begin(vertices(g)); // default impl if not defined
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
    { forward<VI>(u)->vertex_value(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G&& g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { vertex_value(forward<G>(g), forward<VI>(u)) } -> ranges::forward_range;
  };

  struct fn {
  private:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertex_value(declval<G&&>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertex_value(declval<G&&>(), declval<VI>())));
    }

  public:
    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto& operator()(G&& g, VI u) const noexcept(_vtx_fnc_except<G, VI>()) {
      if constexpr (_vtx_has_member<G, VI>)
        return forward<G>(u)->vertex_value(forward<G>(g));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return vertex_value(forward<G>(g), forward<G>(u));
    }
  };
} // namespace _vertex_value_

inline namespace _cpo_ {
  inline constexpr _vertex_value_::fn vertex_value{};
}

} // namespace std

#endif //GRAPH_ACCESS_HPP