// Advanced C++ - Classes and Inheritance Example
#include <iostream>
#include <string>
#include <memory>

/*
 * OBJECT-ORIENTED PROGRAMMING IN C++
 * 
 * This example demonstrates key OOP concepts in C++:
 * 
 * 1. Classes: User-defined types that encapsulate data (attributes) and behavior (methods)
 * 2. Inheritance: Creating new classes based on existing ones
 * 3. Polymorphism: Using a base class reference/pointer to access derived class functionality
 * 4. Encapsulation: Hiding implementation details with access specifiers (public, protected, private)
 * 5. Abstraction: Using abstract classes with pure virtual functions
 * 
 * The example also shows modern C++ practices like:
 * - Smart pointers for automatic memory management
 * - Move semantics for efficient resource transfer
 * - Override keyword for clear method overriding
 * - Virtual destructors for proper cleanup in inheritance hierarchies
 */

// Base class with virtual methods
class Animal {
protected:
    // Protected members are accessible by derived classes but not from outside
    std::string name;
    int age;

public:
    // Constructor with initialization list
    Animal(std::string name, int age) : name(std::move(name)), age(age) {
        std::cout << "Animal constructor called" << std::endl;
    }
    
    // Virtual method - can be overridden by derived classes
    // Virtual enables runtime polymorphism
    virtual void makeSound() const {
        std::cout << "Some generic animal sound" << std::endl;
    }
    
    // Pure virtual method (= 0) - must be implemented by derived classes
    // This makes Animal an abstract class that cannot be instantiated directly
    virtual void move() const = 0;
    
    // Non-virtual method - cannot be overridden (only hidden)
    void displayInfo() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
    
    // Virtual destructor - critical for proper cleanup in polymorphic hierarchies
    // Without this, derived class destructors might not be called when deleting through a base pointer
    virtual ~Animal() {
        std::cout << "Animal destructor called" << std::endl;
    }
};

// Derived class Dog - inherits from Animal
class Dog : public Animal {
private:
    std::string breed;
    
public:
    // Using base class constructor with initialization list
    // The order of initialization is:
    // 1. Base class members (in the order they're declared in the base class)
    // 2. Derived class members (in the order they're declared in the derived class)
    Dog(std::string name, int age, std::string breed) 
        : Animal(std::move(name), age), breed(std::move(breed)) {
        std::cout << "Dog constructor called" << std::endl;
    }
    
    // Override keyword explicitly marks this as an override of a virtual function
    // This provides compile-time checking that you're actually overriding something
    void makeSound() const override {
        std::cout << "Woof! Woof!" << std::endl;
    }
    
    // Implementing the pure virtual method from the base class
    void move() const override {
        std::cout << "Dog runs on four legs" << std::endl;
    }
    
    // Additional method specific to Dog
    void displayBreed() const {
        std::cout << "Breed: " << breed << std::endl;
    }
    
    // Derived class destructor
    ~Dog() override {
        std::cout << "Dog destructor called" << std::endl;
    }
};

// Another derived class
class Bird : public Animal {
private:
    bool canFly;
    
public:
    Bird(std::string name, int age, bool canFly)
        : Animal(std::move(name), age), canFly(canFly) {
        std::cout << "Bird constructor called" << std::endl;
    }
    
    void makeSound() const override {
        std::cout << "Chirp! Chirp!" << std::endl;
    }
    
    void move() const override {
        if (canFly) {
            std::cout << "Bird flies through the air" << std::endl;
        } else {
            std::cout << "Bird hops around" << std::endl;
        }
    }
    
    ~Bird() override {
        std::cout << "Bird destructor called" << std::endl;
    }
};

int main() {
    std::cout << "===== Classes and Inheritance Example =====" << std::endl;
    
    // Cannot instantiate Animal directly since it has pure virtual functions
    // Animal genericAnimal("Generic", 1); // This would cause a compilation error
    
    /*
     * POLYMORPHISM AND SMART POINTERS
     * 
     * Here we're using smart pointers (unique_ptr) to:
     * 1. Manage memory automatically - no need for manual delete
     * 2. Express ownership semantics - unique_ptr indicates exclusive ownership
     * 3. Prevent resource leaks even if exceptions occur
     * 
     * The polymorphism is demonstrated by:
     * 1. Storing derived class objects (Dog, Bird) in base class pointers (Animal*)
     * 2. Calling virtual methods that are resolved at runtime based on the actual object type
     */
    
    // Using smart pointers for memory management
    std::unique_ptr<Animal> dog = std::make_unique<Dog>("Buddy", 3, "Golden Retriever");
    std::unique_ptr<Animal> bird = std::make_unique<Bird>("Tweety", 1, true);
    
    // Polymorphic behavior
    std::cout << "\nPolymorphic function calls:" << std::endl;
    dog->displayInfo();  // Non-virtual method from base class
    dog->makeSound();    // Virtual method, calls Dog's version
    dog->move();         // Pure virtual method, calls Dog's implementation
    
    bird->displayInfo(); // Non-virtual method from base class
    bird->makeSound();   // Virtual method, calls Bird's version
    bird->move();        // Pure virtual method, calls Bird's implementation
    
    /*
     * DOWNCASTING
     * 
     * Sometimes we need to access derived-class specific methods from a base class pointer.
     * This requires a downcast, which should be done safely using dynamic_cast.
     * dynamic_cast returns nullptr if the cast is invalid.
     */
    
    // Casting to access derived class methods
    std::cout << "\nAccessing derived class specific method:" << std::endl;
    if (auto dogPtr = dynamic_cast<Dog*>(dog.get())) {
        dogPtr->displayBreed();  // Dog-specific method
    }
    
    std::cout << "\nDestructors will be called automatically when smart pointers go out of scope" << std::endl;
    
    /*
     * DESTRUCTION ORDER
     * 
     * When objects are destroyed, the order is:
     * 1. Derived class destructor
     * 2. Base class destructor
     * 
     * This is the reverse of the construction order, ensuring proper cleanup.
     */
    
    return 0;
} 