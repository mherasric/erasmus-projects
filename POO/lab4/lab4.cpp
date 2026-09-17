#include <iostream>
#include <stdexcept>

// Deque class definition to implement a double-ended queue
class Deque {
private:
    int* data;         // Pointer to dynamically allocated array storing elements
    size_t capacity;   // Total capacity of the array
    size_t size;       // Current number of elements
    size_t frontIndex; // Index of the front element
    size_t backIndex;  // Index of the back element

    // Helper function to resize the deque when it becomes full
    void resize() {
        size_t newCapacity = capacity * 2; // Double the current capacity
        int* newData = new int[newCapacity]; // Allocate new larger array
        
        // Copy existing elements to new array in correct order
        for (size_t i = 0; i < size; i++) {
            newData[i] = data[(frontIndex + i) % capacity];
        }

        delete[] data; // Delete old array
        data = newData; // Point to new array
        frontIndex = 0; // Reset front to start
        backIndex = size - 1; // Set back to last element
        capacity = newCapacity; // Update capacity
    }

public:
    // Default constructor - initializes an empty deque
    Deque() : capacity(4), size(0), frontIndex(0), backIndex(0) {
        data = new int[capacity]; // Allocate initial array with capacity 4
    }

    // Destructor - cleans up dynamically allocated memory
    ~Deque() {
        delete[] data; // Free the allocated array
    }

    // Copy constructor - creates a deep copy of another deque
    Deque(const Deque& other) : capacity(other.capacity), size(other.size), 
                               frontIndex(other.frontIndex), backIndex(other.backIndex) {
        data = new int[capacity]; // Allocate new array
        for (size_t i = 0; i < capacity; i++) { // Copy all elements
            data[i] = other.data[i];
        }
    }

    // Copy assignment operator - assigns contents of another deque
    Deque& operator=(const Deque& other) {
        if (this != &other) { // Check for self-assignment
            int* newData = new int[other.capacity]; // Allocate new array
            for (size_t i = 0; i < other.size; i++) { // Copy all elements
                newData[i] = other.data[(other.frontIndex + i) % other.capacity];
            }
            delete[] data; // Delete old data
            data = newData; 
            capacity = other.capacity;
            size = other.size;
            frontIndex = 0;
            backIndex = size - 1;
        }
        return *this; // Return reference to current object
    
    }

    // Adds an element to the front of the deque
    void push_front(int value) {
        if (size == capacity) { // If deque is full, resize it
            resize(); // Increase capacity
        }
        frontIndex = (frontIndex - 1 + capacity) % capacity; // Move front index back
        data[frontIndex] = value; // Insert value
        size++; // Increment size
        if (size == 1) backIndex = frontIndex; // Update back if first element
    }

    // Adds an element to the back of the deque
    void push_back(int value) {
        if (size == capacity) { // If deque is full, resize it
            resize();
        }
        if (size > 0) { // If not empty
            backIndex = (backIndex + 1) % capacity; // Move back index forward
        }
        data[backIndex] = value; //Insert value
        size++; //Increment size
        if (size == 1) frontIndex = backIndex; // Update front if first element
    }
    

    // Removes the front element from the deque
    void pop_front() {
        if (size == 0) { // Check if it is empty
            throw std::runtime_error("Deque is empty");
        }
        frontIndex = (frontIndex + 1) % capacity; // Move front index forward
        size--; //Decrease size
    }

    // Removes the back element
    void pop_back() {
        if (size == 0) {
            throw std::runtime_error("Deque is empty");
        }
        backIndex = (backIndex - 1 + capacity) % capacity; // Move back index back
        size--;
    }

    // Returns the front element without removing it
    int top() const {
        if (size == 0) {
            throw std::runtime_error("Deque is empty");
        }
        return data[frontIndex]; //Return front element
    }

    // Returns the back element without removing it
    int back() const {
        if (size == 0) {
            throw std::runtime_error("Deque is empty");
        }
        return data[backIndex]; //Return back element
    }

    // Stream insertion operator for output
    friend std::ostream& operator<<(std::ostream& os, const Deque& dq) {
        os << "["; // Start with opening bracket
        for (size_t i = 0; i < dq.size; i++) { // Iterate through all elements
            os << dq.data[(dq.frontIndex + i) % dq.capacity]; // Print each element
            if (i < dq.size - 1) os << ", "; // Add comma between elements
        }
        os << "]"; // End with closing bracket
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Deque& dq) { // Stream extraction operator for input
        int value;
        is >> value; // Read value from input
        dq.push_back(value); //Add to back of deque
        return is;
    }

    bool empty() const { return size == 0; } // Helper function to check if deque is empty
    size_t getSize() const { return size; } // Helper function to get current size
};

void runTests() {
    std::cout << "--- Starting All Tests ---\n";

    // Test 1: Basic operations
    std::cout << "Test 1: BASIC OPERATIONS\n";
    Deque dq1; //Creaty empty duque
    dq1.push_back(1); //Add 1 to back
    dq1.push_back(2); //Add 2 to back
    dq1.push_front(0); //Add  to front
    std::cout << "Deque: " << dq1 << "\n"; // Should print [0, 1, 2]
    std::cout << "Top: " << dq1.top() << ", Back: " << dq1.back() << "\n"; // Should be 0, 2
    dq1.pop_front(); //remove 0
    dq1.pop_back(); //remove 2
    std::cout << "After pop: " << dq1 << "\n"; // Should print [1]
    std::cout << "Test 1: END TEST1\n\n";

    // Test 2: Copy constructor
    std::cout << "Test 2: COPY CONSTRUCTOR\n";
    Deque dq2(dq1); //create copy of dq1
    dq2.push_back(3); //modify copy
    std::cout << "Original: " << dq1 << "\n"; // Should print [1]
    std::cout << "Copy: " << dq2 << "\n"; // Should print [1, 3]
    std::cout << "Test 2: END TEST2\n\n";

    // Test 3: Assignment operator
    std::cout << "Test 3: ASSIGNMENT OPERATOR\n";
    Deque dq3; //create new deque
    dq3 = dq2; //asign dq2 to dq3
    dq3.push_front(-1); //modify assigned deque
    std::cout << "Original: " << dq2 << "\n"; // Should print [1, 3]
    std::cout << "Assigned: " << dq3 << "\n"; // Should print [-1, 1, 3]
    std::cout << "Test 3: END TEST3\n\n";

    // Test 4: Empty deque operations
    std::cout << "Test 4: EMPTY DEQUE OPERATIONS\n";
    Deque dq4; //create empty deque
    try {
        dq4.pop_front(); //try to pop from empty deque
    } catch (const std::runtime_error& e) {
        std::cout << "Caught error: " << e.what() << "\n"; // Should catch error
    }
    std::cout << "Empty deque: " << dq4 << "\n"; // Should print []
    std::cout << "Test 4: END TEST4\n\n";

    // Test 5: Resizing
    std::cout << "Test 5: RESIZING\n";
    Deque dq5; //create empty deque
    for (int i = 0; i < 10; i++) {
        dq5.push_back(i); //add 0-9 to back
    }
    std::cout << "Resized deque: " << dq5 << "\n"; // Should print [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    std::cout << "Size: " << dq5.getSize() << "\n"; // Should print 10
    std::cout << "Test 5: END TEST5\n";

    std::cout << "--- All Tests Completed ---\n";
}

int main() {
    std::cout << "Program Start\n";
    runTests();
    std::cout << "Program End\n";
    return 0;
}
