// Advanced C++ - STL Containers and Algorithms
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>

/*
 * NOTE: This is a simplified version of the STL containers and algorithms example.

 *
 * This simplified version focuses on:
 * - Basic sequence containers: vector, list, deque
 * - Basic associative containers: set, map
 * - Simple algorithms: sort, find, min/max
 * - Container operations without heavy template usage
 *
 * STL (Standard Template Library) is a powerful collection of:
 * 1. Containers: Data structures like vector, list, map, set
 * 2. Algorithms: Functions that operate on containers like sort, find, count
 * 3. Iterators: Objects that provide access to container elements
 */

// Helper function to print a vector
void printVector(const std::string& name, const std::vector<int>& vec) {
    std::cout << name << ": ";
    for (int element : vec) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

// Helper function to print a list
void printList(const std::string& name, const std::list<int>& lst) {
    std::cout << name << ": ";
    for (int element : lst) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

// Helper function to print a map
void printMap(const std::string& name, const std::map<std::string, int>& map) {
    std::cout << name << ": ";
    for (const auto& pair : map) {
        std::cout << "{" << pair.first << ": " << pair.second << "} ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "===== STL Containers and Algorithms Example =====" << std::endl;

    // ============ Sequence Containers ============
    std::cout << "\n--- Sequence Containers ---" << std::endl;
    
    /*
     * VECTOR
     * - Dynamic array that grows automatically
     * - Fast random access (constant time)
     * - Fast insertion/removal at the end
     * - Slow insertion/removal in the middle or beginning
     * - Elements stored contiguously in memory
     */
    // Vector - Dynamic array
    std::vector<int> vec = {5, 2, 8, 1, 7, 3};
    printVector("Vector", vec);
    
    // Random access and modification
    vec[2] = 10;
    vec.push_back(9);
    printVector("Modified vector", vec);
    
    // Size and capacity
    std::cout << "Vector size: " << vec.size() << std::endl;
    std::cout << "Vector capacity: " << vec.capacity() << std::endl;
    
    /*
     * LIST
     * - Doubly-linked list
     * - No random access
     * - Fast insertion/removal anywhere
     * - Elements not stored contiguously
     * - More memory overhead per element
     */
    // List - Doubly linked list
    std::list<int> lst = {5, 2, 8, 1, 7, 3};
    printList("List", lst);
    
    // Efficient insertion and removal
    auto it = std::find(lst.begin(), lst.end(), 8);
    if (it != lst.end()) {
        lst.insert(it, 6);  // Insert 6 before 8
    }
    printList("Modified list", lst);
    
    /*
     * DEQUE (Double-Ended Queue)
     * - Similar to vector but efficient at both ends
     * - Fast random access
     * - Fast insertion/removal at beginning and end
     * - Slower insertion/removal in the middle
     */
    // Deque - Double-ended queue
    std::deque<int> dq = {5, 2, 8, 1, 7, 3};
    std::cout << "Deque: ";
    for (int n : dq) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Efficient push/pop at both ends
    dq.push_front(0);
    dq.push_back(9);
    std::cout << "Modified deque: ";
    for (int n : dq) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    // ============ Associative Containers ============
    std::cout << "\n--- Associative Containers ---" << std::endl;
    
    /*
     * SET
     * - Collection of unique keys, sorted by keys
     * - Fast search, insertion, removal (logarithmic time)
     * - Cannot modify elements in-place (would break ordering)
     * - Usually implemented as balanced binary trees
     */
    // Set - Collection of unique keys
    std::set<int> s = {5, 2, 8, 1, 7, 3, 5, 2};  // Duplicates are ignored
    std::cout << "Set: ";
    for (int n : s) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    
    // Fast search
    bool found = s.find(7) != s.end();
    std::cout << "Is 7 in set? " << (found ? "Yes" : "No") << std::endl;
    
    /*
     * MAP
     * - Collection of key-value pairs, sorted by keys
     * - Fast search, insertion, removal (logarithmic time)
     * - Each key has exactly one value
     * - Usually implemented as balanced binary trees
     */
    // Map - Key-value pairs with unique keys
    std::map<std::string, int> ages = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 22}
    };
    printMap("Map", ages);
    
    // Accessing and modifying
    ages["David"] = 28;  // Add new entry
    ages["Bob"] = 31;    // Modify existing
    printMap("Modified map", ages);

    // ============ STL Algorithms ============
    std::cout << "\n--- Basic STL Algorithms ---" << std::endl;
    
    /*
     * ALGORITHMS
     * - Generic functions that work with ranges of elements
     * - Separated from containers, work through iterators
     * - Provide operations like searching, sorting, transforming, etc.
     * - Most are in the <algorithm> header
     */
    std::vector<int> numbers = {5, 2, 8, 1, 7, 3, 9, 4, 6};
    printVector("Original numbers", numbers);
    
    // Sorting
    std::sort(numbers.begin(), numbers.end());
    printVector("Sorted numbers", numbers);
    
    // Finding
    auto find_it = std::find(numbers.begin(), numbers.end(), 7);
    if (find_it != numbers.end()) {
        std::cout << "Found 7 at position: " << std::distance(numbers.begin(), find_it) << std::endl;
    }
    
    // Counting
    int count_gt_5 = 0;
    for (int n : numbers) {
        if (n > 5) count_gt_5++;
    }
    std::cout << "Numbers greater than 5: " << count_gt_5 << std::endl;
    
    // Transform (square each number)
    std::vector<int> squared(numbers.size());
    for (size_t i = 0; i < numbers.size(); ++i) {
        squared[i] = numbers[i] * numbers[i];
    }
    printVector("Squared numbers", squared);
    
    // Accumulate (sum)
    int sum = 0;
    for (int n : numbers) {
        sum += n;
    }
    std::cout << "Sum of all numbers: " << sum << std::endl;
    
    // Min/Max
    int min = *std::min_element(numbers.begin(), numbers.end());
    int max = *std::max_element(numbers.begin(), numbers.end());
    std::cout << "Min: " << min << ", Max: " << max << std::endl;
    
    return 0;
} 