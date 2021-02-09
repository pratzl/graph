#include <catch2/catch.hpp>
//#include <range/v3/range/concepts.hpp>
#include <ranges>
#include <vector>
#include <type_traits>

using std::is_const_v;
using std::remove_const_t;
using std::add_const_t;
using std::conditional;
using std::conditional_t;
using std::ranges::iterator_t;
using std::ranges::range_value_t;
using std::ranges::range_reference_t;


namespace std {

template <typename R,
          typename P      = identity,
          typename Result = conditional<is_integral_v<::ranges::projected<::ranges::iterator_t<R>, P>>,
                                        ::ranges::projected<::ranges::iterator_t<R>, P>,
                                        double>::type>
constexpr auto mean(R&& r, P proj = P{}) {
  Result result = Result();
  Result n      = Result();
  for (const auto& v : r) {
    result += v;
    ++n;
  }
  return result / n;
}

} // namespace std


TEST_CASE("test", "[test][temp]") {
  std::vector<double> vec{1, 2, 3, 4, 5};
  //double              m = std::mean(vec);
}

TEST_CASE("test is_floating_point", "[test][temp][is_floating_point]") {
  //
  //std::is_floating_point<std::projected<::ranges::iterator_t<R>, P>::value_type>::value;
  using R    = std::vector<double>;
  using P    = std::identity;
  bool is_f1 = std::is_floating_point_v<R>;
  //bool is_fl2 = std::is_floating_point_v<std::ranges::projected<std::ranges::iterator_t<R>, P>::value_type>;
  //bool is_fl3 = std::is_floating_point<std::projected<::ranges::iterator_t<R>, P>::value_type>::value;
}


TEST_CASE("[conversion]", "[conversion]") {
  using std::is_convertible_v;
  using std::is_same_v;
  using V = std::vector<int>;
  REQUIRE(is_convertible_v<V::iterator, V::const_iterator>);
  REQUIRE(!is_convertible_v<V::const_iterator, V::iterator>);
  REQUIRE(!is_convertible_v<V::const_iterator, double*>);
  REQUIRE(is_convertible_v<int, const int&>);
  REQUIRE(!is_convertible_v<const int, int&>);
  REQUIRE(is_same_v<int, int>);
  REQUIRE(!is_same_v<int, const int>);
}

// ToT aquires const/non-const characteristics of FromT
template <typename FromT, typename ToT>
struct apply_const : public conditional<is_const_v<FromT>, add_const_t<ToT>, remove_const_t<ToT>> {};
template <typename FromT, typename ToT>
using apply_const_t = typename apply_const<FromT, ToT>::type;

struct Object {
  using things_type = std::vector<int>;
  using thongs_type = std::vector<int>;
};

template <typename O>
struct trial_traits;

template <>
struct trial_traits<Object> {
  using things_type = typename Object::things_type;
  using thongs_type = typename Object::thongs_type;
};

template <typename O>
using things_t = apply_const_t<O, typename trial_traits<remove_const_t<O>>::things_type>;
template <typename O>
using thongs_t = apply_const_t<O, typename trial_traits<remove_const_t<O>>::thongs_type>;


TEST_CASE("[traits]", "[traits][nonconst]") {
  things_t<Object> things = {0, 1, 2, 3};
  for (auto& t : things) {
    t += 1;
  }
}

TEST_CASE("[const traits]", "[traits][const]") {
  //using Obj = thongs_t<const Object>;
  using Obj = const Object;
  static_assert(is_const_v<Obj>);
  static_assert(!is_const_v<Obj::thongs_type>);
  static_assert(is_const_v<const Obj::thongs_type>);

  static_assert(is_const_v<thongs_t<Obj>>);

  thongs_t<const Object> thongs;

  //static_assert(is_const_v<things_t<Obj>>);

  //static_assert(is_const_v<thongs_t<const Object>>);
  //static_assert(is_const_v<range_reference_t<const Object>>);
  //Obj thongs = {0, 1, 2, 3};
  //for (auto& t : thongs) {
  // t += 1; // compile error (can't modify)
  //}
}
