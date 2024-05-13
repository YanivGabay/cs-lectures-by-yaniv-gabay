/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 06 - Ptrs and Structs
 * File: 02_structs.cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-13
 *
 * Overview:
 * This example is a simple employee management system.
 * It demonstrates how to use structs and pointers to structs in a more advanced way.
 * This version uses dynamic memory allocation for employee management.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
#include <cstring>

// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 50;
const int MAX_EMPLOYEES = 10;

// ---------- Structs ----------
struct Date {
    int day;
    int month;
    int year;
};

struct Employee {
    int id;
    char name[MAX_SIZE];
    Date birthDate;
};

struct Department {
    int id;
    char name[50];
    Employee* employees[MAX_EMPLOYEES];
    int employeeCount;
};

// ---------- Function Prototypes ----------
Employee* createEmployee(int id, const char* name, int day, int month, int year);
Department* createDepartment(int id, const char* name);
void addEmployeeToDepartment(Department* dept, Employee* emp);
void listEmployeesInDepartment(const Department* dept);
void deleteDepartment(Department* dept);

void displayMenu();
void handleAddEmployeeToDepartment(Department* dept);
void handleListEmployeesInDepartment(const Department* dept);
void deleteDepartment(Department* dept) ;
// ---------- Main Function ----------
int main() {
    Department* dept = createDepartment(1, "Engineering");
    int choice;

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleAddEmployeeToDepartment(dept);
                break;
            case 2:
                handleListEmployeesInDepartment(dept);
                break;
            case 3:
                std::cout << "Exiting...\n";
                
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    deleteDepartment(dept);
    return 0;
}

// ---------- Functions ----------
Employee* createEmployee(int id, const char* name, int day, int month, int year) {
    Employee* emp = new Employee;
    emp->id = id;
    strcpy(emp->name, name);
    emp->birthDate = {day, month, year};
    return emp;
}

Department* createDepartment(int id, const char* name) {
    Department* dept = new Department;
    if (dept == nullptr) {
        std::cout << "Failed to create department. Exiting...\n";
        exit(1);
    }
    dept->id = id;
    strcpy(dept->name, name);
    dept->employeeCount = 0;
    return dept;
}

void addEmployeeToDepartment(Department* dept, Employee* emp) {
    if (dept->employeeCount < MAX_EMPLOYEES) {
        dept->employees[dept->employeeCount++] = emp;
        std::cout << "Employee added successfully.\n";
    } else {
        std::cout << "Department is full, cannot add more employees.\n";
        delete emp; // Cleanup if not added
    }
}

void listEmployeesInDepartment(const Department* dept) {
    std::cout << "Employees in Department " << dept->name << ":\n";
    for (int i = 0; i < dept->employeeCount; ++i) {
        const Employee* emp = dept->employees[i];
        std::cout << "ID: " << emp->id
                  << ", Name: " << emp->name
                  << ", Birth Date: " << emp->birthDate.day << "/"
                  << emp->birthDate.month << "/"
                  << emp->birthDate.year << std::endl;
    }
}

void deleteDepartment(Department* dept) {
    for (int i = 0; i < dept->employeeCount; ++i) {
        delete dept->employees[i];
    }
    delete dept;
}

void displayMenu() {
    std::cout << "Employee Management System\n";
    std::cout << "1. Add Employee to Department\n";
    std::cout << "2. List Employees in Department\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

void handleAddEmployeeToDepartment(Department* dept) {
    int id, day, month, year;
    char name[MAX_SIZE];

    std::cout << "Enter Employee ID: ";
    std::cin >> id;
    std::cout << "Enter Employee Name: ";
    std::cin.ignore();
    std::cin.getline(name, MAX_SIZE);
    std::cout << "Enter Birth Date (day month year): ";
    std::cin >> day >> month >> year;

    Employee* emp = createEmployee(id, name, day, month, year);
    if(emp == nullptr) {
        std::cout << "Failed to create employee. Exiting...\n";
        deleteDepartment(dept);
        exit(1);
    }
    addEmployeeToDepartment(dept, emp);
}

void handleListEmployeesInDepartment(const Department* dept) {
    listEmployeesInDepartment(dept);
}
