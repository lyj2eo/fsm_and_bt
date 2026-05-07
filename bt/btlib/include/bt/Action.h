#pragma once

#include "Node.h"

#include <functional>
#include <string>
#include <utility>

namespace bt {

// "한 줄짜리 액션"을 손쉽게 만들기 위한 노드.
// 클래스 상속 없이 람다나 함수 포인터를 넣어 동작을 정의한다.
//
// 사용 예:
//   auto a = std::make_unique<bt::Action>("OpenGripper", []{
//       std::cout << "open\n";
//       return bt::Status::Success;
//   });
class Action : public Node {
public:
    using Func = std::function<Status()>;

    Action(std::string name, Func func)
        : name_(std::move(name)), func_(std::move(func)) {}

    Status tick() override;
    std::string name() const override { return name_; }

private:
    std::string name_;
    Func        func_;
};

} // namespace bt
