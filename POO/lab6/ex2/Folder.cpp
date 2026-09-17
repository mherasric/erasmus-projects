#include "Folder.h"
#include <iostream>

// Constructor just sets the folder path
Folder::Folder(const std::string& p)
    : FSComponent(p) {}

// Add a new file or folder to this folder
void Folder::addComponent(std::shared_ptr<FSComponent> comp) {
    contents.push_back(comp);
}

// Calculate total size of folder (including all subfolders and files)
unsigned int Folder::getSize() const {
    unsigned int totalSize = 0;
    for (const auto& item : contents) {
        totalSize += item->getSize();  // Polymorphic call
    }
    return totalSize;
}

// Display the folder path and contents
void Folder::display(int indent) const {
    std::cout << std::string(indent, ' ') << "Folder: " << path
              << " (" << getSize() << " bytes total)" << std::endl;

    for (const auto& item : contents) {
        item->display(indent + 4);  // Indent sub-items
    }
}
