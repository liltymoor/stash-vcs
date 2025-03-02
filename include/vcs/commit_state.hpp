#pragma once
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
#include <filesystem>

#include "state_diff.hpp"

struct LineDiff;
struct FileDiff;
using DiffResult = std::map<std::string, FileDiff>;

/**
 * @class FileContent
 * @brief Represents the content of a file, including its lines and full content.
 */
class FileContent {
public:
    /// Default constructor.
    FileContent() = default;

    /**
     * @brief Constructor that initializes FileContent with a map of lines.
     * @param lines A map of line numbers to their content.
     */
    explicit FileContent(const std::map<int, std::string>& lines);

    /**
     * @brief Constructor that reads file content from a given path.
     * @param filePath The path to the file.
     */
    explicit FileContent(const std::filesystem::path& filePath);

    /**
     * @brief Retrieves the content of a specific line.
     * @param lineNumber The line number to retrieve.
     * @return The content of the specified line.
     */
    std::string getLine(int lineNumber) const;

    /**
     * @brief Retrieves all lines of the file.
     * @return A map of line numbers to their content.
     */
    const std::map<int, std::string>& getAllLines() const;

    /**
     * @brief Retrieves the full content of the file as a single string.
     * @return The full content of the file.
     */
    std::string getFullContent() const;

    uint32_t applyContentChanges(const std::vector<LineDiff>& diff);

    /**
     * @brief Checks if the file content is empty.
     * @return true if the file content is empty, otherwise false.
     */
    bool isEmpty() const;

private:
    /**
     * @brief Adds a line to the file content.
     * @param lineNumber The line number.
     * @param line The content of the line.
     */
    void addLine(int lineNumber, const std::string& line);

    /**
     * @brief Reads file content from a given path.
     * @param filePath The path to the file.
     */
    void readFromFile(const std::filesystem::path& filePath);

    std::map<int, std::string> lines; ///< Map of line numbers to their content.
};

/**
 * @class File
 * @brief Represents a file with its path and content.
 */
class File {
private:
    std::filesystem::path filepath; ///< The path to the file.
    FileContent cachedContent;      ///< Cached content of the file.

public:
    /// Default constructor.
    File() = default;

    /**
     * @brief Constructor that initializes the file with a given path.
     * @param filepath The path to the file.
     */
    File(const std::filesystem::path& filepath);

    /**
     * @brief Retrieves the path of the file.
     * @return The path of the file.
     */
    std::filesystem::path get_path() const;

    /**
     * @brief Moves the file to a new location.
     * @param to The new path for the file.
     */
    void move(const std::filesystem::path& to);

    void applyFileChanges(const FileDiff& diff);

    /**
     * @brief Flushes content to a file at a given path.
     * @param filepath The path to the file.
     * @param content The content to write.
     */
     void flush();

    /**
     * @brief Retrieves the content of the file.
     * @return The content of the file.
     */
    FileContent get_content();

    /**
     * @brief Reads the content of a file from a given path.
     * @param filepath The path to the file.
     * @return The content of the file as a string.
     */
    static std::string read(const std::string &filepath);

    /**
     * @brief Checks if a string is a regular expression.
     * @param str The string to check.
     * @return true if the string is a regular expression, otherwise false.
     */
    static bool isRegexp(const std::string &str);

    /**
     * @brief Checks if filename matches pattern.
     * @param filename The filename to check.
     * @param pattern The pattern filename can matches.
     * @param isRegexp If pattern is regexp.
     * @return true if the filename matches the regexp or simple filename pattern, otherwise false.
     */
    static bool matchesPattern(const std::string& filename, const std::string& pattern, const bool& isRegexp);

    /**
     * @brief Copies files from a source pattern to a target directory.
     * @param source_pattern The source pattern for files.
     * @param target_dir The target directory.
     * @param use_regex Whether to use regex for matching files.
     * @return The number of files copied.
     */
    static uint32_t copy_files(const std::string& source_pattern, const std::string& target_dir, bool use_regex = true);

    /**
     * @brief Cleans a directory by removing all files.
     * @param str The path to the directory.
     */
    static void clean_dir(const std::string &str);

    /**
     * @brief Converts file time to a string representation.
     * @param ft The file time to convert.
     * @return The string representation of the file time.
     */
    static std::string file_time_to_string(const std::filesystem::file_time_type& ft);

    /**
     * @brief Retrieves all files from a directory.
     * @param dir The path to the directory.
     * @return A map of filenames to File objects.
     */
    static std::unordered_map<std::string, File> getFilesFromDir(const std::filesystem::path& dir);
};

/**
 * @class Commit
 * @brief Represents a commit in the version control system.
 */
class Commit;

/**
 * @class CommitState
 * @brief Represents the state of files in a commit.
 */
class CommitState {
public:
    /**
     * @brief Adds a file to the commit state.
     * @param filename The name of the file.
     * @param file The File object representing the file.
     */
    void addFile(const std::string &filename, const File &file);

    /**
     * @brief Retrieves the content of a file in the commit state.
     * @param filename The name of the file.
     * @return The content of the file.
     */
    FileContent getFileContent(const std::string &filename);

    /**
     * @brief Retrieves all files in the commit state.
     * @return A map of filenames to File objects.
     */
    const std::unordered_map<std::string, File> &getFiles();

private:
    std::unordered_map<std::string, File> files; ///< Map of filenames to File objects.
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
    static DiffResult diff(const std::shared_ptr<Commit> commit1, const std::shared_ptr<Commit> commit2);
};

#endif // ITMO_VCS_COMMITSTATE_H