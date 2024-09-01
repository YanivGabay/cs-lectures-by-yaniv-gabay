/********************************************************************
 * Course: Course Name
 * Lecture: Lecture Number - Lecture Title
 * File: question2.cc
 * 
 * Author: Your Name
 * Date: 2024-08-CURRENT_DAY
 * 
 * Overview:
 * Brief description of the file's purpose.
 * 
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/






// basicly in a binary search tree, given a k
// we need to return the kth smallest
// so if k=0, we return the smallest element
// if k=1, we return the second smallest element
// ...

//so you should think INORDER TRAVERSAL
// why? because inorder traversal of a binary search tree is sorted, from smallest to largest
// so if we do an inorder traversal and keep track of the number of nodes we have visited
// we can return the kth node we visit

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------

// ---------- Structs ----------
struct Node {
    int _data;
    struct Node *_next = nullptr;
    struct Node **_dividing = nullptr;

    //## I ADDED THIS JUST FOR THE PRINT FUNCTION
    //## TO MAKE IT EASIER TO PRINT THE DIVIDING VALUES
    //## YOU CAN REMOVE IT
    int _dividing_size = 0;
};

// ---------- Function Prototypes ----------
void add_new_value(const int data, Node *head);
Node* allocate_new_node(const int data);
void find_last_and_count_dividers(Node *head, const int data, Node *&last, int &count_dividers);
void connect_new_node_to_list(Node *&last, Node *new_node);
void connect_new_node_to_dividers(Node *head, Node *last, Node *new_node, const int data, int count_dividers);
void print_list_with_dividing(Node *head);


// ---------- Main Function ----------
int main() {
    // we are given a linked list
    // where each value, has an array of ptrs to the values it itself divide
    // for example:
    // 10 -> 20 ->12 ->5 -> 2 -> NULL;
    // 2 will be connected via the **_dividing to 10, 20 , 12 cus its dividing them
    // 5 will be connected via the **_dividing to 10, 20, cus its dividing them
    // lets build the example list

    //######### JUST A PRIMITIVE WAY TO BUILD THE EXAMPLE LIST #########
    Node *head = new Node;
    head->_data = 10;
    head->_next = new Node;
    head->_next->_data = 20;
    head->_next->_next = new Node;
    head->_next->_next->_data = 12;
    head->_next->_next->_next = new Node;
    head->_next->_next->_next->_data = 5;
    head->_next->_next->_next->_next = new Node;
    head->_next->_next->_next->_next->_data = 2;

    // now lets connect the dividing
    head->_next->_next->_next->_dividing = new Node*[2];
    head->_next->_next->_next->_dividing_size = 2;
    head->_next->_next->_next->_dividing[0] = head;
    head->_next->_next->_next->_dividing[1] = head->_next;
    head->_next->_next->_next->_next->_dividing = new Node*[3];
    head->_next->_next->_next->_next->_dividing_size = 3;
    head->_next->_next->_next->_next->_dividing[0] = head;
    head->_next->_next->_next->_next->_dividing[1] = head->_next;
    head->_next->_next->_next->_next->_dividing[2] = head->_next->_next;
    // ### END OF BUILDING THE EXAMPLE LIST ###

    //we need to create a function that add a new node to the list (end of it)
    add_new_value(3, head); // 3 should be connected to 12 

    print_list_with_dividing(head);
    cout << "End of the program" << endl;
    return 0;
}

// ---------- Functions ----------

// Function to allocate a new node
Node* allocate_new_node(const int data) {
    Node *new_node = new Node;
    if (new_node == nullptr) {
        return nullptr;
    }
    new_node->_data = data;
    new_node->_next = nullptr;
    new_node->_dividing = nullptr;
    return new_node;
}

// Function to find the last node and count dividers
void find_last_and_count_dividers(Node *head, const int data, Node *&last, int &count_dividers) {
    if (head == nullptr) {
        return;
    }
    cout << "Finding last node and counting dividers" << endl;
    count_dividers = 0;
    last = head;  // Initialize last to head
    while (last->_next != nullptr) {  // Traverse till the actual last node
        cout << "Checking if " << data << " divides " << last->_data << endl;
        if ( last->_data%data  == 0) {  // Check if current node's data divides the new data
            count_dividers++;
        }
        last = last->_next;  // Move to the next node
    }
    // Check division for the actual last node
    if (data % last->_data == 0) {
        count_dividers++;
    }
    cout << "Last node found: " << last->_data << endl;
}

// Function to connect the new node to the list
void connect_new_node_to_list(Node *&last, Node *new_node) {
    if (last != nullptr) {
        last->_next = new_node;  // Append new node to the end of the list
    }
    last = new_node;  // Update last to new node
}


// Function to connect the new node to the nodes it divides
void connect_new_node_to_dividers(Node *head, Node *last, Node *new_node, const int data, int count_dividers) {
    if (count_dividers == 0) {
        return;
    }

    new_node->_dividing = new Node*[count_dividers]; 
    if (new_node->_dividing == nullptr) {
        return;
    }
    // this part is just for printing function its not needed
    new_node->_dividing_size = count_dividers;

    cout << "Connecting new node to " << count_dividers << " dividers" << endl;

    Node *current = head;
    int index = 0;
    while (current != last && index < count_dividers) {
        if (current->_data % data == 0) {
            new_node->_dividing[index] = current;
            index++;
        }
        current = current->_next;
    }
}

// Main function to add a new value
void add_new_value(const int data, Node *head) {
    if (head == nullptr) {
        return;
    }
    cout << "Adding new value: " << data << endl;

    Node *new_node = allocate_new_node(data);
    if (new_node == nullptr) {
        return;
    }
    cout << "New node allocated" << endl;

    Node *last = nullptr;
    int count_dividers = 0;
    find_last_and_count_dividers(head, data, last, count_dividers);
    cout << "Number of dividers: " << count_dividers << endl;
    connect_new_node_to_list(last, new_node);
    connect_new_node_to_dividers(head, last, new_node, data, count_dividers);
}

// Function to print the list with dividers
void print_list_with_dividing(Node *head) {
    Node *current = head;
    while (current != nullptr) {
        cout << current->_data << " -> ";
        if (current->_dividing != nullptr) {
            for (int i = 0; i < current->_dividing_size; i++) {
                cout << "the value " << current->_data << " divides " << current->_dividing[i]->_data << "| ";
            }
        }
        cout << endl;
        current = current->_next;
    }
}