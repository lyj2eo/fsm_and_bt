#include "bt/Blackboard.h"
#include <iostream>

namespace bt {

void Blackboard::set(const std::string& key, double value) 
{
    values_[key] = value;
    std::cout << "[Blackboard] set " << key << " = " << value << std::endl;
}

double Blackboard::get(const std::string& key, double default_value) const 
{
    auto it = values_.find(key);
    if (it == values_.end()) {
        return default_value;
    }
    std::cout << "[Blackboard] get " << key << " = " << std::get<double>(it->second) << std::endl;

    return std::get<double>(it->second);
}

bool Blackboard::has(const std::string& key) const 
{
    std::cout << "[Blackboard] has " << key << " = " << (values_.find(key) != values_.end()) << std::endl;
    return values_.find(key) != values_.end();
}

} // namespace bt