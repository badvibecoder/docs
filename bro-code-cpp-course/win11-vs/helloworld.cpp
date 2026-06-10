#include <iostream> // Allows for input and output streams
#include "helloworld.h"
/*

block comment

*/
namespace helloworld {
	int main() {
		// standard character output
		// left shift operater for text output
		// shift left << and add std endl will end the line
		std::cout << "I like pizza" << std::endl;
		std::cout << "It's really good" << std::endl; // less performant but flushes the output buff
		std::cout << "We can also end a line with this" << '\n'; // better performance

		int x; // declaration
		x = 5; // assignment
		int y = 6;
		int sum = x + y;

		// assigning a double to an int will truncate the floating portion
		double price = 10.24;

		// characters, single character
		char firstinit = 'C';

		// Boolean
		bool student = true;

		// Strings
		std::string name = "Chanche Dogman";

		std::cout << '\n';
		std::cout << name << '\n';
		std::cout << "Hello " << name << '\n';
		std::cout << student << '\n'; // will display as 1, not true
		std::cout << firstinit << '\n';
		std::cout << x << '\n';
		std::cout << y << '\n';
		std::cout << sum << '\n';
		std::cout << price << '\n';

		return 0; // return 0, program exec successfully
	}
}