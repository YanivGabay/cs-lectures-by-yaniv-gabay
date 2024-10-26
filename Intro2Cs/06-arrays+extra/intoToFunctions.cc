
///////------//lecture 6 - intro to functions//------///////

/*
    intro to functions
    void functions doesnt return anything
    void functions are used to do some work
    and not return anything back to the caller
*/

// include section
#include <iostream>
#include <cstdlib>
using ::std::cout;
using ::std::endl;
// const section

// prototypes section
void sum1(); // prototype of sum1()
//using section
using::std::cin;
using::std::cout;
int main()
{

    cout << "program execution starts here\n" ;
        sum1();
    cout << " program execution ends here\n";
        return (EXIT_SUCCESS);
    // num1 and num2 are only known at sum1()
    return 0;
}
//---------------------------------------------
void sum1()
{   //variables here are local
    //outside this scope, noone knows who they are
    int num1, num2;
    cin >> num1 >> num2;
    cout << num1 + num2 << endl;
}