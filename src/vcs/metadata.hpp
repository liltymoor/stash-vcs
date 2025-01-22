//
// Created by Timmie on 22.01.2025.
//

#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <map>

// Константы формата
#define META_START_MARKER    '['
#define META_END_MARKER      ']'
#define META_DATA_SEPARATOR  "\n"
#define META_BLOCK_SEPARATOR "\n\n"

class MetadataHandler {
public:
    static void save(const std::string& filepath, const std::map<std::string, std::string>& data);
    static std::map<std::string, std::string> load(const std::string& filepath);
};

#endif //METADATA_HPP
