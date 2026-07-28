#include <iostream>
#include <string>

int main() {

    std::cout << "\n\nBreak & Continue\n\n";

    for (int i = 1; i < 21; i++) {
        if (i == 18) {
            std::cout << "To high";
            break;
        }
    }

    return 0;
}