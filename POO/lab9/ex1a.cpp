#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

// -------- LINKED LIST----------

// Template definition for a Node in the linked list(it keeps a data and a pointer)
template <typename T>
class Node {
public:
    T data;            // Data of generic type T(it could be int, float, string, etc)
    Node* next;        // Pointer to the next node

    // Constructor to initialize the node with a value
    Node(T value) {
        data = value;
        next = nullptr;
    }
};

// Template definition for the LinkedList class
template <typename T>
class LinkedList {
private:
    Node<T>* head;     // Pointer to the first node

public:
    // Constructor to initialize an empty list
    LinkedList() {
        head = nullptr;
    }

    // Destructor to clean up memory
    ~LinkedList() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // Function to add a node at the end of the list
    void append(T value) { 
        Node<T>* newNode = new Node<T>(value); //we create a new node with the value
        if (head == nullptr) { //if the list is empty, this node is the first one(head)
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next; //if not, we add it in the end of the lsit, after going through the whole list
            }
            temp->next = newNode;
        }
    }

    // Function to print all elements in the list
    void display() const {
        if (isEmpty()) {
            throw runtime_error("Cannot display: The list is empty.");
        }
        Node<T>* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }

    // Remove first: Delete the first node in the list
    void removeFirst() {
        if (isEmpty()) {
            throw runtime_error("Cannot remove: The list is empty.");
        }
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }

    // isEmpty: Check if the list is empty
    bool isEmpty() const {
        return head == nullptr;
    }

};

// Main function to test the template linked list
int main() {
    // Integer list
    LinkedList<int> intList;

    try {
        intList.append(10);
        intList.append(20);
        intList.append(30);
        intList.display();

        intList.removeFirst();
        intList.display();

        intList.removeFirst();
        intList.removeFirst();
        intList.removeFirst(); // This will throw an exception
    } catch (const exception& e) {
        cout << "Exception caught (intList): " << e.what() << endl;
    }

    // Float list
    LinkedList<float> floatList;
    try {
        floatList.append(1.50);
        floatList.append(5.6);
        floatList.append(4.5);
        floatList.display();

        floatList.removeFirst();
        floatList.removeFirst();
        floatList.removeFirst();
        floatList.display(); // Will throw exception (list is empty)
    } catch (const exception& e) {
        cout << "Exception caught (floatList): " << e.what() << endl;
    }

    // String list
    LinkedList<string> stringList;
    try {
        stringList.append("Hola");
        stringList.append("Templates");
        stringList.display();

        stringList.removeFirst();
        stringList.removeFirst();
        stringList.removeFirst(); // Will throw exception
    } catch (const exception& e) {
        cout << "Exception caught (stringList): " << e.what() << endl;
    }

    return 0;
}

//we are using the exception-handling (throw) when:
// - the list is empty and we call display()
// - the list is empty and we call removeFirst()

//and in the main(), we use try-catch to catch those errors and avoid that the programs stop working
