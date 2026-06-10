#include <iostream>
#include "constants.h"

namespace constants {
	int main() {

		const double PI = 3.14159; // Common for const names to be UPPER
		const double RADIUS = 10;
		double circumference = 2 * PI * RADIUS;

		std::cout << "Constants" << '\n';
		std::cout << circumference << "cm" << "\n";


		return 0;
	}
}