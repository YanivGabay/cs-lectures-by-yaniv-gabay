/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:


 * this is the effient way to count the number of subtrees in a binary tree that all the nodes in the subtree are even.
 * we use post order traversal to solve this problem.
 * we send a variable by reference to the function, and we increase it by 1 if the subtree is all even.
 * the post order allows us to send information from the left and right subtree to the parent.


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


int countSubTrees(const Node* root);
bool actualCount(const Node* root, int& counter);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 4);
    insert_into_tree(root, -14);

    display(root);

    int numSubtrees = countSubTrees(root);
    std::cout << "Number of subtrees where all nodes are even: " << numSubtrees << std::endl;
    return 0;
}

// ---------- Functions ----------
int countSubTrees(const Node* root)
{
    int counter = 0;
    actualCount(root, counter);
    return counter;
}
bool actualCount(const Node* root, int& counter)
{
    if (root == nullptr)
    {
        return true;
    }
    bool left = actualCount(root->left, counter);
    bool right = actualCount(root->right, counter);
    if (left && right && root->data % 2 == 0)
    {
        counter++;
        return true;
    }
    return false;
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