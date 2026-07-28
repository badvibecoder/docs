
#include <iostream>

int main() {

    int age;

	std::cout << "\n" << "If Statements" << "\n" << "\n";

    std::cout << "Enter your age: ";
    std::cin >> age;

    if(age >= 18 && age < 55){
        std::cout << "Welcome Adult" << "\n";
    }
    else if(age >= 55 && age < 100){
        std::cout << "Do you enjoy Paul Newman movies?" << "\n";
    }
    else if(age <= 0){
    std::cout << "Welcome time traveler" << "\n";
    }
    else if(age >= 100){
    std::cout << "You died" << "\n";
    }
    else{
        std::cout << "You are denied, age harder" << "\n";
    }

	return 0;
}