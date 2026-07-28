#include <iostream>
#include "typedefs.h"
#include <vector>

// long name to alias
typedef std::vector<std::pair<std::string, int>> pairlist_t; // append _t

typedef std::string text_t;

using number_t = int; // newer style

// now we can reference the type pairlist

namespace typedefs {
	int main() {

		std::cout << "Typedef and type alias" << '\n' << '\n';

		// typedef = reserved keyword used to create an alias for another dtype

		std::vector<std::pair<std::string, int>> pairlist;
		pairlist_t myaliaspairlist; // much simpler

		text_t firstName = "Chanche";

		std::cout << firstName << '\n';

		number_t myint = 55;

		std::cout << myint << '\n';

		// replaced with 'using' keyword, typedef is the older keyword

		return 0;
	}
}