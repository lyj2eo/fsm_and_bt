#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <cstddef>

namespace bt {

inline std::ostream& operator<<(std::ostream& os, const std::vector<double>& values)
{
    os << "[";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << values[i];
    }
    os << "]";
    return os;
}

class Blackboard {
public:
    using Value = std::variant<int, double, bool, std::string, std::vector<double>>;

    template <typename T>
    void set(const std::string& key, T value)
    {
        if (!this) {
            std::cout << ">> blackboard is null\n";
        }

        std::cout << "[Blackboard] set " << key << " = " << value << std::endl;
        values_.insert({key, value});
    }

    template <typename T>
    void update(const std::string& key, T value)
    {
        if (!this) {
            std::cout << ">> blackboard is null\n";
        }

        std::cout << "[Blackboard] update " << key << " = " << value << std::endl;
        values_.at(key) = value;
    }

    template <typename T>
    T get(const std::string& key) const
    {
        try {
            const auto& value = values_.at(key);
            const T result = std::get<T>(value);

            std::cout << "[Blackboard] get " << key << "\n";

            return result;

        } catch (const std::out_of_range&) {
            std::cout << "[Blackboard] get failed: key '" << key << "' not found\n";
            return T{};

        } catch (const std::bad_variant_access&) {
            std::cout << "[Blackboard] get failed: key '" << key << "' type mismatch\n";
            return T{};
        }
    }

    bool has(const std::string& key) const
    {
        // std::cout << "[Blackboard] has " << key << " = " << (values_.find(key) != values_.end()) << std::endl;
        return values_.find(key) != values_.end();
    }

private:
    std::unordered_map<std::string, Value> values_;
};

} // namespace bt
