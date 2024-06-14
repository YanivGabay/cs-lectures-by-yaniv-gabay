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



int countSubtreesWithSum(Node* root, int targetSum, int& count) ;
int subtreeSum(Node* root) ;
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 4);
    insert_into_tree(root, -14);

    display(root);
    int targetSum = -10;
    int count = 0;
    countSubtreesWithSum(root, targetSum, count);
    std::cout << "Number of subtrees with sum " << targetSum << ": " << count << std::endl;
    return 0;
}

// ---------- Functions ----------
int subtreeSum(Node* root) {
    if (root == nullptr) return 0;
    return root->data + subtreeSum(root->left) + subtreeSum(root->right);
}

// Function to count how many subtrees meet the specified sum condition
int countSubtreesWithSum(Node* root, int targetSum, int& count) {
    if (root == nullptr) return 0;
    int sum = subtreeSum(root);
    if (sum == targetSum) {
        count++;
    }
    countSubtreesWithSum(root->left, targetSum, count);
    countSubtreesWithSum(root->right, targetSum, count);
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