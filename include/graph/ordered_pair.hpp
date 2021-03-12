
#include <concepts>
#include <ranges>
#include <type_traits>
#include <utility> // pair

#ifndef ORDERED_PAIR_HPP
#  define ORDERED_PAIR_HPP

namespace std {

//
// ordered_pair
//
template <typename T1, typename T2>
struct ordered_pair : public pair<T1, T2> {
  using base_t      = pair<T1, T2>;
  using first_type  = typename base_t::first_type;
  using second_type = typename base_t::second_type;

  constexpr ordered_pair()                        = default;
  constexpr ordered_pair(const ordered_pair&)     = default;
  constexpr ordered_pair(ordered_pair&&) noexcept = default;
  //constexpr ~ordered_pair() = default; // dtor not defined in pair<>

  template <typename U1, typename U2>
  constexpr ordered_pair(const U1& x, const U2& y);

  template <typename U1, typename U2>
  constexpr ordered_pair(U1&& x, U2&& y) noexcept;

  template <typename U1, typename U2>
  constexpr explicit ordered_pair(const pair<U1, U2>& other);

  template <typename U1, typename U2>
  constexpr explicit ordered_pair(pair<U1, U2>&& other);

  constexpr ordered_pair& operator=(const ordered_pair& other);
  constexpr ordered_pair& operator=(ordered_pair&& other) noexcept;

  constexpr void
  swap(ordered_pair& other) noexcept(is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>);
};

#  if defined(_MSC_VER)
template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);

#  else
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2);
#  endif // _MSC_VER


template <typename T1, typename T2>
constexpr void
swap(ordered_pair<T1, T2>& v1,
     ordered_pair<T1, T2>& v2) noexcept(is_nothrow_swappable_v<typename ordered_pair<T1, T2>::first_type>&&
                                              is_nothrow_swappable_v<typename ordered_pair<T1, T2>::second_type>);

template <class T1, class T2>
constexpr auto make_ordered_pair(T1&& v1, T2&& v2);

//
// unordered_pair
//
template <typename T1, typename T2>
struct unordered_pair : public pair<T1, T2> {
  using base_t      = pair<T1, T2>;
  using first_type  = typename base_t::first_type;
  using second_type = typename base_t::second_type;

  constexpr unordered_pair()                          = default;
  constexpr unordered_pair(const unordered_pair&)     = default;
  constexpr unordered_pair(unordered_pair&&) noexcept = default;
  //constexpr ~unordered_pair() = default; // destructor not defined in pair<>

  template <typename U1, typename U2>
  constexpr unordered_pair(const U1& x, const U2& y);

  template <typename U1, typename U2>
  constexpr unordered_pair(U1&& x, U2&& y) noexcept;

  template <typename U1, typename U2>
  constexpr explicit unordered_pair(const pair<U1, U2>& other);

  template <typename U1, typename U2>
  constexpr explicit unordered_pair(pair<U1, U2>&& other);

  constexpr unordered_pair& operator=(const unordered_pair& other);

  constexpr unordered_pair& operator=(unordered_pair&& other) noexcept;

  constexpr void
  swap(unordered_pair& other) noexcept(is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>);
};

#  if defined(_MSC_VER)
template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
#  else
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2);
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2);
#  endif

template <typename T1, typename T2>
constexpr void
swap(unordered_pair<T1, T2>& v1,
     unordered_pair<T1, T2>& v2) noexcept(is_nothrow_swappable_v<typename unordered_pair<T1, T2>::first_type>&&
                                                is_nothrow_swappable_v<typename unordered_pair<T1, T2>::second_type>);

template <class T1, class T2>
constexpr auto make_unordered_pair(T1&& v1, T2&& v2);

} // namespace std

#endif

#include "detail/ordered_pair_impl.hpp"
