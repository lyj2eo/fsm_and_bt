#pragma once

#include "Node.h"

#include <memory>
#include <vector>

namespace bt {

class Composite : public Node {
public:
    void addChild(std::unique_ptr<Node> child);

protected:
    std::vector<std::unique_ptr<Node>> children_;
};

} // namespace bt
