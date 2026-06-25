#include <iostream>

int main() {

	std::cout << "Size Of Operator\n\n";

    // determines size in bytes of var, dtype, class, objects etc...

    double gpa = 2.5;
    std::string name = "Bro"; // strings are reference datatypes and point to a location, always 32bytes
    char grade = 'F'; // 1 byte for a char
    bool student = true; // 1 byte
    char grades[] = {'A','B','C','D','F','I'}; // 5 bytes
    std::string students[] = {"T-800","MrChanche","Ted Rosevelt","Alien2"}; // 128 bytes

    std::cout << sizeof(gpa) << " bytes\n";
    std::cout << sizeof(name) << " bytes\n";
    std::cout << sizeof(grade) << " bytes\n";
    std::cout << sizeof(student) << " bytes\n";
    std::cout << sizeof(grades) << " bytes\n";
    std::cout << sizeof(grades[0]) << " bytes\n";
    std::cout << sizeof(grades) / sizeof(grades[0]) << " elements\n";
    std::cout << sizeof(students) << " bytes\n";
    std::cout << sizeof(students) / sizeof(students[0]) << " elements\n";

	return 0;
}