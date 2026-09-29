/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:


 * another example of effienct subtree question
 * we need to return the pointer to the node that has the maximum sum of all nodes in the subtree.
 * we are given a binary tree, so not a BST

 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
#include <climits>
// ---------- Using Section ----------
using std::cin;
using std::cout;
using std::endl;

// ---------- Structs ----------
struct Node {
    int data;
    Node* right = nullptr;
    Node* left= nullptr;
};

// ---------- Function Prototypes ----------
void insert_into_tree(Node*& root, int data);
void display(const Node* root);

Node* biggestSumNode(Node* root);
int actualBiggestSumNode(Node* root, Node*& maxSumNode, int& biggestSum);


// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    // basic and ugly way to create a binary tree
    root = new Node;
    root->data = 2;
    root->left = new Node;
    root->left->data = 20;
    root->right = new Node;
    root->right->data = 1;
    root->left->left = new Node;
    root->left->left->data = 4;
    root->left->right = new Node;
    root->left->right->data = 16;

    Node* maxSumNode = nullptr;
    maxSumNode = biggestSumNode(root);
    display(root);

    cout << "The node with the biggest subtree sum is: " << maxSumNode->data << endl;
   
    return 0;
}

// ---------- Functions ----------
Node* biggestSumNode(Node* root)
{
    Node* maxSumNode = nullptr;
    int biggestSum = INT_MIN;
    actualBiggestSumNode(root, maxSumNode, biggestSum);
    cout << "The biggest sum is: " << biggestSum << endl;
    return maxSumNode;
}
int actualBiggestSumNode(Node* root, Node*& maxSumNode, int& biggestSum)
{
    if (root == nullptr)
    {
        return 0;
    }
    int leftSum = actualBiggestSumNode(root->left, maxSumNode, biggestSum);
    int rightSum = actualBiggestSumNode(root->right, maxSumNode, biggestSum);
    int sum = leftSum + rightSum + root->data;
    if (sum > biggestSum)
    {
        biggestSum = sum;
        maxSumNode = root;
    }
    return sum;
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