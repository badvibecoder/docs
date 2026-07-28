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

// Selected device: Intel(R) Arc(TM) Pro B70 Graphics