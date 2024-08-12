/********************************************************************
 * Course: Modolar Programming
 * Lecture: 13 - tests
 * File: 2022A-1-avg-salary-diff.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-07-26
 * 
 * Overview:
 * 
 * 
 * Notes:
 * Any additional notes related to this file.
 *******************************************************************/

// ---------- Include Section ----------
#include <iostream>
// Add more #include directives as needed
#include <algorithm>
// ---------- Using Section ----------
using std::cout;
using std::cin;
using std::endl;

// ---------- Constants ----------


// ---------- Structs ----------

struct Node {
    int _salary;
    struct Node *_left, *_right;

};

struct NodeSalaryDiff {

    int _sum_diff;
   
    int _count_bigger; // Change the member name from '_count_bigger_nodes' to '_count_bigger'
};
// ---------- Function Prototypes ----------
double calc_avg_delta(const Node *root);
int calc_avg_delta_helper(const Node *root, NodeSalaryDiff &info_stats);


// ---------- Main Function ----------
int main() {
    // Main function code
    //lets build a ugly primitive tree using
    //the question example to test

    Node *root = new Node{100000, nullptr, nullptr};
    root->_left = new Node{30000, nullptr, nullptr};
    root->_right = new Node{70000, nullptr, nullptr};
    root->_left->_left = new Node{13000, nullptr, nullptr};
    root->_left->_right = new Node{20000, nullptr, nullptr};
    root->_left->_right->_right = new Node{26000, nullptr, nullptr};
    root->_left->_left->_left = new Node{35000, nullptr, nullptr};
    root->_left->_left->_right = new Node{20000, nullptr, nullptr};
    
    cout << "Finished building the tree" << endl;

    double avg = calc_avg_delta(root);
    cout << "The average diff between managers who earn less than their employes is: " << avg << endl;
    
    return 0;
}

// ---------- Functions ----------
double calc_avg_delta(const Node *root) {
    // so what we need to do
    // each node will need to return:
    // the max salary in the subtree
    // the sum of difference between the max salary and the salary of the node
    // the number of nodes, that have an employee (any child) that bigger than them.
    if (root == nullptr) {
        return 0;
    }
    //we will use a helper function
    struct NodeSalaryDiff info_stats = {0, 0};
    calc_avg_delta_helper(root, info_stats);

    if (info_stats._count_bigger == 0) {
        return 0;
    }

    return (double)info_stats._sum_diff / info_stats._count_bigger;
}

int calc_avg_delta_helper(const Node *root, NodeSalaryDiff &info_stats) {
    if (root == nullptr) {
        return 0;
    }
    //post order traversal
    int left_max = calc_avg_delta_helper(root->_left, info_stats);
    int right_max = calc_avg_delta_helper(root->_right, info_stats);

    //if both left and right are 0, then we are in a leaf
    //so we return the salary of the node
    if (left_max == 0 && right_max == 0) {
        return root->_salary;
    }

    
    int max_salary = std::max(root->_salary, std::max(left_max, right_max));
    
    if (root->_salary < max_salary) {
        info_stats._count_bigger++;
        info_stats._sum_diff += (max_salary - root->_salary);
    }


    return max_salary;
}