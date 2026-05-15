#pragma once
#include "Decorator.h"

namespace bt {

class Retry : public Decorator {
public:
    explicit Retry(int max_attempts){
        max_attempts_ = max_attempts;
        attempts_ = 0;
    };
    Status tick() override;
    std::string name() const override { return "Retry"; }

private:
    int max_attempts_;
    int attempts_;
};
}
