#include <iostream>

// Function prototypes
int factorial(int n);

// Main function
int main() {
    // Test the factorial function
    int num = 5;
    std::cout << "Calculating the factorial of " << num << std::endl;
    std::cout << "Final: Factorial of " << num << " is: " << factorial(num) << std::endl;
    return 0;
}

// Constants

// 0! = 1
// 1! = 1
// 2! = 2 * 1 = 2
// 3! = 3 * 2 * 1 = 6

// Function to calculate factorial recursively
int factorial(int n) {
    // Base case: factorial of 0 is 1
    if (n == 0 || n == 1) {
        std::cout << "Base case reached: factorial of " << n << " is 1" << std::endl;
        return 1;
    }
    // Recursive case: factorial(n) = n * factorial(n-1)
    else {
        std::cout << "Calculating factorial of " << n << " = " << n << " * factorial(" << n - 1 << ")" << std::endl;
        int result = n * factorial(n - 1);
        std::cout << "Factorial of " << n << " is: " << result << std::endl;
        return result;
    }
}
