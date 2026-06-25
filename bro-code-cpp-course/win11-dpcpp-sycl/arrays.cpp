#include <iostream>

int main() {

    std::cout << "\n\nArrays\n\n";

	// a data structure that can hold multiple values
    // arrays elements should be of the same dtype

    std::string cars[] = {"My Car","Your Car","His Car"};

    std::cout << cars << "\n\n";

    for (int i = 0; i < cars->length(); i++) {

        std::cout << cars[i] << "\n";

    }

	return 0;
}