
#include <iostream>


//2017 A question 1
// this solution got all the points for this question


struct Node
{
    int _data;
    Node* _right;
    Node* _left;
};


using std::cout ;
using std::endl;


const struct Node* get_biggest_sub_tree(const struct Node* root);
const struct Node* get_biggest_sub_tree(const struct Node* root, int& nodes, int& diff, int& max_nodes, const struct Node* max_sub_tree);


int main()
{
    Node* root = new Node{4, nullptr, nullptr};
    root->_left = new Node{6, nullptr, nullptr};
    root->_right = new Node{8, nullptr, nullptr};
    root->_left->_left = new Node{10, nullptr, nullptr};
    root->_left->_right = new Node{12, nullptr, nullptr};
    root->_right->_left = new Node{3, nullptr, nullptr};
    root->_right->_right = new Node{9, nullptr, nullptr};

 
    const Node* largestEvenMajoritySubtree = get_biggest_sub_tree(root);

    if (largestEvenMajoritySubtree) {
        cout << "Root of the largest even-majority subtree: " << largestEvenMajoritySubtree->_data << endl;
    } else {
        cout << "No even-majority subtree found." << endl;
    }
    return 0;
}


const struct Node* get_biggest_sub_tree(const struct Node* root)
{
    if (root == nullptr)
    {
        return nullptr;
    }
   
   int nodes,
        diff,
        max_nodes = 0;
    const struct Node* max_sub_tree = nullptr;

    return get_biggest_sub_tree(root, nodes, diff, max_nodes, max_sub_tree);
}

const struct Node* get_biggest_sub_tree(const struct Node* root, int& nodes, int& diff, int& max_nodes, const struct Node* max_sub_tree)
{
    if (root == nullptr)
    {
        nodes = 0;
        diff = 0;
        return nullptr;
    }

    int left_nodes,
        left_diff,
        right_nodes,
        right_diff;

    const struct Node* left_sub_tree = get_biggest_sub_tree(root->_left, left_nodes, left_diff, max_nodes, max_sub_tree);
    const struct Node* right_sub_tree = get_biggest_sub_tree(root->_right, right_nodes, right_diff, max_nodes, max_sub_tree);

    nodes = left_nodes + right_nodes + 1;
    diff = left_diff + right_diff + (root->_data % 2 == 0) ? 1 : -1;

    if(diff> 0 && nodes > max_nodes)
    {
        max_nodes = nodes;
        max_sub_tree = root;
    }

    return max_sub_tree;
}