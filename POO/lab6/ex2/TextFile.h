#pragma once
#include "FSComponent.h"

class TextFile : public FSComponent {
private:
    unsigned int size; // Size in bytes

public:
    TextFile(const std::string& p, unsigned int s);
    unsigned int getSize() const override;
    void display(int indent = 0) const override;
};
