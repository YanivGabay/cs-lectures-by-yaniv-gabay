/********************************************************************
 * Course: Modolar Programming
 * Lecture: 13 - tests
 * File: 2022A-2-sequence-nodes.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-07-26
 * 
 * Overview:
 * 
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

// ---------- Structs ----------
struct Node {
    int _data;
    struct Node *_right;
};

struct SequenceNodes {
    int _data;
    struct SequenceNodes *_next, *_prev;
    struct SequenceNodes *_plus_one_right,
     *_minus_one_left;

};
// ---------- Function Prototypes ----------
void print_list(struct Node *head);
void print_list(struct SequenceNodes *head);
struct SequenceNodes* transform_list(struct Node *head);
bool search_for_value(struct SequenceNodes *head, int value);
void create_list(struct Node *original_list_current, struct SequenceNodes *new_head,
                 struct SequenceNodes *new_current, struct SequenceNodes *&tail);
struct Node* create_basic_list(int* arr, int size);
void print_plus_right(struct SequenceNodes *head);
void print_minus_left(struct SequenceNodes *head);
void connect_neighbors(struct SequenceNodes *head, bool is_plus, struct SequenceNodes *tail);
// ---------- Main Function ----------
int main() {

    // lets recreated the example simple
    // linked list from the question
    int arr[] = {-4,5,7,-3,8,-2,6,5,6};
    int size = sizeof(arr) / sizeof(arr[0]);
    struct Node *head = create_basic_list(arr, size);
    print_list(head);
    cout << "finished creating the list" << endl;

    // so lets think exactly what we need to do
    // we need to create a new list based on the original list
    // 1. they need to be in the same order
    // 2. no duplicates (the original list has duplicates)
    // 3. we have the plus one right and minus one left pointers 
    //    to connect aswell as a regular list.

    // if you knew hashmaps, its a very good solution for 
    // removing duplicated etc. but you DONT know it.
    
    // so what we do?
    //its very easy, first remove duplicates
    // in o(n^2) time complexity
    // then create the new list with the pointers
    // in o(n-(removed dupes)) time complexity
    // than connected the plus one right and minus one left
    // with other o(n^2) time complexity, cause we need to
    // search for the right node to connect to per each node

    //i would ask in the test yoram:
    // do i need to delete the original list?
    // do i need to release the memory of the new list??
   
    struct SequenceNodes *new_head = transform_list(head);
    cout << "finished removing duplicates" << endl;
    print_list(new_head);



    return 0;
}

// ---------- Functions ----------
void print_plus_right(struct SequenceNodes *head) {
    if (head == nullptr) {
        cout << "Empty list" << endl;
        return;
    }
    struct SequenceNodes *current = head;
    while (current != nullptr) {
        cout << current->_data << " ";
        if (current->_plus_one_right != nullptr) {
            cout << "plus one right: " << current->_plus_one_right->_data << " ";
        }
        cout << endl;
        current = current->_next;
    }
    cout << endl;
}
void print_minus_left(struct SequenceNodes *head) {
    if (head == nullptr) {
        cout << "Empty list" << endl;
        return;
    }
    struct SequenceNodes *current = head;
    while (current != nullptr) {
        cout << current->_data << " ";
        if (current->_minus_one_left != nullptr) {
            cout << "minus one left: " << current->_minus_one_left->_data << " ";
        }
        cout << endl;
        current = current->_prev;
    }
    cout << endl;
}
struct SequenceNodes* transform_list(struct Node *head) {
    if (head == nullptr) {
        return nullptr;
    }

    struct Node *original_list_current = head;
    struct SequenceNodes *new_head = new SequenceNodes{original_list_current->_data, nullptr, nullptr, nullptr};

    struct SequenceNodes *new_current = new_head;
    original_list_current = original_list_current->_right;
    struct SequenceNodes *tail = new_current;
    create_list(original_list_current, new_head, new_current, tail);

    connect_neighbors(new_head, true, tail);
    connect_neighbors(new_head, false,tail);
    
  
    cout << "printing plus one right" << endl;
    print_plus_right(new_head);
    cout << "printing minus one left" << endl;
    print_minus_left(tail);


    return new_head;
}

void connect_neighbors(struct SequenceNodes *head, bool is_plus, struct SequenceNodes *tail) {
    if (head == nullptr) {
        return;
    }
    int diff = is_plus ? 1 : -1;
    struct SequenceNodes *current = is_plus ? head : tail;

    while (current != nullptr) {
        struct SequenceNodes *current_search = is_plus ? current->_next : current->_prev;
        while (current_search != nullptr) {
            if (current_search->_data == current->_data + diff) {
                if (is_plus) {
                    current->_plus_one_right = current_search;
                } else {
                    current->_minus_one_left = current_search;
                }
                break;
            }
            current_search = is_plus ? current_search->_next : current_search->_prev;
        }
        current = is_plus ? current->_next : current->_prev;
    }


   
}


void create_list(struct Node *original_list_current, struct SequenceNodes *new_head,
                 struct SequenceNodes *new_current, struct SequenceNodes *&tail) {
    while (original_list_current != nullptr) {
        bool is_found = search_for_value(new_head, original_list_current->_data);

        if (!is_found) {
            new_current->_next = new SequenceNodes{original_list_current->_data, nullptr, new_current, nullptr, nullptr};
            new_current = new_current->_next;
            tail = new_current;
            cout << "tail in create list: " << tail->_data << endl;
        }
        original_list_current = original_list_current->_right;
    }
}
bool search_for_value(struct SequenceNodes *head, int value) {
    
    if (head == nullptr) {
        return false;
    }

    struct SequenceNodes *current = head;
    while (current != nullptr) {
        if (current->_data == value) {
            return true;
        }
        current = current->_next;
    }
    return false;
}

void print_list(struct SequenceNodes *head) {
    if (head == nullptr) {
        cout << "Empty list" << endl;
        return;
    }
    struct SequenceNodes *current = head;
    while (current != nullptr) {
        cout << current->_data << " ";
        current = current->_next;
    }
    cout << endl;
}

void print_list(struct Node *head) {
    if (head == nullptr) {
        cout << "Empty list" << endl;
        return;
    }
    struct Node *current = head;
    while (current != nullptr) {
        cout << current->_data << " ";
        current = current->_right;
    }
    cout << endl;
}
struct Node* create_basic_list(int* arr, int size) {
    struct Node *head = new Node{arr[0], nullptr};
    struct Node *current = head;
    for (int i = 1; i < size; i++) {
        current->_right = new Node{arr[i], nullptr};
        current = current->_right;
    }
    return head;
}