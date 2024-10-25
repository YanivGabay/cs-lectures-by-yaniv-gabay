#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

/*
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

*/

//possible solution for the two schools problem
struct Student {
    char name[50];
    int age;
    float gpa;

    // Fields specific to School 1
    int id;
    int classNumber;

    // Fields specific to School 2
    char address[50];
    char phone[15];

    // Generic function to print basic information
    void printBasicInfo() {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "GPA: " << gpa << endl;
    }

    // Function to print information required by School 1
    void printSchool1Info() {
        printBasicInfo();
        cout << "ID: " << id << endl;
        cout << "Class: " << classNumber << endl;
    }

    // Function to print information required by School 2
    void printSchool2Info() {
        printBasicInfo();
        cout << "Address: " << address << endl;
        cout << "Phone: " << phone << endl;
    }
};

// Function to print student details
void printStudent(const Student& student) {
    std::cout << "Name: " << student.name << std::endl;
    std::cout << "Age: " << student.age << std::endl;
    std::cout << "GPA: " << student.gpa << std::endl;
    //now here we have a problem, how do we know which school is using the program?
    //we have to add a check for the school
    //so either pass a new parameter to the function
    //or create a new print function for each school
}
void printStudent1(const Student& student) {
    std::cout << "Name: " << student.name << std::endl;
    std::cout << "Age: " << student.age << std::endl;
    std::cout << "GPA: " << student.gpa << std::endl;
    std::cout << "ID: " << student.id << std::endl;
    
}
void printStudent2(const Student& student) {
    std::cout << "Name: " << student.name << std::endl;
    std::cout << "Age: " << student.age << std::endl;
    std::cout << "GPA: " << student.gpa << std::endl;
    std::cout << "Address: " << student.address << std::endl;
    std::cout << "Phone: " << student.phone << std::endl;
}

///// WHAT WILL HAPPEN IF WE HAVE 100 SCHOOLS?
///// NOT TALKING ABOUT INIT STUDENT FUNCTION, WHERE WE WILL NEED TO ADD MORE PARAMETERS


// HOPEFULLY YOU NOW UNDERSTAND THE LIMITATIONS OF STRUCTS
// AND WHY WE NEED OOP
// so now we will go into the basics of OOP

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
    student1.printBasicInfo();
    student1.printSchool1Info();

    return 0;
}
