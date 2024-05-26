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

We will see here, that our division into functions, make the code very very simple.
i really recommended to divide your linked list code into functions, it will make your code much more readable and maintainable.
We can already write the actions  we need:

1. create a head pointer = nullptr
2. some read of data into the nodes
3. dyanmic allocation of a node and values to it (keep the pointer!!!)
4. location of the node (if head is nullptr, then the head is the new node, 
                    else we need to decide our logic (start,end,sorted, etc..) )
5. back to step 2 until we want to stop
6. dont forget to delete all allocated memory


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
    Node* prev;
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

// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    const int numbers[] = { 10, 20, 225, 30 ,-5,8,53,6,111,-9,0,1,1245153,1,535435, 4535,1,-1,2,4,5,4 };
    cout << "enterting data recursion\n";
    int arrSize = sizeof(numbers) / sizeof(numbers[0]);
    enterDataLoop(head, numbers,arrSize);
    cout << "Linked List: ";
    printList(head,HEAD);
    
    //now lets ask ourselves questions:
    //1. how can we print the list from the end to the start?
    //2. find the max value in the list
    //3. search for two negative numbers in a row

    //1:
    cout << "Linked List from the end: ";
    reverseList(&head);
    printList(head,HEAD);
    //two ways to do it:
    //a. we can go through the list and save the last node
    //b. we 

  
   
    deleteList(head);
    return 0;
}

// ---------- Functions ----------

//question 1:
//we will just reverse the list and print it
// 3 ptrs: previous, current, next
void reverseList(Node** head_ref) {
    Node* previous = nullptr;      // To keep track of the previous element, initially null
    Node* current = *head_ref;     // Start with the head of the list
    Node* next = nullptr;          // To store the next node

    while (current != nullptr) {   // Traverse the list
        next = current->next;      // Store next node
        current->next = previous;  // Reverse current node's pointer
        previous = current;        // Move pointers one position ahead.
        current = next;
    }
    *head_ref = previous;          // Update the head pointer to the last element
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
    //before we change the head, we need to update the prev pointer of the current head
    head->prev = newNode;
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
        else
        {
            current = current->prev;
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
