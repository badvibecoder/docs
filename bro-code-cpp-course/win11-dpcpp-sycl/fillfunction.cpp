#include <iostream>

int main() {

	std::cout << "\n\nFill Function\n\n";

    // begining , ending, value

    // create an empty array 10 elements
    const int SIZE = 99;
    std::string foods[SIZE];

    fill(foods, foods + (SIZE / 3), "Pizza");
    fill(foods + (SIZE / 3), foods + (SIZE / 3) * 2, "Burgers");
    fill(foods + (SIZE / 3) * 2, foods + SIZE, "Hotdogs");

    for(int i = 0; i < sizeof(foods)/sizeof(foods[0]); i++){
        std::cout << foods[i] << "\n";
    }


	return 0;
}