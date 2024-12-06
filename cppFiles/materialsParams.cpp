#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

nlohmann::json readJSON(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file) {
        throw std::runtime_error("No chance to read " + fileName + " file");
    }

    nlohmann::json jsonData;
    file >> jsonData;
    return jsonData;
}
