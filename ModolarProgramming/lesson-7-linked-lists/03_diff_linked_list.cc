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
};

// ---------- Function Prototypes ----------
Node* createNode(int data);
void append(Node*& head, int data);
void printList(const Node* head);
void deleteList(Node*& head);
void enterDataLoop(Node*& head);
void insertNodeAtStart(Node*& head, Node* newNode);
void enterData(Node*& head);
// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    cout << "enterting data recursion\n";
    enterData(head);
    cout << "enterting data with loop\n";
    enterDataLoop(head);
    cout << "Linked List: ";
    printList(head);

    deleteList(head);
    return 0;
}

// ---------- Functions ----------
void enterData(Node*& head) {
    int data;
    cout << "Enter a number (-1 to stop): ";
    cin >> data;
    if (data == -1) {
        return;
    }
    append(head, data);
    enterData(head);
}//can also write the same function with a loop
void enterDataLoop(Node*& head) {
    int data;
    while (true) {
        cout << "Enter a number (-1 to stop): ";
        cin >> data;
        if (data == -1) {
            break;
        }
        append(head, data);
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
void printList(const Node* head) {
    const Node* current = head;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
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
