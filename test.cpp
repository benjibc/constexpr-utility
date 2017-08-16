#include "./const_hashmap.h"
#include <cassert>
#include <iostream>

int main() {
  static_assert(cexpr::fnv1a("hello") == 0x4f9f2cab, "did not pass for fnv1a"); 
  static_assert(cexpr::fnv1("hello") == 0xe31c0e3f, "did not pass for fnv1"); 
  cexpr::HashMap<10, uint32_t, uint32_t> map(1, 2, 3, 4, 5);
  std::cout << map.size() << std::endl;

  constexpr std::array<uint32_t, 3> lhs{1, 3, 5};
  constexpr std::array<uint32_t, 3> rhs{2, 4, 6};
  constexpr auto merged = cexpr::merge<uint32_t, 3, 3>(lhs, rhs);
  static_assert(merged[0], 1);
	return 0;
}
