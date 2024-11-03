#include <iostream>
#include <cstring>

class Student {
private:
    char name[50];
    int age;
    float gpa;

public:
    // Constructor initializes data members for all students
    Student(const char* name, int age, float gpa) {
        strncpy(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0'; // Ensure null-termination
        this->age = age;
        this->gpa = gpa;
    }

    // Virtual print function, can be overridden by derived classes
    virtual void print() const {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "GPA: " << gpa << std::endl;
    }

    // Virtual destructor to allow derived class destructors to be called
    virtual ~Student() = default; 
};

class School1Student : public Student {
private:
    int id;
    int classNumber;

public:
    // Inherits constructor from Student and initializes additional fields
    School1Student(const char* name, int age, float gpa, int id, int classNumber)
    : Student(name, age, gpa), id(id), classNumber(classNumber) {}

    // Overrides the print function to include School 1 specific details
    void print() const override {
        Student::print();
        std::cout << "ID: " << id << std::endl;
        std::cout << "Class: " << classNumber << std::endl;
    }
};

class School2Student : public Student {
private:
    char address[50];
    char phone[15];

public:
    // Inherits constructor from Student and initializes additional fields
    School2Student(const char* name, int age, float gpa, const char* address, const char* phone)
    : Student(name, age, gpa) {
        strncpy(this->address, address, sizeof(this->address) - 1);
        this->address[sizeof(this->address) - 1] = '\0';
        strncpy(this->phone, phone, sizeof(this->phone) - 1);
        this->phone[sizeof(this->phone) - 1] = '\0';
    }

    // Overrides the print function to include School 2 specific details
    void print() const override {
        Student::print();
        std::cout << "Address: " << address << std::endl;
        std::cout << "Phone: " << phone << std::endl;
    }
};

int main() {
    School1Student s1("John Doe", 20, 3.5, 123, 10);
    School2Student s2("Jane Smith", 21, 3.7, "123 Maple St", "555-1234");

    std::cout << "Student from School 1:" << std::endl;
    s1.print();
    std::cout << std::endl;

    std::cout << "Student from School 2:" << std::endl;
    s2.print();

    return 0;
}

