#pragma once

#include <array>
#include "./base.h"

namespace cexpr {

template <typename ForwardIterator, typename UnaryOp>
constexpr bool all_of(ForwardIterator begin, ForwardIterator end, UnaryOp op) {
  while (begin != end) {
    if (!op(*begin)) {
      return false;
    }
    begin++;
  }
  return true;
}

template <typename ForwardIterator, typename UnaryOp>
constexpr bool any_of(ForwardIterator begin, ForwardIterator end, UnaryOp op) {
  while (begin != end) {
    if (op(*begin)) {
      return true;
    }
    begin++;
  }
  return false;
}

template <typename ForwardIterator, typename UnaryOp>
constexpr bool none_of(ForwardIterator begin, ForwardIterator end, UnaryOp op) {
  return !any_of(begin, end, op);
}

// this foreach is slightly difference, since input is
// immutable in constexpr setting we need to do a copy
template <
    size_t N, typename UnaryOp, typename ForwardIterator,
    typename T = typename std::iterator_traits<ForwardIterator>::value_type>
constexpr std::array<typename std::result_of<UnaryOp(const T&)>::type, N>
for_each_n(ForwardIterator begin, ForwardIterator end, UnaryOp unaryOp) {
  std::array<typename std::result_of<UnaryOp(const T&)>::type, N> output{};

  for (int i = 0; i < N && begin != end; i++, begin++) {
    auto& ref = getRef(output, i);
    ref = unaryOp(*begin);
  }
  return output;
}

template <
    size_t size, typename UnaryOp, typename ForwardIterator,
    typename T = typename std::iterator_traits<ForwardIterator>::value_type>
constexpr std::array<typename std::result_of<UnaryOp(const T&)>::type, size>
for_each(ForwardIterator begin, ForwardIterator end, UnaryOp unaryOp) {
  return for_each_n<size>(begin, end, unaryOp);
}

template <typename T, size_t size, typename UnaryOp,
          typename Out = typename std::result_of<UnaryOp(const T&)>::type>
constexpr std::array<Out, size> for_each_array(
    const std::array<T, size>& cinput, UnaryOp unaryOp) {
  return for_each<size>(cinput.begin(), cinput.end(), unaryOp);
}

template <typename UnaryOp, typename T, size_t size>
void for_each(const std::array<T, size>& cinput, UnaryOp unaryOp) {
  return for_each_n<size>(cinput.cbegin(), cinput.cend(), unaryOp);
}

template <typename ForwardIterator, typename T>
constexpr size_t count(
  ForwardIterator begin, ForwardIterator end, const T& val) {
  size_t cnt = 0;
  for (; begin != end; begin++) {
    cnt += *begin == val;
  }
  return cnt;
}

// mismatch cannot be implemented because iterator is currently
// not constexpr

template <typename ForwardIterator, typename BinaryPredicate>
constexpr bool equal(ForwardIterator lBegin,
                     ForwardIterator lEnd, ForwardIterator rBegin,
                     ForwardIterator rEnd,
                     BinaryPredicate binaryPredicate) {
  for (; lBegin != lEnd && rBegin != rEnd; lBegin++, rBegin++) {
    if (!binaryPredicate(*lBegin, *rBegin)) {
      return false;
    }
  }
  // if they did not run to the end together, then they are not equal
  return lBegin == lEnd && rBegin == rEnd;
}

template <typename ForwardIterator>
constexpr bool equal(ForwardIterator lBegin,
                     ForwardIterator lEnd, ForwardIterator rBegin,
                     ForwardIterator rEnd) {
  using T = typename std::iterator_traits<ForwardIterator>::value_type;
  return equal(lBegin, lEnd, rBegin, rEnd, [](const T& lhs, const T& rhs) {
    return lhs == rhs; 
  });
}

// for find_if we will return index, otherwise return size
template <typename ForwardIterator, typename UnaryPredicate>
constexpr ForwardIterator find_if(
  ForwardIterator begin,
  ForwardIterator end,
  UnaryPredicate predicate) {
  for (; begin != end; begin++) {
    if (predicate(*begin)) {
      return begin;
    }
  }
  return end;
}

template<typename Predicate, typename T>
struct UnaryPredicateNegation {
  constexpr UnaryPredicateNegation(Predicate predicate): predicate_(predicate) {}

  constexpr bool operator()(const T& val) {
    return !predicate_(val);
  }

