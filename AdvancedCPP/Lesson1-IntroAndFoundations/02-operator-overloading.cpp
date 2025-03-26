// Advanced C++ - Operator Overloading Example
#include <iostream>
#include <string>
#include <sstream>

/*
 * OPERATOR OVERLOADING IN C++
 * 
 * Operator overloading allows you to define custom behavior for C++ operators when
 * they are used with user-defined types. This makes your classes feel more natural
 * and intuitive to use.
 * 
 * This example demonstrates:
 * 1. Overloading arithmetic operators (+, -, *, unary -)
 * 2. Overloading comparison operators (==, !=)
 * 3. Overloading increment/decrement operators (++, --)
 * 4. Overloading stream operators (<<, >>)
 * 5. Conversion operators (type casting)
 * 6. Copy/move constructors and assignment operators
 * 
 * IMPORTANT GUIDELINES:
 * - Operators should behave intuitively (like their built-in counterparts)
 * - Operator overloading should respect the precedence and associativity of the operator
 * - Consider overloading related operators as a group (e.g., == and !=)
 * - Be careful with return types (e.g., comparison operators should return bool)
 */

class Complex {
private:
    double real;
    double imag;

public:
    // Constructor with default values
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    /*
     * COPY AND MOVE SEMANTICS
     * 
     * Copy constructor: Creates a new object as a copy of an existing one
     * Move constructor: Efficiently transfers resources from a temporary object
     * 
     * Assignment operators do the same for already-constructed objects
     */
    
    // Copy constructor
    Complex(const Complex& other) : real(other.real), imag(other.imag) {
        std::cout << "Copy constructor called" << std::endl;
    }

    // Move constructor
    Complex(Complex&& other) : real(other.real), imag(other.imag) {
        std::cout << "Move constructor called" << std::endl;
        other.real = 0.0;
        other.imag = 0.0;
    }

    // Assignment operator
    Complex& operator=(const Complex& other) {
        std::cout << "Assignment operator called" << std::endl;
        // Check for self-assignment
        if (this != &other) {
            real = other.real;
            imag = other.imag;
        }
        return *this;
    }

    // Move assignment operator
    Complex& operator=(Complex&& other) {
        std::cout << "Move assignment operator called" << std::endl;
        if (this != &other) {
            real = other.real;
            imag = other.imag;
            other.real = 0.0;
            other.imag = 0.0;
        }
        return *this;
    }

    /*
     * BINARY ARITHMETIC OPERATORS
     * 
     * These can be implemented as:
     * 1. Member functions (first operand is the object itself)
     * 2. Non-member functions (typically for symmetric operations)
     * 
     * Member function syntax: ReturnType operator+(const OtherType& other) const;
     * Non-member function syntax: ReturnType operator+(const Type1& a, const Type2& b);
     */
    
    // Binary operator+ (member function)
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Binary operator- (member function)
    Complex operator-(const Complex& other) const {
        return Complex(real - other.real, imag - other.imag);
    }

    // Binary operator* (member function)
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }

    /*
     * UNARY OPERATORS
     * 
     * Unary operators like -, ++, -- take no arguments when defined as member functions
     * The prefix ++ and -- should return a reference, while postfix returns by value
     */
    
    // Unary operator- (negation)
    Complex operator-() const {
        return Complex(-real, -imag);
    }

    // Pre-increment operator (++x)
    Complex& operator++() {
        ++real;
        return *this;
    }

    // Post-increment operator (x++)
    // Note the dummy int parameter to distinguish from pre-increment
    Complex operator++(int) {
        Complex temp(*this);  // Save current state
        ++real;               // Increment
        return temp;          // Return saved state
    }

    /*
     * COMPARISON OPERATORS
     * 
     * These should return bool and typically come in pairs (==, !=)
     * In C++20 and later, you can define <=> (spaceship operator) instead
     */
    
    // Comparison operators
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }

    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }

    /*
     * CONVERSION OPERATORS
     * 
     * Allow casting to other types using the type cast syntax
     * The 'explicit' keyword prevents implicit conversions
     */
    
    // Conversion operator to string
    explicit operator std::string() const {
        std::stringstream ss;
        ss << real;
        if (imag >= 0) {
            ss << " + " << imag << "i";
        } else {
            ss << " - " << -imag << "i";
        }
        return ss.str();
    }

    /*
     * FRIEND FUNCTIONS
     * 
     * Friend functions have access to private members but are not member functions
     * Often used for operators where the left-hand operand isn't an instance of the class
     * Common for stream operators (<<, >>)
     */
    
    // Friend function for output stream operator
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

    // Friend function for input stream operator
    friend std::istream& operator>>(std::istream& is, Complex& c);

    // Getter methods
    double getReal() const { return real; }
    double getImag() const { return imag; }
};

