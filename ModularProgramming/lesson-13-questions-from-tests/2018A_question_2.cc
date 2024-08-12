






#include <iostream>

struct Node
{
    int _data;
    struct Node* _next;
};

struct Node* newNode(int data);
struct Node* find_biggest_sequence(struct Node* head);
void collectValues(struct Node* head, bool* arr);
int find_highest_sequence(bool* arr, int &position);
struct Node* create_list(bool* arr, int position, int highest_seq);


using std::cout;
using std::endl;

int main()
{
    struct Node* head = newNode(17);
    head->_next = newNode(112);
    head->_next->_next = newNode(6);
    head->_next->_next->_next = newNode(4);
    head->_next->_next->_next->_next = newNode(5);
    head->_next->_next->_next->_next->_next = newNode(346);
    head->_next->_next->_next->_next->_next->_next = newNode(7);
    head->_next->_next->_next->_next->_next->_next->_next = newNode(841);


    cout << "finished building the linked list" << endl;

    //given this linked list , we need to return a pointer to a linked list 
    // that contains the longest sequence of increasing numbers
    // so in this case the function should return a pointer to the node with the value 4 -> 5 -> 6 -> 7

    //we are given that the range of the values is between 1 to 1000
    // so we can use an array of 1000 elements to store the values (array size is 1001 because the values are between 0 to 1000)
    // and then we can iterate over the array to find the longest sequence of increasing numbers

    struct Node* biggest_sequence_head = find_biggest_sequence(head);

    if (biggest_sequence_head)
    {
        struct Node* current = biggest_sequence_head;
        while (current)
        {
            cout << current->_data << " ";
            current = current->_next;
        }
        cout << endl;
    } 
    else
    {
        cout << "No sequence in the linked list" << endl;
    }

    return 0;
}


struct Node* find_biggest_sequence(struct Node* head)
{
    bool arr[1001] = {0};
    
    collectValues(head, arr);

    int position = 0;
    int highest_seq = find_highest_sequence(arr, position);

    struct Node* new_head = NULL;

    new_head = create_list(arr, position, highest_seq);

    return new_head;
}

struct Node* create_list(bool* arr, int position, int highest_seq)
{
    struct Node* head = NULL;
    struct Node* current = NULL;
    for (int i = position; i < position + highest_seq; i++)
    {
        if (head == NULL)
        {
            head = newNode(i);
            current = head;
        }
        else
        {
            current->_next = newNode(i);
            current = current->_next;
        }
    }

    return head;
}

int find_highest_sequence(bool* arr,int &position)
{
    int highest_seq = 0;
    int current_sequence = 0;
    for (int i = 0; i < 1000; i++)
    {
        if (arr[i] == 1)
        {
            current_sequence++;
        }
        else
        {
            if (current_sequence > highest_seq)
            {
                highest_seq = current_sequence;
                position = i - current_sequence;
                current_sequence = 0;
                
            }
        }
    }

    return highest_seq;
}

void collectValues(struct Node* head, bool* arr)
{
    if (head != NULL)
    {
        while (head != NULL)
        {
            arr[head->_data] = 1;
            cout << "adding " << head->_data << " to the array" << endl;
            head = head->_next;
        }
        
    }
}

Node* newNode(int data)
{
    struct Node* node = new Node;
    node->_data = data;
    node->_next = NULL;
    return(node);
}

