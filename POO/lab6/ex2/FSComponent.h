#pragma once
#include <string>

// Abstract base class for all file system components
class FSComponent {
protected:
    std::string path; // Common attribute: the path of the component

public:
    FSComponent(const std::string& p) : path(p) {}
    virtual ~FSComponent() = default;

    // Pure virtual function to get the size of the component
    virtual unsigned int getSize() const = 0;

    // Pure virtual function to display component info
    virtual void display(int indent = 0) const = 0;
};
