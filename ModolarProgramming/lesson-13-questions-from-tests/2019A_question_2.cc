






#include <iostream>


struct TNode {
    int _data;
    TNode *_left;
    TNode *_right;
    TNode *_next;    
};

struct TNode *connect_primes(TNode *root);
struct TNode *connect_primes_helper(TNode *root, TNode *&new_list_head, TNode *&new_list_tail);
bool is_prime(int n);


using std::cout;
using std::endl;

int main()
{
    TNode *root = new TNode{5, nullptr, nullptr, nullptr};
    root->_left = new TNode{112, nullptr, nullptr, nullptr};
    root->_right = new TNode{252, nullptr, nullptr, nullptr};
    root->_left->_left = new TNode{3, nullptr, nullptr, nullptr};
    root->_left->_right = new TNode{141, nullptr, nullptr, nullptr};
    root->_right->_left = new TNode{7, nullptr, nullptr, nullptr};
    root->_right->_right = new TNode{13, nullptr, nullptr, nullptr};
    

    // we need to connect with the _next the primers nodes
    // we already "get" bool is_prime(int) function

    struct TNode *new_list = connect_primes(root);

    if (new_list)
    {
        struct TNode *current = new_list;
        while (current)
        {
            cout << current->_data << " ";
            current = current->_next;
        }
        cout << endl;
    } 
    else
    {
        cout << "No primes in the tree" << endl;
    }

    return 0;
}


struct TNode *connect_primes(TNode *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }
    
    struct TNode *new_list_head = nullptr;
    struct TNode *new_list_tail = nullptr;

    return connect_primes_helper(root, new_list_head, new_list_tail)? new_list_head : nullptr;


}

struct TNode *connect_primes_helper(TNode *root, TNode *&new_list_head, TNode *&new_list_tail)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (is_prime(root->_data))
    {
        if (new_list_head == nullptr)
        {
            new_list_head = root;
            new_list_tail = root;
        }
        else
        {
            new_list_tail->_next = root;
            new_list_tail = root;
        }
    }

    connect_primes_helper(root->_left, new_list_head, new_list_tail);
    connect_primes_helper(root->_right, new_list_head, new_list_tail);

    return new_list_head;
}


//just a copy paste is prime for the sake of the example
bool is_prime(int n)
{
    if (n <= 1)
    {
        return false;
    }
    if (n <= 3)
    {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0)
    {
        return false;
    }
    for (int i = 5; i * i <= n; i = i + 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
        {
            return false;
        }
    }
    return true;
}


