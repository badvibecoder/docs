#include <iostream>

int main() {

	std::cout << "Ternary Operator\n";
	// condition ? expression1 : expression2
	// ?: = replacement to an if/else statement
	// x = 7 ? do this : or if not do that

	int grade = 75;

	(grade >= 60) ? std::cout << "You pass Ternary!\n" : std::cout << "You fail Ternary!\n";

	// 1:15:55 

	return 0;
}