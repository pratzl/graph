
#ifndef GRAPH_FWD_IMPL_HPP
#define GRAPH_FWD_IMPL_HPP

namespace std::graph {

template <typename T>
constexpr auto value(T& gve) noexcept -> decltype(user_value(gve)) {
  return user_value(gve);
}

//
// vertex Functions
//


} // namespace std::graph

#endif // GRAPH_FWD_IMPL_HPP
