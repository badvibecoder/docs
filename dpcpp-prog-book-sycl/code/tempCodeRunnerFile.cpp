  for (std::size_t i = 0; i < sz; ++i)
      std::cout << result[i] << ' ';
  std::cout << '\n';

  free(result, q);