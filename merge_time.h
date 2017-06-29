/*
 * merge_time.h
 *
 *  Created on: Jun 28, 2017
 *      Author: Matthew Gregory Krupa
 */

#ifndef SRC_MERGE_TIME_H_
#define SRC_MERGE_TIME_H_

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "gnu_merge_without_buffer.h"
#include "merge.h"
#include "random_helpers.h"

template<class T> std::chrono::nanoseconds TimeStdInplaceMergeWithOutBufferOnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  {
  std::inplace_merge(vec.begin(), vec.begin() + start_second, vec.end());
  vec = vec_original;
  }
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::inplace_merge(vec.begin(), vec.begin() + start_second, vec.end());
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

template<class T> std::chrono::nanoseconds TimeStdMergeOnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  {
  std::vector<T> temp_vec(vec.size());
  std::merge(vec.begin(), vec.begin() + start_second, vec.begin() + start_second, vec.end(), temp_vec.begin());
  vec = vec_original;
  }
  //volatile T vol_time = 0;
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<T> temp_vec(vec.size());
    std::merge(vec.begin(), vec.begin() + start_second, vec.begin() + start_second, vec.end(), temp_vec.begin());
    vec = temp_vec;
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

template<class T> std::chrono::nanoseconds TimeMergeWithOutBufferOnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  {
  MergeWithOutBuffer(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
  vec = vec_original;
  }
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    MergeWithOutBuffer(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

template<class T> std::chrono::nanoseconds TimeStdMergeWithOutBufferOnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  auto comp = [](const T &lhs, const T &rhs) -> bool{return lhs < rhs;};
  {
  gnu::gnu__merge_without_buffer(vec.begin(), vec.begin() + start_second, vec.end(), start_second, vec.size() - start_second, comp);
  vec = vec_original;
  }
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    gnu::gnu__merge_without_buffer(vec.begin(), vec.begin() + start_second, vec.end(), start_second, vec.size() - start_second, comp);
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

template<class T> std::chrono::nanoseconds TimeMergeWithOutBuffer1OnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  {
  MergeWithOutBufferTrim1(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
  vec = vec_original;
  }
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    MergeWithOutBufferTrim1(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

template<class T> std::chrono::nanoseconds TimeMergeWithOutBuffer2OnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original, std::size_t start_second, std::size_t num_repititions = 1) {
  std::chrono::nanoseconds total{0};
  {
  MergeWithOutBufferTrim2(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
  vec = vec_original;
  }
  for (std::size_t num_repititions_counter = 0; num_repititions_counter < num_repititions; num_repititions_counter++) {
    auto start_time = std::chrono::high_resolution_clock::now();
    MergeWithOutBufferTrim2(vec.begin(), vec.begin() + (start_second - 1), vec.begin() + start_second, vec.end());
    total += std::chrono::high_resolution_clock::now() - start_time;
    vec = vec_original;
  }
  return static_cast<std::chrono::nanoseconds>(total);
}

struct TotalTimes {
  std::chrono::nanoseconds std_merge{0};
  std::chrono::nanoseconds std_inplace_merge{0};
  std::chrono::nanoseconds merge_without_buffer{0};
  std::chrono::nanoseconds merge_without_buffer2{0};
  std::chrono::nanoseconds merge_without_buffer1{0};
  std::chrono::nanoseconds gnu_merge_without_buffer{0};

  std::size_t max_nano_str_width = 1;

  TotalTimes &operator+=(const TotalTimes &rhs) {
    std_inplace_merge         += rhs.std_inplace_merge;
    std_merge                 += rhs.std_merge;
    merge_without_buffer      += rhs.merge_without_buffer;
    gnu_merge_without_buffer  += rhs.gnu_merge_without_buffer;
    merge_without_buffer1     += rhs.merge_without_buffer1;
    merge_without_buffer2     += rhs.merge_without_buffer2;
    return *this;
  }
  template<class T> static std::size_t GetStringWidth(T value) {
    return std::to_string(value).length();
  }
  std::string GetTimeStr(std::chrono::nanoseconds nano_time, long double divisor) {
    std::stringstream strm;
    if (divisor == 0.0l)
      divisor = 1.0l;
    long long nano = static_cast<long long>(nano_time.count() / divisor);
    auto nano_str_length = GetStringWidth(nano);
    if (nano_str_length > max_nano_str_width)
      max_nano_str_width = nano_str_length;
    strm << std::setw(max_nano_str_width) << nano << " ns = ";
    strm << std::setw(max_nano_str_width > 3 ? max_nano_str_width - 3 : 1) << (nano/1000) << " mus = ";
    strm << std::setw(max_nano_str_width > 6 ? max_nano_str_width - 6 : 1) << (nano/1000000) << " ms = ";
    strm << std::setw(max_nano_str_width > 9 ? max_nano_str_width - 9 : 1) << (nano/1000000000) << " s";
    return strm.str();
  }
  std::string GetAveragesStr(std::size_t divisor) {
    std::stringstream strm;
    long double divisor_ld = static_cast<long double>(divisor);
    strm << "Merge algorithms that use a buffer:\n";
    strm << "std::merge ave               = " << GetTimeStr(std_merge, divisor_ld) << '\n';
    strm << "Merge algorithms that use a buffer if available:\n";
    strm << "std::inplace_merge ave       = " << GetTimeStr(std_inplace_merge, divisor_ld) << '\n';
    strm << "Merge algorithms that do not use a buffer:\n";
    strm << "merge_without_buffer ave     = " << GetTimeStr(merge_without_buffer, divisor_ld) << '\n';
    strm << "merge_without_buffer2 ave    = " << GetTimeStr(merge_without_buffer2, divisor_ld) << '\n';
    strm << "merge_without_buffer1 ave    = " << GetTimeStr(merge_without_buffer1, divisor_ld) << '\n';
    strm << "gnu_merge_without_buffer ave = " << GetTimeStr(gnu_merge_without_buffer, divisor_ld) << '\n';
    return strm.str();
  }
};

template<class T> inline TotalTimes TimeMergesOnGivenVec(std::vector<T> &vec, const std::vector<T> &vec_original,
                                                  std::size_t start_second,
                                                  std::size_t num_repititions_per_vec = 1) {
  TotalTimes total_times;
  try {
    total_times.std_merge += TimeStdMergeOnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeStdMergeOnGivenVec() Failed." << std::endl;
  }
  try {
    total_times.std_inplace_merge += TimeStdInplaceMergeWithOutBufferOnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeStdInplaceMergeWithOutBufferOnGivenVec() Failed." << std::endl;
  }
  try {
    total_times.merge_without_buffer += TimeMergeWithOutBufferOnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeMergeWithOutBufferOnGivenVec() Failed." << std::endl;
  }
  try {
    total_times.merge_without_buffer2 += TimeMergeWithOutBuffer2OnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeStdMergeWithOutBuffer2OnGivenVec() Failed." << std::endl;
  }
  try {
    total_times.merge_without_buffer1 += TimeMergeWithOutBuffer1OnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeStdMergeWithOutBuffer1OnGivenVec() Failed." << std::endl;
  }
  try {
    total_times.gnu_merge_without_buffer += TimeStdMergeWithOutBufferOnGivenVec(vec, vec_original, start_second, num_repititions_per_vec);
  } catch (...) {
    std::cout << "TimeStdMergeWithOutBufferOnGivenVec() Failed." << std::endl;
  }
  return total_times;
}


/* Example call
  std::size_t vec_size, num_tests_per_vec_size, num_repititions_per_vec;
  auto start_right = -2;
  auto pick_new_start_right_for_each_new_vec = true;
  auto print_vec_averages = false;
  auto print_total_averages = true;
  vec_size = (1u << 6);
  num_tests_per_vec_size = (1u << 10);
  num_tests_per_vec_size = (1u << 8);
  TimeMergesOnGivenVecSize<int>(vec_size, num_tests_per_vec_size, num_repititions_per_vec,
                    start_right, pick_new_start_right_for_each_new_vec,
                    print_vec_averages, print_total_averages, false,
                    0, 100 * vec_size);
 */
template<class T> TotalTimes TimeMergesOnGivenVecSize(std::size_t vec_size,
                    std::size_t num_tests_per_vec_size = 1,
                    std::size_t num_repititions_per_vec = 1,
                    int start_right = -1, //set to -1 to pick it to be the midpoint,
                        //and set it to be < -1 to pick it randomly
                    bool pick_new_start_right_for_each_new_vec = false,
                    bool print_vec_averages = false,
                    bool print_total_averages = true,
                    bool verbose = false,
                    T lower_bound = std::numeric_limits<T>::min(),
                    T upper_bound = std::numeric_limits<T>::max()) {
  if (start_right == -1) {
    start_right = vec_size / 2;
  } else if (start_right < -1) {
    std::random_device rnd_device;
    std::mt19937 generator(rnd_device());
    std::uniform_int_distribution<T> dist(1, vec_size - 1);
    start_right =  dist(generator);
  }

  std::vector<T> vec_original(vec_size);
  auto vec = vec_original;

  TotalTimes total_times;
  for (std::size_t i = 0; i < num_tests_per_vec_size; i++) {
    randomhelpers::FillVectorWithRandomNumbers(vec_original, lower_bound, upper_bound);

    auto end_left    = start_right - 1;
    int length_left  = start_right;
    int length_right = vec_size - start_right;

    //Sort the left and right subvectors
    std::sort(vec_original.begin(), vec_original.begin() + start_right);
    std::sort(vec_original.begin() + start_right, vec_original.end());
    vec = vec_original;

    if (verbose) {
      printhelpers::PrintLine("_", 80, true);
      std::cout << "start_left = 0 \tend_left =\t" << end_left
                << "\tstart_right =\t" << start_right
                << "\tend_right =\t"   << (vec_size - 1)  << std::endl;
      printhelpers::PrintNondecreasingSubsequences(vec.begin(),  length_left, false);
      printhelpers::PrintNondecreasingSubsequences(vec.begin() + start_right, length_right, true);
    }

    auto times = TimeMergesOnGivenVec(vec, vec_original, start_right, num_repititions_per_vec);
    total_times += times;

    if (print_vec_averages)
      std::cout << times.GetAveragesStr(num_repititions_per_vec);

    if (pick_new_start_right_for_each_new_vec) {
      std::random_device rnd_device;
      std::mt19937 generator(rnd_device());
      std::uniform_int_distribution<T> dist(1, vec_size - 1);
      start_right =  dist(generator);
    }
  }
  if (print_total_averages)
    std::cout << total_times.GetAveragesStr(num_repititions_per_vec);
  return total_times;
}

#endif /* SRC_MERGE_TIME_H_ */