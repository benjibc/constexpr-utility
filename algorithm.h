#pragma once

#include <array>
#include "./base.h"

namespace cexpr {

template<typename T, size_t size>
constexpr bool all_of(
  const std::array<T, size>& input,
  bool(*predicate)(const T&)) {
  for(int i = 0; i < size; i++) {
    if (!predicate(input[i])) {
      return false;
    }
  }
  return true;
}

template<typename T, size_t size>
constexpr bool any_of(
  const std::array<T, size>& input,
  bool(*predicate)(const T&)) {
  for(int i = 0; i < size; i++) {
    if (predicate(input[i])) {
      return true;
    }
  }
  return false;
}

template<typename T, size_t size>
constexpr bool none_of(
  const std::array<T, size>& input,
  bool(*predicate)(const T&)) {
  return !any_of(input, predicate);
}

// this foreach is slightly difference, since input is
// immutable in constexpr setting we need to do a copy
template<size_t N, typename UnaryOp, typename T, size_t size>
constexpr std::array<
	typename std::result_of<UnaryOp(const T&)>::type,
	size>
for_each_n(
  const std::array<T, size>& cinput,
	UnaryOp unaryOp) {

	std::array<typename std::result_of<UnaryOp(const T&)>::type, N> output{};

  for (int i = 0; i < N; i++) {
    auto& ref = getRef(output, i);
    ref = unaryOp(cinput[i]);
  } 
  return output;
}

template<typename UnaryOp, typename T, size_t size>
constexpr std::array<
	typename std::result_of<UnaryOp(const T&)>::type,
	size>
for_each(
  const std::array<T, size>& cinput,
	UnaryOp unaryOp) {
  return for_each_n<size>(cinput, unaryOp);
}

template<typename T, size_t size>
constexpr size_t count(
  const std::array<T, size>& cinput,
  const T& val) {

  size_t cnt = 0;
  for (int i = 0; i < size; i++) {
    cnt += cinput[i] == val;
  } 
  return cnt;
}

// mismatch cannot be implemented because iterator is currently
// not constexpr

template<typename T, size_t size>
constexpr bool equal(
  const std::array<T, size>& lhs,
  const std::array<T, size>& rhs,
  bool(*binaryPredicate)(const T&, const T&)) {
  
  for (int i = 0; i < size; i++) {
    if (!binaryPredicate(lhs[i], rhs[i])) {
      return false;
    }
  } 
  return true; 
}

template<typename T, size_t size>
constexpr bool equal(
  const std::array<T, size>& lhs,
  const std::array<T, size>& rhs) {
  
  for (int i = 0; i < size; i++) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  } 
  return true; 
}

// for find_if we will return index, otherwise return size
template<typename T, size_t size>
constexpr size_t find_if(
  const std::array<T, size>& input,
  bool(*predicate)(const T&)) {

  for (int i = 0; i < size; i++) {
    if (predicate(input[i])) {
      return i;
    }
  }
  return size;
}

// if I can figure out how to chain functions in constexpr 
template<typename T, size_t size>
constexpr size_t find_not_if(
  const std::array<T, size>& input,
  bool(*predicate)(const T&)) {

  for (int i = 0; i < size; i++) {
    if (!predicate(input[i])) {
      return i;
    }
  }
  return size;
}

// did not implement find_end

template<typename T, size_t size>
constexpr size_t adjacent_find(const std::array<T, size>& input) {
  for (int i = 0; i < size - 1; i++) {
    if (input[i] == input[i+1]) {
      return i;
    }
  }
  return size;
}

// below are all the things I did not implement
// copy_if
// (C++11)
// 	copies a range of elements to a new location
// (function template)
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
// 	assigns the results of successive function calls to every element in a range
// (function template)
// generate_n
// 	assigns the results of successive function calls to N elements in a range
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
// 	copies a range, replacing elements satisfying specific criteria with another value
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
// 	creates a copy of some range of elements that contains no consecutive duplicates
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
// 	partially sorts the given range making sure that it is partitioned by the given element
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
// 	generates the next greater lexicographic permutation of a range of elements
// (function template)
// prev_permutation
// 	generates the next smaller lexicographic permutation of a range of elements
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
// 	similar to std::partial_sum, excludes the ith input element from the ith sum
// (function template)
// inclusive_scan
// (C++17)
// 	similar to std::partial_sum, includes the ith input element in the ith sum
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
