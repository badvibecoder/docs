#include <iostream>
#include <string>

void showBalance();
double deposit();
double withdraw(double bal);
void loadMenu();

double balance = 100.00;
int menu = 0;
int control = 1;


int main() {


    do {

        loadMenu();

        switch (menu) {
            case 0:
                break;
            case 1:
                showBalance();
                menu = 0;
                break;
            case 2:
                deposit();
                menu = 0;
                break;
            case 3:
                withdraw(balance);
                menu = 0;
                break;
            case 4:
                control = 0;
                break;
        }

    } while (control == 1);

    return 0;
    
}

void showBalance(){

    std::cout << "\nBalance: " << balance << "\n";
    menu = 0;

}

double deposit(){

    std::string input;
    std::cout << "Enter Deposit Amount: \n";
    std::cin >> input;
    balance += std::stod(input);
    menu = 0;

}

double withdraw(double bal){

    std::string input;
    std::cout << "Enter Withdrawl Amount: \n";
    std::cin >> input;
    double amount = std::stod(input);
    if(bal >= amount){
        bal -= amount;
        balance = bal;
    } else {
        std::cout << "Insufficient Funds\n";
    }

}

void loadMenu(){

    // load menu
    std::cout << "\n\nWelcome To The Bank\n\n";
    std::cout << "1 - Show Balance\n";
    std::cout << "2 - Deposit\n";
    std::cout << "3 - Withdrawl\n";
    std::cout << "4 - Exit\n\n";
    std::cout << "Enter Selection: \n";
    std::cin >> menu;

}