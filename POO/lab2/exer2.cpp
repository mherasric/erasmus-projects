#include <cassert>
#include <iostream>

// Implementation of my_strchr
const char* my_strchr(const char* str, char character) {
    // Traverse the string using a pointer
    while (*str != '\0') {
        if (*str == character) {
            return str; // Found the character, return pointer to it
        }
        str++; // Move to the next character
    }

    // After the loop, str points to the null terminator
    // If we're searching for the null terminator, return the pointer
    if (character == '\0') {
        return str;
    }

    // Character not found
    return nullptr;
}

void runTests() {
    std::cout << "Running tests...\n";

    // Test 1: Character exists in the middle
    const char* str1 = "hello";
    const char* result1 = my_strchr(str1, 'l');
    assert(result1 != nullptr && *result1 == 'l' && result1 == str1 + 2);
    std::cout << "Test 1 passed: Found 'l' in \"hello\"\n";

    // Test 2: Character at the beginning
    const char* result2 = my_strchr(str1, 'h');
    assert(result2 != nullptr && *result2 == 'h' && result2 == str1);
    std::cout << "Test 2 passed: Found 'h' in \"hello\"\n";

    // Test 3: Character at the end
    const char* result3 = my_strchr(str1, 'o');
    assert(result3 != nullptr && *result3 == 'o' && result3 == str1 + 4);
    std::cout << "Test 3 passed: Found 'o' in \"hello\"\n";

    // Test 4: Character does not exist
    const char* result4 = my_strchr(str1, 'x');
    assert(result4 == nullptr);
    std::cout << "Test 4 passed: 'x' not found in \"hello\"\n";

    // Test 5: Empty string
    const char* str2 = "";
    const char* result5 = my_strchr(str2, 'a');
    assert(result5 == nullptr);
    std::cout << "Test 5 passed: 'a' not found in empty string\n";

    // Test 6: Search for null terminator
    const char* result6 = my_strchr(str1, '\0');
    assert(result6 != nullptr && *result6 == '\0' && result6 == str1 + 5);
    std::cout << "Test 6 passed: Found null terminator in \"hello\"\n";

    std::cout << "All tests passed!\n";
}

int main() {
    runTests();

    // User interaction
    char str[100];
    char ch;
    std::cout << "\nEnter a string: ";
    std::cin.getline(str, 100);
    std::cout << "Enter a character to search for: ";
    std::cin >> ch;

    const char* result = my_strchr(str, ch);
    if (result != nullptr) {
        std::cout << "Character '" << ch << "' found at position: " << (result - str) << "\n";
        std::cout << "Substring from that position: " << result << "\n";
    } else {
        std::cout << "Character '" << ch << "' not found in the string.\n";
    }

    return 0;
}
