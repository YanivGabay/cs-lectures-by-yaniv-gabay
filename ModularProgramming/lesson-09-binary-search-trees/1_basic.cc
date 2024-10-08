/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:


 * what important to understand here, we will show a code that will count the number of subtrees in a 
    binary tree that all the nodes in the subtree are even.
 * this is NOT the efficient way to do it, but i show it before, so you can understand the concept.
 * the effiecient way will be shown in lesson 6 in the same folder.
 * try to think after you understand, why it is not efficient, how you can make it efficient.


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
    Node* right;
    Node* left;
};

// ---------- Function Prototypes ----------
void insert_into_tree(Node*& root, int data);
void display(const Node* root);


bool allEven(Node* root) ;
int countSubtrees(Node* root);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 4);
    insert_into_tree(root, -14);

    display(root);

    int numSubtrees = countSubtrees(root);
    std::cout << "Number of subtrees where all nodes are even: " << numSubtrees << std::endl;
    return 0;
}

// ---------- Functions ----------
// Function to check if all nodes in a subtree satisfy a condition
bool allEven(Node* root) {
    if (root == nullptr) return true;
    if (root->data % 2 != 0) return false;
    return allEven(root->left) && allEven(root->right);
}

// Function to count how many subtrees meet a specific condition
int countSubtrees(Node* root) {
    if (root == nullptr) return 0;
    int count = (allEven(root) ? 1 : 0) + countSubtrees(root->left) + countSubtrees(root->right);
    return count;
}

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