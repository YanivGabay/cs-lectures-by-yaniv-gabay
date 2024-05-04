/********************************************************************
 * Course: Modolar Programming
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
// Add more #include directives as needed
#include <iomanip> 
#include <cstring>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 50;
// Add more constants as needed

// ---------- Structs ----------
struct Student {
    int id;
    char name[MAX_SIZE];
    double grade;
};

struct Dynamic2DArray {
    Student** data;
    int courses;
    int* lines_length;
};

// ---------- Function Prototypes ----------

void initializeStudent(Student& student, int id, const char* name, double grade);
void printAllStudents(const Dynamic2DArray& data) ;
void printStudent(const Student& student);
void initCourse(Dynamic2DArray& data, int course_num);
// ---------- Main Function ----------



int main() {
    int courses, studentsPerCourse;
    std::cout << "Enter the number of courses: ";
    std::cin >> courses;


    Dynamic2DArray data;
    data.courses = courses;
    data.data = new Student*[courses];
    if (data.data == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        return 1;
    }


    for (int course_num = 0; course_num < courses; course_num++)
    {
         initCourse(data, course_num);
    }
    
   printAllStudents(data);
 
    return 0;


}


// ---------- Functions ----------
void printAllStudents(const Dynamic2DArray& data) {
    for (int course_num = 0; course_num < data.courses; course_num++)
    {
        std::cout << "Course " << course_num << " details:" << std::endl;
        for (int student_num = 0; student_num < data.lines_length[course_num]; student_num++)
        {   
            std::cout << "-----------------------------------\n";
            printStudent(data.data[course_num][student_num]);
            
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
    std::cout << "Enter the number of students in course " << course_num+1 << ": ";
    std::cin >> data.lines_length[course_num];
    data.data[course_num] = new Student[data.lines_length[course_num]];

    if (data.data[course_num] == nullptr) {
        std::cerr << "Memory allocation failed" << std::endl;
        exit(1);
    }

    for (int student_num = 0; student_num < data.lines_length[course_num]; student_num++)
    {
        int id;
        char name[MAX_SIZE];
        double grade;
        std::cout << "Enter student " << student_num+1 << " id: ";
        std::cin >> id;
        std::cout << "Enter student " << student_num+1 << " name: ";
        std::cin.ignore(); // ignore the newline character left in the buffer by std::cin >> id;
        // (or use std::cin.get() instead of std::cin.ignore())
        std::cin.getline(name, MAX_SIZE);
        std::cout << "Enter student " << student_num+1 << " grade: ";
        std::cin >> grade;
        initializeStudent(data.data[course_num][student_num], id, name, grade);
    }
}

void initializeStudent(Student& student, int id, const char* name, double grade) {
    student.id = id;
    strcpy(student.name, name);
    student.grade = grade;
}