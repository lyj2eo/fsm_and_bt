#pragma once

#include "Status.h"

#include <string>

namespace bt {

class Node {
public:
    virtual ~Node() = default;

    virtual Status tick() = 0;
    virtual std::string name() const = 0;
};

} // namespace bt
