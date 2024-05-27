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
bool deleteNodeByValue(Node*& head, int value);
// ---------- Main Function ----------
int main() {
    Node* head = nullptr; // Initialize the head of the list to nullptr
    const int numbers[] = { 10, 20, 225, 30 ,-5,-8,53,6,111,-9,0,1,1245153,1,535435, 4535,1,-1,2,4,5,4 };
    cout << "enterting data recursion\n";
    int arrSize = sizeof(numbers) / sizeof(numbers[0]);
    enterDataLoop(head, numbers,arrSize);
    cout << "Linked List: ";
    printList(head,HEAD);
    
    
    //given this linked list
    //build a function that
    // deletes a specific node data
    // k = 225

    if(deleteNodeByValue(head, 225))
    {
        cout << "Linked List after deleting 225: ";
        printList(head,HEAD);
    }
    else
    {
        cout << "225 not found in the list\n";
    }
  
   
    deleteList(head);
    return 0;
}

// ---------- Functions ----------

bool deleteNodeByValue(Node*& head, int value)
{   //we get a not sorted linked list
    //or sorted linked list

    Node* front;
    Node* back;// also called rear
    Node* temp;

    //you should always check if the list is empty
    if(head == nullptr)
    {
        return false;
    }
    //if the head is the value we want to delete
  
    if(checkForHead(head,value))
    {
        return true;
    }


    //if the value is not in the head
    back = head;
    front = head->next;
    while(front!=nullptr)
    {
        if(front->data == value)
        {
            
            temp = front;
            back->next = front->next;
            delete temp;
            return true;
        }
        back = front;
        front = front->next;
    }

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
