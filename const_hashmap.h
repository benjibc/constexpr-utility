#pragma once
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <cmath>
#include <utility>
#include "./algorithm.h"

namespace cexpr {

namespace {
constexpr uint32_t offset = 0x811C9DC5;
constexpr uint32_t prime = 0x01000193;
}


constexpr uint32_t fnv1(const char* const& csrc) {
  uint32_t start = offset;
	const char* src = csrc;
  while (*src != '\0') {
    start = (static_cast<uint32_t>(*src) * prime) ^ start;
    src++;
  }
  return start;
}

struct fnv1Hasher {
	constexpr uint32_t operator()(const char* const& csrc) {
		return fnv1(csrc);
	}
};

constexpr uint32_t fnv1a(const char* const& csrc) {
	const char* src = csrc;
  uint32_t start = offset;
  while (*src != '\0') {
    start = (static_cast<uint32_t>(*src) ^ start) * prime;
    src++;
  }
  return start;
}

template<size_t cbegin, size_t bufSize, typename T, size_t size>
constexpr std::array<T, bufSize> copyRange(
  const std::array<T, size> full) {
  size_t begin = cbegin;
  size_t end = begin + bufSize;

  std::array<T, bufSize> dst{};
  size_t dstIdx = 0;
  while (begin != cbegin + bufSize) {
    auto& dstRef = getRef(dst, dstIdx);
    dstRef = full[begin];
    begin++; dstIdx++;
  }
  return dst;
}

template<typename T, size_t size1, size_t size2>
constexpr std::array<T, size1 + size2> merge(
  const std::array<T, size1>& lhs,
  const std::array<T, size2>& rhs) {
  
  std::array<T, size1 + size2> buf{};
  size_t lhsHead = 0;
  size_t rhsHead = 0;
  size_t bufIdx = 0;

  while (lhsHead != size1 && rhsHead != size2) {
    auto lhsVal = lhs[lhsHead];
    auto rhsVal = rhs[rhsHead];
    auto& bufRef = getRef(buf, bufIdx);
    if (lhsVal < rhsVal) {
      bufRef = lhsVal;
      lhsHead++;
    } else {
      bufRef = rhsVal;
      rhsHead++;
    }
    bufIdx++;
  }

  while (lhsHead != size1) {
    auto lhsVal = lhs[lhsHead];
    auto& bufRef = getRef(buf, bufIdx);
    bufRef = lhsVal;
    bufIdx++;
    lhsHead++;
  }
  while (rhsHead != size2) {
    auto rhsVal = rhs[rhsHead];
    auto& bufRef = getRef(buf, bufIdx);
    bufRef = rhsVal;
    bufIdx++;
    rhsHead++;
  }
  return buf;
}

template<typename T, size_t size>
constexpr std::array<T, size> mergeSort(
  const std::array<T, size> arr) {
  if (size <= 1) {
    return arr;
  }

  constexpr size_t middle = size /2;
  auto left = mergeSort(copyRange<0, middle>(arr));
  auto right = mergeSort(copyRange<middle, size - middle>(arr));
  return merge<T, middle, size - middle>(left, right);
}

template<typename First, typename Second>
constexpr First getFirst(
	const std::pair<First, Second>& pair) {
	return pair.first;
}

// hash all entries and try to find the best size
template<typename Hash, typename T, size_t size>
constexpr size_t getPerfectHashSize(
	const std::array<T, size>& buf) {
	
	// first get all the hashes
	Hash hasher;
	auto keys = for_each(
		buf,
		getFirst<typename T::first_type, typename T::second_type>);
	auto hashes = for_each(keys, hasher);

	// then try to find if there is a chance that not all
	// of the hashes will get a cache collision
	for (size_t i = 0; i < 2000; i++) {
		size_t modBase = (size + i * 3) * 2.71828;
		std::array<size_t, size> modded{};
		for (int i = 0; i < size; i++) {
			auto& ref = getRef(modded, i);
			ref = getRef(hashes, i) % modBase;
		}
		
		// if there are repeats, we need to increase modBase
		// size, rehash everything and try again

		auto sorted = mergeSort(modded);
		if (size == adjacent_find(sorted)) {
			return modBase;
		}
	}

	return 0;
}

template<
	typename HashFunc,
	size_t dst,
	size_t src,
	typename Key,
	typename Value>
constexpr std::array<std::pair<Key, Value>, dst>
transformWithHash(
	const std::array<std::pair<Key, Value>, src>& srcArr) {

	std::array<std::pair<Key, Value>, dst> dstArr{};
	HashFunc hasher;
	for (int i = 0; i < src; i++) {
		size_t hashVal = hasher(srcArr[i].first);
	 	size_t modded = hashVal % dst;
		auto& ref = getRef(dstArr, modded);
		ref.first = srcArr[i].first;
		ref.second = srcArr[i].second;
	}
	
	return dstArr;
}

template<
  size_t bufSize,
  size_t mapSize,
  typename Key,
  typename Value,
  typename hash=std::hash<Key>
>
class HashMap {
 public:
  constexpr HashMap(
		const std::array<std::pair<Key, Value>, bufSize>& arr):
    buf_(transformWithHash<hash, mapSize>(arr)) {}

	constexpr Value get(const Key& key) const {
		hash hasher;
		size_t idx = hasher(key) % mapSize;
		if (buf_[idx].first == key) {
			return buf_[idx].second;
		} else {
			return Value();
		}
	}

	void print() const {
		std::cout << "show buf size " << buf_.size();
		for(auto it = buf_.begin(); it != buf_.end(); it++) {
			if (it->first) {
				std::cout << "key " << it->first << " idx "
									<< std::distance(buf_.begin(), it) << std::endl;
			}
		}
	}

 private:
  std::array<std::pair<Key, Value>, mapSize> buf_;
};

}
