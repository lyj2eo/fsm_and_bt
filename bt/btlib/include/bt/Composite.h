#pragma once

#include "Node.h"

#include <memory>
#include <vector>

namespace bt {

// 자식 노드들을 가지는 노드의 공통 베이스. Sequence / Selector 가 이걸 상속한다.
//
// addChild 는 이미 구현되어 있다 (학습 포인트가 아니므로 미리 제공).
class Composite : public Node {
public:
    void addChild(std::unique_ptr<Node> child);

protected:
    std::vector<std::unique_ptr<Node>> children_;
};

} // namespace bt
