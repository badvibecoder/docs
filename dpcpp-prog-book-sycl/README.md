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

- Primer
  - https://www.youtube.com/watch?v=-TkoO8Z07hI
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

## Method 1 Run on a device of any type

Device select does not take into account the actual code running.

The queue class constructor:

```cpp
class queue {
 public:
  // Create a queue associated with a default
  // (implementation chosen) device.
  queue(const property_list & = {});

  queue(const async_handler &, const property_list & = {});

  // Create a queue using a DeviceSelector.
  // A DeviceSelector is a callable that ranks
  // devices numerically. There are a few SYCL-defined
  // device selectors available such as 
  // cpu_selector_v and gpu_selector_v.
  template <typename DeviceSelector>
  explicit queue(const DeviceSelector &deviceSelector,
                 const property_list &propList = {});

  // Create a queue associated with an explicit device to
  // which the program already holds a reference.
  queue(const device &, const property_list & = {});

  // Create a queue associated with a device in a specific
  // SYCL context. A device selector may be used in place
  // of a device.
  queue(const context &, const device &,
        const property_list & = {});
};
```

A queue is bound to a single device, which is set on the construction of the queue. Work submitted to that queue can only reach the related device. Queues cannot spread work over multiple devices. 

Multiple queues can be created to leverage multiple devices. Multiple queues can be creates for a single device, ie vector acceleration and tensor accelerators.

When constructing a queue if the constructor does not take any args it just chooses an available device. In most cases this will be the cpu.

Pull the available/select device.

```cpp
#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;
int main() {
// Create queue on whatever default device that the
// implementation chooses. Implicit use of
// default_selector_v
queue q;
std::cout << "Selected device: "
<< q.get_device().get_info<info::device::name>()
<< "\n";
return 0;
}
```

```bash
Selected device: Intel(R) Arc(TM) Pro B70 Graphics
```

## Method 2 Using CPU to Debug

CPU Advantages

Page 43 (Real page 73)

- Dev on any box before you spin up on an HPC cluster
- Debugging easily
- Backup when no accelerators are available

Passing `cpu_selector_v` to the queue constructor you can tell the code to bind the queue to the cpu.

```cpp
#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

int main() {
  // Create queue to use the CPU device explicitly
  queue q{cpu_selector_v};

  std::cout << "Selected device: "
            << q.get_device().get_info<info::device::name>()
            << "\n";
  std::cout
      << " -> Device vendor: "
      << q.get_device().get_info<info::device::vendor>()
      << "\n";

  return 0;
}
```

```bash
Selected device: Intel(R) Core(TM) Ultra 7 270K Plus
```

## Method 3 GPU (or other accelerators)

Page 45 (Real Page 75)

These can be cpu, gpus, dsp, fpga, etc...

We can use device selectors to determine which device the queue will go to. The queue class has constructors that can accept `DeviceSelector` as a const referencing a device. 

4 Built in `DeviceSelector`s.

- `default_selector_v`
- `cpu_selector_v`
- `gpu_selector_v`
- `acelerator_selector_v`
    - any device that identifies itself as an accelerator

in `dpc++` there is an additional fpga extension which can be accessed with `fpga_selector_v`

GPU selector example:

```cpp
#include <iostream>
#include <sycl/sycl.hpp>
using namespace sycl;

int main() {
  // Create queue bound to an available GPU device
  queue q{gpu_selector_v};

  std::cout << "Selected device: "
            << q.get_device().get_info<info::device::name>()
            << "\n";
  std::cout
      << " -> Device vendor: "
      << q.get_device().get_info<info::device::vendor>()
      << "\n";

  return 0;
}
```

Output:

```bash
Selected device: Intel(R) Arc(TM) Pro B70 Graphics
```

### Check what devices are available

```cpp
#include <iostream>
#include <string>
#include <sycl/ext/intel/fpga_extensions.hpp>  // For fpga_selector_v
#include <sycl/sycl.hpp>
using namespace sycl;

void output_dev_info(const device& dev,
                     const std::string& selector_name) {
  std::cout << selector_name << ": Selected device: "
            << dev.get_info<info::device::name>() << "\n";
  std::cout << "                  -> Device vendor: "
            << dev.get_info<info::device::vendor>() << "\n";
}

int main() {
  try {
    output_dev_info(device{default_selector_v},
                    "default_selector_v");
  } catch (sycl::exception& e) {
    std::cout << "No SYCL devices found.\n";
  }
  try {
    output_dev_info(device{cpu_selector_v}, "cpu_selector_v");
  } catch (sycl::exception& e) {
    std::cout << "No CPU devices found.\n";
  }
  try {
    output_dev_info(device{gpu_selector_v}, "gpu_selector_v");
  } catch (sycl::exception& e) {
    std::cout << "No GPU devices found.\n";
  }
  try {
    output_dev_info(device{accelerator_selector_v},
                    "accelerator_selector_v");
  } catch (sycl::exception& e) {
    std::cout << "No accelerator devices found.\n";
  }
  try {
    output_dev_info(device{ext::intel::fpga_selector_v},
                    "fpga_selector_v");
  } catch (sycl::exception& e) {
    std::cout << "No FPGA devices found.\n";
  }

  return 0;
}
```

