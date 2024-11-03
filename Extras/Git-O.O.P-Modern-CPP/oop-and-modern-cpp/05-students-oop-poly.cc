#include <iostream>
#include <cstring>

class Student {
protected:
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

class School3Student : public Student {
private:
    bool is_late;

public:
    // Inherits constructor from Student and initializes additional fields
    School3Student(const char* name, int age, float gpa,bool is_it_late)
    : Student(name, age, gpa),is_late(is_it_late) {
      
    }

    // Overrides the print function to include School 2 specific details
    void print() const override {
        Student::print();
        std::cout << "is he late?" << is_late << std::endl;
    }
};

int main() {
    School1Student s1("John Doe", 20, 3.5, 123, 10);
    
    School2Student s2("Jane Smith", 21, 3.7, "123 Maple St", "555-1234");
    School3Student s3("John Doe", 20, 3.5,true);
    // Using base class pointers to demonstrate polymorphism
    Student* studentArray[3];
    studentArray[0] = &s1;  // Pointing to a School1Student object
    studentArray[1] = &s2;  // Pointing to a School2Student object
    studentArray[2] = &s3;

    // Loop through the array and call print, which will call the appropriate overridden function
    for (int i = 0; i < 3; ++i) {
        std::cout << "Printing information for student " << (i + 1) << ":" << std::endl;
        studentArray[i]->print(); // Calls the correct print() method depending on the object type
        std::cout << std::endl;
    }

    return 0;
}


