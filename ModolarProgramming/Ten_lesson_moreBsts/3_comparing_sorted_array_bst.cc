/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:
 * 
 * 
 * 
 * 




binary search tree, is a sorted binary tree, we will compare it
with a sorted array, againt the actions of:
inserting, deleting, searching, and displaying the tree.


Sorted array:
Inserting: O(n) - why? because we need to shift all the elements to the right.
Deleting: O(n) - why? because we need to shift all the elements to the left.
Finding: O(log(n)) - why? because we can use binary search.

Binary search tree:
Inserting: O(log(n)) - O(n) - why? because in the worst case, the tree will be a linked list, and we will need to do n recursive calls.
Deleting: O(log(n)) - O(n) - why? because in the worst case, the tree will be a linked list, and we will need to do n recursive calls.
Finding: O(log(n)) - O(n)

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
    Node* right;
    Node* left;
};

// ---------- Function Prototypes ----------




// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 4);
    insert_into_tree(root, -14);

    display(root);

    return 0;
}

// ---------- Functions ----------


void display(const Node* root) {
    if (root == nullptr) {
        return;
    }
    display(root->left);
    cout << root->data << " ";
    display(root->right);
}
void insert_into_tree(Node*& root, int data) {
    if (root == nullptr) {
        root = new Node;
        //check allocation
        root->data = data;
        root->left = nullptr;
        root->right = nullptr;
    } else if (data < root->data) {
        insert_into_tree(root->left, data);
    } else {
        insert_into_tree(root->right, data);
    }
}