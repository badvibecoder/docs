#include <iostream>\
#include "typeconversion.h"

namespace typeconversion {
	int main() {

		std::cout << "Type Conversion" << '\n';

		// conversion of a value from one dtype to another
		// implicit = automatic
		// explicit = precede value with a new dtype (dtype)

		// Implicit cast

		int x = 3.14; // we just implicitly casted this to an int
		// even thought its a double

		double y = 3.14;

		std::cout << x << '\n';
		std::cout << y << '\n';

		std::cout << "cast" << '\n';

		double z = (int)5.3423;

		std::cout << z << " Originally 5.3423" << '\n';

		char i = 100;

		std::cout << i << '\n';

		std::cout << (char)254 << " ascii char 254" << '\n';



		return 0;
	}
}