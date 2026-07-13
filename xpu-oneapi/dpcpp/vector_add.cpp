#include <iostream>
#include <sycl/sycl.hpp>

int main() {
    const size_t N = 1024;
    
    sycl::queue q;
    std::cout << "Running on device: " << q.get_device().get_info<sycl::info::device::name>() << "\n";

    // Allocate USM shared memory so both CPU and Device (GPU) can see it
    int* a = sycl::malloc_shared<int>(N, q);
    int* b = sycl::malloc_shared<int>(N, q);
    int* sum = sycl::malloc_shared<int>(N, q);

    // Initialize host data
    for (size_t i = 0; i < N; ++i) {
        a[i] = i;
        b[i] = i * 2;
        sum[i] = 0;
    }

    // Execute kernel safely with flat pointers
    q.parallel_for(sycl::range<1>(N), [=](sycl::id<1> i) {
        sum[i] = a[i] + b[i];
    }).wait();

    // Verify results on the host
    for (size_t i = 0; i < N; ++i) {
        if (sum[i] != (int)(i + i * 2)) {
            std::cout << "Verification failed at index " << i << "!\n";
            sycl::free(a, q);
            sycl::free(b, q);
            sycl::free(sum, q);
            return 1;
        }
    }

    std::cout << "Verification successful!\n";

    // Free allocated USM resources
    sycl::free(a, q);
    sycl::free(b, q);
    sycl::free(sum, q);
    
    return 0;
}
