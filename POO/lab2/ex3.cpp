#include <cassert>
#include <iostream>

// Implementation of find_all
void find_all(const char* str, char character, int* positions, unsigned int cap, unsigned int* len) {
    // Initialize positions array with -1
    for (unsigned int i = 0; i < cap; i++) {
        positions[i] = -1;
    }

    // Initialize len to 0
    *len = 0;

    // Traverse the string using a pointer
    unsigned int pos = 0; // Current position in the string
    unsigned int stored = 0; // Number of positions stored in the array

    while (*str != '\0') {
        if (*str == character) {
            // Increment the total count of occurrences
            (*len)++;
            // Store the position if we haven't reached capacity
            if (stored < cap) {
                positions[stored] = pos;
                stored++;
            }
        }
        str++; // Move to the next character
        pos++; // Increment the position counter
    }

    // Check if we're searching for the null terminator
    if (character == '\0') {
        (*len)++;
        if (stored < cap) {
            positions[stored] = pos;
        }
    }
}

void runTests() {
    std::cout << "Running tests...\n";

    // Test 1: Character appears multiple times
    const char* str1 = "hello";
    int positions1[3] = {-1, -1, -1}; // Capacity 3
    unsigned int len1;
    find_all(str1, 'l', positions1, 3, &len1); // Search for 'l' in "hello"
    assert(len1 == 2 && "There should be 2 'l's in \"hello\"");
    assert(positions1[0] == 2 && positions1[1] == 3 && positions1[2] == -1); // Positions [2, 3]
    std::cout << "Test 1 passed: Found 'l' at positions [2, 3] in \"hello\"\n";

    // Test 2: Character appears once
    int positions2[3] = {-1, -1, -1};
    unsigned int len2;
    find_all(str1, 'h', positions2, 3, &len2);
    assert(len2 == 1 && "There should be 1 'h' in \"hello\"");
    assert(positions2[0] == 0 && positions2[1] == -1 && positions2[2] == -1);
    std::cout << "Test 2 passed: Found 'h' at position [0] in \"hello\"\n";

    // Test 3: Character does not appear
    int positions3[3] = {-1, -1, -1};
    unsigned int len3;
    find_all(str1, 'x', positions3, 3, &len3);
    assert(len3 == 0 && "There should be 0 'x's in \"hello\"");
    assert(positions3[0] == -1 && positions3[1] == -1 && positions3[2] == -1);
    std::cout << "Test 3 passed: 'x' not found in \"hello\"\n";

    // Test 4: Empty string
    const char* str2 = "";
    int positions4[3] = {-1, -1, -1};
    unsigned int len4;
    find_all(str2, 'a', positions4, 3, &len4);
    assert(len4 == 0 && "There should be 0 'a's in empty string");
    assert(positions4[0] == -1 && positions4[1] == -1 && positions4[2] == -1);
    std::cout << "Test 4 passed: 'a' not found in empty string\n";

    // Test 5: Capacity smaller than number of occurrences
    const char* str3 = "banana";
    int positions5[2] = {-1, -1};
    unsigned int len5;
    find_all(str3, 'a', positions5, 2, &len5);
    assert(len5 == 3 && "There should be 3 'a's in \"banana\"");
    assert(positions5[0] == 1 && positions5[1] == 3);
    std::cout << "Test 5 passed: Found 'a' at positions [1, 3] in \"banana\" (cap=2)\n";

    // Test 6: Search for null terminator
    int positions6[3] = {-1, -1, -1};
    unsigned int len6;
    find_all(str1, '\0', positions6, 3, &len6);
    assert(len6 == 1 && "There should be 1 null terminator in \"hello\"");
    assert(positions6[0] == 5 && positions6[1] == -1 && positions6[2] == -1);
    std::cout << "Test 6 passed: Found null terminator at position [5] in \"hello\"\n";

    std::cout << "All tests passed!\n";
}

int main() {
    runTests();

    // User interaction
    char str[100];
    char ch;
    int positions[50];
    unsigned int len;
    unsigned int cap = 50;

    std::cout << "\nEnter a string: ";
    std::cin.getline(str, 100);
    std::cout << "Enter a character to search for: ";
    std::cin >> ch;

    find_all(str, ch, positions, cap, &len);

    if (len == 0) {
        std::cout << "Character '" << ch << "' not found in the string.\n";
    } else {
        std::cout << "Character '" << ch << "' found " << len << " times at positions: ";
        for (unsigned int i = 0; i < cap && positions[i] != -1; i++) {
            std::cout << positions[i];
            if (i < len && (i + 1 < cap && positions[i + 1] != -1)) {
                std::cout << ", ";
            }
        }
        std::cout << "\n";
        if (len > cap) {
            std::cout << "Note: Only the first " << cap << " positions were stored due to capacity limit.\n";
        }
    }

    return 0;
}