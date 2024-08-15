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

now we have _prev pointer in the node struct aswell as the _next pointer
and we will also have a tail pointer to the last node in the list


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
void append(Node*& head, int data, Node*& tail);
void printList(const Node* head,Ptr_Type type);
void deleteList(Node*& head);
void enterDataLoop(Node*& head, Node*& tail);
void insertNodeAtStart(Node*& head, Node* newNode);
void enterData(Node*& head, Node*& tail);
// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    Node* tail = nullptr;
    cout << "enterting data recursion\n";
    enterData(head, tail);
    cout << "enterting data with loop\n";
    enterDataLoop(head, tail);


    cout << "Linked List given head (->next): ";
    printList(head,HEAD);
     cout << "Linked List given tail (->prev): ";
    printList(tail,TAIL);

    cout << "last Node data: " << tail->data << endl;
    deleteList(head);
    return 0;
}

// ---------- Functions ----------
void enterData(Node*& head, Node*& tail) {
    int data;
    cout << "Enter a number (-1 to stop): ";
    cin >> data;
    if (data == -1) {
        return;
    }
    append(head, data, tail);
    enterData(head, tail);
}//can also write the same function with a loop
void enterDataLoop(Node*& head, Node*& tail) {
    int data;
    while (true) {
        cout << "Enter a number (-1 to stop): ";
        cin >> data;
        if (data == -1) {
            break;
        }
        append(head, data, tail);
    }
}
Node* createNode(const int data) {
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = nullptr;
    return newNode;
}

void append(Node*& head, int data, Node*& tail) {
    Node* newNode = createNode(data);
    if (head == nullptr) {
        head = newNode;
        tail = newNode; // can also tail = head  
        //for the case of inserting nodes at the start of the 
        //list, this will be the only time we need to update the tail
        //cus the first node will be the last node

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
