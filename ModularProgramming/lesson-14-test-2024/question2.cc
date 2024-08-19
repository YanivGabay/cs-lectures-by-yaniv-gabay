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
int const EXAMPLE_SIZE = 8;
// ---------- Structs ----------
struct Node {
    int data;
    Node* left;
    Node* right;
};

// ---------- Function Prototypes ----------
void insert(Node* &root, int data);
Node* getWantedNode(Node* root, int k);
Node* getWantedNode(Node* root, int k, int &count);


// ---------- Main Function ----------
int main() {
    

    /////////// the following code is just to create a binary search tree///////////
    int tree_items[EXAMPLE_SIZE] = {10,5,7,20,32,27,22,43};
    Node* root = nullptr;
    for(int i = 0; i < EXAMPLE_SIZE; i++) {
        insert(root, tree_items[i]);
    }

    ////////// example of using the inorder traversal to get the kth smallest element////////
    // according to the question we need to create a function that get: root, k
    // and return the kth smallest element
    int k = 15; // for example
    // we should return the 3rd smallest element
    // so , 5, 7, 10 - the 3rd smallest element is 10
    Node* wanted = getWantedNode(root, k);
    if (wanted == nullptr) {
        cout << "the " << k << "th smallest element is not found" << endl;
        return 0;
    }
    cout << "the " << k << "th smallest element is: " << wanted->data << endl;
    return 0;
}

// ---------- Functions ----------
///this is the sandwich function for the recursion///

Node* getWantedNode(Node* root,const int k) {
    int count = 0;
    Node* wanted = getWantedNode(root, k, count);
    if (wanted == nullptr || count < k) {
        return nullptr;
    }
    return wanted;
}

/// @brief get the kth smallest element in the binary search tree BST in 
/// classic inorder traversal
/// @param root the root of the tree
/// @param k the kth element we want to get
/// @param count the number of nodes we have visited
/// @return the kth smallest element
Node* getWantedNode(Node* root,const int k,int &count) {
    if(root == nullptr) {
        return nullptr;
    }
    Node* left = getWantedNode(root->left, k, count);
    // if we found the kth smallest element in the left subtree
    // return it
    if(left != nullptr) {
        return left;
    }
    // if we visited k nodes, return the current node
    //leafs return nullptr
    
    if(count == k) {
        return root;
    }
    //because k=0 is the smallest element
    //we ++ after we check
    count++;

    return getWantedNode(root->right, k, count);
}






// just to create the binary search tree for the example

/// @brief insert new node in the binary search tree BST
/// @param root the root of the tree
/// @param data int value to insert 
void insert(Node* &root, int data) {
    if(root == nullptr) {
        root = new Node;
        root->data = data;
        root->left = nullptr;
        root->right = nullptr;
    } else {
        if(data < root->data) {
            insert(root->left, data);
        } else {
            insert(root->right, data);
        }
    }
}