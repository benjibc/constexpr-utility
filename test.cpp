#include "./const_hashmap.h"
#include "./algorithm.h"
#include <cassert>
#include <iostream>

constexpr bool isOne(const uint32_t& in) {
  return in != 1;
}

constexpr uint32_t incr(const uint32_t& in) {
  return in + 1;
}

std::string emptyHandler() {
	return "";
}

std::string settingHandler() {
	return "settings!!";
}


int main() {
  static_assert(cexpr::fnv1a("hello") == 0x4f9f2cab, "did not pass for fnv1a"); 
  static_assert(cexpr::fnv1("hello") == 0xe31c0e3f, "did not pass for fnv1"); 

  constexpr std::array<uint32_t, 3> lhs{1, 3, 5};
  constexpr std::array<uint32_t, 3> rhs{2, 4, 6};
  constexpr auto merged = cexpr::merge<uint32_t, 3, 3>(lhs, rhs);
  static_assert(merged[0] == 1, "element must be 1");
  static_assert(merged[5] == 6, "element must be 6");

  constexpr std::array<uint32_t, 3> unsorted{5, 3, 1};
  constexpr auto sorted = cexpr::mergeSort(unsorted);
  static_assert(sorted[0] == 1, "element must be 1");
  static_assert(sorted[1] == 3, "element must be 3");
  static_assert(sorted[2] == 5, "element must be 5");

  // test algorithm file
  static_assert(!cexpr::all_of(unsorted, isOne), "not all are 1");
  static_assert(cexpr::any_of(unsorted, isOne), "at least one 1");
  static_assert(!cexpr::none_of(unsorted, isOne), "there is a 1");
  constexpr auto incred = cexpr::for_each(unsorted, incr);
  for(auto it = incred.begin(); it != incred.end(); it++) {
    std::cout << *it << std::endl;
  }

  // static_assert(cexpr::none_of(incred, isOne), "there is no more 1");
  static_assert(0 == cexpr::count(incred, 1U), "there is no more 1");
  static_assert(1 == cexpr::count(incred, 2U), "There is only one 2");
  static_assert(!cexpr::equal(incred, unsorted), "not equal");
  static_assert(cexpr::adjacent_find(incred) == incred.size(), "not equal");
  constexpr std::array<uint32_t, 7> conseq{1, 2, 3, 4, 4, 5, 5};
  static_assert(cexpr::adjacent_find(conseq) == 3, "at 3");

	constexpr std::array<std::pair<const char*, std::string(*)()>, 10> urls{{
		{"/login", emptyHandler},
		{"/profile",emptyHandler},
		{"/feed",emptyHandler},
		{"/notification",emptyHandler},
		{"/settings",settingHandler},
		{"/videos",emptyHandler},
		{"/notes",emptyHandler},
		{"/logout",emptyHandler},
		{"/messenger",emptyHandler},
		{"/stories",emptyHandler}
	}};
	
	constexpr auto mapSize = cexpr::getPerfectHashSize<cexpr::fnv1Hasher>(urls);

  constexpr cexpr::HashMap<
		10,
		mapSize,
		const char*,
		std::string(*)(),
		cexpr::fnv1Hasher> map(urls);

	static_assert(map.get("hello") == nullptr, "no pointer");
	static_assert(map.get("/login") == emptyHandler, "empty handler");
	static_assert(map.get("/settings") == settingHandler, "setting!");
	std::cout << (void*)(map.get("/login")) << std::endl;
	std::cout << (void*)(map.get("/settings")) << std::endl;

	return 0;
}