I ommitted the fpga configurations from the test but here is the output:

```bash
default_selector_v: Selected device: Intel(R) Arc(TM) Pro B70 Graphics
                  -> Device vendor: Intel(R) Corporation
cpu_selector_v: Selected device: Intel(R) Core(TM) Ultra 7 270K Plus
                  -> Device vendor: Intel(R) Corporation
gpu_selector_v: Selected device: Intel(R) Arc(TM) Pro B70 Graphics
                  -> Device vendor: Intel(R) Corporation
```

Page 49 (Real Page 79)

If a device cannot be selected a `runtime_error` exception will be thrown.

### Method 4 Using multiple devices

We will use the cpu and gpu

```cpp
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
```

We see the following output:

```bash
Selected device 1: Intel(R) Arc(TM) Pro B70 Graphics
Selected device 2: Intel(R) Core(TM) Ultra 7 270K Plus
```

### Method 5 Custom or Very Specific Device selection

Properties of devices in SYCL are `aspects`. 

An aspect maybe:

- gpu
- host_debuggable
- fp64
- online_compiler

You will need to look at the SYCL spec to get a full list: https://registry.khronos.org/SYCL/specs/sycl-2020/html/sycl-2020.html

The `aspect_selector` can be used, taking comma-delimited aspects. All `aspect`s must be available on a device for the device to be chose. IE, if you want FP64 but the device can only do FP32 it will not be selected.

The `aspect_selector` also has an alternate form that takes two `std::vectors`, the first has `aspect`s that must be present on the device and the second contains `aspect`s that must NOT be on the device.

This would be good for select devices that DO NOT have `emulation` or `emulated` aspects, ie test devices.

There are also additional resolution available with custom selectors to score devices to get a bit more granular.

Page 54 (Real Page 84)

**Graphs:** These are hardware/software pipelines, static in nature, and mostly immutable. The goal of a graph is to look at your code, hardware and strip away anything not needed (ie CPU related ops) so it provided a hard blueprint of device specific hardware commands to execute.

In SYCL we have Task Graphs and Command Graphs.

- Task Graph (Built by runtime[the lib/engine, aka the background software layer that is managing the execution])
  - implicit, sycl runtime engine acts as an automated traffic controller
- Command Graph (SYCL Graph)
  - explicit, your telling sycl to take a task graph and do something exact with it
  - Running against AMD the SYCL Graph lowers into HIP Graphs, same for CUDA.

Page 55 (Real Page 85)

The runtime controls resolution of dependencies and controls triggering of node executions asynchronously.

### Where is the device code?

Code patterns will look complex but the patterns typically remain the same across all device code. 

In this code snip the device code is just:

```cpp
  q.submit([&](handler& h) {
    accessor acc{B, h};

    h.parallel_for(size,[=](auto& idx) { acc[idx] = idx; });
  });
```

one line within the `parallel_for`:

```cpp
[=](auto& idx) { acc[idx] = idx; }
```

- `[=]`
  - All variables used in the lambda function are captured by value
- `auto& idx`
  - auto&, let the kernel use the refences by index and not copy
  - idx is the work item index
- `acc[idx] = idx`
  - acc is the device accessor to the buffer
    - The accessor is read/write access to the data in the buffer
  - acc can then access the work items in the specific element position

### Actions

`parallel_for` is a within a command group, which is submitted to a queue, the queue defines the device which the work is to be performanced.

Within the command group there are two types of code:

- Host code
  - sets up dependencies defining a safe runtime
- at most one call to action code
  - queues on device or performs manual memory ops like copy

Work type:

- device code execution
  - `single_task` single instance of a device function
  - `parallel_for` multiple forms, launch device code
- explicit memory operation
  - `copy` copy data between locations specified by accessor, pointer, shared pointer
  - `update_host` update host data backing of a buffer object
  - `fill` init data in buffer to a specific value

Only 1 action maybe called within a command group, it will error if there is more than 1.

All of this is because the code is ran async on a non-cpu device.

Three classes of code

- Host code
  - drives teh application
- Host code within a command group
  - runs on processor
- An action
  - defines async work

