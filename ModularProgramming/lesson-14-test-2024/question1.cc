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
using std::cin;
using std::cout;
using std::endl;

// ---------- Constants ----------

// ---------- Structs ----------
// important i added default nullptr !
struct Primary_node
{
    int _primary_data;
    struct Node *_right = nullptr;
    struct Primary_node *_down = nullptr;
};
struct Node
{
    int _data;
    struct Node *_right = nullptr;
};
// ---------- Function Prototypes ----------
void insert_by_terms(struct Primary_node *head, int data);
void print(struct Primary_node *head);
void allocate_new_primary(struct Primary_node *head, int data, int smallest);
int smallest_primary_div(const int data);
bool insert_to_right(struct Primary_node *head, int data, struct Primary_node *&tail);
void check_alloc(void *ptr);

// ---------- Main Function ----------
int main()
{

    ///////////////////////// VERY PRIMITIVE WAY TO SET THE EXAMPLE /////////////////////////
    //  THE SAME AS THE TEST
    // IMPORTANT , THE EXAMPLE GIVEN IN THE TEST (CHECK THE .md FILE) , ISNT BY THE "RULES"
    // WE WERE GIVEN IN THE QUESTION
    struct Primary_node *head = new Primary_node;
    head->_primary_data = 5;
    head->_right = new Node;
    head->_right->_data = 50;
    head->_right->_right = new Node;
    head->_right->_right->_data = 25;

    head->_down = new Primary_node;
    head->_down->_primary_data = 2;
    head->_down->_right = new Node;
    head->_down->_right->_data = 36;

    head->_down->_down = new Primary_node;
    head->_down->_down->_primary_data = 11;
    head->_down->_down->_right = new Node;
    head->_down->_down->_right->_data = 77;
    head->_down->_down->_right->_right = new Node;
    head->_down->_down->_right->_right->_data = 121;
    head->_down->_down->_right->_right->_right = new Node;
    head->_down->_down->_right->_right->_right->_data = 33;

    ////////////////////////////////////////////////////////////////////////////////////////
    // solution including print statements:

    cout << "----Before inserting----" << endl;
    print(head);
    insert_by_terms(head, 3);
    cout << "----After inserting 3----" << endl;
    print(head);
    insert_by_terms(head, 39);
    cout << "----After inserting 39----" << endl;
    print(head);

    return 0;
}

// ---------- Functions ----------
void insert_by_terms(struct Primary_node *head, const int data)
{
    /*
    the rules are:
    if data is % [one of primary nodes] = 0
    then we insert it in to the right using the
    node ptr (alloc node)
     if it doesnt divide by any of the primary nodes
     we insert it in the down
     but its smallest primary divi

     */

    struct Primary_node *tail = head;
    if (insert_to_right(head, data,tail))
    {
        cout << "inserted to right" << endl;
        return;
    }
    /// you can make this code even better, and save this current
    // and pass it into the allocate_new_primary already,
    // instead of getting in again
    // which in the edge cases of all input numbers are primes
 
    // we need to find the smallest primary div of the number
    int smallest = smallest_primary_div(data);

    // we are at this point, so if it wasnt divided by
    //  other primary nodes
    // now we have the smallest primary divi
    // now we insert this smallest primary as a new primary node
    //  and the value itself to the right
    allocate_new_primary(tail, data, smallest);
    return;
}
void allocate_new_primary(struct Primary_node *current, const int data, const int smallest)
{

    // if we reach here, so the insert to right already updated the current
    // so the "latest" primary node is the one that we need to insert the new primary node after
    if (current->_down == nullptr)
    {
        struct Primary_node *new_primary = new Primary_node;
        check_alloc(new_primary);
        new_primary->_primary_data = smallest;
        new_primary->_right = new Node;
        check_alloc(new_primary->_right);
        new_primary->_right->_data = data;
        current->_down = new_primary;
        return; // can use break aswell
    }
}

int smallest_primary_div(const int data)
{
    for (int i = 2; i <= data; i++) // NOT data/2
    {
        if (data % i == 0)
        {
            return i;
        }
    }
    return 0;
}

bool insert_to_right(struct Primary_node *head, const int data, struct Primary_node *&tail)
{
    struct Primary_node *current = head;
    while (current != nullptr)
    {
       
        if (data % current->_primary_data == 0)
        {
            // insert to the right
            struct Node *new_node = new Node;
            check_alloc(new_node);
            new_node->_data = data;
            new_node->_right = current->_right;
            current->_right = new_node;
            return true;
        }
        if (current->_down == nullptr)
        {
            tail = current;
        }
        current = current->_down;
    }
    return false;
}

void check_alloc(void *ptr)
{
    if (ptr == nullptr)
    {
        cout << "Memory allocation failed" << endl;
        exit(1);
    }
}

////////////PRINT FUNCTION FOR DEBUGGING
void print(struct Primary_node *head)
{
    cout << "=========PRINTING THE LIST =================" << endl;
    struct Primary_node *current = head;
    while (current != nullptr)
    {
        cout << "Primary Node: " << current->_primary_data << endl;
        struct Node *current_right = current->_right;
        while (current_right != nullptr)
        {
            cout << "Right Node: " << current_right->_data << endl;
            current_right = current_right->_right;
        }
        current = current->_down;
    }
}
