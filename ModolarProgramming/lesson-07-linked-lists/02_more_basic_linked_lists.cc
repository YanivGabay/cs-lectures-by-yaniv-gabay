/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 07 - Linked Lists
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-26
 *

 *******************************************************************/

/*
how to insert data into a linked list with recursion and with a loop
from the user

*/

// this program will show how to insert data into a linked list with recursion and with a loop
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
        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
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
