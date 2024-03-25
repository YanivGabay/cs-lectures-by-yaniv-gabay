/********************************************************************
 * Course: Modolar Programming in C++
 * Lecture: 1 - Structs
 * File: Structs.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-03-25
 * 
 * Overview:
 * Show some examples of using files
 * 
 * Notes:
 * 
 *******************************************************************/





// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------

struct Point {
    double _x;
    double _y;
};

struct Circle {
    Point _center;
    double _radius;
};

// Add more structs as needed

// ---------- Function Prototypes ----------
void insert_values(struct Point& point);


// ---------- Main Function ----------
int main() {
    struct Point mypoint;
    
    struct Point p1 = {1,2};
    struct Point p2 = {3,4};
    struct Circle c1 = {p1,5};

    c1._center._x = 10;

    cout << "p1 x:" << p1._x << endl;
    

    cout << "p1: (" << p1._x << "," << p1._y << ")" << endl;
    cout << "p2: (" << p2._x << "," << p2._y << ")" << endl;
    cout << "c1 center: (" << c1._center._x << "," << c1._center._y << ")" << "radius:" << c1._radius << endl;

    struct Point new_point;
    insert_values(new_point);
    cout << "new_point: (" << new_point._x << "," << new_point._y << ")" << endl;

    return 0;
}

// ---------- Functions ----------
void insert_values(struct Point& point)
{
    cin >> point._x;
    cin >> point._y;

}

//if we just pass a struct we create a copy
void insert_values_copy(struct Point point)
{
    ;
}
//if we want to send an the struct, without creating 
// a copy, and without changing the original struct
// we can use const
void insert_values_const(const struct Point& point)
{
    ;
}
