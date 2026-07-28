#include <iostream>
#include <ctime> // This may not be needed depending on env

int main() {

	std::cout << "Random Number Generator\n\n";

    srand(time(0)); // Pass in current time for randomness
    int randNum = rand() % 5 + 1; // Modulus of random numbers plus an extra number

    switch(randNum){
        case 1: std::cout << "You win prize 1\n";
                break;
        case 2: std::cout << "You win prize 2\n";
                break;
        case 3: std::cout << "You win prize 3\n";
                break;
        case 4: std::cout << "You win prize 4\n";
                break;
        case 5: std::cout << "You win prize 5\n";
                break;
    }

	return 0;
}