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
