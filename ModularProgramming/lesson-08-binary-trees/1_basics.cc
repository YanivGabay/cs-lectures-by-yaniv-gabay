/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:



we will talk about binary tree and a binary search tree
what is the diff?
a binary tree is a tree where each node has at most 2 children
a binary search tree is a binary tree where the left child is smaller than the parent and the right child is bigger than the parent


why we use bst?
what is the maximum time to find a value?
what is the maximum time to insert a value?






answer: between O(log n) and O(n) depending on the tree, can somone think
why?

https://en.wikipedia.org/wiki/Binary_search_tree



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
    Node* right;//next //prev
    Node* left ;
};

// ---------- Function Prototypes ----------
void insert_into_tree(Node*& root, int data);
void display(const Node* root);
void delete_tree(Node*& root);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 5);
    insert_into_tree(root, -15);
    cout << "Binary Tree: ";
    display(root);
    delete_tree(root);
    


    
    return 0;
}

// ---------- Functions ----------
// ---------- Functions ----------
void delete_tree(Node*& root) {
    if (root == nullptr) {
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
    root = nullptr;
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
        root = new (std::nothrow) Node;
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