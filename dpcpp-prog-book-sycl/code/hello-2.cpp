// Copyright (C) 2023 Intel Corporation

// SPDX-License-Identifier: MIT

#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

constexpr std::array<int, 6> secret{ 2, 2, 2, 2, 2, 2 };

constexpr std::size_t sz = secret.size();

int main() {
  queue q;

  int *result = malloc_shared<int>(sz, q);
  std::memcpy(result, secret.data(), sz * sizeof(int));

  q.parallel_for(sz, [=](auto& i) {
     result[i] -= 1;
   }).wait();

  for (std::size_t i = 0; i < sz; ++i)
      std::cout << result[i] << ' ';
  std::cout << '\n';

  free(result, q);
  return 0;
}