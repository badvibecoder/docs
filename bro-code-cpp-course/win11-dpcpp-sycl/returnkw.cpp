#include <iostream>
#include <chrono>

std::string currentTime(){
    return "Morning";
}

int main() {

	std::cout << "Return Keyword\n\n";

    // return a value back to the spot where you called it

    std::string nowTime = currentTime();

    std::cout << nowTime ;

	return 0;
}