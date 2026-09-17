// -----------------STACK----------------
// A stack is a data structure which works as:
// -The last element in come in is the first one is come out
// -You can only add or remove elements from above

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// Template class for a Stack 
template <typename T>
class Stack {
private:
    T arr[100];     // Array to hold stack elements (max size 100)
    int top;        // Index of the top element, to know where is the last element

public:
    // Constructor to initialize the stack
    Stack() {
        top = -1;   // Stack is initially empty
    }

    // Push: Add element to the top of the stack
    void push(T value) {
        if (top >= 99) {
            throw overflow_error("Stack overflow: Cannot push to a full stack.");
        }
        arr[++top] = value; //we add an element on the top of the stack, so we increase the top
    }

    // Pop: Remove and return the top element
    T pop() {
        if (isEmpty()) {
            throw underflow_error("Stack underflow: Cannot pop from an empty stack.");
        }   
        return arr[top--];
    }

    // Peek: See the top element without removing it
    T peek() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty: Cannot peek.");
        }
        return arr[top];
    }

    // isEmpty: Check if the stack is empty
    bool isEmpty() {
        return top == -1;
    }
};

// Main function to test the Stack with different data types
int main() {
    // Stack for integers
    Stack<int> intStack;

    try {
        intStack.push(1);
        intStack.push(2);
        intStack.push(3);
        cout << "Integer Stack top: " << intStack.pop() << endl;
        cout << "Integer Stack top: " << intStack.pop() << endl;
        cout << "Integer Stack top: " << intStack.pop() << endl;
        // This pop will trigger an exception
        cout << "Integer Stack top: " << intStack.pop() << endl;
    } catch (const exception& e) {
        cout << "Exception caught (intStack): " << e.what() << endl;
    }

    // Stack for floats
    Stack<float> floatStack;

    try {
        floatStack.push(3.14f);
        floatStack.push(2.71f);
        cout << "Float Stack top: " << floatStack.pop() << endl;
        cout << "Float Stack top: " << floatStack.pop() << endl;
        cout << "Float Stack top: " << floatStack.pop() << endl; // Will cause exception
    } catch (const exception& e) {
        cout << "Exception caught (floatStack): " << e.what() << endl;
    }

    // Stack for strings
    Stack<string> stringStack;

    try {
        stringStack.push("Hello");
        stringStack.push("World");
        cout << "String Stack top: " << stringStack.peek() << endl;
        cout << "String Stack top: " << stringStack.pop() << endl;
        cout << "String Stack top: " << stringStack.pop() << endl;
        cout << "String Stack top: " << stringStack.pop() << endl; // Will cause exception
    } catch (const exception& e) {
        cout << "Exception caught (stringStack): " << e.what() << endl;
    }

    return 0;
}