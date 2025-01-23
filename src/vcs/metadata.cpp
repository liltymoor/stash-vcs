#include "metadata.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void MetadataHandler::save(
    const std::string& filepath,
    const std::map<std::string, std::string>& data
) {
    std::map<std::string, std::string> all_data;

    if (std::filesystem::exists(filepath)) {
        all_data = load(filepath);
    }

    for (const auto& [tag, value] : data) {
        all_data[tag] = value;
    }

    std::ofstream file(filepath);
    for (const auto& [tag, value] : all_data) {
        file << META_START_MARKER << tag << META_END_MARKER
             << " " << META_DATA_SEPARATOR << " "  // [tag] =
             << value << "\n";                      // value
    }
}

std::map<std::string, std::string> MetadataHandler::load(
    const std::string& filepath
) {
    std::map<std::string, std::string> data;
    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line)) {
        size_t start_pos = line.find(META_START_MARKER);
        size_t end_pos = line.find(META_END_MARKER);

        if (start_pos == std::string::npos || end_pos == std::string::npos) continue;

        std::string tag = line.substr(start_pos + 1, end_pos - start_pos - 1);

        size_t sep_pos = line.find(META_DATA_SEPARATOR, end_pos + 1);
        if (sep_pos == std::string::npos) continue;

        std::string value = line.substr(sep_pos + 1);
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](int ch) {
            return !std::isspace(ch);
        }));

        data[tag] = value;
    }

    return data;
}