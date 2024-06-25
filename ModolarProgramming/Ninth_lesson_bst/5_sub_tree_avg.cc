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
#include <limits.h> // int min
#include <iomanip> // std::setw
#include <cmath> // std::pow
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

// Helper structure to store multiple return values from the recursive function
struct SubtreeData {
    int sum;        // Sum of all nodes in the subtree
    int count;      // Number of nodes in the subtree
    double average; // Average of the subtree
};


// ---------- Function Prototypes ----------
void insert_into_tree(Node*& root, int data);
void display(const Node* root);

SubtreeData findMaxAverageSubtree(Node* root, double& maxAverage, Node*& maxNode) ;
double getMaxAverageSubtree(Node* root) ;
void printTree(Node* root, int indent = 0, int increment = 10);
void printNode(Node* node, int indent);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 60);
    insert_into_tree(root, 30);
    insert_into_tree(root, -4);
    insert_into_tree(root, -14);

     display(root);
     std::cout << std::endl;
    int initialIndent = 40;  // Set this to a value that approximately centers the root in your console
    printTree(root, initialIndent);  // Start printing the tree from the root
    double max_avg = getMaxAverageSubtree(root);

   


    return 0;
}

// ---------- Functions ----------

// Recursive function to find maximum average of a subtree
SubtreeData findMaxAverageSubtree(Node* root, double& maxAverage, Node*& maxNode) {
    if (!root) return {0, 0, 0.0};

    // Recur for left and right subtrees
    SubtreeData left = findMaxAverageSubtree(root->left, maxAverage, maxNode);
    SubtreeData right = findMaxAverageSubtree(root->right, maxAverage, maxNode);

    // Current subtree stats
    SubtreeData current;
    current.sum = root->data + left.sum + right.sum;
    current.count = 1 + left.count + right.count;
    current.average = (double)(current.sum) / current.count;

    // Check if the current subtree has a new maximum average
    if (current.average > maxAverage) {
        maxAverage = current.average;
        maxNode = root; // Store the root of the subtree with max average
    }

    return current;
}

// Utility function to start the recursion and return the max average
double getMaxAverageSubtree(Node* root) {
    double maxAverage = INT_MIN; //new concept, you can just use 0, but there are possiblities of a negative number
    //so using INT_MIN is a good idea
    Node* maxNode = nullptr;
    findMaxAverageSubtree(root, maxAverage, maxNode);
    cout << "Root of max average subtree: " << (maxNode ? maxNode->data : -1) << endl;
   // printSubtree(maxNode);
    return maxAverage;
}
void printNode(Node* node, int indent) {
    if (!node) return;
    std::cout << std::setw(indent) << node->data << "\n";
}

// Recursive function to print the BST with the root first and proper alignment
void printTree(Node* root, int indent , int increment) {
    if (root == nullptr) {
        return;
    }

    // Set the initial position for the root or any node based on current indent
    printNode(root, indent);

    // Print the left subtree by decreasing indentation
    printTree(root->left, indent - increment, increment);

    // Print the right subtree by increasing indentation
    printTree(root->right, indent + increment, increment);
}
void display(const Node* root) {
    if (root == nullptr) {
        return;
    }
     cout << root->data << " ";
    display(root->left);
   
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