#pragma once

#include "Status.h"

#include <string>

namespace bt {

// BT 의 모든 노드(액션, 컨디션, 컴포지트, 데코레이터 ...)의 공통 인터페이스.
//
//   tick() — 한 번 평가하고 결과를 돌려준다.
//   name() — 디버그 출력에 쓸 이름.
class Node {
public:
    virtual ~Node() = default;

    virtual Status tick() = 0;
    virtual std::string name() const = 0;
};

} // namespace bt
