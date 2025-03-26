// Advanced C++ - Smart Pointers and Memory Management
#include <iostream>
#include <memory>
#include <string>

/*

 * The simplified version focuses on the basic usage of the three main smart pointer types:
 * - unique_ptr: Exclusive ownership
 * - shared_ptr: Shared ownership with reference counting
 * - weak_ptr: Non-owning reference to shared_ptr managed objects
 */

/*
 * EXPLANATION ABOUT THE RESOURCE CLASS DESIGN:
 * 
 * The Resource class in this example uses a raw pointer (int* data) as a member
 * to simulate resource allocation and demonstrate memory management concepts. 
 * This is designed this way for several reasons:
 * 
 * 1. It shows the RAII (Resource Acquisition Is Initialization) principle - the
 *    Resource class allocates memory in its constructor and deallocates it in its destructor.
 * 
 * 2. This creates a scenario where memory management is necessary - without proper
 *    cleanup, this would cause a memory leak.
 * 
 * 3. The smart pointers (unique_ptr, shared_ptr) then manage instances of this Resource class,
 *    ensuring that the Resource destructor gets called at the appropriate time, which
 *    in turn properly deallocates the raw pointer.
 * 
 * It's actually a nested ownership situation:
 * - The smart pointer owns the Resource object
 * - The Resource object owns the raw int array
 * 
 * This design helps demonstrate how smart pointers provide automatic cleanup even when
 * the objects they manage have their own manual resource management. When the smart pointer
 * goes out of scope, it automatically destroys the Resource object, which in turn calls
 * its destructor that cleans up the raw pointer.
 * 
 * This is a common pattern in C++ where class implementations might use raw pointers internally,
 * but the class itself is managed by smart pointers externally.
 */

// A simple class to demonstrate resource management
class Resource {
private:
    std::string name;
    int* data;

public:
    // Constructor allocates resources
    Resource(const std::string& name) : name(name) {
        std::cout << "Resource '" << this->name << "' created" << std::endl;
        data = new int[100]; // Allocate some memory
        
        // Initialize with some values
        for (int i = 0; i < 100; ++i) {
            data[i] = i;
        }
    }
    
    // Destructor frees resources
    ~Resource() {
        std::cout << "Resource '" << name << "' destroyed" << std::endl;
        delete[] data;
    }
    
    // Access the name
    const std::string& getName() const {
        return name;
    }
    
    // Access data
    int getValue(int index) const {
        if (index < 0 || index >= 100) {
            throw std::out_of_range("Index out of bounds");
        }
        return data[index];
    }
};

// Function to demonstrate using raw pointers
void useRawPointer() {
    std::cout << "\n--- Raw Pointer Example ---" << std::endl;
    
    // Allocate a resource with a raw pointer
    Resource* rawPtr = new Resource("RawResource");
    
    // Use the resource
    std::cout << "Using " << rawPtr->getName() << std::endl;
    std::cout << "Value at index 10: " << rawPtr->getValue(10) << std::endl;
    
    // We must remember to delete the resource!
    delete rawPtr;
    
    // The pointer is now dangling - using it would cause undefined behavior
    // std::cout << rawPtr->getName() << std::endl; // DANGEROUS!
}

// Function to demonstrate unique_ptr
void useUniquePtr() {
    std::cout << "\n--- unique_ptr Example ---" << std::endl;
    
    // Create a unique_ptr to a Resource
    std::unique_ptr<Resource> uniquePtr = std::make_unique<Resource>("UniqueResource");
    
    // Use the resource
    std::cout << "Using " << uniquePtr->getName() << std::endl;
    std::cout << "Value at index 20: " << uniquePtr->getValue(20) << std::endl;
    
    // No need to delete - uniquePtr will automatically delete the resource when it goes out of scope
    
    // Ownership transfer
    std::cout << "\nTransferring ownership:" << std::endl;
    std::unique_ptr<Resource> newOwner = std::move(uniquePtr);
    
    // Original pointer is now null
    if (uniquePtr == nullptr) {
        std::cout << "Original pointer is now null" << std::endl;
    }
    
    // New owner has the resource
    std::cout << "New owner has: " << newOwner->getName() << std::endl;
    
    // Resource will be automatically deleted when newOwner goes out of scope
}

// Function to demonstrate shared_ptr
void useSharedPtr() {
    std::cout << "\n--- shared_ptr Example ---" << std::endl;
    
    // Create a shared_ptr
    std::shared_ptr<Resource> sharedPtr1 = std::make_shared<Resource>("SharedResource");
    
    // Check reference count
    std::cout << "Reference count: " << sharedPtr1.use_count() << std::endl;
    
    // Create another shared_ptr pointing to the same resource
    {
        std::cout << "\nCreating second shared_ptr:" << std::endl;
        std::shared_ptr<Resource> sharedPtr2 = sharedPtr1;
        
        // Both pointers share ownership
        std::cout << "sharedPtr1 points to: " << sharedPtr1->getName() << std::endl;
        std::cout << "sharedPtr2 points to: " << sharedPtr2->getName() << std::endl;
        std::cout << "Reference count: " << sharedPtr1.use_count() << std::endl;
        
        // At the end of this scope, sharedPtr2 is destroyed, but the resource remains
        std::cout << "End of inner scope - sharedPtr2 will be destroyed" << std::endl;
    }
    
    // Reference count decreases, but resource still exists
    std::cout << "After inner scope, reference count: " << sharedPtr1.use_count() << std::endl;
    std::cout << "Resource still accessible: " << sharedPtr1->getName() << std::endl;
    
    // When sharedPtr1 goes out of scope, the resource will be deleted
}

// Function to demonstrate weak_ptr
void useWeakPtr() {
    std::cout << "\n--- weak_ptr Example ---" << std::endl;
    
    // Create a shared_ptr
    std::shared_ptr<Resource> sharedPtr = std::make_shared<Resource>("WeakResource");
    
    // Create a weak_ptr to the resource
    std::weak_ptr<Resource> weakPtr = sharedPtr;
    
    std::cout << "shared_ptr reference count: " << sharedPtr.use_count() << std::endl;
    
    // Using the weak_ptr
    if (auto tempShared = weakPtr.lock()) {
        // Successfully locked - resource exists
        std::cout << "Accessed resource via weak_ptr: " << tempShared->getName() << std::endl;
    }
    
    // Reset the shared_ptr
    std::cout << "\nResetting the shared_ptr:" << std::endl;
    sharedPtr.reset();
    
    // Check if weak_ptr is expired
    std::cout << "Is weak_ptr expired? " << (weakPtr.expired() ? "Yes" : "No") << std::endl;
    
    // Try to use the weak_ptr
    if (auto tempShared = weakPtr.lock()) {
        std::cout << "Resource still exists" << std::endl;
    } else {
        std::cout << "Resource no longer exists" << std::endl;
    }
}

int main() {
    std::cout << "===== Smart Pointers and Memory Management Example =====" << std::endl;
    
    // Demonstrate each pointer type
    useRawPointer();
    useUniquePtr();
    useSharedPtr();
    useWeakPtr();
    
    std::cout << "\nProgram completed successfully" << std::endl;
    return 0;
} 