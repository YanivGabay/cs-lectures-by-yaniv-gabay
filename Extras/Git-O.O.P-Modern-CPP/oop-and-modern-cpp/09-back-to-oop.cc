#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Person {
public:
    Person(const std::string& name, int age) : name(name), age(age) {}

    std::string getName() const { return name; }
    int getAge() const { return age; }

    void setName(const std::string& name) { this->name = name; }
    void setAge(int age) { this->age = age; }

    void print() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }

private:
    std::string name;
    int age;
};

/**
 * @brief Main function demonstrating the use of std::vector with custom objects.
 * 
 * This function creates a vector of Person objects and populates it using emplace_back.
 * The use of emplace_back is preferred over push_back in this context because it constructs
 * the Person objects in place, avoiding unnecessary copies or moves.
 * 
 * The function then prints the original list of people, sorts the list by age using a lambda
 * function, and prints the sorted list.
 * 
 * @return int Exit status of the program.
 */
    bool compareByAge(const Person& a, const Person& b) {
        return a.getAge() < b.getAge();
    }
int main() {
    std::vector<Person> people;
    people.emplace_back("Alice", 30);//creating a new person object and adding it to the end of the vector
    people.emplace_back("Bob", 25);
    people.emplace_back("Charlie", 35);

    std::cout << "Original list:" << std::endl;
    for (const auto& person : people) { // you know auto, but why auto&???????? clue: we dont want to copy the object
        person.print();
    }

    /*
    std::sort(people.begin(), people.end(), ...):

This is a call to the std::sort function from the Standard Library, which sorts a range of elements (in this case, from people.begin() to people.end()) based on a comparison function provided as the third argument.
people is a std::vector (or any container) of Person objects.
Lambda Function: [](const Person& a, const Person& b):

The part [] defines a lambda. It’s essentially an anonymous, inline function.
The [] is called the capture clause, which allows the lambda to access variables from the enclosing scope (more on that later).
(const Person& a, const Person& b) defines the lambda's parameters. This lambda takes two Person objects by reference, meaning it can access the Person objects without making a copy.
return a.getAge() < b.getAge();:

This is the body of the lambda function. The lambda compares two Person objects based on their age by calling the getAge() method on both.
The getAge() function is assumed to return the age of a person.
The lambda returns true if a is younger than b, meaning the std::sort will place a before b.
Putting It All Together:

The lambda is passed as the third argument to std::sort, and it tells std::sort how to compare two Person objects.
std::sort will repeatedly call the lambda for pairs of Person objects in the people vector, sorting them in ascending order of age.
Key Concepts for Someone New to Lambdas:
Lambdas are inline anonymous functions that can be passed as arguments (as in this case) or stored in variables.
Capture clause []: You can capture variables from the outside scope by putting them inside the brackets ([]), though in this case, it's empty because no outside variables are needed.
Parameters (const Person& a, const Person& b): These specify the inputs to the lambda, similar to regular function parameters.
Return type: The lambda can infer the return type based on the body (a.getAge() < b.getAge() returns a boolean in this case), so it's not explicitly stated.
    
    
    */
    std::sort(people.begin(), people.end(), 
    [](const Person& a, const Person& b) {
        return a.getAge() < b.getAge();
    }
    );

    //dont like that lambda? we can use a function pointer instead
    // Step 1: Define a comparison function


    // Step 2: Use the function with std::sort
    std::sort(people.begin(), people.end(), compareByAge);


    std::cout << "\nSorted by age:" << std::endl;
    for (const auto& person : people) {
        person.print();
    }

    return 0;
}