#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using std::vector;

int main()
{
    // Create a vector of integers
    /**
     * @brief A vector of integers initialized with values from 1 to 5.
     *  [1][2][3]
     * pop_back
     * [1][2]
     * This vector is used to demonstrate basic operations on the std::vector container.
     *
     * Operations that can be performed on this vector include:
     * - Accessing elements using the `at()` method or the subscript operator `[]`.
     * - Adding elements using the `push_back()` method.
     * - Removing elements using the `pop_back()` method.
     * - Inserting elements at a specific position using the `insert()` method.
     * - Removing elements from a specific position using the `erase()` method.
     * - Checking the size of the vector using the `size()` method.
     * - Checking if the vector is empty using the `empty()` method.
     * - Clearing all elements from the vector using the `clear()` method.
     * - Iterating over the elements using iterators.
     *

     */
    //vector is a dynamic size sequence container (similar to arrays)
    vector<int> array;
    array.push_back(5);
    std::cout << "array[0]:" << array[0] << std::endl;
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    numbers.push_back(6);             // Adds 6 to the end of the vector
    int firstElement = numbers.at(0); // Accesses the first element (1)
    numbers.pop_back();               // Removes the last element (6)
    
    // Print the elements of the vector
    std::cout << "Vector elements: ";
    //range loops, similar to python
    for (int num : numbers)
    {
        std::cout << num << " ";
    }

    std::cout << std::endl;
    //you can also use the auto keyword for automated type deduction
    //it is even recommended to use it in most cases
    for (auto num : numbers)
    {
        std::cout << num << " ";
    }

    // Calculate the sum of the elements
    int sum = std::accumulate(numbers.begin(), numbers.end()-2, 0);
    std::cout << "\n Sum of elements: " << sum << std::endl;

    // Find the maximum element
    auto maxElement = std::max_element(numbers.begin(), numbers.end());
    std::cout << "Maximum element: " << *maxElement << std::endl;

    // Sort the vector in descending order
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());

    
    std::cout << "Vector elements after sorting in descending order: ";
    for (int num : numbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}