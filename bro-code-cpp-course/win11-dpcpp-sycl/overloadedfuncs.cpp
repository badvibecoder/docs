#include <iostream>

void bakePizza(){
    std::cout << "Pizza Bakes\n\n";
}
void bakePizza(std::string topping1){
    std::cout << "Pizza Bakes: " << topping1 << "\n\n";
}
void bakePizza(std::string topping1, std::string topping2){
    std::cout << "Pizza Bakes: " << topping1 << " and " << topping2 << "\n\n";
}

int main() {

	std::cout << "Overloaded Functions\n\n";

    bakePizza();

    bakePizza("Stuff");

    bakePizza("carbs", "woodash");

	return 0;
}