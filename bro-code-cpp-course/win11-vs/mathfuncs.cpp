#include <iostream>
#include "mathfuncs.h"

namespace mathfuncs {
	int main() {

		std::cout << "Useful Math Functions" << "\n";

		// 8 useful math related functions

		double x = 3;
		double y = 4;
		double z; //declared but not assigned

		z = std::max(x, y);
		std::cout << z << "\n";

		z = std::min(x, y);
		std::cout << z << "\n";

		// 53:31

		return 0;
	}
}