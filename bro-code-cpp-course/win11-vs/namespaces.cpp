#include <iostream>
#include "namespaces.h"

namespace first {
	int x = 1;
}

namespace second {
	int x = 2;
}

namespace namespaces {
	int main() {

		// We can create name spaces within namespaces

		int x = 0;

		std::cout << x << '\n';
		std::cout << first::x << '\n'; // access the first namespace var x
		std::cout << second::x << '\n'; // access the second

		// We can use 'using namespace first' as well
		// then we could just call on x if main() does not contain it

		// You can also call 'using namespace std' to
		// avoid calling std::cout, you can just call cout << 
		// std has a lot of names, and is easily conflicted

		return 0;
	}
}