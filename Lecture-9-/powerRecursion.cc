#include <iostream>

// Function prototypes
int power(int base, int exponent);

// Main function
int main() {
    // Test the power function
    int base = 2;
    int exponent = 5;
    std::cout << "Calculating " << base << " raised to the power of " << exponent << std::endl;
    std::cout << base << " raised to the power of " << exponent << " is: " << power(base, exponent) << std::endl;
    return 0;
}

// Constants

// Function to calculate power recursively
int power(int base, int exponent) {
    // Base case: if exponent is 0, return 1
    if (exponent == 0) {
        std::cout << "Base case reached: " << base << " raised to the power of " << exponent << " = 1" << std::endl;
        return 1;
    }
    // Recursive case: power(base, exponent) = base * power(base, exponent - 1)
    else {
        int result = base * power(base, exponent - 1);
        std::cout << "Calculating " << base << " raised to the power of " << exponent << " = " << base << " * " << base << " raised to the power of " << exponent - 1 << std::endl;
        std::cout << base << " raised to the power of " << exponent << " is: " << result << std::endl;
        return result;
    }
}
