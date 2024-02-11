/*


*/


/*

*/

// include section
#include <iostream>
#include <cstdlib>
#include <ctime> // For time()
#include <cstring>
// using section
using ::std::cout;
using ::std::endl;



// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;

// const section
enum colors_t
{
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WHITE,
    BLACK = 15
};
enum grades_t
{
    A = 90,
    B = 80,
    C = 70,
    D = 60,
    F = 50
};

// prototypes section
colors_t random_color_generator();
void printArray(int arr[], int size);
void compare_grades(int arr[], grades_t grades[], int size);
void take_grades(int arr[], int size);
void printArray(grades_t arr[], int size);
colors_t get_black_color();
//main section
int main()
{
   
   
    //basic enum usage
    colors_t screenColor = colors_t::RED;

    if (screenColor == colors_t::RED)
    {
        cout << "The screen color is red!" << endl;
    }
    else
    {
        cout << "The screen color is not red!" << endl;
    }
    
    colors_t random_color = random_color_generator();

    cout << "Random color is: " << random_color << endl;

    cout << "black color is valued at: " << get_black_color() << endl;
    const int size = 5;
    //lets make an array of enums
    grades_t grades[size] = {};
    int array[size] = {0}; //actual grades
    //empty array
    //input function into int array
    take_grades(array,size);
    //print the array
    printArray(array, size);
    //convert it into grades array
    compare_grades(array, grades, size);
    printArray(grades, size);
    //print the grades array
  return 0;
}

//-------
void compare_grades(int arr[], grades_t grades[], int size) {
    for (int i = 0; i < size; i++) {
        int gradeValue = arr[i];
        if (gradeValue >= grades_t::A) {
            grades[i] = grades_t::A;
        } else if (gradeValue >= grades_t::B) {
            grades[i] = grades_t::B;
        } else if (gradeValue >= grades_t::C) {
            grades[i] = grades_t::C;
        } else if (gradeValue >= grades_t::D) {
            grades[i] = grades_t::D;
        } else {
            grades[i] = grades_t::F;
        }
    }
}


void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}
void printArray(grades_t arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << (arr[i]) << " "; 
    }
    std::cout << std::endl;
}
void take_grades(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << "Enter grade " << i + 1 << ": ";
        cin >> arr[i];
    }
}
colors_t random_color_generator()
{
    return colors_t(rand() % 6);
}
colors_t get_black_color()
{
    return colors_t::BLACK;
}