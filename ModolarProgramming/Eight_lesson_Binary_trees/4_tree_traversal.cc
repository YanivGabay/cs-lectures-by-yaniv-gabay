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


    cout << "starting traversal" << endl;
    cout << "Inorder: ";
    inorder(root);
    cout << endl;
    cout << "Preorder: ";
    preorder(root);
    cout << endl;
    cout << "Postorder: ";
    postorder(root);
    cout << endl;

    

    return 0;
}

// ---------- Functions ----------
void inorder(const Node* root)
 {
    if (root == nullptr) {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}
void preorder(const Node* root)
 {
    if (root == nullptr) {
        return;
    }
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}
void postorder(const Node* root)
 {
    if (root == nullptr) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
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