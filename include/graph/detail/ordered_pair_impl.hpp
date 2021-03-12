#ifndef ORDERED_PAIR_IMPL_HPP
#define ORDERED_PAIR_IMPL_HPP

namespace std {

//
// ordered_pair
//
template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr ordered_pair<T1, T2>::ordered_pair(const U1& x, const U2& y) : base_t(x, y) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr ordered_pair<T1, T2>::ordered_pair(U1&& x, U2&& y) noexcept : base_t(forward<U1>(x), forward<U2>(y)) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr ordered_pair<T1, T2>::ordered_pair(const pair<U1, U2>& other) : base_t(other) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr ordered_pair<T1, T2>::ordered_pair(pair<U1, U2>&& other) : base_t(forward<pair<U1, U2>>(other)) {}

template <typename T1, typename T2>
constexpr ordered_pair<T1, T2>& ordered_pair<T1, T2>::operator=(const ordered_pair& other) {
  base_t::operator=(other);
  return *this;
}

template <typename T1, typename T2>
constexpr ordered_pair<T1, T2>& ordered_pair<T1, T2>::operator=(ordered_pair&& other) noexcept {
  base_t::operator=(forward<base_t>(other));
  return *this;
}

template <typename T1, typename T2>
constexpr void ordered_pair<T1, T2>::swap(ordered_pair& other) noexcept(
      is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>) {
  base_t::swap(other);
}

#if defined(_MSC_VER)
template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator==(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator!=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator<(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator>(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator<=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator>=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator==(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator!=(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator<(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator>(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator<=(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator>=(static_cast<const pair<T1, T2>&>(v1), v2);
}


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator==(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator!=(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator<(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator>(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator<=(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator>=(v1, static_cast<const pair<T1, T2>&>(v2));
}
#else
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const ordered_pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  return operator<=>(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const ordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  const pair<T1, T2>& v1b(v1);
  return              operator<=>(v1b, v2);
}
template <typename T1, typename T2>
[[nodiscard]] constexpr auto operator<=>(const pair<T1, T2>& v1, const ordered_pair<T1, T2>& v2) {
  const pair<T1, T2>& v2b(v2);
  return              operator<=>(v1, v2b);
}
#endif

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
template <typename U1, typename U2>
constexpr unordered_pair<T1, T2>::unordered_pair(const U1& x, const U2& y) : base_t(x, y) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr unordered_pair<T1, T2>::unordered_pair(U1&& x, U2&& y) noexcept : base_t(forward<U1>(x), forward<U2>(y)) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr unordered_pair<T1, T2>::unordered_pair(const pair<U1, U2>& other) : base_t(other) {}

template <typename T1, typename T2>
template <typename U1, typename U2>
constexpr unordered_pair<T1, T2>::unordered_pair(pair<U1, U2>&& other) : base_t(forward<pair<U1, U2>>(other)) {}

template <typename T1, typename T2>
constexpr unordered_pair<T1, T2>& unordered_pair<T1, T2>::operator=(const unordered_pair& other) {
  base_t::operator=(other);
  return *this;
}

template <typename T1, typename T2>
constexpr unordered_pair<T1, T2>& unordered_pair<T1, T2>::operator=(unordered_pair&& other) noexcept {
  base_t::operator=(forward<base_t>(other));
  return *this;
}

template <typename T1, typename T2>
constexpr void unordered_pair<T1, T2>::swap(unordered_pair& other) noexcept(
      is_nothrow_swappable_v<first_type>&& is_nothrow_swappable_v<second_type>) {
  base_t::swap(other);
}

#if defined(_MSC_VER)
template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator==(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator!=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator>(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator>=(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator==(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator!=(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator<(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator>(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator<=(static_cast<const pair<T1, T2>&>(v1), v2);
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator>=(static_cast<const pair<T1, T2>&>(v1), v2);
}


template <class T1, class T2>
[[nodiscard]] constexpr bool operator==(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator==(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator!=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator!=(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator>(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator<=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<=(v1, static_cast<const pair<T1, T2>&>(v2));
}

template <class T1, class T2>
[[nodiscard]] constexpr bool operator>=(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator>=(v1, static_cast<const pair<T1, T2>&>(v2));
}
#else
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const unordered_pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<=>(static_cast<const pair<T1, T2>&>(v1), static_cast<const pair<T1, T2>&>(v2));
}
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const unordered_pair<T1, T2>& v1, const pair<T1, T2>& v2) {
  return operator<=>(static_cast<const pair<T1, T2>&>(v1), v2);
}
template <typename T1, typename T2>
[[nodiscard]] auto operator<=>(const pair<T1, T2>& v1, const unordered_pair<T1, T2>& v2) {
  return operator<=>(v1, static_cast<const pair<T1, T2>&>(v2));
}
#endif

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

} // namespace std

#endif //ORDERED_PAIR_IMPL_HPP
