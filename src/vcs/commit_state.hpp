/**
 * @file commit_state.hpp
 * @brief Header file for classes related to commit states and file operations.
 */

#ifndef ITMO_VCS_COMMITSTATE_H
#define ITMO_VCS_COMMITSTATE_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "repository.hpp"

/**
 * @class File
 * @brief Provides static methods for reading and writing files.
 */
class File {
public:
    /**
     * @brief Reads the content of a file.
     * @param filepath The path to the file.
     * @return The content of the file as a string.
     * @throws std::runtime_error If the file cannot be opened.
     */
    static std::string read(const std::string& filepath);

    /**
     * @brief Writes content to a file.
     * @param filepath The path to the file.
     * @param content The content to write.
     * @throws std::runtime_error If the file cannot be opened.
     */
    static void write(const std::string& filepath, const std::string& content);
};

/**
 * @class FileContent
 * @brief Represents the content of a file as a collection of lines.
 */
class FileContent {
public:
    /**
     * @brief Default constructor.
     */
    FileContent() = default;

    /**
     * @brief Constructs a FileContent object from a map of lines.
     * @param lines A map of line numbers to their content.
     */
    explicit FileContent(const std::map<int, std::string>& lines);

    /**
     * @brief Gets the content of a specific line.
     * @param lineNumber The line number.
     * @return The content of the line, or an empty string if the line does not exist.
     */
    std::string getLine(int lineNumber) const;

    /**
     * @brief Gets all lines in the file.
     * @return A map of line numbers to their content.
     */
    const std::map<int, std::string>& getAllLines() const;

    /**
     * @brief Gets the full content of the file as a single string.
     * @return The full content of the file.
     */
    std::string getFullContent() const;

private:
    /**
     * @brief Adds a line to the file content.
     * @param lineNumber The line number.
     * @param line The content of the line.
     */
    void addLine(int lineNumber, const std::string& line);

    std::map<int, std::string> lines; ///< A map of line numbers to their content.
};

/**
 * @class CommitState
 * @brief Represents the state of a commit, including all files and their content.
 */
class CommitState {
public:
    /**
     * @brief Adds a file to the commit state.
     * @param filename The name of the file.
     * @param content The content of the file.
     */
    void addFile(const std::string& filename, const FileContent& content);

    /**
     * @brief Gets the content of a specific file.
     * @param filename The name of the file.
     * @return The content of the file.
     */
    FileContent getFileContent(const std::string& filename) const;

    /**
     * @brief Gets all files in the commit state.
     * @return A map of filenames to their content.
     */
    const std::unordered_map<std::string, FileContent>& getFiles() const;

private:
    std::unordered_map<std::string, FileContent> files; ///< A map of filenames to their content.
};

/**
 * @class CommitUtils
 * @brief Provides utility functions for working with commits.
 */
class CommitUtils {
public:
    /**
     * @brief Computes the difference between two commits.
     * @param commit1 The first commit.
     * @param commit2 The second commit.
     */
    static void diff(const Commit& commit1, const Commit& commit2);
};

#endif // ITMO_VCS_COMMITSTATE_H