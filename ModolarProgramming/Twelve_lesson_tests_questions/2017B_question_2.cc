






#include <iostream>

struct Node
{
    int _data;
    struct Node* _left;
    struct Node* _right;
};

struct Node* newNode(int data);
int balanced_helper(const Node* root, const Node*& max_sub_tree, int& balance, bool is_left);
const Node * find_best_balanaed_tree(Node* root);

using std::cout;
using std::endl;

int main()
{
    struct Node* root = newNode(17);
    root->_left = newNode(12);
    root->_right = newNode(25);
    root->_left->_left = newNode(10);
    root->_left->_right = newNode(14);
    root->_right->_left = newNode(22);
    root->_right->_right = newNode(35);
   
    root->_left->_left->_right = newNode(13);
    root->_left->_right->_left = newNode(11);
    root->_right->_left->_left = newNode(20);
    
  
    root->_right->_right->_right = newNode(37);
    cout << "finished building the tree" << endl;

    const Node* max_sub_tree = find_best_balanaed_tree(root);
    cout << "Root of the largest even-majority subtree: " << max_sub_tree->_data << endl;


    return 0;
}


const Node * find_best_balanaed_tree(Node* root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    int balance = -1;
    bool is_left = true;

    const Node* max_sub_tree = nullptr;

    balanced_helper(root, max_sub_tree, balance, is_left);

    return max_sub_tree;
}

int balanced_helper(const Node* root,const  Node*& max_sub_tree, int& balance, bool is_left)
{
    if (root == nullptr)
    {
        balance = 0;
        return balance;
    }

    int left_balance;
    int right_balance;

    left_balance = balanced_helper(root->_left, max_sub_tree, left_balance, true);
    right_balance = balanced_helper(root->_right, max_sub_tree, right_balance, false);

    if(left_balance == right_balance && left_balance > balance)
    {
       balance = left_balance;
       max_sub_tree = root;
    }
    if (is_left)
        return left_balance + 1;
    return right_balance + 1;
}

struct Node* newNode(int data)
{
    struct Node* node = new Node;
    node->_data = data;
    node->_left = nullptr;
    node->_right = nullptr;
    return node;
}