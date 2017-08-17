#pragma once

#include <array>

namespace cexpr {

template<typename Arr>
constexpr typename Arr::value_type& getRef(
  const Arr& buf,
  size_t bufIdx) {
  return const_cast<typename Arr::value_type &>(
    static_cast<Arr const&>(buf)[bufIdx]);
}

}
