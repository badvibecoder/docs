#include <iostream>
#include "userinput.h"
#include <string>

namespace userinput {
	int main() {

		std::cout << "User Input" << "\n";
		// 44:13 https://www.youtube.com/watch?v=-TkoO8Z07hI

		// Character input with extraction operator >>

		std::string name; // declare var before use
		std::string fullname;
		int age;

		std::cout << "What is your name?: ";

		std::cin >> name;

		std::cout << "What is your age?: ";

		std::cin >> age;

		//std::cin.ignore(); // clear the newline char from buffer
		// This is required because we are reading the entire line.

		std::cout << "What is your full name?: ";

		// or instead of ignore() we can clear leading whitespace

		std::getline(std::cin >> std::ws, fullname);

		std::cout << '\n' << "Your name is " << name;

		std::cout << '\n' << "Your age is " << age;

		// So far we are only reading strings, and not spaced
		// If you need first last you need to get the entire line

		
		// we can use the std getline, 
		std::cout << '\n' << "Your full name is " << fullname;

		return 0;
	}
}