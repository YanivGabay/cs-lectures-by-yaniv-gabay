/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 07 - Linked Lists
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-26
 *
 *
 *******************************************************************/



/*

Given a linked list, create a function that will remove all duplicates from the list.
for example:
Linked List: 
10 20 225 30 -5 -8 53 6 111 -9 0 1 1245153 1 535435 4535 1 -1 2 4 5 4
Linked List after removing duplicates: 
10 20 225 30 -5 -8 53 6 111 -9 0 1 1245153 535435 4535 -1 2 4 5

// so the function should remove all the duplicates from the linked list
// the "fastest" way to do this is to use a hash table
// but you have not learned about hash tables yet (or stls)
// so you will have to do this the "slow" way
// by comparing each node to all the other nodes in the list
// and removing the duplicates
// you can do this by using a nested loop
// the outer loop will iterate over each node in the list
// the inner loop will iterate over each node in the list



*/


// ---------- Include Section ----------
#include <iostream>

// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Structs ----------
struct Node {
    int data;
    Node* next;
   
};


enum Ptr_Type {
    HEAD,
    TAIL
};

// ---------- Function Prototypes ----------
Node* createNode(int data);
void append(Node*& head, int data);
void printList(const Node* head,Ptr_Type type);
void deleteList(Node*& head);
void enterDataLoop(Node*& head,const int* numbers, int arrSize);
void insertNodeAtStart(Node*& head, Node* newNode);


void removeDuplicates(Node*& head);
bool checkForHead(Node*& head, int value);

// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    const int numbers[] = { 10,10,10,10, 20, 225, 30 ,-5,-8,53,6,111,-9,0,1,1245153,1,535435, 4535,1,-1,2,4,5,4 };
    cout << "enterting data recursion\n";
    int arrSize = sizeof(numbers) / sizeof(numbers[0]);
    enterDataLoop(head, numbers,arrSize);
    cout << "Linked List: ";
    printList(head,HEAD);
    removeDuplicates(head);
    cout << "Linked List after removing duplicates: ";
    printList(head,HEAD);
    
 

   
   
    deleteList(head);
    return 0;
}

// ---------- Functions ----------
void removeDuplicates(Node*& head)
{
    Node* current = head;
    while(current != nullptr)
    {
        Node* temp = current;
        while(temp->next != nullptr)
        {
            if(current->data == temp->next->data)
            {
                Node* duplicate = temp->next;
                temp->next = temp->next->next;
                delete duplicate;
            }
            else
            {
                temp = temp->next;
            }
        }
        current = current->next;
    }
}
//this is a function
bool checkForHead(Node*& head, int value)
{
    if(head->data == value)
    {
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }
    return false;
}

void enterDataLoop(Node*& head,const int* numbers, int arrSize) {
   
    for (int i = 0; i < arrSize; i++) {
        append(head, numbers[i]);
    }
}
Node* createNode(const int data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = nullptr;
    return newNode;
}

void append(Node*& head, int data) {
    Node* newNode = createNode(data);
    if (head == nullptr) {
        head = newNode;
        


    } else {
        //so if we want to add a new node at
        //the start
        //this is the only part of the code that needs to be changed.

    //OLD CODE:
       // Node* current = head;
      //  while (current->next != nullptr) {
        //    current = current->next;
        //}
        //current->next = newNode;
    //NEW CODE:
        insertNodeAtStart(head, newNode);
    }
}
void insertNodeAtStart(Node*& head, Node* newNode) {
    
    newNode->next = head;

    head = newNode;
}
void printList(const Node* ptr,Ptr_Type type) {
    const Node* current = ptr;
    while (current != nullptr) {
        cout << current->data << " ";

        if (type == HEAD)
        {
            current = current->next;
        }
      
       
    }
    cout << endl;
}

void deleteList(Node*& head) {
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}
