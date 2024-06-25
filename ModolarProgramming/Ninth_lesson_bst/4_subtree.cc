/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:

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



void subTreesNegativeOnly(Node* root, int& count) ;
int subtreeSum(Node* root) ;
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, -4);
    insert_into_tree(root, -14);

    display(root);
    int count = 0;
    subTreesNegativeOnly(root, count);
    std::cout << "Number of subtrees with all negatives "  << ": " << count << std::endl;
    return 0;
}

// ---------- Functions ----------
bool allNegatives(Node* root) {
    if (root == nullptr) return true;  // Consider an empty tree as satisfying the condition.
    return root->data < 0 && allNegatives(root->left) && allNegatives(root->right);
}


// Function to count how many subtrees meet the specified sum condition
void subTreesNegativeOnly(Node* root, int& count) {
    if (root == nullptr) return;
    if (allNegatives(root)) {
        count++;
    }
    subTreesNegativeOnly(root->left, count);
    subTreesNegativeOnly(root->right, count);
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