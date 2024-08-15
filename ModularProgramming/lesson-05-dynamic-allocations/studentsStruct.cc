/********************************************************************
 * Course: Modular Programming
 * Lecture: 5 - Dynamic Allocation
 * File: 1_basicAlloc.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-05-4
 * 
 * Overview:
 * 
 * 
 * Notes:
 * 
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
#include <iomanip>
#include <cstring>

// ---------- Using Section ----------


// ---------- Constants ----------
const int MAX_SIZE = 50;
// Add more constants as needed

// ---------- Structs ----------
struct Student {
    int id;
    char name[MAX_SIZE];
    double grade;
};

struct Courses {
    Student* students;
    int students_num;
    char course_name[MAX_SIZE];
};

struct Dynamic2DArray {
    Courses* data;
    int courses;
};

// ---------- Function Prototypes ----------

void initializeStudent(Student& student, int id, const char* name, double grade);
void printAllStudents(const Dynamic2DArray& data);
void printStudent(const Student& student);
void initCourse(Dynamic2DArray& data, int course_num);
void initCourseStudents(Dynamic2DArray& data, int course_num);
void cleanUp(Dynamic2DArray& data);

// ---------- Main Function ----------

int main() {
    int courses;
    std::cout << "Enter the number of courses: ";
    std::cin >> courses;

    Dynamic2DArray data;
    data.courses = courses;
    data.data = new Courses[courses];
    if (data.data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }

    for (int course_num = 0; course_num < courses; ++course_num) {
        initCourse(data, course_num);
    }

    printAllStudents(data);
    cleanUp(data);

    return 0;
}

// ---------- Functions ----------
void printAllStudents(const Dynamic2DArray& data) {
    for (int course_num = 0; course_num < data.courses; ++course_num) {
        std::cout << "Course " << course_num+1 << " " << data.data->course_name << " details:" << std::endl;
        for (int student_num = 0; student_num < data.data[course_num].students_num; ++student_num) {
            std::cout << "-----------------------------------\n";
            printStudent(data.data[course_num].students[student_num]);
        }
    }
}

void printStudent(const Student& student) {
    std::cout << "Student details:" << std::endl;
    std::cout << "Student id: " << student.id << std::endl;
    std::cout << "Student name: " << student.name << std::endl;
    std::cout << "Student grade: " << student.grade << std::endl;
    std::cout << std::endl;
}

void initCourse(Dynamic2DArray& data, int course_num) {
    std::cout << "Enter the number of students in course " << course_num + 1 << ": ";
    std::cin >> data.data[course_num].students_num;
    std::cin.ignore(); // ignore the newline character left in the buffer by std::cin >> data.data[course_num].students_num
    std::cout << "Enter the name of course " << course_num + 1 << ": ";
    std::cin.getline(data.data[course_num].course_name, MAX_SIZE);
    data.data[course_num].students = new Student[data.data[course_num].students_num];

    if (data.data[course_num].students == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }

    initCourseStudents(data, course_num);
}

void initCourseStudents(Dynamic2DArray& data, int course_num) {
    for (int student_num = 0; student_num < data.data[course_num].students_num; ++student_num) {
        int id;
        char name[MAX_SIZE];
        double grade;
        std::cout << "Enter student " << student_num + 1 << " id: ";
        std::cin >> id;
        std::cout << "Enter student " << student_num + 1 << " name: ";
        std::cin.ignore(); // ignore the newline character left in the buffer by std::cin >> id
        std::cin.getline(name, MAX_SIZE);
        std::cout << "Enter student " << student_num + 1 << " grade: ";
        std::cin >> grade;
        initializeStudent(data.data[course_num].students[student_num], id, name, grade);
    }
}

void initializeStudent(Student& student, int id, const char* name, double grade) {
    student.id = id;
    strncpy(student.name, name, MAX_SIZE - 1);
    student.name[MAX_SIZE - 1] = '\0'; // Ensure null termination
    student.grade = grade;
}

void cleanUp(Dynamic2DArray& data) {
    for (int course_num = 0; course_num < data.courses; ++course_num) {
        delete[] data.data[course_num].students;
    }
    delete[] data.data;
}
