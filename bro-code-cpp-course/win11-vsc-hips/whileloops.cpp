#include <iostream>
#include <string>

int main() {

	std::cout << "\n\nWhile Loops\n\n";

    std::string name;

    while (name.empty()){

        std::cout << "Enter your name: ";
        std::getline(std::cin, name);

    }

	return 0;
}