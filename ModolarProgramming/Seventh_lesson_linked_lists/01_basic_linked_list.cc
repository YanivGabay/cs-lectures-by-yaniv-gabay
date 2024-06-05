/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 07 - Linked Lists
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-05-26
 *
 * Overview:
 * This example demonstrates the basic implementation of a singly linked list
 * in C++. The linked list allows adding elements and printing the list.
 *******************************************************************/

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

// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr

    append(head, 10);
    append(head, 20);
    append(head, 30);

    cout << "Linked List: ";
    printList(head);

    deleteList(head);
    return 0;
}

// ---------- Functions ----------
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
        int jump = current->data;//jump
        Node* after_jumping = jumpByValue(current,jump);
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
}
Node* jumpByValue(Node* current, int jump){
    int abs_value = abs(jump);
    
    for(int i = 0; i < abs_value; i++){
        if(jump < 0)
        current = current->prev;
        else 
        current = current->next;
    }
    return current;
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
