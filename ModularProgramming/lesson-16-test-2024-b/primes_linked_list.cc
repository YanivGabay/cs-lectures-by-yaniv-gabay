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

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
//example n
const int N = 20;
// ---------- Structs ----------
struct Prime_node {
    int prime_num;
    Prime_node *next;
};
struct Num_node {
    int num;
    Num_node *next;
    Prime_node *dividers[N];
};


// ---------- Function Prototypes ----------
void add_nums_to_list(Prime_node* prime_head, Num_node*& num_head,const int to_be_added);
Prime_node* allocatePrimeNode(const int prime_num);
bool is_prime(const int num);
int* get_dividers(const int num, int& size_array);
Num_node* allocateNode(const int to_be_added);


// ---------- Main Function ----------
int main() {
    
    //we are given two heads of the linked lists
    Prime_node* prime_head ;
    Num_node* num_head ;
    int to_be_added = 15; // example value to be added
    add_nums_to_list(prime_head, num_head,to_be_added);
    return 0;
}

// ---------- Functions ----------
void add_nums_to_list(Prime_node* prime_head, Num_node*& num_head,const int to_be_added){
    //first we add the new number to the head of the nums list
    Num_node* new_num = allocateNode(to_be_added);
    //now connect the new number to the list
    if (num_head == nullptr){
        num_head = new_num;
    }
    else {
        new_num->next = num_head;
        num_head = new_num;
    }
    //at this point we want to get all the prime dividers of the new number
    int size_array = 0;
    int* divisors = get_dividers(to_be_added, size_array);
    //now we have all the divisors of the number
    //now only need to connect the prime divisors to the prime list
    //the prime list is ordered in ascending order
    //so we will keep two pointers to the prime list
    Prime_node* current = prime_head;
    Prime_node* prev = nullptr;

    for(int i = 0; i < size_array||divisors[i]==0; i++){

        if((divisors[i]==current->prime_num))
        {
            new_num->dividers[i] = current;
            prev = current;
            current = current->next;

        }
        else if(divisors[i] < current->prime_num){
            Prime_node* new_prime = allocatePrimeNode(divisors[i]);
            //check alloc
            new_num->dividers[i] = new_prime;
            new_prime->next = current;
            if(prev == nullptr){
                prime_head = new_prime;
            }
            else{
                prev->next = new_prime;
            }
            prev = new_prime;
            current = new_prime->next;
        }
        else //if the prime we need to add it the biggest prime in the list
        {
            Prime_node* new_prime = allocatePrimeNode(divisors[i]);
            //check alloc
            new_num->dividers[i] = new_prime;
            current->next = new_prime;
        }
       

    }


}
Prime_node* allocatePrimeNode(const int prime_num){
    Prime_node* new_prime = new Prime_node;
    //check alloc
    new_prime->prime_num = prime_num;
    new_prime->next = nullptr;
    return new_prime;
}
bool is_prime(const int num){
    if(num < 2){
        return false;
    }
    for(int i = 2; i <= sqrt(num); i++){
        if(num % i == 0){
            return false;
        }
    }
    return true;
}
int* get_dividers(const int num, int& size_array){
    int* dividers = new int[sqrt(num)+1];//max number of dividers
    memset(dividers, 0, sizeof(int)*sqrt(num)+1); //initialize the array
    //check alloc
    int index = 0;

    for(int i = 2; i <= sqrt(num); i++){
        if(num % i == 0){
            dividers[index] = i;
            index++;
        }
    }
    //check if the number itself is a prime
    //we have a function to check if a number is prime
    if(is_prime(num)){
        dividers[index] = num;
        index++;
    }
  
}


Num_node* allocateNode(const int to_be_added){
    Num_node* new_node = new Num_node;
    //check alloc
    new_node->num = to_be_added;
    new_node->next = nullptr;
    for(int i = 0; i < N; i++){
        new_node->dividers[i] = nullptr;
    }
    return new_node;
}
