

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