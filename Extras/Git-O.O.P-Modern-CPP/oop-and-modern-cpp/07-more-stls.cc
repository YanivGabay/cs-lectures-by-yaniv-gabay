#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <numeric>

// Example 1: String manipulations using std::string and STL algorithms
void exampleStringManipulations() {
    std::string str = "Hello, World!";
    
    // Convert to uppercase
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    std::cout << "Uppercase: " << str << std::endl;

    // Reverse the string
    std::reverse(str.begin(), str.end());
    std::cout << "Reversed: " << str << std::endl;

    // Count occurrences of a character
    char ch = 'L';
    int count = std::count(str.begin(), str.end(), ch);
    std::cout << "Count of '" << ch << "': " << count << std::endl;

    // Remove all spaces
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    
    std::cout << "Without spaces: " << str << std::endl;
}

// Example 2: Using std::vector and std::sort
void exampleVectorSort() {
    std::vector<int> numbers = {5, 3, 1, 4, 2};
    
    std::sort(numbers.begin(), numbers.end());
    std::cout << "Sorted vector: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}



// Example 3: Using std::set
void exampleSet() {
    
    std::set<int> uniqueNumbers = {1, 2, 3, 4, 5, 5, 4};
    std::cout << "Unique numbers: ";
    for (auto num : uniqueNumbers) {
        std::cout << num << " ";
    }
    
    std::cout << std::endl;
}


int main() {
    exampleVectorSort();
    exampleStringManipulations();
    exampleSet();

    return 0;
}