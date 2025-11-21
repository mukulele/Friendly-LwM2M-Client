
// JsonConfig: Reads a JSON config file and provides access to parameters.
// Requires nlohmann_json (https://github.com/nlohmann/json)
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

class JsonConfig {
public:
    // Load config from file. Returns true on success.
    bool load(const std::string& filename);

    // Get a value by key (returns std::nullopt if not found or wrong type)
    template<typename T>
    std::optional<T> get(const std::string& key) const {
        if (!data_.contains(key)) return std::nullopt;
        try {
            return data_.at(key).get<T>();
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    nlohmann::json data_;
};
