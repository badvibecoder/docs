#include <iostream>

int main() {

	std::cout << "For Each Loop\n\n";

    // foreach loop = traverse over iterable data set

    std::string cars[] = {"My Car","Your Car","His Car"};

    for(std::string car : cars){
        std::cout << car << "\n";
    }

	return 0;
}