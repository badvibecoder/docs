#include <iostream>
#include "arithmetic.h"

namespace arithmetic {
	int main() {

		std::cout << "Arithmetic" << "\n";

		// + - * /

		int students = 20;

		students = students + 1;
		students += 1;

		// increment
		students++;

		students -= 1;

		//decrement

		students--;

		students = students * 2;
		students *=2;

		students = students / 2;
		students /= 2;

		// We are working with an int so we cannot access fp data

		int remainder = students % 5;

		students /= 5; // it shaves off the dec/remainder

		// use % to determine even or odd

		int even = 4 % 2;
		int odd = 5 % 2;






		std::cout << students << '\n';

		std::cout << remainder << '\n';

		std::cout << even << '\n';

		std::cout << odd << '\n';

		return 0;
	}
}