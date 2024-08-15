






#include <iostream>
#include <climits>
struct Node
{
    int _data;
    struct Node* _left;
    struct Node* _right;
};

struct TreeInfo
{
    int _max;
    int _min;
    int _size;
};

struct TreeVariance
{
    struct Node* _node;
    double _variance;
};

struct Node* newNode(int data);
struct Node* calculate_variance(struct Node* root);
struct TreeInfo calculate_variance_helper(struct Node* root, struct TreeVariance& highest_variance);



using std::cout;
using std::endl;

int main()
{
    struct Node* root = newNode(12);
    root->_left = newNode(10);
    root->_right = newNode(13);
    root->_left->_left = newNode(3);
    root->_left->_right = newNode(12);
 
    root->_right->_right = newNode(25);
   
   
    root->_right->_right->_left = newNode(23);
    //same tree as example should return a ptr to the 13 node
    
    cout << "finished building the tree" << endl;
    
    //so variance of a tree is (max - min) / size of tree
    // we need to return the ptr to the node with the highest variance subtree

    struct Node* highest_variance_node = NULL;
    highest_variance_node = calculate_variance(root);

    if (highest_variance_node)
    {
        cout << "The node (subtree) with the highest variance is: " << highest_variance_node->_data << endl;
    } 
    else
    {
        cout << "No nodes in the tree" << endl;
    }

    return 0;
}


struct Node* calculate_variance(struct Node* root)
{
    struct TreeVariance highest_variance;
    highest_variance._variance = 0;
    highest_variance._node = NULL;
    //struct TreeInfo tree_info;
    calculate_variance_helper(root, highest_variance);
    
    


    return highest_variance._node;
}

struct TreeInfo calculate_variance_helper(struct Node* root,
                                          struct TreeVariance& highest_variance)
{
    if (root == NULL)
    {
        return {INT_MIN,INT_MAX,0};
    }
    
    struct TreeInfo left = calculate_variance_helper(root->_left, highest_variance);
    
    struct TreeInfo right = calculate_variance_helper(root->_right, highest_variance);

   
    int curr_size = left._size + right._size + 1;
    int curr_max = std::max(root->_data, std::max(left._max, right._max));
    int curr_min = std::min(root->_data, std::min(left._min, right._min));
    cout << "curr_max: " << curr_max << endl;
    cout << "curr_min: " << curr_min << endl;
    double curr_variance = (curr_max - curr_min) / curr_size;
    cout << "curr_variance: " << curr_variance << endl;
    if (curr_variance > highest_variance._variance)
    {
        highest_variance._variance = curr_variance;
        highest_variance._node = root;
        cout << "highest_variance._node->_data: " << highest_variance._node->_data << endl;
        cout << "highest_variance._variance: " << highest_variance._variance << endl;
    }
    cout << "finished calculating variance" << endl;
    return {curr_max, curr_min, curr_size};

 
    
}

Node* newNode(int data)
{
    struct Node* node = new Node;
    node->_data = data;
    node->_left = NULL;
    node->_right = NULL;
    return(node);
}
