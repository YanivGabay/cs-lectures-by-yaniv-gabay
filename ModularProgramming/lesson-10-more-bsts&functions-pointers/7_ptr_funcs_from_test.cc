/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-25
 *
 * Overview:
 * 
    
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
    struct Node* next;
    struct Node* prev;
};

// ---------- Function Prototypes ----------
bool isPrime(int num);
bool isPositive(int num);
Node* getNext(Node* current);
Node* getPrev(Node* current);
Node* searchList(Node* start_position, bool (*condition)(int),Node* (*next)(Node*));
void enterData(Node*& head, Node*& tail) ; 
void insertNodeAtStart(Node*& head, Node* newNode);
Node* createNode(const int data) ;
void append(Node*& head, int data, Node*& tail);
void printData(Node* head);
int main() {

    Node* head = nullptr; // Initialize the head of the list to nullptr
    Node* tail = nullptr;
    cout << "enterting data recursion\n";
    enterData(head, tail);
    cout << "enterting data with loop\n";
    printData(head);


    //call the function with the starting position, the condition, and the next function
    Node* wanted = searchList(tail, isPrime, getPrev);
    if (wanted != NULL) {
        cout << "Found " << wanted->data << endl;
    } else {
        cout << "Not found" << endl;
    }



    return 0;
}

// ---------- Functions ----------
void printData(Node* head)
{
    Node* current = head;
    while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;

}
void insertNodeAtStart(Node*& head, Node* newNode)
 {
    newNode->next = head;
    //before we change the head, we need to update the prev pointer of the current head
    head->prev = newNode;
    head = newNode;
}
Node* createNode(const int data) 
{
    Node* newNode = new Node;
    newNode->data = data;
    newNode->next = nullptr;
    return newNode;
}
void append(Node*& head, int data, Node*& tail)
 {
    Node* newNode = createNode(data);
    if (head == nullptr) {
        head = newNode;
        tail = newNode; // can also tail = head  
     

    } else {
       
        insertNodeAtStart(head, newNode);
    }
}

void enterData(Node*& head, Node*& tail) {
    int data;
    cout << "Enter a number (-1 to stop): ";
    cin >> data;
    if (data == -1) {
        return;
    }
    append(head, data, tail);
    enterData(head, tail);

}
Node* searchList(Node* start_position, bool (*condition)(int),Node* (*next)(Node*)) {
    Node* node = start_position;
    while (node != NULL) {
        if (condition(node->data)) {
            printf("Found %d\n", node->data);
            return node;
        }
        node = next(node);
        
    }
    return NULL;
}


bool isPrime(int num) {
    if (num <= 1) 
    return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) 
        return false;
    }
    return true;
}

bool isPositive(int num) {
    return num > 0;
}

// Function to move to the next node
Node* getNext(Node* current) {
    return current->next;
}

// Function to move to the previous node
Node* getPrev(Node* current) {
    return current->prev;
}