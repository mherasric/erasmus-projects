#include "Folder.h"
#include "TextFile.h"
#include <memory>
#include <iostream>

int main() {
    // Create a root folder
    std::shared_ptr<Folder> root = std::make_shared<Folder>("root");

    // Add text files to root
    root->addComponent(std::make_shared<TextFile>("file1.txt", 1200));
    root->addComponent(std::make_shared<TextFile>("file2.txt", 800));

    // Create a subfolder
    std::shared_ptr<Folder> subFolder = std::make_shared<Folder>("docs");
    subFolder->addComponent(std::make_shared<TextFile>("doc1.txt", 500));
    subFolder->addComponent(std::make_shared<TextFile>("doc2.txt", 250));

    // Add subfolder to root
    root->addComponent(subFolder);

    // Display the file system
    std::cout << "\n--- File System Structure ---\n";
    root->display();

    return 0;
}
