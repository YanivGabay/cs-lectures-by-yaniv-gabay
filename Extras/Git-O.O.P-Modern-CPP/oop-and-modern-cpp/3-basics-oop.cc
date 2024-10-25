#include <iostream>
#include <string>

// Define a class named 'Person'
class Person {
private: //what is the difference between private and public?
    std::string name;
    int age;

public:
    // Constructor
    //basicly its a "recipe" how we can create this object
    //it has the same name as the class
    //it can have parameters
    //it can have default values for the parameters
    //you can have multiple constructors
    Person(std::string n, int a) : name(n), age(a) {}
    //the {} is the body of the constructor
    // we can init the field there instead of the init list
    //but we can also do more things in the body
    //like checking the values of the parameters
    //or doing some other operations
    //or even throw an exception
    //or call another constructor
    //or call another function
    // example:

    // Person(std::string n, int a) {
    //     if (a < 0) {
    //         throw std::invalid_argument("Age cannot be negative");
    //     }
    //     name = n;
    //     age = a;
    // can also use this-> to refer to the object fields
    //     this->name = n;
    //     this->age = a;
    // }

    //there are also deconstructors
    //they are called when the object is destroyed
    //they are used to clean up the object
    //they have the same name as the class but with a ~ in front
    //they have no parameters
    //they have no return type
    //example:
    // ~Person() {} //this is the default deconstructor
    //you can also have a custom deconstructor
    //but its not needed in this case
    //example:
     ~Person() {
         std::cout << "Person object destroyed" << std::endl;
         //each time a person object is destroyed
            //this message will be printed
     }

    // Getter for name
    //getters are our way to "get" the value inside the object
    //as you saw before the name std::string name is private
    // its means we cannot just do person1.name = "Alice";
    //we have to use the getter to get the value
    // or a setter to set the value
    std::string getName() const {
        return name;
    }
    // Setter for name
    // so we can set the name like this
    // person1.setName("Alice");
    void setName(const std::string& n) {
        name = n;
    }
    // Getter for age
    int getAge() const { /////////first time you saw const on the right side of the function
    // its isnt complicated, it does 2 important things, 1. prevent changing the object inside this function
    // 2. when we create a const person, we could still call this function
        return age;
    }
    int setAge(int a) {
        age = a;
    }
    // Method to display person details
    void display() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};

int main() {
    // Create an object of the Person class
    Person person1("Alice", 30);
    // you can use "new" to create an object on the heap
    // Person* person1 = new Person("Alice", 30);

    person1.display();
    //lets change the name
    person1.setName("Bob");
    //lets change the age
    person1.setAge(40);
    //lets display the details
    person1.display();



    return 0;
}