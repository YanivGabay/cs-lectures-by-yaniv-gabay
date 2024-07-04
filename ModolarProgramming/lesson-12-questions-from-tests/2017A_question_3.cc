

/********************************************************************
 * Course: Course Name
 * Lecture: Lecture Number - Lecture Title
 * File: question_a.cc
 * 
 * Author: Your Name
 * Date: 2024-06-CURRENT_DAY
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
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------
const int MAX_SIZE = 100;
// Add more constants as needed

// ---------- Structs ----------
struct Node {
    int data;
    struct Node * next;
};
// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main() {
    // we already did this in @ModolarProgramming/Seventh_lesson_linked_lists/07_given_linked_list.cc
}

// ---------- Functions ----------
void removeDuplicates(Node*& head)
{
    Node* current = head;
    while(current != nullptr)
    {
        Node* temp = current;
        while(temp->next != nullptr)
        {
            if(current->data == temp->next->data)
            {
                Node* duplicate = temp->next;
                temp->next = temp->next->next;
                delete duplicate;
            }
            else
            {
                temp = temp->next;
            }
        }
        current = current->next;
    }
}

//if the list is sorted we can remove dupes in O(n) time complexity 
// total nlogn + n = nlogn
void removeDuplicates(struct Node** head_ref) {
    struct Node* current = *head_ref;

    // Pointer to store the next pointer of a node to be deleted
    struct Node* next_next;

    // Traverse the list till last node
    while (current->next != NULL) {
        // If current node is equal to next node
        if (current->data == current->next->data) {
            next_next = current->next->next;
            delete current->next;
            current->next = next_next;
        } else {
            current = current->next; // only advance if no deletion
        }
    }
}