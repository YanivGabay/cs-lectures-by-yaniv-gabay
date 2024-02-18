#include <iostream>

// Function prototypes
int sumOfDigits(int n);

// Main function
int main() {
    // Test the sumOfDigits function
    int num = 12345;
    std::cout << "Calculating the sum of digits of " << num << std::endl;
    std::cout << "The sum of digits of " << num << " is: " << sumOfDigits(num) << std::endl;
    return 0;
}

// Constants

// Function to calculate sum of digits recursively
int sumOfDigits(int n) {
    // Base case: if n is a single digit number, return n
    if (n < 10) {
        std::cout << "Base case reached: sumOfDigits(" << n << ") = " << n << std::endl;
        return n;
    }
    // Recursive case: sumOfDigits(n) = n % 10 + sumOfDigits(n / 10)
    else {
        int sum = n % 10 + sumOfDigits(n / 10);
        std::cout << "Calculating sumOfDigits(" << n << ") = " << n % 10 << " + sumOfDigits(" << n / 10 << ")" << std::endl;
        std::cout << "Sum of digits of " << n << " is: " << sum << std::endl;
        return sum;
    }
}