  Predicate predicate_;
};

// if I can figure out how to chain functions in constexpr
template <typename ForwardIterator, typename UnaryPredicate>
constexpr ForwardIterator find_nof_if(
  ForwardIterator begin,
  ForwardIterator end,
  UnaryPredicate predicate) {
  using T = typename std::iterator_traits<ForwardIterator>::value_type;
  return find_if(begin, end, UnaryPredicateNegation<UnaryPredicate, T>(predicate));
}

// did not implement find_end

template<class ForwardIt, class BinaryPredicate>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last, 
                        BinaryPredicate p)
{
    if (first == last) {
        return last;
    }
    ForwardIt next = first;
    ++next;
    for (; next != last; ++next, ++first) {
        if (p(*first, *next)) {
            return first;
        }
    }
    return last;
}

template<class ForwardIt>
constexpr ForwardIt adjacent_find(ForwardIt first, ForwardIt last) {
  using T = typename std::iterator_traits<ForwardIt>::value_type;
  return adjacent_find(first, last, [](const T& lhs, const T& rhs) {
    return lhs == rhs;
  });
}


template <typename InputIt, typename OutputIt, typename UnaryPredicate>
constexpr OutputIt copy_if(InputIt first, InputIt last,
                  OutputIt d_first,
                  UnaryPredicate pred) {
  for(; first != last; first++) {
    if (pred(*first)) {
      *d_first = *first;
      d_first++;
    }
  }
  return d_first;
}


template <typename InputIt, typename OutputIt>
constexpr OutputIt copy(InputIt first, InputIt last,
                  OutputIt d_first) {
  return copy_if(first, last, d_first, [](const auto& val) {
    return true;
  });
}

// copy_n
// (C++11)
// 	copies a number of elements to a new location
// (function template)

// copy_backward
// 	copies a range of elements in backwards order
// (function template)

// move
// (C++11)
// 	moves a range of elements to a new location
// (function template)

// move_backward
// (C++11)
// 	moves a range of elements to a new location in backwards order
// (function template)

// fill
// 	copy-assigns the given value to every element in a range

// (function template)
// fill_n
// 	copy-assigns the given value to N elements in a range
// (function template)

// transform
// 	applies a function to a range of elements
// (function template)

// generate
// 	assigns the results of successive function calls to every element in a
// range

// (function template)
// generate_n
// 	assigns the results of successive function calls to N elements in a
// range
// (function template)
// removeremove_if
// 	removes elements satisfying specific criteria
// (function template)
// remove_copyremove_copy_if
// 	copies a range of elements omitting those that satisfy specific criteria
// (function template)
// replacereplace_if
// 	replaces all values satisfying specific criteria with another value
// (function template)
// replace_copyreplace_copy_if
// 	copies a range, replacing elements satisfying specific criteria with
// another value
// (function template)
// swap
// 	swaps the values of two objects
// (function template)
// swap_ranges
// 	swaps two ranges of elements
// (function template)
// iter_swap
// 	swaps the elements pointed to by two iterators
// (function template)
// reverse
// 	reverses the order of elements in a range
// (function template)
// reverse_copy
// 	creates a copy of a range that is reversed
// (function template)
// rotate
// 	rotates the order of elements in a range
// (function template)
// rotate_copy
// 	copies and rotate a range of elements
// (function template)

// random_shuffleshuffle
// (until C++17)(C++11)
// 	randomly re-orders elements in a range
// (function template)
// sample
// (C++17)
// 	selects n random elements from a sequence
// (function template)
// unique
// 	removes consecutive duplicate elements in a range
// (function template)
// unique_copy
// 	creates a copy of some range of elements that contains no consecutive
// duplicates
// (function template)
// Partitioning operations
// Defined in header <algorithm>
// is_partitioned
// (C++11)
// 	determines if the range is partitioned by the given predicate
// (function template)
// partition
// 	divides a range of elements into two groups
// (function template)
// partition_copy
// (C++11)
// 	copies a range dividing the elements into two groups
// (function template)
// stable_partition
// 	divides elements into two groups while preserving their relative order
// (function template)
// partition_point
// (C++11)
// 	locates the partition point of a partitioned range
// (function template)
// Sorting operations
// Defined in header <algorithm>

// is_sorted
// (C++11)
// 	checks whether a range is sorted into ascending order
// (function template)

// There seems to be some issue with <functional>, so I am going to
// implement my own

template<typename Comparator, typename T>
struct ReverseComparator {
  constexpr ReverseComparator(Comparator compare): compare_(compare) {}
  constexpr bool operator()(const T& lhs, const T& rhs) {
    return compare_(rhs, lhs);
  }

  Comparator compare_;
};

template <class ForwardIt, class Compare>
constexpr ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp) {
  using T = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt it = adjacent_find(first, last, ReverseComparator<Compare, T>(comp));
  return it == last ? last : ++it;
}

