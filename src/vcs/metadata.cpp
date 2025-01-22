//
// Created by Timmie on 22.01.2025.
//

#include <iostream>
#include <fstream>
#include "metadata.hpp"
#include <stdexcept>


void MetadataHandler::save(const std::string& filepath, const std::map<std::string, std::string>& data) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing");
    }

    for (const auto& [tag, value] : data) {
        file << META_START_MARKER << tag << META_END_MARKER << META_DATA_SEPARATOR;
        file << value << META_BLOCK_SEPARATOR;
    }
}

std::map<std::string, std::string> MetadataHandler::load(const std::string& filepath) {
    std::map<std::string, std::string> data;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for reading");
    }

    std::string line;
    std::string current_tag;
    std::string current_value;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.front() == META_START_MARKER && line.back() == META_END_MARKER) {
            if (!current_tag.empty()) {
                data[current_tag] = current_value;
                current_tag.clear();
                current_value.clear();
            }
            current_tag = line.substr(1, line.size() - 2);
        }
        else {
            if (current_tag.empty()) {
                throw std::runtime_error("Data found without a tag");
            }
            current_value = line;
            data[current_tag] = current_value;
            current_tag.clear();
        }
    }

    if (!current_tag.empty()) {
        data[current_tag] = current_value;
    }

    return data;
}
