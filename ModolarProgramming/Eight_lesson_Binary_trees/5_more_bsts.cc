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



// GREAT page on traversal of trees:
//https://en.wikipedia.org/wiki/Tree_traversal
// we will go over it.

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

int countNodes(const Node* root);
void inorder(const Node* root);
int sumNodes(const Node* root) ;
void preorder(const Node* root);

void postorder(const Node* root);

// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 5);
    insert_into_tree(root, -15);

    // return the sum of the nodes in the binary tree
    cout << "Sum of nodes: " << sumNodes(root) << endl;
    

    return 0;
}

// ---------- Functions ----------
void delete_tree(Node*& root) {
    if (root == nullptr) {
        return;
    }
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
    //some like this, some not, but it is a good practice in my opinion
    root = nullptr;
}


int sumNodes(const Node* root) {
    if (root == nullptr) {
        return 0;
    }
    return root->data + sumNodes(root->left) + sumNodes(root->right);
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