template< class ForwardIt, class Compare>
constexpr bool is_sorted(ForwardIt first, ForwardIt last, Compare comp) {
  return is_sorted_until(first, last, comp) == last;  
}

template< class ForwardIt>
constexpr bool is_sorted(ForwardIt first, ForwardIt last) {
  using T = typename std::iterator_traits<ForwardIt>::value_type;
  return is_sorted_until(first, last, [](const T& lhs, const T& rhs){
    return lhs <= rhs;
  }) == last;  
}

// is_sorted_until
// (C++11)
// 	finds the largest sorted subrange
// (function template)
// sort
// 	sorts a range into ascending order
// (function template)
// partial_sort
// 	sorts the first N elements of a range
// (function template)
// partial_sort_copy
// 	copies and partially sorts a range of elements
// (function template)
// stable_sort
// 	sorts a range of elements while preserving order between equal elements
// (function template)
// nth_element
// 	partially sorts the given range making sure that it is partitioned by
// the given element
// (function template)
// Binary search operations (on sorted ranges)
// Defined in header <algorithm>
// lower_bound
// 	returns an iterator to the first element not less than the given value
// (function template)
// upper_bound
// 	returns an iterator to the first element greater than a certain value
// (function template)
// binary_search
// 	determines if an element exists in a certain range
// (function template)
// equal_range
// 	returns range of elements matching a specific key
// (function template)
// Set operations (on sorted ranges)
// Defined in header <algorithm>
// merge
// 	merges two sorted ranges
// (function template)
// inplace_merge
// 	merges two ordered ranges in-place
// (function template)
// includes
// 	returns true if one set is a subset of another
// (function template)
// set_difference
// 	computes the difference between two sets
// (function template)
// set_intersection
// 	computes the intersection of two sets
// (function template)
// set_symmetric_difference
// 	computes the symmetric difference between two sets
// (function template)
// set_union
// 	computes the union of two sets
// (function template)
// Heap operations
// Defined in header <algorithm>
// is_heap
// (C++11)
// 	checks if the given range is a max heap
// (function template)
// is_heap_until
// (C++11)
// 	finds the largest subrange that is a max heap
// (function template)
// make_heap
// 	creates a max heap out of a range of elements
// (function template)
// push_heap
// 	adds an element to a max heap
// (function template)
// pop_heap
// 	removes the largest element from a max heap
// (function template)
// sort_heap
// 	turns a max heap into a range of elements sorted in ascending order
// (function template)
// Minimum/maximum operations
// Defined in header <algorithm>
// max
// 	returns the greater of the given values
// (function template)
// max_element
// 	returns the largest element in a range
// (function template)
// min
// 	returns the smaller of the given values
// (function template)
// min_element
// 	returns the smallest element in a range
// (function template)
// minmax
// (C++11)
// 	returns the smaller and larger of two elements
// (function template)
// minmax_element
// (C++11)
// 	returns the smallest and the largest elements in a range
// (function template)
// clamp
// (C++17)
// 	clamps a value between a pair of boundary values
// (function template)
// lexicographical_compare
// 	returns true if one range is lexicographically less than another
// (function template)
// is_permutation
// (C++11)
// 	determines if a sequence is a permutation of another sequence
// (function template)
// next_permutation
// 	generates the next greater lexicographic permutation of a range of
// elements
// (function template)
// prev_permutation
// 	generates the next smaller lexicographic permutation of a range of
// elements
// (function template)
// Numeric operations
// Defined in header <numeric>
// iota
// (C++11)
// 	fills a range with successive increments of the starting value
// (function template)
// accumulate
// 	sums up a range of elements
// (function template)
// inner_product
// 	computes the inner product of two ranges of elements
// (function template)
// adjacent_difference
// 	computes the differences between adjacent elements in a range
// (function template)
// partial_sum
// 	computes the partial sum of a range of elements
// (function template)
// reduce
// (C++17)
// 	similar to std::accumulate, except out of order
// (function template)
// exclusive_scan
// (C++17)
// 	similar to std::partial_sum, excludes the ith input element from the ith
// sum
// (function template)
// inclusive_scan
// (C++17)
// 	similar to std::partial_sum, includes the ith input element in the ith
// sum
// (function template)
// transform_reduce
// (C++17)
// 	applies a functor, then reduces out of order
// (function template)
// transform_exclusive_scan
// (C++17)
// 	applies a functor, then calculates exclusive scan
// (function template)
// transform_inclusive_scan
// (C++17)
// 	applies a functor, then calculates inclusive scan
// (function template)
}
