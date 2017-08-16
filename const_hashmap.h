#include <array>
#include <cstdint>
#include <iostream>

namespace cexpr {

namespace {
constexpr uint32_t offset = 0x811C9DC5;
constexpr uint32_t prime = 0x01000193;
}

constexpr uint32_t fnv1(const char* src) {
  uint32_t start = offset;
  while (*src != '\0') {
    start = (static_cast<uint32_t>(*src) * prime) ^ start;
    src++;
  }
  return start;
}

constexpr uint32_t fnv1a(const char* src) {
  uint32_t start = offset;
  while (*src != '\0') {
    start = (static_cast<uint32_t>(*src) ^ start) * prime;
    src++;
  }
  return start;
}

template<typename T, size_t size, size_t sub>
constexpr std::array<T, sub> copyRange(
  const std::array<T, size> full,
  size_t begin,
  size_t end) {
  static_assert(begin + sub == end);

  std::array<T, sub> dst;
  while (begin != end) {
    dst 
  }  
}

template<typename Arr>
typename Arr::value_type& getRef(
  const Arr& buf,
  size_t bufIdx) {
  return const_cast<Arr::value_type &>(static_cast<Arr const&>(buf)[bufIdx]);
}

template<typename T, size_t size>
constexpr std::array<T, size> mergeSort(
  const std::array<T, size> arr) {
  size_t middle = size /2;
     
}

template<typename T, size_t size1, size_t size2>
constexpr std::array<T, size1 + size2> merge(
  const std::array<T, size1> lhs,
  const std::array<T, size2> rhs) {
  
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

template<
  size_t bufSize,
  typename Key,
  typename Value,
  typename hash=std::hash<Key>
>
class HashMap {
 public:
  template<class...U>
  constexpr HashMap(U...u):
    m_size(sizeof...(U)),
    buf_() {}

  constexpr size_t size() {
    return m_size;
  }

 private:
  size_t m_size;
  std::array<std::pair<Key, Value>, bufSize> buf_;
};

}
