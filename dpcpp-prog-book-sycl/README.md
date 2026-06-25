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

## Queues and Actions

Queues are the connection to the device, and the only way to get data into the device. Actions are things placed in the queue.

- Actions
    - Code execute
    - Memory ops

This is asynchronous, the cpu continue to run the code, the actions are punted to the accelerator/device/fpga/gpu/npu/etc...

The goal is to increase throughput and reduce latency of data processing.

Scaling, how much the program speeds up as the workload and spread across cores/processors increases. Does 100 jobs over 100 cores run as fast as 1 job on 1 core. This moves the bottleneck to a different location.

Kernel of work are issues by the host os to the devices. 

## Data-Parallel Programming

Programs should be single source, you can divide a program into multiple files but SYCL is designs to run a single programs that encompasses all processing.

SYCL can target many different devices and types at the same time asynchronously.

## Kernel Code

- Not supported
    - dynamic polymorphism
    - dynamic memory allocations
    - static vars
    - function points
    - runtime type information
    - exception handling
    - virtual member functions
    - variadic functions
    - recusion

## Kernel Vector Addition (DAXPY)

Double Precision A times X plus Y

SYCL is very close to native C++ because it is native C++

```cpp
// C++ loop
for (int i=0;i<n;i++) {
  z[i] = alpha * x[i] + y[i];
}

// SYCL kernel
q.parallel_for(range{n},[=](id<1> i) {
  z[i] = alpha * x[i] + y[i];
}).wait();
```

## Race Conditions

We will need to account for multiple parts of the program access the same things, using `.wait()`, event dependencies, creating ordered queues, dependence on memcpy and parallel_for. Some GPUs also cannot run parallel functions in expected ways negating the race condition. Synchronization can lead to race condition avoidance but has performance penalties.

SYCL uses out of order queues by default.

Its easier to debug race conditions by have all the code execute against the cpu.

## Deadlock

When 2 or more actions are blocked (processes, threads). Each is waiting on a resource or to complete a task.

Each time we use a way, sync, or lock we are open to deadlocks.

## cpp Lambda Expressions

These are a concise way to write anonymous function objects (closures) directly at the location they are used.

```cpp
[ capture-list ] ( params ) -> ret { body }
```

- `[]` Capture clause/list defined which vars the enclosing scope are accessible inside the lambda
    - `[]` empty, no vars from outer scope can be used
    - `[=]` capture all by value, copies all references variables in the lambda, originals cannot be modified
    - `[&]` capture all by reference, this can modify outer scope originals
- `( params )` This is your input argument, matches standard function params and can be omitted if not needed
- `-> ret` Return type, if its not specified it is inferred from the return statements. The lack of a statement or return with no value implied a void return. SYCL kernels must always have a return type of void.
- `{ body }` Actual code containing the logic

Capture one var by value and another by reference: `auto lambda = [i, &j](int k0, int& l0) -> int {`

Page 26 (Real page 56)

If you are building code for compatiblity you may need to set the fsycl compiler to spir64 `icpx -fsycl -fsycl-targets=spir64` during compilation. However this is a wide generic type for compatibility you will lose a lot of platform specific performance. You will want to look into architecture specific compilation using SYCL to target performance for benchmarking.

NOTE: bare `-fsycl` will package code as SPIR-V and not know how to target XMX/Xe Extensions or other special hardware. You will want to modify how the code compiles and use ahead-of-time AOT targets to force native instructions for the specialized hardware.

Example:

```bash
icpx -fsycl -O3 -fp-model=fast -fsycl-targets=spir64_x86_64,spir64_gen -o intel_benchmark benchmark.cpp
```

## Concurrency vs Parallelism

- concurrency: two or more threads are making some progress
- parallelism: two or more threads are executing simultaneously

# Chapter 2 Where Code Executes

Page 31 (Real page 61)

Dont drive in the fast lane, drive in all lanes.

## Single Source

SYCL are single source programs. Code to the on kernels and orchestrate the execute are in the same program.

Most of your code will execute on the host/cpu, only a small amount of code is actually ran on the device within the queue.

- Host code = cpu
- Device code = accelerator
    - Executes async from the host code

The host submits device code to the device and then it tracks and starts work.

Certain SYCL functions are only available within device code. Things that can only run on devices will only run on devices.

- Actions: work that is submitted to queues

## Choosing Devices

There are a number of methods to control where device code will execute

- method 1 - Run device code somewhere, simple, no specifics
- method 2 - explicity run on cpu, mostly for debug
- method 3 - gpu or accelerator
- method 4 - heterogeneous devices, ie cpu gpu and fpga
- method 5 - select a specific device class like 'all fpgas'

Build and debug in method 2 and only move to specific hardware when code is fully tested.

Page 37 (Real page 67)