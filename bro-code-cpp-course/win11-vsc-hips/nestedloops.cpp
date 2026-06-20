#include <iostream>
#include <string>

int main() {

    std::cout << "\n\nNested Loops\n\n";

    for (int i = 1; i < 10; i++) {
        std::cout << "Outside Loop: " << i << "\n";
        for (int j = 1; j < 10; j++) {
            std::cout << "Inside Loop: " << j << "\n";
            for (int k = 1; k < 10; k++) {
                std::cout << "Micro Loop: " << k << "\n";
            }
        }
    }

    return 0;
}