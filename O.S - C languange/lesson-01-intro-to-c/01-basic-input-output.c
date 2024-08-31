






//differences  between c and cpp
/*

#include <iostream>

int main() {
    int number;
    std::cout << "Enter a number: ";
    std::cin >> number;
    std::cout << "You entered: " << number << std::endl;
    return 0;
}


*/

// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>
//https://en.cppreference.com/w/c/io/fscanf
int main()
{
    int number;
    printf("Enter a number: ");
    scanf("%d", &number);
    // there is also fscanf() function in c
    //check out more in the link
    printf("You entered: %d\n", number);



    //harder example:
    int i, j;
    float x, y;
    char str1[10];

     char input[] = "25 54.32E-1 Thompson 56789";
    /* parse as follows:
       %d: an integer
       %f: a floating-point value
       %9s: a string of at most 9 non-whitespace characters
       %2d: two-digit integer (digits 5 and 6)
       %f:  a floating-point value (digits 7, 8, 9)
       %*d: an integer which isn't stored anywhere
       ' ': all consecutive whitespace
       %3[0-9]: a string of at most 3 decimal digits (digits 5 and 6)
      */
    int ret = sscanf(input, "%d%f%9s%2d%f%*d",
                     &i, &x, str1, &j, &y);
 
    printf("Converted %d fields:\n"
           "i = %d\n"
           "x = %f\n"
           "str1 = %s\n"
           "j = %d\n"
           "y = %f\n",
           
           ret, i, x, str1, j, y);

    return 0;
}