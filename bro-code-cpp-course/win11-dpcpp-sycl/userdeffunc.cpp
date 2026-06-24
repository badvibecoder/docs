#include <iostream>

// put user made funcs before main()

void myFunc(){
	std::cout << "My custom function\n";
}

// We can also declare funcs here, but write them later in the prog

void myOtherFunc();

// Pass the arg

void myPassArgsFunc(std::string named){

	std::cout << "Hello: " << named << "\n";

}

int main() {

	std::string name = "Chanche";

	std::cout << "\n\nUser Defined Functions\n\n";

	myFunc();

	myOtherFunc();

	myPassArgsFunc(name);

	return 0;
}

void myOtherFunc(){
	std::cout <<  "This func is at the bottom\n";
}