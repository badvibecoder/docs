#include <iostream>

double getTotal(double prices[], int size);

int main() {

	std::cout << "Pass Array to Function\n\n";

    double prices[] = {5.55, 6.10, 9.35, 65.00};
    int size = sizeof(prices) / sizeof(prices[0]);
    double total = getTotal(prices, size);

    std::cout << "$" << total;

	return 0;
}

double getTotal(double prices[], int size) {

    double total = 0;

    for(int i = 0; i < size; i++){
        total += prices[i];
    }

    return total;
}

// 37 on the video https://www.youtube.com/watch?v=-TkoO8Z07hI&list=WL&index=6&t=5752s