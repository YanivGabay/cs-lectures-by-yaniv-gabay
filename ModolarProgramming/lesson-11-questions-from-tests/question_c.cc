

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
 struct Point {
        int _x;
        int _y;
    } ;

struct Node {
    int _id;
    struct Point _data;
    struct Node * _left;
    struct Node * _right;
};
// Add more structs as needed

// ---------- Function Prototypes ----------

// ---------- Main Function ----------
int main() {
  
}

// ---------- Functions ----------

int sub_same_quarter(const struct Node *root, 
                     int &max_counter, 
                     int &max_quarter, 
                     const struct Node *&max_node)  
{ 
    int left, right; 
     
    if(root == NULL) 
        return 0; 
     
    if(root->_left == NULL && root->_right == NULL)  
    { 
        if(max_counter < 1)  
        { 
            max_counter = 1; 
            max_node = root; 
            max_quarter = quarter(root); 
        } 
        return 1; 
    } 
     
    left = sub_same_quarter(root->_left, max_counter, max_quarter, max_node); 
    right = sub_same_quarter(root->_right, max_counter, max_quarter, max_node); 
     
    if((left != 0 || right != 0) && 
        same_q(root, root->_left) && 
        same_q(root, root->_right))  
    { 
        if(right + left + 1 > max_counter)  
        { 
            max_counter = right + left + 1; 
            max_node = root; 
            max_quarter = quarter(root); 
            return right + left + 1; 
        } 
    }     
    return 0; 
}

//---------------------------------------
bool same_q(const struct Node * root, const struct Node * child)  
{ 
    if(child == NULL) 
        return true; 
     
    if(quarter(root) == quarter(child)) 
        return true; 
     
    return false; 
}

//---------------------------------------
int quarter(const struct Node * root)  
{    
    if(root == NULL) 
        return 0;     
    if(root->_data._x > 0 && root->_data._y > 0) 
        return 1;    
    if(root->_data._x < 0 && root->_data._y > 0) 
        return 2; 
    if(root->_data._x < 0 && root->_data._y < 0) 
        return 3;     
    return 4; 
}
