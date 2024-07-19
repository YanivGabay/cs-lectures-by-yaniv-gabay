






#include <iostream>

struct Node {
    int _data;
    struct Node* _next ; 
    
};
struct Node2 {
    int _data;
    struct Node2* _next;
    struct Node2* _prev;
};

enum INSERT_DIRECTION
{
    INSERT_AT_START,
    INSERT_AT_END
};


struct Node* newNode(int data);
struct Node2* newNode2(int data);
void insertIntoList(Node2*& head, enum INSERT_DIRECTION direction, Node2*& node);
struct Node2* manipulateList(Node* root);


using std::cout;
using std::endl;

int main()
{
    //simple list of 4 nodes
    // 17 -> 112 -> 252 -> 102
    struct Node* root = newNode(17);
    root->_next = newNode(112);
    root->_next->_next = newNode(252);
    root->_next->_next->_next = newNode(102);

    //cout << "example list" << endl;
    //cout << root->_data << " -> " << root->_next->_data << " -> " << root->_next->_next->_data << " -> " << root->_next->_next->_next->_data << endl;


    //so given this input we need to output the following:
    //with the Node2 struct, and pointers two ways.
    // 17 -> 112 -> 252 -> 102 -> 102 -> 252 -> 112 -> 17

    

    Node2* new_list = manipulateList(root);

    if (new_list)
    {
        Node2* current = new_list;
        while (current)
        {
            cout << current->_data << " -> ";
            current = current->_next;
        }
        cout << endl;
    }
    else
    {
        cout << "The list is empty" << endl;
    }
    return 0;
}

struct Node2* manipulateList(Node* root) {
    //your code here
    Node2* list_head_insert_start = NULL;
    Node2* list_head_insert_end = NULL;

    if (!root)
    {
        //need to check for nulls in return values
        cout << "The root is null" << endl;
        return NULL;
    }
    

    Node* current = root;

    while (current)
    {
       
        Node2* new_node = newNode2(current->_data);
        Node2* new_node2 = newNode2(current->_data);
        insertIntoList(list_head_insert_start, INSERT_AT_START, new_node);
        insertIntoList(list_head_insert_end, INSERT_AT_END, new_node2);
        
        current = current->_next; //move to the next node
       
    }

    //now we need to connect the two lists
    //at his example, we take the insert at end, and connect its end to the head
    // of the insert at start
    if (list_head_insert_end)
    {
        cout << "The list head tail is not null" << endl;
        Node2* current = list_head_insert_end;
        while (current->_next)
        {
            current = current->_next;
        }
        current->_next = list_head_insert_start;
        list_head_insert_start->_prev = current;

    }


    cout << "The list is ready" << endl;

    return list_head_insert_end;


}

void insertIntoList(Node2*& head,enum INSERT_DIRECTION direction, Node2*& node) {
    if (!node) // we got a bad node for some reason
    {
        cout << "The node is null" << endl;
        return;
    }

    if (!head) // head is null, so we create.
    {
        head = node;
        return;
    }

 

    if (direction == INSERT_AT_START)
    {
       
        node->_next = head;
        head->_prev = node;
        head = node;
    }
    else
    {
       
        Node2* current = head;
     
        while (current->_next)
        {
            current = current->_next;
        }
        current->_next = node;
        node->_prev = current;
    }
    cout << "The list is updated" << endl;
}

Node2* newNode2(int data) {
    Node2* node = new Node2;//no throw 
    //check alloc
    node->_data = data;
    node->_next = NULL;
    node->_prev = NULL;
    return node;
}



struct Node* newNode(int data) {
    struct Node* node = new Node;
    node->_data = data;
    node->_next = NULL;
    return node;
}