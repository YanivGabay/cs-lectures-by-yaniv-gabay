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
struct Node
{
    int data;
    Node *right;
    Node *left;
};

// ---------- Function Prototypes ----------
void insert_into_tree(Node *&root, int data);
void display(const Node *root);
int countNodes(const Node *root);
// ---------- Main Function ----------
int main()
{
    Node *root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 5);
    insert_into_tree(root, -15);
    cout << "Binary Tree: ";
    display(root);
    cout << endl;
    cout << "Number of nodes: " << countNodes(root) << endl;

    return 0;
}

// ---------- Functions ----------

// we will need to count the nodes in the binary tree
// this is abit hard to understand at the start
int countNodes(const Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}
// yoram does it like this
int countNodes(const Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }
    int left = countNodes(root->left);
    int right = countNodes(root->right);
    return 1 + left + right;
}

void display(const Node *root)
{
    if (root == nullptr)
    {
        return;
    }
    display(root->left);
    cout << root->data << " ";
    display(root->right);
}
void insert_into_tree(Node *&root, int data)
{
    if (root == nullptr)
    {
        root = new Node;
        // check allocation
        root->data = data;
        root->left = nullptr;
        root->right = nullptr;
    }
    else if (data < root->data)
    {
        insert_into_tree(root->left, data);
    }
    else
    {
        insert_into_tree(root->right, data);
    }
}