This entire section seems to be just pointing at that there is 90% of the code that is ran host side on cpu and a tiny sliver of code that will run in the future asynchronously via the queue, that code runs on the device.

### Host Tasks

Within code submitted to the queue we can execute host side code via the method `host_task` for things like `std::cout` during debugging.

```cpp
  q.submit([&](handler& h) {
    h.depends_on(eA);
    h.host_task([=]() {
      for (int i = 0; i < N; i++)
        std::cout << "host_task @ " << i << " = " << A[i]
                  << "\n";
    });
  });
```

`h.host_task([=]() {` allows us to run host tasks from within the queue submitted to the device. This code is still executed async from the rest of the host code.

# Chapter 3 Data Management

Page 67 (Real Page 97)


Pausing here, need to recap the above and step through some examples:

```cpp
#include <sycl/sycl.hpp>
#include <iostream>
#include <vector>

int main() {

    // Create a const N, this can be used to scale the dynamic array up or down
    const size_t N = 10;

    // we are creating a std::vector<int> dynamic array from a int primitive
    // Fill the array named data with N number of elements, value of int 1
    std::vector<int> data(N, 1);

    sycl::queue q(sycl::gpu_selector_v);
    // sycl::queue q()
        // We want to setup the sycl queue, access sycl namespace and instantiate the queue with an object instance of q
    // sycl::gpu_selector_v
        // we are selecting the device to execute on for parallelism, which is the first available gpu

    std::cout << "Running on: " 
              << q.get_device().get_info<sycl::info::device::name>() << "\n";


    // Plain cpp scope block
    {
        sycl::buffer<int, 1> buffer(data.data(), sycl::range<1>(N));
        // sycl::buffer<int, 1>
        // buffer(data.data() 
            // Hand over the pointers for the dyn array data to the buffer
                // via the data() method dynarray.data() method
        // sycl::range<1>(N)
        //  // 1 is the dimension, this is a 1D datashape
                // 1D line elements, numbers
                // 2D image or table
                // 3D physics, space, video frames
            // N is the number of elements

        q.submit([&](sycl::handler& h) {
            // q.submit()
                // This method allows us to send the requested command group to the queue for execution
            // [&]
                // captures external host vars by reference so they can be used in the command group
            // Command Group - a specific task like moving data or executing kernel
            // sycl::handler& h
                // create and pass reference to a command group, this h handler object is used to configure the command group with accessors, kernels etc..
            
            // Accessor's or Getter's are mthods uses to retreive values typically private or protected from an object
            // in sycl the accessor is not a method, its a heavy-duty object
                // used as a secure memory checkout counter, it gets perms from
                // the runtime to access specific buffers on the device
            sycl::accessor acc(buffer, h, sycl::write_only);
            // sycl::accessor acc() instatiate the accessor method
            // we are passing in
                // the buffer we created with data.data()
                // the handler we created
                // we are telling accessor that this is write_only operation
                    // This should setup when we execute the next block that
                    // we are writing data back to the buffers references

            // queueing actions to the device with parallel_for
            h.parallel_for(sycl::range<1>(N), [=](sycl::id<1> idx) {
            // h.parallel_for() command group handler method that parallelizes actions to the device
            // sycl::range<1>(N) the data range is 1D with a size of N
            // [=] captures variables from the surrounding host scope by value, the accessor
            // (sycl::id<1> idx) 1 dimension, id is the coordinate class
                // id<1> is [x]
                // id<2> is [x,y]
                // id<3> is [x,y,z] dimensions
                // idx is the variable, and holds that threads specific job number
                    // this will be passed to the device

                acc[idx] += 1; // device code
                // acc[] is the buffer accessor
                    // because device code cannot be accessed by standard cpp pointers/vecotrs memory must be explicitly managed
                    // This is a secure view into the a memory buffer that lives on the device
                // idx is the subscript array index
                    // each idx is unique for every parallel execution
                    // this means each parallel thread targets a different memory slot in the accessor
                    // This allows us to scale up the computation
                // += 1; read the mem addressed dictated by idx, modify it using the gpu ALU to add 1 to the value, write the new value to the same memory location as that specific threads idx.
            });

        });
    } // on block close the buffer is destroyed

    std::cout << "Verification (first 5 elements should be 2):\n";
    for (int i = 0; i < 5; i++) {
        std::cout << "data[" << i << "] = " << data[i] << "\n";
    }

    return 0;
}
```

More human readable version:

```text
Includes

main func

    instantiate data

    create queue and select device

    within a plain scope block

        instantiate the buffer, dimensionality, pass in data

        send command group to the queue handler

            setup the accessor to be able to use the data, handler, and read/write ops

            setup device queueing actions with parallel_for

                run device code
```