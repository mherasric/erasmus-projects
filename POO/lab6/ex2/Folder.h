#pragma once
#include "FSComponent.h"
#include <vector>
#include <memory>

class Folder : public FSComponent {
private:
    std::vector<std::shared_ptr<FSComponent>> contents;

public:
    Folder(const std::string& p);
    void addComponent(std::shared_ptr<FSComponent> comp);
    unsigned int getSize() const override;
    void display(int indent = 0) const override;
};
