#include <iostream>
#include <cmath>

int main() {

	double a, b, c;

	std::cout << "Enter side A: ";
	std::cin >> a;

	std::cout << "Enter side B: ";
	std::cin >> b;

	//a = pow(a, 2);
	//b = pow(b, 2)
	c = sqrt(pow(a, 2) + pow(b, 2));

	std::cout << "\n" << "Side C: " << c << "\n";

	// 58:46 min

	return 0;
}