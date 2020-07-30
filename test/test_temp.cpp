#include <catch2/catch.hpp>
#include <range/v3/range/concepts.hpp>
#include <vector>

namespace std {

template <typename R,
          typename P      = identity,
          typename Result = conditional<integral<::ranges::projected<::ranges::iterator_t<R>, P>>,
                                        ::ranges::projected<::ranges::iterator_t<R>, P>,
                                        double>::type>
constexpr auto mean(R&& r, P proj = P{}) {
  Result result = Result();
  Result n      = Result();
  for (auto const& v : r) {
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
