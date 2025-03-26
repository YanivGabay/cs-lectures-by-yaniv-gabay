// Advanced C++ - Basic Templates Introduction
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>  // Required for typeid operator

/*
 * This is a simplified introduction to templates in C++.
 * The more advanced concepts have been removed for clarity.
 * This example focuses on:
 * 1. Basic function templates
 * 2. Simple class templates
 * 3. Template specialization
 * 
 * WHAT ARE TEMPLATES?
 * Templates are a powerful feature of C++ that allows for generic programming.
 * They enable you to write code that works with any data type without having to
 * rewrite the same logic for each type. The compiler generates the appropriate
 * code for each type when the template is used.
 * 
 * WHY USE TEMPLATES?
 * - Code reusability: Write once, use with many different types
 * - Type safety: Compiler checks types at compile time
 * - Performance: No runtime overhead as code is generated at compile time
 * - Flexibility: Can work with user-defined types as well as built-in types
 */

//----------------------------------------------------------------------------
// 1. Basic Function Templates
//----------------------------------------------------------------------------

/*
 * FUNCTION TEMPLATES
 * 
 * A function template defines a family of functions for different types.
 * The compiler generates a specific function for each type when you use the template.
 * 
 * Syntax:
 *    template <typename T>
 *    return_type function_name(parameters) { ... }
 * 
 * Notes:
 * - 'typename' and 'class' keywords can be used interchangeably in this context
 * - Type deduction happens automatically when the function is called
 * - You can have multiple template parameters (T, U, etc.)
 */

// Function template for finding the maximum of two values of any type
template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
    // This works for any type T that supports the > operator
    // (like int, double, char, and user-defined types with operator> overloaded)
}

// Function template with multiple type parameters
template <typename T, typename U>
auto add(T a, U b) {
    return a + b;  // Return type is deduced automatically using 'auto'
    // This works for any types T and U that support the + operator
}

// Function to print any type of array
template <typename T>
void printArray(T arr[], int size) {
    std::cout << "Array contents: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    // This works for arrays of any type T that can be printed using operator<<
}

//----------------------------------------------------------------------------
// 2. Simple Class Template
//----------------------------------------------------------------------------

/*
 * CLASS TEMPLATES
 * 
 * Similar to function templates, class templates allow you to define a blueprint
 * for a family of classes. The compiler generates a specific class for each type.
 * 
 * Syntax:
 *    template <typename T>
 *    class ClassName { ... };
 * 
 * Notes:
 * - When using a class template, you must specify the type: Stack<int>, Stack<string>
 * - Member functions can be defined inside or outside the class
 * - You can have default template parameters
 * - You can specialize class templates for specific types
 */

// A basic stack class template
template <typename T>
class Stack {
private:
    std::vector<T> elements;  // Using vector to store elements

public:
    // Push an element onto the stack
    void push(const T& element) {
        elements.push_back(element);
    }

    // Pop an element from the stack
    T pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        
        T top = elements.back();
        elements.pop_back();
        return top;
    }

    // Check if the stack is empty
    bool isEmpty() const {
        return elements.empty();
    }

    // Get the current size of the stack
    size_t size() const {
        return elements.size();
    }
};

//----------------------------------------------------------------------------
// 3. Template Specialization
//----------------------------------------------------------------------------

/*
 * TEMPLATE SPECIALIZATION
 * 
 * Template specialization allows you to provide a specific implementation of a template
 * for a particular type, overriding the generic implementation.
 * 
 * Syntax for full specialization:
 *    template <>
 *    return_type function_name<specific_type>(parameters) { ... }
 * 
 * Notes:
 * - Useful when you need different behavior for specific types
 * - Can be applied to both function and class templates
 * - Partial specialization (specializing only some parameters) is only available for class templates
 */

// Primary template for any type
template <typename T>
void printTypeInfo(const T& value) {
    std::cout << "This is a generic type" << std::endl;
    // Default behavior for any type
}

// Specialization for int
template <>
void printTypeInfo(const int& value) {
    std::cout << "This is an integer with value: " << value << std::endl;
    // Special behavior for int type
}

// Specialization for double
template <>
void printTypeInfo(const double& value) {
    std::cout << "This is a double with value: " << value << std::endl;
    // Special behavior for double type
}

// Specialization for C-style string (const char*)
template <>
void printTypeInfo(const char* const& value) {
    std::cout << "This is a string with value: " << value << std::endl;
    // Special behavior for C-style string
}

int main() {
    std::cout << "===== Simple Templates Example =====" << std::endl;

    // 1. Basic Function Templates
    std::cout << "\n--- Basic Function Templates ---" << std::endl;
    
    // Using the maximum template function with different types
    int int_max = maximum(10, 20);
    std::cout << "Maximum of 10 and 20: " << int_max << std::endl;
    
    double double_max = maximum(3.14, 2.71);
    std::cout << "Maximum of 3.14 and 2.71: " << double_max << std::endl;
    
    char char_max = maximum('A', 'Z');
    std::cout << "Maximum of 'A' and 'Z': " << char_max << std::endl;
    
    // Using the add template function with mixed types
    auto sum1 = add(5, 3.5);
    std::cout << "5 + 3.5 = " << sum1 << " (type: " << (typeid(sum1).name()) << ")" << std::endl;
    
    auto sum2 = add(std::string("Hello, "), "World!");
    std::cout << "String concatenation: " << sum2 << std::endl;
    
    // Array printing template function
    int int_array[] = {1, 2, 3, 4, 5};
    double double_array[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    
    printArray(int_array, 5);
    printArray(double_array, 5);

    // 2. Class Template
    std::cout << "\n--- Class Template Example ---" << std::endl;
    
    // Create an integer stack
    Stack<int> int_stack;
    int_stack.push(10);
    int_stack.push(20);
    int_stack.push(30);
    
    std::cout << "Integer stack size: " << int_stack.size() << std::endl;
    std::cout << "Popping elements: ";
    while (!int_stack.isEmpty()) {
        std::cout << int_stack.pop() << " ";
    }
    std::cout << std::endl;
    
    // Create a string stack
    Stack<std::string> string_stack;
    string_stack.push("Hello");
    string_stack.push("C++");
    string_stack.push("Templates");
    
    std::cout << "String stack size: " << string_stack.size() << std::endl;
    std::cout << "Popping elements: ";
    while (!string_stack.isEmpty()) {
        std::cout << string_stack.pop() << " ";
    }
    std::cout << std::endl;
    
    // 3. Template Specialization
    std::cout << "\n--- Template Specialization Example ---" << std::endl;
    
    int i = 42;
    double d = 3.14159;
    std::string s = "Hello";
    
    printTypeInfo(i);       // Uses int specialization
    printTypeInfo(d);       // Uses double specialization
    printTypeInfo("Hello"); // Uses C-string specialization
    printTypeInfo(s);       // Uses generic template (std::string)
    
    /*
     * IMPORTANT CONCEPTS TO REMEMBER:
     * 
     * 1. Template Instantiation: The compiler generates code for each type when the template is used
     * 
     * 2. Type Requirements: Templates impose implicit requirements on the types they work with
     *    (e.g., the maximum function requires the type to support the > operator)
     * 
     * 3. Template Parameters: Can be types, non-type parameters (like integers), or other templates
     * 
     * 4. Code Bloat: Be aware that excessive use of templates can increase compile time and binary size
     *    since code is generated for each type
     * 
     * 5. Error Messages: Template errors can be complex and difficult to understand because
     *    they occur during instantiation
     */
    
    return 0;
} 