/*
 * NON-MEMBER OPERATOR OVERLOADS
 * 
 * Used for:
 * 1. Symmetric operations (e.g., number + object and object + number)
 * 2. When the left operand isn't the class type
 * 3. When the left operand is a type you can't modify
 */

// Binary operator+ as a free function (addition with scalar)
Complex operator+(const Complex& c, double scalar) {
    return Complex(c.getReal() + scalar, c.getImag());
}

// Binary operator+ as a free function (addition with scalar, reversed order)
Complex operator+(double scalar, const Complex& c) {
    return Complex(scalar + c.getReal(), c.getImag());
}

/*
 * STREAM OPERATORS
 * 
 * Customizes how objects are printed or read
 * Should return the stream to allow for chaining (cout << a << b)
 */

// Output stream operator<<
std::ostream& operator<<(std::ostream& os, const Complex& c) {
    if (c.imag >= 0) {
        os << c.real << " + " << c.imag << "i";
    } else {
        os << c.real << " - " << -c.imag << "i";
    }
    return os;
}

// Input stream operator>>
std::istream& operator>>(std::istream& is, Complex& c) {
    std::cout << "Enter real part: ";
    is >> c.real;
    std::cout << "Enter imaginary part: ";
    is >> c.imag;
    return is;
}

int main() {
    std::cout << "===== Operator Overloading Example =====" << std::endl;
    
    Complex c1(3.0, 4.0);
    Complex c2(1.0, -2.0);
    
    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "c2 = " << c2 << std::endl;
    
    // Using overloaded operators
    Complex c3 = c1 + c2;
    std::cout << "c1 + c2 = " << c3 << std::endl;
    
    Complex c4 = c1 - c2;
    std::cout << "c1 - c2 = " << c4 << std::endl;
    
    Complex c5 = c1 * c2;
    std::cout << "c1 * c2 = " << c5 << std::endl;
    
    Complex c6 = -c1;
    std::cout << "-c1 = " << c6 << std::endl;
    
    // Pre and post increment
    Complex c7 = c1;
    std::cout << "c7 = " << c7 << std::endl;
    std::cout << "++c7 = " << ++c7 << std::endl;
    std::cout << "c7 after pre-increment = " << c7 << std::endl;
    std::cout << "c7++ = " << c7++ << std::endl;
    std::cout << "c7 after post-increment = " << c7 << std::endl;
    
    // Comparison operators
    std::cout << "c1 == c2: " << (c1 == c2) << std::endl;
    std::cout << "c1 != c2: " << (c1 != c2) << std::endl;
    
    // Using free function operators
    Complex c8 = c1 + 5.0;
    std::cout << "c1 + 5.0 = " << c8 << std::endl;
    
    Complex c9 = 2.5 + c2;
    std::cout << "2.5 + c2 = " << c9 << std::endl;
    
    // Using conversion operator
    std::string c1String = static_cast<std::string>(c1);
    std::cout << "c1 as string: " << c1String << std::endl;
    
    // Manual input example (commented out to avoid breaking automatic execution)
    /*
    Complex userInput;
    std::cin >> userInput;
    std::cout << "You entered: " << userInput << std::endl;
    */
    
    return 0;
} 