#include <iostream>

int main() {

    int num; // random number
    int guess; // user entered data
    int tries; // tracking user attempts

    srand(time(NULL));

    num = (rand() % 100 + 1);

    std::cout << "Number Guessing Game\n\n";

    do {

        tries++;

        std::cout << "\n\nEnter a guess (1-100): \n\n";

        std::cin >> guess;

        if (guess > num) {
            std::cout << "\nYou Guessed High!\n";
        } else if (guess < num) {
            std::cout << "\nYou Guessed Low!\n";
        } else if (guess == num) {
            std::cout << "\nCorrect!\n";
            std::cout << "\nYou guess in: " << tries << " tries!";
        } else {
            std::cout << "Something broke";
        }

    } while (guess != num);



	return 0;
}