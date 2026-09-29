#include <iostream>
using namespace std;

// Base class
class Animal {
private:
    bool isEating;
    bool isSleeping;

public:
    Animal() : isEating(false), isSleeping(false) {}

    void setEating(bool eating) {
        isEating = eating;
    }

    bool getEating() const {
        return isEating;
    }

    void setSleeping(bool sleeping) {
        isSleeping = sleeping;
    }

    bool getSleeping() const {
        return isSleeping;
    }

    void eat() {
        setEating(true);
        cout << "I can eat!" << endl;
    }

    void sleep() {
        setSleeping(true);
        cout << "I can sleep!" << endl;
    }
};

// Derived class
class Dog : Animal {
public:
    void bark() {
        cout << "I can bark! Woof woof!" << endl;
    }

    // Public functions to access base class functionality
    void dogEat() {
        eat();
    }

    void dogSleep() {
        sleep();
    }

    bool isDogEating() const {
        return getEating();
    }

    bool isDogSleeping() const {
        return getSleeping();
    }
};;

int main() {
    Dog myDog;
    myDog.dogEat();   // Calling base class function through public function
    myDog.dogSleep(); // Calling base class function through public function
    myDog.bark();     // Calling derived class function

    cout << "Is the dog eating? " << (myDog.isDogEating() ? "Yes" : "No") << endl;
    cout << "Is the dog sleeping? " << (myDog.isDogSleeping() ? "Yes" : "No") << endl;

    return 0;
}