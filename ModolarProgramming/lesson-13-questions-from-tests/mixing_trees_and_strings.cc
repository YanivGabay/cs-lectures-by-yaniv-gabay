






#include <iostream>
#include <climits>
#include <cstring>
struct Node
{
    char _data;
    struct Node* _left;
    struct Node* _right;
};

struct Node* newNode(char data);
void question_1(struct Node* root);
void inorder(struct Node* root, int letters[]);
void found_letters(int letters[]);


void question_2(struct Node*root , char* name);
bool name_exists(int letters[],char* name);
const int NUM_OF_LETTERS = 26;

using std::cout;
using std::endl;

int main()
{
    struct Node* root = newNode('y');
    root->_left = newNode('a');
    root->_right = newNode('a');
    root->_left->_left = newNode('n');
    root->_left->_right = newNode('i');
    root->_right->_left = newNode('n');
    root->_right->_right = newNode('s');

   

    cout << "finished building the tree" << endl;


    /////// 3 Questions:
    // the tree contains only letters (ranging from a-z so total of 26 letters)
    // the tree is not search tree

    // 1. given a tree of chars, we need to return the letter by the inorder traversal (if it was a binary search tree)
    // 2. given a tree of chars, and a given string name: "yaniv" we must say if its exists (not in order)
    // 3. given a tree of chars, and a given string name: "yan" we must say if exists in order and how many times


    //1
    // inorder traversal just mean sorted from lowest to highest
    // so it doesnt matter how we traverse this tree cus ITS NOT A SEARCH TREE
    // we will host all the letters in a 26 size int array
    question_1(root);
    char name[] = "yaa";
    question_2(root, name);


    return 0;
}

void question_2(struct Node* root,char* name)
{
    //2
    // we need to check if the string "yaniv" exists in the tree
    // we use the same method as question 1 , and just check after if the string exists
    int letters[NUM_OF_LETTERS] = {0};

    inorder(root,letters);

    bool found = name_exists(letters,name);

    if (found )
    cout << "found the string" << endl;
    else
    cout << "not found" << endl;
}

bool name_exists(int letters[], char* name)
{
    cout << "str len of name is:" << strlen(name) << endl;
    int counter = 0;
    for (int i = 0; i < strlen(name); i++)
    {
        if(letters[name[i]-'a']>0)
        {
                counter++;
                //this is for cases we have duplicated, we want to make sure
                //for strings like "nnn" , it will still work
                letters[name[i]-'a']--;
        }
        else return false;
    
    }
    return true;
    


}

void question_1(struct Node* root)
{
    if (root == NULL)
    {
        return;
    }
     //why int array? each cell represent a letter
    // and the value in the cell will represent the number of times we saw this letter
    // than we can print it out in order

    int letters[NUM_OF_LETTERS] = {0};

    inorder(root, letters);
    found_letters(letters);


}

//quiz what is the running time of this general question
//and specifically for this function
void found_letters(int letters[])
{
    for (int i = 0; i < NUM_OF_LETTERS; i++)
    {
        if (letters[i] > 0)
        {
            for (int j = 0; j < letters[i]; j++)
            {
                cout << (char)(i + 'a') << " ";
            }
          
        }
    }
    cout << endl;
}
void inorder(struct Node* root, int letters[])
{
    if (root == NULL)
    {
        return;
    }

    inorder(root->_left, letters);
    //if root->_data is 'a' than 'a' - 'a' = 0 cell
    //if root->_data is 'b' than 'b' - 'a' = 1 cell 
    // etc
    letters[root->_data - 'a']++;

    inorder(root->_right, letters);
}

Node* newNode(char data)
{
    struct Node* node = new Node;
    node->_data = data;
    node->_left = NULL;
    node->_right = NULL;
    return(node);
}
