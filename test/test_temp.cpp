#include <catch2/catch.hpp>
#include <range/v3/range/concepts.hpp>
#include <vector>
#include <type_traits>

//namespace ranges = ::ranges;

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
  double              m = std::mean(vec);
}

TEST_CASE("test is_floating_point", "[test][temp][is_floating_point]") {
  //
  //std::is_floating_point<std::projected<::ranges::iterator_t<R>, P>::value_type>::value;
  using R     = std::vector<double>;
  using P     = ::ranges::identity;
  bool is_f1  = std::is_floating_point_v<R>;
  bool is_fl2 = std::is_floating_point_v<::ranges::projected<::ranges::iterator_t<R>, P>::value_type>;
  //bool is_fl3 = std::is_floating_point<std::projected<::ranges::iterator_t<R>, P>::value_type>::value;
}
