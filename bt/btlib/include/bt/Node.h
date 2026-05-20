#pragma once

#include "Status.h"

#include <string>

namespace bt {

class Node {
public:
    virtual ~Node() = default;

    virtual Status tick() = 0;
    const std::string& name() const {return name_;}
protected:
    explicit Node(const std::string& name) : name_(name) {}
    Node() = default;
    std::string name_{""};

};

} // namespace bt
