//
// Created by Timmie on 22.01.2025.
//

#ifndef METADATA_HPP
#define METADATA_HPP

#include <string>
#include <map>

// Format constants
#define META_START_MARKER    '[' ///< Start marker for metadata blocks.
#define META_END_MARKER      ']' ///< End marker for metadata blocks.
#define META_DATA_SEPARATOR  "=" ///< Separator for key-value pairs in metadata.
#define META_BLOCK_SEPARATOR "\n\n" ///< Separator between metadata blocks.

/**
 * @class MetadataHandler
 * @brief Handles saving and loading metadata from files.
 */
class MetadataHandler {
public:
    /**
     * @brief Saves metadata to a file.
     * @param filepath The path to the file.
     * @param data The metadata to save.
     */
    static void save(const std::string& filepath, const std::map<std::string, std::string>& data);

    /**
     * @brief Loads metadata from a file.
     * @param filepath The path to the file.
     * @return A map of key-value pairs representing the metadata.
     */
    static std::map<std::string, std::string> load(const std::string& filepath);
};

#endif // METADATA_HPP