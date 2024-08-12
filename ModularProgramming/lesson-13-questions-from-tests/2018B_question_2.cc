






#include <iostream>

struct Node
{
    int _data;
    struct Node* _left;
    struct Node* _right;
};

struct Node* newNode(int data);
int countNodes(Node* root);
void collectValues(Node* root, int* arr, int& arrIndex);
int findValueIfBST(Node* root, int index);
void sortArray(int arr[], int size);

using std::cout;
using std::endl;

int main()
{
    struct Node* root = newNode(17);
    root->_left = newNode(112);
    root->_right = newNode(252);
    root->_left->_left = newNode(102);
    root->_left->_right = newNode(141);
    root->_right->_left = newNode(223);
    root->_right->_right = newNode(325);
   
    root->_left->_left->_right = newNode(13);
    root->_left->_right->_left = newNode(11);
    root->_right->_left->_left = newNode(20);

  
    root->_right->_right->_right = newNode(37);
    cout << "finished building the tree" << endl;

    cout << "The value at index 3 is: " << findValueIfBST(root, 3) << endl;
    cout << "The value at index 5 is: " << findValueIfBST(root, 5) << endl;
    cout << "The value at index 7 is: " << findValueIfBST(root, 7) << endl;

    for (int i = 0; i < 10; i++)
    {
        cout << "The value at index " << i << " is: " << findValueIfBST(root, i) << endl;
    }

    return 0;
}

int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    } else {
        return 1 + countNodes(root->_left) + countNodes(root->_right);
    }
}

void collectValues(Node* root, int* arr, int& arrIndex) {
    if (root != NULL) {
        collectValues(root->_left, arr, arrIndex);
        arr[arrIndex++] = root->_data;
        collectValues(root->_right, arr, arrIndex);
    }
}

// Function to find the value at the given index if the tree were a BST
int findValueIfBST(Node* root, int index) {
    int n = countNodes(root);
    int* arr = new int[n];
    int arrIndex = 0;
    collectValues(root, arr, arrIndex);

    // i 95% sure you dont need to write this function
    // just say we use use quicksort which will be at most cases O(nlogn)
    sortArray(arr, n);

    // Check if the index is within the bounds
    if (index >= 0 && index < n) {
        int result = arr[index];
        delete[] arr; // Clean up the allocated memory
        return result;
    } else {
        delete[] arr; // Clean up the allocated memory
        
    }
}


Node* newNode(int data)
{
    struct Node* node = new Node;
    node->_data = data;
    node->_left = NULL;
    node->_right = NULL;
    return(node);
}

//basic bubble sort just for this to work
//DONT USE IT,ITS O(n^2) AND BAD INDEXES NAMES
void sortArray(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}