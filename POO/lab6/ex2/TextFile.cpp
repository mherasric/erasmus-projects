#include "TextFile.h"
#include <iostream>
#include <string>

// Constructor to initialize path and size
TextFile::TextFile(const std::string& p, unsigned int s)
    : FSComponent(p), size(s) {}

// Simply return the size of the text file
unsigned int TextFile::getSize() const {
    return size;
}

// Print the file's path and size with optional indentation
void TextFile::display(int indent) const {
    std::cout << std::string(indent, ' ') << "TextFile: " << path
              << " (" << size << " bytes)" << std::endl;
}
