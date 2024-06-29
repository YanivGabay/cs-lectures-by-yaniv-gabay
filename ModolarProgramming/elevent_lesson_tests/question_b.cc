

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
    int _data;
    struct Node * _left;
    struct Node * _right;
};
// Add more structs as needed

// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main() {
    
}

// ---------- Functions ----------

// // ---------- Functions ----------
unsigned get_2child_from_1child(const struct Node *root)  
{  
    unsigned has_2child ;  
    return get_2child_from_1child(root, has_2child) ;  
}  
//**************************************************************************
unsigned get_2child_from_1child(const struct Node *root, unsigned &has_2child)  
{  
    if (root == NULL)  
        return NULL ;  
     
    unsigned has_2child_left = 0, has_2child_right = 0 ;  
    unsigned has_2child_from_1child_left = get_2child_from_1child(root->_left, has_2child_left) ;  
    unsigned has_2child_from_1child_right = get_2child_from_1child(root->_right, has_2child_right) ;  
     
    has_2child = has_2child_left + has_2child_right ;  
    unsigned has_2child_from_1child = has_2child_from_1child_left + has_2child_from_1child_right ;  
     
    if (root->_left != NULL && root->_right != NULL)  
        has_2child ++ ;  
    else if (root->_left != NULL || root->_right != NULL)  
    {  
        has_2child_from_1child += has_2child ;  
        has_2child = 0 ;  
    }  
    return has_2child_from_1child ;  
}  
