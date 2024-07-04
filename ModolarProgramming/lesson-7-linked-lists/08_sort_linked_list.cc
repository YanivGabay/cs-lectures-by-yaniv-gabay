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



bool checkForHead(Node*& head, int value);
struct Node* sortLinkedList(Node*& head);
void split(Node* head, Node*& list1, Node*& list2);
struct Node* merge(Node* list1, Node* list2);
// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    const int numbers[] = { 10,10,10,10, 20, 225, 30 ,-5,-8,53,6,111,-9,0,1,1245153,1,535435, 4535,1,-1,2,4,5,4 };
    cout << "enterting data recursion\n";
    int arrSize = sizeof(numbers) / sizeof(numbers[0]);
    enterDataLoop(head, numbers,arrSize);
    cout << "Linked List: ";
    printList(head,HEAD);
    head = sortLinkedList(head);
    cout << "Linked List after sorting: ";
    printList(head,HEAD);
    
 

   
   
    deleteList(head);
    return 0;
}

// ---------- Functions ----------
struct Node* sortLinkedList(Node*& head)
{
  Node *list1 = nullptr;
    Node *list2 = nullptr;
    Node *list3 = nullptr;

    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }
    split(head, list1, list2);
    list1 = sortLinkedList(list1);
    list2 = sortLinkedList(list2);
    list3 = merge(list1, list2);
    return list3;
  
}
void split(Node* head, Node*& list1, Node*& list2)
{
    bool toggle = true;
    Node* temp;

    while (head != nullptr)
    {
        temp = head;
        head = head->next;
        temp->next = nullptr;

        if (toggle)
        {
            insertNodeAtStart(list1, temp);
        }
        else
        {
            insertNodeAtStart(list2, temp);
        }
        toggle = !toggle;
    }
}
struct Node* merge(Node* list1, Node* list2)
{
    Node* head = nullptr;
    Node* last = nullptr;
    Node* temp = nullptr;
    
    while (list1 != nullptr && list2 != nullptr)
    {
        if (list1->data <= list2->data)
        {
            temp = list1;
            list1 = list1->next;
        }
        else
        {
            temp = list2;
            list2 = list2->next;
        }
        temp->next = nullptr;
        if (head == nullptr)
        {
            head = temp;
            last = temp;
        }
        else
        {
            last->next = temp;
            last = temp;
        }
       
    }
    if(list1 == nullptr)
    {
        last->next = list2;
    }
    else
    {
        last->next = list1;
    }
 
    

    return head;
}
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
