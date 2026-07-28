#include <iostream>

int main() {

	std::cout << "Calculator" << "\n";

	// Create char for operator
	char op;
	double num1;
	double num2;
	double results;

	std::cout << "********** CALCULATOR **********\n";

	std::cout << "Enter (+ - * /): ";
	std::cin >> op;

	std::cout << "Enter 1st Number: ";
	std::cin >> num1;

	std::cout << "Enter 2nd Number: ";
	std::cin >> num2;

	switch(op){
		case '+':
			results = num1 + num2;
			std::cout << "Result: " << results << "\n";
			break;
		case '-':
			results = num1 - num2;
			std::cout << "Result: " << results << "\n";
			break;
		case '*':
			results = num1 * num2;
			std::cout << "Result: " << results << "\n";
			break;
		case '/':
			results = num1 / num2;
			std::cout << "Result: " << results << "\n";
			break;
		default:
			std::cout << "Invalid Operator\n";
			break;
		}

	std::cout << "********************************\n";

	return 0;
}