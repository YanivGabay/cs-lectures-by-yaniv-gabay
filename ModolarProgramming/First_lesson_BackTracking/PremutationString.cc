/********************************************************************
 * Course: Course Name
 * Lecture: Lecture 1 - BackTracking
 * File: PremutationString.cc
 * 
 * Author: Yaniv Gabay
 * Date: 2024-04-02
 * 
 * Overview:
 * small example of premutation of a stringt
 * PAY atteinion, you havnt learn std::string
 * or std::swap
 * but it is just for learning porpuses
 * stdstring is simillar to the array of chars
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


// ---------- Function Prototypes ----------
void permute(std::string a, int l, int r) ;

// ---------- Main Function ----------
int main() {
    std::string str = "ABC";
    int n = str.size();
    cout << "n is equal to:" << n << endl;
    permute(str, 0, n - 1);
    return 0;
}

// ---------- Functions ----------
//we start here, when l = 0 and r = n-1 which is  2
void permute(std::string a, int l, int r) {
    // Base case: if the left and right indices are the same, we've found a permutation
    if (l == r)
    {
        cout << "Permutation: " << a << endl;
        return;
    }
    else {
        // Permutation made by swapping each character with every other character
        for (int i = l; i <= r; i++) {
            // Swap characters at indices l and i
            cout << "Swapping " << a[l] << " with " << a[i] << endl;
            std::swap(a[l], a[i]);
            cout << "After swapping: " << a << endl;

            // Recurse for the rest of the string
            permute(a, l + 1, r);

            // Backtrack: swap back to the original configuration
            cout << "Backtracking: swapping " << a[l] << " back with " << a[i] << endl;
            std::swap(a[l], a[i]);
            cout << "After backtracking: " << a << endl;
        }
    }
}