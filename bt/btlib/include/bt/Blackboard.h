#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

namespace bt {

class Blackboard {
public:
    using Value = std::variant<int, double, std::vector<double>>;

    // void set(const std::string& key, int value);
    void set(const std::string& key, double value);
    // void set(const std::string& key, std::vector<double> value);

    // int get(const std::string& key, int default_value = 0) const;
    double get(const std::string& key, double default_value = 0.0) const;
    // std::vector<double> get(const std::string& key, const std::vector<double>& default_value = {}) const;
    // TODO Template

    bool has(const std::string& key) const;

private:
    std::unordered_map<std::string, Value> values_;
};

} // namespace bt