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


// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed

// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
int const MAX_SIZE = 1000;
// ---------- Structs ----------
struct Node {
    int data;
    Node* left;
    Node* right;
};

// ---------- Function Prototypes ----------


// ---------- Main Function ----------
int main() {
    //we are given a bst with duplicated values
    //values are only between 0 till 999
    //we need to convert it to a bst without duplicates
    //with building a new tree
    Node* old_root;
    Node* new_tree = build_new_tree(old_root);

  
}

// ---------- Functions ----------
Node* build_new_tree(Node* root) {

    int counter_array[MAX_SIZE] = {0};
    int highest_value = -1; //not really neccesary but just to optimize little
    count_values(root, counter_array, highest_value);

    Node* new_root = nullptr;
    for (int i = 0; i <= highest_value; i++) {
        if (counter_array[i] != 1) {
            new_root = insert(new_root, i);
        }
    }
    return new_root;
}
void count_values(Node* root, int counter_array[],int &highest_value) {
    if (root == nullptr) {
        return;
    }
    
    count_values(root->left, counter_array, highest_value);
    counter_array[root->data]++;
    if (root->data > highest_value) {
        highest_value = root->data;
    }
    count_values(root->right, counter_array, highest_value);
}
//in the test you dont need to create this function
Node* insert(Node*& root,int value)
{
    if (root == nullptr)
    {

    }
}