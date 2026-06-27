#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

int main() {
  queue my_gpu_queue(gpu_selector_v);
  queue my_cpu_queue(cpu_selector_v);

  std::cout << "Selected device 1: "
            << my_gpu_queue.get_device()
                   .get_info<info::device::name>()
            << "\n";

  std::cout << "Selected device 2: "
            << my_cpu_queue.get_device()
                   .get_info<info::device::name>()
            << "\n";

  return 0;
}

