#include <iostream>
#include <cmath>

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

	//z = pow(2, 3);
	//z = sqrt(9);
	//z = abs(-3);
	//z = round(x); // round
	//z = ceil(x); // round up
	z = floor(x); // round down

	std::cout << z << "\n";

	return 0;


}
