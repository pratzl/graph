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


namespace _vertices_ {
  template <typename T>
  void vertices(std::initializer_list<T>) = delete;

  template <typename G>
  concept _gph_has_member = requires(G g) {
    { forward<G>(g).vertices() } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_member = requires(G g, VI u) {
    forward_iterator<VI>;
    { forward<VI>(u)->vertices(forward<G>(g)) } -> ranges::forward_range;
  };

  template <typename G>
  concept _gph_has_ADL = requires(G g) {
    is_reference_v<G>;
    { vertices(forward<G>(g)) } -> ranges::forward_range;
  };
  template <typename G, typename VI>
  concept _vtx_has_ADL = requires(G g, VI u) {
    is_reference_v<G>;
    forward_iterator<VI>;
    { vertices(forward<G>(g), forward<VI>(u)) } -> ranges::forward_range;
  };

  struct fn {
  private:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    static consteval bool _gph_fnc_except() {
      if constexpr (_gph_has_member<G>)
        return noexcept(_detail::_decay_copy(declval<G>().vertices()));
      else if constexpr (_gph_has_ADL<G>)
        return noexcept(_detail::_decay_copy(vertices(declval<G>())));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    static consteval bool _vtx_fnc_except() {
      if constexpr (_vtx_has_member<G, VI>)
        return noexcept(_detail::_decay_copy(declval<VI>()->vertices(declval<G>())));
      else if constexpr (_vtx_has_ADL<G, VI>)
        return noexcept(_detail::_decay_copy(vertices(declval<G>(), declval<VI>())));
    }

  public:
    template <typename G>
    requires _gph_has_member<G> || _gph_has_ADL<G>
    constexpr auto operator()(G&& g) const noexcept(_gph_fnc_except<G>()) {
      if constexpr (_gph_has_member<G>)
        return forward<G>(g).vertices();
      else if constexpr (_gph_has_ADL<G>)
        return vertices(forward<G>(g));
    }

    template <typename G, typename VI>
    requires _vtx_has_member<G, VI> || _vtx_has_ADL<G, VI>
    constexpr auto operator()(G&& g, VI&& u) const noexcept(_vtx_fnc_except<G, VI>()) {
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

} // namespace std

#endif //GRAPH_ACCESS_HPP
