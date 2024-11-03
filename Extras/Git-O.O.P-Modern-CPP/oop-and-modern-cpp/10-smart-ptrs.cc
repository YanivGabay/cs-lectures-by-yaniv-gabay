#include <iostream>
#include <memory>
#include <vector>

using std::vector;

class MyClass {
public:
    MyClass() {
        std::cout << "MyClass Constructor" << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass Destructor" << std::endl;
    }
    void display() {
        std::cout << "Hello from MyClass!" << std::endl;
    }
};

int main() {
    // Using unique_ptr
    std::unique_ptr<MyClass> uniquePtr = std::make_unique<MyClass>();
    uniquePtr->display();
    //its very common to use unique ptr
    // they allow one instance of that object, and when that object is finished,
    // we dont need to worry about a ptr left.
    ////////////////// Last year code for comprasion
    // struct Node* head = SOME NEW ALLOCATION
    // when we finished with that head, if we remebered to make it nullptr again.
    // with unique ptr it does it for us, after the desconstructs of the object.
    

    //you can create a vector of unique ptr of that class
    //vectors, where each cell is a unique ptr, to that class.
    std::vector<std::unique_ptr<MyClass>> new_vector;
        

    // Using shared_ptr
    //they allow multiply instanced of that same ptr, or multiply ptr to the same value.
    //we can always check the count, of how many ptrs, are to that object.
    //usefull for stuff you want to share.
    
    std::shared_ptr<MyClass> sharedPtr1 = std::make_shared<MyClass>();
    {
        std::shared_ptr<MyClass> sharedPtr2 = sharedPtr1;
        sharedPtr2->display();
        std::cout << "Shared pointer count: " << sharedPtr1.use_count() << std::endl;
    }
    std::cout << "Shared pointer count after inner scope: " << sharedPtr1.use_count() << std::endl;
    //there are more pointrs avaialble, but those are in my opinion will be the most important.

    return 0;
}