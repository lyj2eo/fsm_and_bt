#pragma once
#include "Decorator.h"

namespace bt {

class Retry : public Decorator {
public:
    
    explicit Retry(const std::string& name, int max_attempts)
    {
        name_ = name;
        max_attempts_ = max_attempts;
        attempts_ = 0;
    }
    
    Retry() : Retry("Retry", max_attempts_) {}
    
    Status tick() override;

private:
    int max_attempts_;
    int attempts_; 
};
} // namespace bt
