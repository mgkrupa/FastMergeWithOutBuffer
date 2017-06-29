/*
 * merge_common.h
 *
 *  Created on: Jun 29, 2017
 *      Author: diagoras
 */

#ifndef SRC_MERGE_COMMON_H_
#define SRC_MERGE_COMMON_H_

#include <cassert>

/* Finds the SMALLEST integer  0 <= d < length IF it exists such that
 *  (*) *(end_left - d) <= *(start_right + d), and
 *  otherwise, if such a d does NOT exist then it returns length - 1.
 * Assumes that [start_left : start_index + length - 1] is non-decreasing,
 *  that all these elements exist, and that BOTH subvectors have size >= length
 *  and that length > 0, where start_left = end_left - (length - 1).
 * WARNING: POTENTIAL FALSE POSITIVE: There are two ways that this function may
 *  return length - 1:
 *  1) Such a d does NOT exist. i.e. *start_index > *end_right.
 *  2) Such a d exists and happens to equal length - 1. i.e.
 *     *start_left <= *end_right and *(start_left + 1) > *(end_right - 1]
 * NOTES:
 *  (1) If *end_left > *(end_left + 1) and such a d exists then d is
 *       necessarily > 0.
 *  (2) E1L1 means end_left and length_left are the inputs (as opposed to say,
 *       start_left and start_right)
 */
template<class RAI>
inline int DisplacementFromMiddleIiteratorToPotentialMediansContiguous_KnownToExist(const RAI end_left, int length) {
  auto start_right   = end_left + 1;
  (void)length--;  //We will now use length_left as if it were d_upper.
  int d_lower = 0; //So that end_left - d_lower = end_left
  do {
    auto d = d_lower + ((length - d_lower)/2);
    if (*(end_left - d) <= *(start_right + d))
      length       = d;
    else
      d_lower       = d + 1;
  } while (d_lower < length) ;
  return d_lower;
}

/* Does the equivalent of: while(start_it < end_it && vec[start_it] <= vec[end_it]) ++start_it;
 *  So if no such
 * Does the same thing as LargestIndexWithValueLessThan(), except that its inputs are different
 *  and also, in addition to performing a binary search, it simultaneously performs a linear search starting
 *  from the end_it
 * SE = inputs are start_it and end_it
 */
template<class ForwardIterator, class T>
ForwardIterator SmallestIteratorWithValueGreaterThan_KnownToExist(ForwardIterator start_it, ForwardIterator end_it, const T value) {
  while (true) {
    //The below lines are optional. They are performed at most
    // std::distance(start_it, d) + 1 times.
    if (*start_it > value) {
      return start_it;
    }
    (void)start_it++;

    //The below lines are optional. They are performed at most
    // std::distance(d, end_it) + 1 times.
    auto cur_length_minus_one = std::distance(start_it, end_it);
    if (*(start_it + cur_length_minus_one - 1) <= value) {
      return end_it;
    }
    (void)end_it--;

    auto d = start_it + std::distance(start_it, end_it) / 2;
    if (*d <= value)
      start_it = d + 1;//Note that start_it will be <= end_it since the desired it is known to exist.
    else
      end_it   = d;
  }
  //At this point, start_it == end_it OR start_it == end_it + 1.
  //return start_it + (vec[start_it] > value ? 0 : 1);
}

/* Does the equivalent of: while(*end_right >= *end_left) end_right--;
 * NOTE: This means that it assumes that such an index exists exists within
 *  [start_it : end_it] so IT DOES NO ITERATOR BOUNDS CHECKING.
 * Does the same thing as LargestIndexWithValueLessThan(), except that its
 *  inputs are different and also, in addition to performing a binary search,
 *  it simultaneously performs a linear search starting from the end_it.
 * Assumes that start_it <= end_it.
 * SE means that the inputs are start_index and end_index.
 * This algorithm find d by performing <= 3 * min {dist(start_it, d) + 1,
 *  dist(d, end_it) + 1, ceil(log_2(dist(start_it, end_it + 1)))} comparisons.
 */
template<class RAI, class T>
inline RAI LargestIteratorWithValueLessThan_KnownToExist(RAI start_it, RAI end_it, const T value) {
  while (true) {//Use: while(start_it < end_it){ if the d is not known to exist.
    //The below three lines are optional. They are performed at most
    // std::distance(d, end_it) + 1 times.
    if (*end_it < value) {
      return end_it;
    }
    (void)end_it--;

    //The below three lines are optional. They are performed at most
    // std::distance(start_it, d) + 1 times.
    if (*(start_it + 1) >= value) {
      return start_it;
    }
    (void)start_it++;

    auto d = start_it + std::distance(start_it, end_it) / 2;
    if (*d < value)
      start_it = d;
    else
      end_it   = d - 1;
  }
}

