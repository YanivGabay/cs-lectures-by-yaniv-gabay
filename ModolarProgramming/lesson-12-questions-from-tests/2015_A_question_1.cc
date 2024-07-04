#include <iostream>

// Node structure
struct Node {
    int _data;
    Node* _next;
};

// Function prototypes
Node* createNode(int data);
void addNode(Node*& head, Node*& tail, int data);
void split_list(Node*& head, Node*& head_pos_list, Node*& head_neg_list);
void printList(Node* head);

int main() {
    // Create a sample linked list: 5 -> -3 -> 0 -> 7 -> -2 -> 0 -> 9 -> -1
    Node* head = createNode(5);
    head->_next = createNode(-3);
    head->_next->_next = createNode(0);
    head->_next->_next->_next = createNode(7);
    head->_next->_next->_next->_next = createNode(-2);
    head->_next->_next->_next->_next->_next = createNode(0);
    head->_next->_next->_next->_next->_next->_next = createNode(9);
    head->_next->_next->_next->_next->_next->_next->_next = createNode(-1);

    Node* head_pos_list = nullptr;
    Node* head_neg_list = nullptr;
    printList(head);
    split_list(head, head_pos_list, head_neg_list);

    std::cout << "Positive List: ";
    printList(head_pos_list);
    std::cout << "Negative List: ";
    printList(head_neg_list);

    // Free memory
    while (head_pos_list != nullptr) {
        Node* temp = head_pos_list;
        head_pos_list = head_pos_list->_next;
        delete temp;
    }
    while (head_neg_list != nullptr) {
        Node* temp = head_neg_list;
        head_neg_list = head_neg_list->_next;
        delete temp;
    }
    while (head != nullptr) {
        Node* temp = head;
        head = head->_next;
        delete temp;
    }

    return 0;
}

// Function to create a new node
Node* createNode(int data) {
    return new Node{data, nullptr};
}

void addNode(Node*& head, Node*& tail, Node* node) {
    node->_next = nullptr; // Disconnect the node from its current list
    if (head == nullptr) {
        head = node;
        tail = node;
    } else {
        tail->_next = node;
        tail = node;
    }
}

// Function to split the list into positive and negative lists
void split_list(Node*& head, Node*& head_pos_list, Node*& head_neg_list) {
    Node* current = head;
    Node* current_positive = nullptr;
    Node* current_negative = nullptr;

    while (current != nullptr) {
        Node* next = current->_next; // Save the next node
        if (current->_data > 0) {
            addNode(head_pos_list, current_positive, current);
        } else if (current->_data < 0) {
            addNode(head_neg_list, current_negative, current);
        }
        current = next;
    }

    // Clear the original list head
    head = nullptr;
}

// Function to print a list
void printList(Node* head) {
    while (head != nullptr) {
        std::cout << head->_data << " ";
        head = head->_next;
    }
    std::cout << std::endl;
}
