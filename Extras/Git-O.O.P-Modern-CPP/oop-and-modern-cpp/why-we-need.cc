#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

/*
so why we need OOP
if you remember, the usage of structs
and lets say we have a struct that represents a student
and we have a function that prints the student details
and another function that initializes the student
and we have to pass the student object to the functions
we can also put a print function inside the struct


*/

// Define a struct for a simple student record
struct Student {
    char name[50];
    int age;
    float gpa;
    char tele[50];
    void printStudent()
    {
        cout << "printing form inside the struct" << endl;
        cout << "Name: " << name << endl;
    }
};

// Function to print student details
void printStudent(const Student& student) {
    std::cout << "Name: " << student.name << std::endl;
    std::cout << "Age: " << student.age << std::endl;
    std::cout << "GPA: " << student.gpa << std::endl;
}

// Function to initialize a student
void initStudent(Student& student, const char* name, int age, float gpa) {
    std::strncpy(student.name, name, sizeof(student.name) - 1);
    student.name[sizeof(student.name) - 1] = '\0'; // Ensure null-termination
    student.age = age;
    student.gpa = gpa;
}

int main() {
    Student student1;

    initStudent(student1, "John Doe", 20, 3.5);
    printStudent(student1);
    student1.printStudent();

    return 0;
}
//CONTINUE TO READ FROM HERE
// so what is the problem?
// what happens if now we want to sell our program to a school
// the school says he want additions to the student record
// their id, and their class
// so we have to go and add these fields to the struct
// and we have to go and add these fields to the print function
// and we have to go and add these fields to the init function
// and what happens if we sell the program to another school
// and they want more fields, which are not needed by the first school