


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
    //Finding the largest subtree within given min and max values
}

// ---------- Functions ----------
struct Node *min_to_max_node(struct Node *root,  
        int min,  
        int max) 
{ 
    int max_nodes_counter = 0; 
    struct Node *max_sub_tree; 
     
    check_sub_tree(root, min, max, max_sub_tree, max_nodes_counter); 
       
    if(max_nodes_counter == 0) 
        return NULL; 
     
    return max_sub_tree; 
}

//---------------------------------------
void check_sub_tree(struct Node *root,  
     int min,  
     int max,  
     struct Node *&max_sub_tree,  
     int &max_nodes_counter) 
{ 
    if(root == NULL) 
        return; 
    int nodes_counter = 0; 
     
    if(check_min_max(root, min, max, nodes_counter) )
        if(max_nodes_counter < nodes_counter) 
        { 
            max_nodes_counter = nodes_counter; 
            max_sub_tree = root; 
        } 
    check_sub_tree(root->_left, min, max, max_sub_tree, max_nodes_counter); 
    check_sub_tree(root->_right, min, max, max_sub_tree, max_nodes_counter); 
}

//---------------------------------------
bool check_min_max(const struct Node *root,  
     int min,  
     int max,   
     int &nodes_counter) 
{ 
    if(root == NULL) 
        return true; 
     
    if(root->_data >= min && root->_data <= max) 
    { 
        nodes_counter++; 
        if(check_min_max(root->_left, min, max, nodes_counter) )
            if(check_min_max(root->_right, min, max, nodes_counter) )
                return true; 
    }  
    nodes_counter = 0; 
    return false; 
}
