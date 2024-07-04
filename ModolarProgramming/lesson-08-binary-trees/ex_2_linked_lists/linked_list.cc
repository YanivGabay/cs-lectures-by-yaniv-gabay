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
struct Node
{
    int data;
    Node *next;
    Node *down;
};

enum Ptr_Type
{
    HEAD,
    TAIL
};

int const VALUE_OF_HEADS = 0;
// ---------- Function Prototypes ----------
Node *createNode(int data);
void append(Node *&head, int data);
void printList(const Node *head, Ptr_Type type);
void deleteList(Node *&head);
void enterDataLoop(Node *&head);

// ---------- Main Function ----------
int main()
{
    Node *head_of_heads = nullptr; // Initialize the head of the list to nullptr

    // we need to do

    
    return 0;
}

// ---------- Functions ----------
void enterRowAmount(Node *&head_of_heads)
{
    while (true)
    {

        int rowAmount;
        cout << "Enter the amount of values in the current row:";
        cin >> rowAmount;

        if(rowAmount == 0)
        return;

        for (int i = 0; i < rowAmount; i++)
        {
            enterDataLoop(head_of_heads, rowAmount);
        }
    }
}

void enterDataLoop(Node *&head_of_heads, int rowAmount)
{
    if(head_of_heads == nullptr)
    {
        head_of_heads = createNode(VALUE_OF_HEADS);
    }
    else
    {
        Node *current = head_of_heads;
        while (current->down != nullptr)
        {
            current = current->down;
        }
        current->down = createNode(VALUE_OF_HEADS);
        
    }
    
    Node *actual_list = nullptr;
    for (int i = 0; i < rowAmount; i++)
    {
        int data;
        cout << "Enter a number: ";
        cin >> data;
        append(actual_list, data);
    }
    head_of_heads->next = actual_list;
    
}

Node *createNode(const int data)
{
    Node *newNode = new Node;
    newNode->data = data;
    newNode->next = nullptr;
    newNode->down = nullptr;
    return newNode;
}

void append(Node *&head, int data)
{
    Node *newNode = createNode(data);
    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        // so if we want to add a new node at
        // the start
        // this is the only part of the code that needs to be changed.

        // OLD CODE:
        Node *current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newNode;
        // NEW CODE:
        //  insertNodeAtStart(head, newNode);
    }
}

void printList(const Node *ptr, Ptr_Type type)
{
    const Node *current = ptr;
    while (current != nullptr)
    {
        cout << current->data << " ";

        if (type == HEAD)
        {
            current = current->next;
        }
    }
    cout << endl;
}

void deleteList(Node *&head)
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp;
    }
}
