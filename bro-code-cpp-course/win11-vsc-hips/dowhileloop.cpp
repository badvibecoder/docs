#include <iostream>
#include <string>

int main() {

	std::cout << "\n\nDo While Loops\n\n";

    int number;

    do {
        std::cout << "Enter a positive number: ";
        std::cin >> number;
    } while (number < 0);

    std::cout << "This # is: " << number;

	return 0;
}