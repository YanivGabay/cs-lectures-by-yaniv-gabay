#include <iostream>

// Function prototypes
int fibonacci(int n);

// Main function
int main() {
    // Test the fibonacci function
    int num = 7;
    std::cout << "Calculating the " << num << "th term of the Fibonacci sequence" << std::endl;
    std::cout << "The " << num << "th term of the Fibonacci sequence is: " << fibonacci(num) << std::endl;
    return 0;
}

// Constants

// Function to calculate nth term of Fibonacci sequence recursively
int fibonacci(int n) {
    // Base cases: fibonacci(0) = 0, fibonacci(1) = 1
    if (n == 0) {
        std::cout << "Base case reached: fibonacci(0) = 0" << std::endl;
        return 0;
    }
    else if (n == 1) {
        std::cout << "Base case reached: fibonacci(1) = 1" << std::endl;
        return 1;
    }
    // Recursive case: fibonacci(n) = fibonacci(n-1) + fibonacci(n-2)
    else {
        std::cout << "Calculating fibonacci(" << n << ") = fibonacci(" << n - 1 << ") + fibonacci(" << n - 2 << ")" << std::endl;
        int result = fibonacci(n - 1) + fibonacci(n - 2);
        std::cout << "Fibonacci(" << n << ") is: " << result << std::endl;
        return result;
    }
}
