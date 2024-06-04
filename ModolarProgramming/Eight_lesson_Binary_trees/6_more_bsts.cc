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


int countPositiveChildrenParents(Node* root);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 5);
    insert_into_tree(root, -15);

    int value = countPositiveChildrenParents(root);
    cout << "Number of nodes with positive children: " << value << endl;

    return 0;
}

// ---------- Functions ----------
int countPositiveChildrenParents(Node* root) {
    if (root == nullptr) {
        return 0;
    }

    int count = 0;
    // Check if both children are positive
    if (root->left && root->right) {
        if (root->left->data > 0 && root->right->data > 0) {
            count = 1;  // Current node satisfies the condition
        }
    }

    // Recursively count in left and right subtrees
    count += countPositiveChildrenParents(root->left);
    count += countPositiveChildrenParents(root->right);
    
    return count;
}

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