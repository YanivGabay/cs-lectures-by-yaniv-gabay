/********************************************************************
 * Course: Modular Programming in C++
 * Lecture: 
 * File: .cc
 *
 * Author: Yaniv Gabay
 * Date: 2024-6-3
 *
 * Overview:
 * 
 * 
 * 
 * 
this example is yoram sum subtree function, given a:
Binary tree (not bst), we want to check,
if for every subtree in the tree, 
that the sum of every subtrees left child is smaller or equal to the sum of the right child.

this is a better version of the previous code, (1)yoram_sum_tree.cc
and runs in O(n) time.
what the difference? what traversal we are using??

https://sites.google.com/view/yoramb-intro2cs/14-%D7%A2%D7%A6%D7%99%D7%9D-%D7%91%D7%99%D7%A0%D7%90%D7%A8%D7%99%D7%99%D7%9D-binary-trees?authuser=0
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




// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 4);
    insert_into_tree(root, -14);

    display(root);

    return 0;
}

// ---------- Functions ----------
bool sum_left_eq_sum_right(const struct Node* root,int &sum)
{
    if (root == nullptr)
    {
        sum = 0;
        return true;
    }

    int suml, sumr;
    bool ok;

    ok = sum_left_eq_sum_right(root->left,suml);
    if (!ok)
    return false;

    ok = sum_left_eq_sum_right(root->right,sumr);

    sum = suml + sumr + root->data;
    return (suml <= sumr);
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