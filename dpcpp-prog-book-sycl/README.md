# Notes from - Data Parallel C++ Programming Accelerated Systems Using C++ and SYCL

- Avoids vendor lock-in
- Programmers express the inherent parallelism in the algorithm
    - Let the compiler/drivers do the lifting
- The problems to solve must be expressed as an algorithm
- Performance portability is first, SYCL can be ran on most all brands of CPU/GPU/SIMD/FPGA
- SYCL is very in-line with current cpp standards
    - cpp17
- 80%-100% of native performance
    - 100% ish on nvidia, 80-90% on amd/intel

## Websites

- https://github.com/Apress/data-parallel-CPP
- https://www.khronos.org/sycl/
- https://sycl.tech/
    - This allows you to run code on the site
    - There is some online education/documentation

## Hello World

```cpp
#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl; // avoid calling sycl repeatedly in the program

const std::string secret{
    "Ifmmp-!xpsme\"\012J(n!tpssz-!Ebwf/!"
    "J(n!bgsbje!J!dbo(u!ep!uibu/!.!IBM\01"};

const auto sz = secret.size();

int main() {
  queue q; // instantiate a queue for work requets to be sent to a device

  char* result = malloc_shared<char>(sz, q); // creats an allocation for shared data with the device

    // char* create a point var for a 1byte character
    // malloc_shared, sycl func to request unified shared memory
    // <char> dictates the memory will hold char type elements
    // (sz, q) size is the number of bytes to allocate
        // q is the sycl q used to bind the allocation to a device/context

  std::memcpy(result, secret.data(), sz); // copies raw blocks of memory


// execute a parallel loop against the target device/accelerator
  q.parallel_for(sz, [=](auto& i) {
     result[i] -= 1;
   }).wait();

   // q.parallel_for(... laucnhes parallel kernel across multiple processing elements on devices managed by the sycl queue q
   // sz is the execution range, tells sycl to spawn sz individual work  items (threads) from 0 to sz-1
   // [=] lambda capture by value, it copies the points (result) and scalars into the lambda function so gpu can access them
   // (auto& i) current thread context, in sycl i is the sycl::id<1> object representing the current 1D global index
   // result[i] -= 1 is the actual work, each thread i reach one char from teh shared mem and subtracts 1 from its value and writes it back
   // .wait() is a blocking call on the host cpu, the cpu halts at this point until the gpu/device finishing running the parallel_for()


  std::cout << result << "\n";
  free(result, q); // releases memory allocation, not the queue itself, this deallocated unified shared memory
  return 0;
}
```

page7 (actual page 37)


