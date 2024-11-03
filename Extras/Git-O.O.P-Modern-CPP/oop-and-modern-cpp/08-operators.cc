#include <iostream>



class Complex
{ 
public:
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // Overload + operator to add two Complex objects
    // Complex c3 = c1 + c2;
    Complex operator+(const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }
    

    // Overload << operator to print a Complex object
    double getReal() const { return real; }
    double getImag() const { return imag; }

private:
    double real, imag;
};
std::ostream &operator<<(std::ostream &os, const Complex &c)
{
    os << c.getReal() << " + " << c.getImag() << "i";
    return os;
}

int main()
{
    Complex c1(3.0, 4.0);
    Complex c2(1.0, 2.0);
    
    Complex c3 = c1 + c2;
    
    std::cout << "c1: " << c1 << std::endl;
    std::cout << "c2: " << c2 << std::endl;
    std::cout << "c3: " << c3 << std::endl;

    return 0;
}