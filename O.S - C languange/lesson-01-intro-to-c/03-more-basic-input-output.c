



// to write the same program in c:
//https://en.cppreference.com/w/c/io //IMPORTANT
#include <stdio.h>

int main()
{

   //how do we read strings like we did in cpp?
   //so instead of cin.getline
   // or cin >> setw(size) to get the string with 
   // null terminator
    
    //https://en.cppreference.com/w/c/io
    // 1. gets = not reccomended not safe why?
    // it keep reading chars, no matter the buffer size, till it each /n or \0
    //example:
    char buffer[100];
    //we can also write as pointers: char *buffer = "Hello"; // static string
    printf("Enter a string: ");
    gets(buffer);  // Unsafe usage
    printf("You entered: %s\n", buffer); //%s is the format specifier for char* strings
    //you can see why its unsafe, what happend if we enter a string bigger than 100?
    // so the alternative is fgets

    char buffer_2[100];
    printf("Enter a string: ");
    fgets(buffer_2, sizeof(buffer_2), stdin);  // Safe usage
    printf("You entered: %s", buffer_2); // fgets includes the newline in the input
    //you notice, i gave the fgets the argument stdin
    //so it will take it from the standart input
    //we will also use fgets with files in the next example


    //some of you might ask, how can we read a single char?
    //first option is just to use scanf:
    char ch;
    printf("Enter a character: ");
    scanf("%c", &ch);  // Read a single character
    printf("You entered: %c\n", ch);
    //at this stage, we entered a char, and pressed enter
    // the scanf only read the first char, so the newline
    //is still in the buffer
    //so we "swallow" it
    scanf("%c", &ch);  // Read a single character
    //we can also use getchar function:aaaaa
    char ch_2;
    printf("Enter a character: ");
    ch_2 = getchar();  // Read a single character
    printf("You entered: %c\n", ch_2);


    const char* s = "Hello";
    //printf is very dynamic.

    printf("\t%s\n", s); // \t is a tab , %s character string , \n newline , s is the char* s
    //we can use 10s to print the string with 10 spaces
    printf("%10s\n", s); // Hello
    //we can use -10s to print the string with 10 spaces but left aligned
    printf("%-10s\n", s); // Hello
    //we can use .3s to print the string with 3 characters
    printf("%.3s\n", s); // Hel
    //we can use 10.3s to print the string with 10 spaces and 3 characters
    printf("%10.3s\n", s); //       Hel
    //we can use basic * to print the string with a variable width
    printf("%*s\n", 10, s); // Hello  /// Suddenly we can use any width we want




}