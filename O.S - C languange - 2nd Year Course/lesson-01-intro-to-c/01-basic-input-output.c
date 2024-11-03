






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
    // Example 1: Parsing an integer
    char input1[] = "42";
    int num;
    sscanf(input1, "%d", &num);
    printf("Parsed integer: %d\n", num);

    // Example 2: Parsing a floating-point value
    char input2[] = "3.14";
    float fnum;
    sscanf(input2, "%f", &fnum);
    printf("Parsed float: %f\n", fnum);

    // Example 3: Parsing a string of at most 9 non-whitespace characters
    // the non-whitespace only check at start for some reason
    char input3[] = "       HelloWorld";
    char input3_a[] = "Hello         World";
    char input3_b[] = "   a b c d e     d   g";
    char str2[10];
    char str2_a[10];
    char str2_b[10];
    sscanf(input3, "%9s", str2);
    printf("Parsed string: |%s|\n", str2);
    sscanf(input3_a, "%9s", str2_a);
    printf("Parsed string: |%s|\n", str2_a);
    sscanf(input3_b, "%9s", str2_b);
    printf("Parsed string: |%s|\n", str2_b);

    // Example 4: Parsing a two-digit integer
    char input4[] = "99";
    int twoDigitNum;
    sscanf(input4, "%2d", &twoDigitNum);
    printf("Parsed two-digit integer: %d\n", twoDigitNum);

    // Example 5: Parsing a floating-point value with scientific notation
    char input5[] = "1.23E4";
    float sciNum;
    sscanf(input5, "%f", &sciNum);
    printf("Parsed scientific float: %f\n", sciNum);

    // Example 6: Parsing a string of at most 3 decimal digits
    char input6[] = "123456";
    char str3[4];
    sscanf(input6, "%3[0-9]", str3);
    printf("Parsed 3-digit string: %s\n", str3);
    //this will stop at first non digit character 
    char input6_a[] = "12a3456";
    char str3_a[4];
    sscanf(input6_a, "%3[0-9]", str3_a);
    printf("Parsed 3-digit string: %s\n", str3_a);


    // Example 7: Parsing multiple fields
    // Harder example:
    int i, j;
    float x, y;
    char str1[10];
    char input[] = "25 54.32E-1 Thompson 56789";
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