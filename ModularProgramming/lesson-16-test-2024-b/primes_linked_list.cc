/********************************************************************
 * Course: Course Name
 * Lecture: Lecture Number - Lecture Title
 * File: question2.cc
 *
 * Author: Your Name
 * Date: 2024-08-CURRENT_DAY
 *
 * Overview:
 * Brief description of the file's purpose.
 *
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <cmath>
#include <cstring>
// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------
// example n
const int N = 20;
// ---------- Structs ----------
struct Prime_node
{
    int prime_num;
    Prime_node *next;
};
struct Num_node
{
    int num;
    Num_node *next;
    Prime_node *dividers[N];
};

// ---------- Function Prototypes ----------
void add_nums_to_list(Prime_node *&prime_head, Num_node *&num_head, const int to_be_added);
Prime_node *allocatePrimeNode(const int prime_num);
bool is_prime(const int num);
int *get_dividers(const int num, int &size_array);
Num_node *allocateNode(const int to_be_added);
void print_num_list(Num_node *num_head);
// ---------- Main Function ----------
int main()
{

    // we are given two heads of the linked lists
    Prime_node *prime_head;
    Num_node *num_head;
    //lets create asimple example
    prime_head = allocatePrimeNode(2);
    prime_head->next = allocatePrimeNode(3);
    prime_head->next->next = allocatePrimeNode(5);
    //prime list is 2->3->5
    num_head = allocateNode(10);
    num_head->dividers[0] = prime_head;
    num_head->dividers[1] = prime_head->next->next;
    num_head->next = allocateNode(20);
    num_head->next->dividers[0] = prime_head;
    num_head->next->dividers[1] = prime_head->next->next;
    num_head->next->next = allocateNode(5);
    num_head->next->next->dividers[0] = prime_head->next->next;
    //num list is 10->20->5 
    // where 10 has dividers 2 and 5
    // 20 has dividers 2 and 5
    // 5 has divider 5
    print_num_list(num_head);

    int to_be_added = 17; // example value to be added
    add_nums_to_list(prime_head, num_head, to_be_added);

    print_num_list(num_head);
    return 0;
}

// ---------- Functions ----------
void add_nums_to_list(Prime_node *&prime_head, Num_node *&num_head, const int to_be_added)
{
    // first we add the new number to the head of the nums list
    Num_node *new_num = allocateNode(to_be_added);
    // now connect the new number to the list
    if (num_head == nullptr)
    {
        num_head = new_num;
    }
    else
    {
        new_num->next = num_head;
        num_head = new_num;
    }
    // at this point we want to get all the prime dividers of the new number
    int size_array = 0;
    int *prime_divisors = get_dividers(to_be_added, size_array);
    // size_array is the number of divisors

    // now we have all the divisors of the number
    // now only need to connect the prime divisors to the prime list
    // the prime list is ordered in ascending order
    // so we will keep two pointers to the prime list
    Prime_node *current = prime_head;
    Prime_node *prev = nullptr;
    cout << "size_array: " << size_array << endl;
    cout << "prime divisors: ";
    int index = 0;
    for (int i = 0; i < size_array; i++)
    {
        cout << "current prime: " << prime_divisors[i] << endl;
        // we run on the prime_divisors array
        // the linked list is ordered in ascending order
        // size_array is the actual prime divisors we have

        // we can have three cases
        // 1. new primer is the smallest
        // 2. new primer is the biggest
        // 3. new primer is between two primes
        // we will handle each case
        // our prime_divisors array is also ordered in ascending order

        if (prev == nullptr && prime_divisors[i] < current->prime_num)
        {
            // case 1
            Prime_node *new_prime = allocatePrimeNode(prime_divisors[i]);
            // check alloc
            // connect the new prime to the head
            new_prime->next = current;
            // update the head
            prime_head = new_prime;
            // prev is now points to head
            prev = prime_head;
            // dont forget to update the dividers array
            // has its own index
            new_num->dividers[index++] = new_prime;
        }

        else if (prev !=nullptr &&prime_divisors[i] > prev->prime_num && prime_divisors[i] < current->prime_num)
        {
            // case 2
            Prime_node *new_prime = allocatePrimeNode(prime_divisors[i]);
            new_prime->next = current;
            prev->next = new_prime;
            prev = current;
            current = current->next;
            new_num->dividers[index++] = new_prime;
        }
        else if (current->next == nullptr && prime_divisors[i] > current->prime_num)
        {
            // case 3 //curr is the last prime in the list
            Prime_node *new_prime = allocatePrimeNode(prime_divisors[i]);
            current->next = new_prime;
            new_num->dividers[index++] = new_prime;

        }
        else if (prime_divisors[i] == current->prime_num)
        {
            // we dont need to add the same prime twice
            //but we need to update the dividers array
            new_num->dividers[index++] = current;
            continue;
        }
        else
        {
            // we need to move the pointers
            prev = current;
            current = current->next;
            i--; // we need to check the same prime again
        }
    }
}
Prime_node *allocatePrimeNode(const int prime_num)
{
    Prime_node *new_prime = new Prime_node;
    // check alloc
    new_prime->prime_num = prime_num;
    new_prime->next = nullptr;
    return new_prime;
}
bool is_prime(const int num)
{
    if (num < 2)
    {
        return false;
    }
    for (int i = 2; i <= sqrt(num); i++)
    {
        if (num % i == 0)
        {
            return false;
        }
    }
    return true;
}
int *get_dividers(const int num, int &size_array)
{
    int max_dividers = sqrt(num) + 1;
    int *dividers = new int[max_dividers];           // max number of dividers
    memset(dividers, 0, sizeof(int) * max_dividers); // initialize the array
    // check alloc
    int index = 0;

    for (int i = 2; i <= num / 2; i++)
    { // run until half of the number
        if (num % i == 0 && is_prime(i))
        { // only if i divides num and is prime
            dividers[index] = i;
            index++;
        }
    }
    // check if the number itself is a prime
    // we have a function to check if a number is prime
    if (is_prime(num))
    {
        dividers[index] = num;
        index++;
    }
    //jut a printing loop, not needed
    for (int i = 0; i < max_dividers; i++)
    {
        
        if (dividers[i] == 0)
        {
            break;
        }
        cout << "dividers from get dividers: " << dividers[i] << endl;
    }
    size_array = index;
    return dividers;
}

Num_node *allocateNode(const int to_be_added)
{
    Num_node *new_node = new Num_node;
    // check alloc
    new_node->num = to_be_added;
    new_node->next = nullptr;
    for (int i = 0; i < N; i++)
    {
        new_node->dividers[i] = nullptr;
    }
    return new_node;
}

void print_num_list(Num_node *num_head)
{
    Num_node *current = num_head;
    while (current != nullptr)
    {
        cout << "Number: " << current->num << " Dividers: ";
        for (int i = 0; i < N; i++)
        {
            if (current->dividers[i] != nullptr)
            {
                cout << current->dividers[i]->prime_num << " ";
            }
        }
        cout << endl;
        current = current->next;
    }
}