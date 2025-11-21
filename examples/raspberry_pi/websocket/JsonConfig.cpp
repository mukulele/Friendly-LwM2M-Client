#include "JsonConfig.h"
#include <fstream>

bool JsonConfig::load(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return false;
    try {
        in >> data_;
        return true;
    } catch (...) {
        return false;
    }
}
