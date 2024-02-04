
///////------//lecture 7 - intro to functions//------///////

/*
   
*/

// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section

// prototypes section
void diff(int number1, int number2); // prototype of sum1()
int differnet(int number1, int number2);
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