/* Assumes that [start, end] is non-decreasing, that the iterator
 *  ele_to_shift points to an element not in the interval, and that the
 *  singleton interval [ele_to_shift, ele_to_shift] is imagined to
 *  "lie to the LEFT" of [start, end]. i.e. that we have the following
 *  situation: ... [ele_to_shift] ... [start, end] ....
 *
 * If it exists then it will find the smallest d > 0 such that
 *  *(start + d) >= *ele_to_shift and then shifts/rotates all elements in
 *  [start, start + d) left-wards (i.e. towards end_left) by 1 and swaps end
 *  with ele_to_shift resulting in [ele_to_shift, start, ..., end] being
 *  non-decreasing.
 * If such a d does not exist then it does nothing.
 * This is a helper function for MergeTrivialCases().
 */
template<class RAI>
inline void RotateLeftByExactlyOneElement(RAI start, RAI end, RAI ele_to_shift) {
  const auto value = *ele_to_shift;
  if (*start >= value)
    return ;
  else
    std::iter_swap(start, ele_to_shift);
  for (auto it = start; it < end && *(it + 1) < value; (void)it++)
    std::iter_swap(it, it + 1);
}

/* Assumes that [start, end] is non-decreasing, that the iterator
 *  ele_to_shift points to an element not in the interval, and that the
 *  singleton interval [ele_to_shift, ele_to_shift] is imagined to
 *  "lie to the RIGHT" of [start, end]. i.e. that we have the following
 *  situation: ... [start, end] ... [ele_to_shift] ....
 *
 * If it exists then it will find the smallest d > 0 such that
 *  *(end - d) <= *ele_to_shift and then shifts/rotates all elements in
 *  (end - d, end] right-wards (i.e. towards end) by 1 and swaps end with
 *  ele_to_shift resulting in [start, ..., end, ele_to_shift] being
 *  non-decreasing.
 * If such a d does not exist then it does nothing.
 * This is a helper function for MergeTrivialCases().
 */
template<class RAI>
inline void RotateRightByExactlyOneElement(RAI start, RAI end, RAI ele_to_shift) {
  const auto value = *ele_to_shift;
  if (*end <= value)
    return ;
  else
    std::iter_swap(end, ele_to_shift);
  for (auto it = end - 1; it > start && *it > value; (void)it--)
    std::iter_swap(it, it + 1);
}

/* Given two ranges [start_left, end_left] and [start_right, end_right], this
 *  function moves all elements in such a way that in the range
 *  [start_left, ... , end_left, start_right, ..., end_right] all elements
 *  will be shifted right by std::distance(start_right, end_right + 1),
 *  resulting in the range:
 *  [start_right, ... , end_right, start_left, ..., end_left].
 * This is a helper function for MergeTrivialCases().
 */
template<class RAI>
inline void ShiftRightSideToTheRightByItsLength(RAI start_left, RAI end_left,
                                         RAI start_right, RAI end_right) {
  auto end_left_plus1  = end_left + 1;
  auto end_right_plus1 = end_right + 1;
  auto length_left  = std::distance(start_left, end_left_plus1);
  auto length_right = std::distance(start_right, end_right_plus1);

  while (length_left > 0 && length_right > 0) {
    RAI start_swap1, end_swap1, start_swap2;
    if (length_left < length_right) {
      start_swap1   = start_left;
      end_swap1     = end_left_plus1;
      start_swap2   = end_right_plus1 - length_left;
      length_right -= length_left;
      end_right    -= length_left;
      end_right_plus1 = end_right + 1;
    } else {
      start_swap1  = start_right;
      end_swap1    = end_right_plus1;
      start_swap2  = start_left;
      length_left -= length_right;
      start_left  += length_right;
    }
    std::swap_ranges(start_swap1, end_swap1, start_swap2);
  }
  return ;
}

/* This is a helper function that merges two ranges when the merge is trivial.
 */
template<class T>
void MergeTrivialCases(T start_left,  T end_left, T start_right, T end_right,
                       T * start_left_out, T * end_right_out) {
  if (start_left <= end_left && start_right <= end_right && *end_left > *start_right) {
    if (*start_left >= *end_right) {
//TO DO: ????CHECK THAT THE BELOW FUNCTION WORKS AS INTENDED????

      ///Note that this function has the same effect as
      // std::rotate(vec.begin() + start_left, vec.begin() + start_right,
      //             vec.begin() + (end_right + 1));
      // except that it works for ranges iterated by distinct objects.
      ShiftRightSideToTheRightByItsLength(start_left, end_left, start_right, end_right);
    } else if (end_left == start_left) {
      RotateLeftByExactlyOneElement(start_right, end_right, end_left);
    } else {// if (start_right == end_right) {
      RotateRightByExactlyOneElement(start_left, end_left, start_right);
    }
  }
  *start_left_out = ++end_left;
  *end_right_out  = start_right;
  return ;
}



#endif /* SRC_MERGE_COMMON_H_ */