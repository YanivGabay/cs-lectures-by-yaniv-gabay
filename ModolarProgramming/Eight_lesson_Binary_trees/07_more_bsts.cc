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
void print_tree(const Node* const root);


int smallest(const Node* const root);
int largest(const Node* const root);
int smallest_biggest_sum(const Node* const root);
int sum_of_leafs(const Node* const root,int &sum);
// ---------- Main Function ----------
int main() {
    Node* root = nullptr; // Initialize the head of the list to nullptr
    insert_into_tree(root, 10);
    insert_into_tree(root, 20);
    insert_into_tree(root, 30);
    insert_into_tree(root, 5);
    insert_into_tree(root, -15);




    //TO BE FIXED
    print_tree(root);
    cout << "smallest and largest sum is: " << smallest_biggest_sum(root) << endl;
    int sum = 0;
    sum = sum_of_leafs(root,sum);
    cout << "sum of leafs is: " << sum;

    return 0;
}

// ---------- Functions ----------
int sum_of_leafs(const Node* const root,int&sum)
{
    cout << "inside sum_of_leafs" << endl;
    if(root == nullptr)
    {
        cout << "root equal null" << endl;
          return 0;
    }
   
    
    if(root->left==nullptr && root->right==nullptr)
    {
        cout << "found a leaf" << endl;
        cout << "returning: " << root->data;
            return root->data;

    }

    
    return sum+sum_of_leafs(root->left,sum) + sum_of_leafs(root->right,sum);

  
}

int smallest_biggest_sum(const Node* const root)
{
    return smallest(root)+largest(root);
}
int smallest(const Node* const root)
{
    //should return -15 +30  
    if(root->left == nullptr)
    return root->data;

    smallest(root->left);


}
int largest(const Node* const root)
{
    if(root->right == nullptr)
    return root->data;

    largest(root->right);
}
void print_tree(const Node* const root)
{
    if(root == nullptr)
    return;

    print_tree(root->left);
    cout << root->data << " ";
    print_tree(root->right);

    return;
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