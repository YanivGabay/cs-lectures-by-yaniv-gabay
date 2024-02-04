
///////------//lecture 7 - intro to functions//------///////

/*
   this examples will show 3 variants of functions:
  # void without return value or modify value inside function
  #  int with return value
   # void with modify value inside function
*/
// its very important to understand this concept, as it will be used in the next lectures


// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section

// prototypes section
void diff(int number1, int number2); // prototype of sum1()
int differnet(int number1, int number2);
void differnet(int number1, int number2,int &result);
// using section
using ::std::cin;
using ::std::cout;
using ::std::endl;
//main section
int main()
{
    int num1, num2, num3;
   std::cin >> num1 >> num2 >> num3;
     std::cout << "--------void functions:-----------" << std::endl;
    diff(num1,num2);
    diff(num2,num3);
    std::cout << "--------return int function:-----------" << std::endl;
    std::cout << differnet(num1,num2) << std::endl;
    int diff = 0;
    std::cout << "--------modify value inside function:-----------" << std::endl;
    differnet(num1,num2,diff);
    std::cout << "diff value after is: " << diff << std::endl;
    return (EXIT_SUCCESS);

}
//---------------------------------------------
void diff(int number1, int number2)
{ // variables here are local
    // outside this scope, noone knows who they are

    cout << number1 - number2 << endl;
}
//---------------------------------------------
int differnet(int number1, int number2)
{ // variables here are local
    // outside this scope, noone knows who they are

   return number1 - number2;
}
//---------------------------------------------
void differnet(int number1, int number2,int &result)
{ // variables here are local
    // outside this scope, noone knows who they are
    result = number1 - number2;
  
}