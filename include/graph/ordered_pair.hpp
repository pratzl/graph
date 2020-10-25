#include <concepts>
//#include <range/v3/range/concepts.hpp>
#include <range/v3/view/subrange.hpp>
#include <type_traits>
#include <utility> // pair

#ifndef ORDERED_PAIR_HPP
#  define ORDERED_PAIR_HPP

namespace std::graph {
namespace ranges = ::ranges; // future = std::ranges

//
// ordered_pair
//
template <typename T1, typename T2>
struct ordered_pair : public pair<T1, T2> {
  using this_t      = ordered_pair<T1, T2>;
  using base_t      = pair<T1, T2>;
  using first_type  = typename base_t::first_type;
  using second_type = typename base_t::second_type;

  constexpr ordered_pair()                        = default;
  constexpr ordered_pair(ordered_pair const&)     = default;
  constexpr ordered_pair(ordered_pair&&) noexcept = default;
  //constexpr ~ordered_pair() = default; // dtor not defined in pair<>

  template <typename U1, typename U2>
  constexpr ordered_pair(U1 const& x, U2 const& y) : base_t(x, y) {}

  template <typename U1, typename U2>
  constexpr ordered_pair(U1&& x, U2&& y) noexcept : base_t(forward<U1>(x), forward<U2>(y)) {}

  template <typename U1, typename U2>
  constexpr explicit ordered_pair(pair<U1, U2> const& other) : base_t(other) {}

  template <typename U1, typename U2>
  constexpr explicit ordered_pair(pair<U1, U2>&& other) : base_t(forward<pair<U1, U2>>(other)) {}

  constexpr ordered_pair& operator=(ordered_pair const& other) {
    base_t::operator=(other);
    return *this;
  }

  constexpr ordered_pair& operator=(ordered_pair&& other) noexcept {
    base_t::operator=(forward<T1>(other));
    return *this;
  }

  constexpr void
  swap(ordered_pair& other) noexcept(is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>) {
    base_t::swap(other);
  }
};

template <typename T1, typename T2>
auto operator<=>(ordered_pair<T1, T2> const& v1, ordered_pair<T1, T2> const& v2) {
  return operator<=>(static_cast<pair<T1, T2> const&>(v1), static_cast<pair<T1, T2> const&>(v2));
}
template <typename T1, typename T2>
auto operator<=>(ordered_pair<T1, T2> const& v1, pair<T1, T2> const& v2) {
  return operator<=>(static_cast<pair<T1, T2> const&>(v1), v2);
}
template <typename T1, typename T2>
auto operator<=>(pair<T1, T2> const& v1, ordered_pair<T1, T2> const& v2) {
  return operator<=>(v1, static_cast<pair<T1, T2> const&>(v2));
}

template <typename T1, typename T2>
constexpr void
swap(ordered_pair<T1, T2>& v1,
     ordered_pair<T1, T2>& v2) noexcept(is_nothrow_swappable_v<typename ordered_pair<T1, T2>::first_type>&&
                                              is_nothrow_swappable_v<typename ordered_pair<T1, T2>::second_type>) {
  v1.swap(v2);
}

template <class T1, class T2>
constexpr auto make_ordered_pair(T1&& v1, T2&& v2) {
  using pair_t = ordered_pair<decay_t<T1>, decay_t<T2>>;
  return pair_t(forward<T1>(v1), forward<T2>(v2));
}


//
// unordered_pair
//
template <typename T1, typename T2>
struct unordered_pair : public pair<T1, T2> {
  using this_t      = unordered_pair<T1, T2>;
  using base_t      = pair<T1, T2>;
  using first_type  = typename base_t::first_type;
  using second_type = typename base_t::second_type;

  constexpr unordered_pair()                          = default;
  constexpr unordered_pair(unordered_pair const&)     = default;
  constexpr unordered_pair(unordered_pair&&) noexcept = default;
  //constexpr ~unordered_pair() = default; // destructor not defined in pair<>

  template <typename U1, typename U2>
  constexpr unordered_pair(U1 const& x, U2 const& y) : base_t(x, y) {}

  template <typename U1, typename U2>
  constexpr unordered_pair(U1&& x, U2&& y) noexcept : base_t(forward<U1>(x), forward<U2>(y)) {}

  template <typename U1, typename U2>
  constexpr explicit unordered_pair(pair<U1, U2> const& other) : base_t(other) {}

  template <typename U1, typename U2>
  constexpr explicit unordered_pair(pair<U1, U2>&& other) : base_t(forward<pair<U1, U2>>(other)) {}

  constexpr unordered_pair& operator=(unordered_pair const& other) {
    base_t::operator=(other);
    return *this;
  }

  constexpr unordered_pair& operator=(unordered_pair&& other) noexcept {
    base_t::operator=(forward<T1>(other));
    return *this;
  }

  constexpr void
  swap(unordered_pair& other) noexcept(is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>) {
    base_t::swap(other);
  }
};

template <typename T1, typename T2>
auto operator<=>(unordered_pair<T1, T2> const& v1, unordered_pair<T1, T2> const& v2) {
  return operator<=>(static_cast<pair<T1, T2> const&>(v1), static_cast<pair<T1, T2> const&>(v2));
}
template <typename T1, typename T2>
auto operator<=>(unordered_pair<T1, T2> const& v1, pair<T1, T2> const& v2) {
  return operator<=>(static_cast<pair<T1, T2> const&>(v1), v2);
}
template <typename T1, typename T2>
auto operator<=>(pair<T1, T2> const& v1, unordered_pair<T1, T2> const& v2) {
  return operator<=>(v1, static_cast<pair<T1, T2> const&>(v2));
}

template <typename T1, typename T2>
constexpr void
swap(unordered_pair<T1, T2>& v1,
     unordered_pair<T1, T2>& v2) noexcept(is_nothrow_swappable_v<typename unordered_pair<T1, T2>::first_type>&&
                                                is_nothrow_swappable_v<typename unordered_pair<T1, T2>::second_type>) {
  v1.swap(v2);
}

template <class T1, class T2>
constexpr auto make_unordered_pair(T1&& v1, T2&& v2) {
  using pair_t = unordered_pair<decay_t<T1>, decay_t<T2>>;
  return pair_t(forward<T1>(v1), forward<T2>(v2));
}

} // namespace std::graph

#endif
