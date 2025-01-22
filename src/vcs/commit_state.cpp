/**
 * @file commit_state.cpp
 * @brief Implementation of classes related to commit states and file operations.
 */

#include "commit_state.hpp"
#include "repository.hpp"

/**
 * @brief Reads the content of a file.
 * @param filepath The path to the file.
 * @return The content of the file as a string.
 * @throws std::runtime_error If the file cannot be opened.
 */
std::string File::read(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

/**
 * @brief Writes content to a file.
 * @param filepath The path to the file.
 * @param content The content to write.
 * @throws std::runtime_error If the file cannot be opened.
 */
void File::write(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    file << content;
}

/**
 * @brief Constructs a FileContent object from a map of lines.
 * @param lines A map of line numbers to their content.
 */
FileContent::FileContent(const std::map<int, std::string>& lines) {
    for (const auto& [lineNumber, line] : lines) {
        addLine(lineNumber, line);
    }
}

/**
 * @brief Adds a line to the file content.
 * @param lineNumber The line number.
 * @param line The content of the line.
 */
void FileContent::addLine(int lineNumber, const std::string& line) {
    lines[lineNumber] = line;
}

/**
 * @brief Gets the content of a specific line.
 * @param lineNumber The line number.
 * @return The content of the line, or an empty string if the line does not exist.
 */
std::string FileContent::getLine(int lineNumber) const {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        return it->second;
    }
    return "";
}

/**
 * @brief Gets all lines in the file.
 * @return A map of line numbers to their content.
 */
const std::map<int, std::string>& FileContent::getAllLines() const {
    return lines;
}

/**
 * @brief Gets the full content of the file as a single string.
 * @return The full content of the file.
 */
std::string FileContent::getFullContent() const {
    std::string content;
    for (const auto& [lineNumber, line] : lines) {
        content += line + "\n";
    }
    if (!content.empty()) {
        content.pop_back(); // Remove the last newline character
    }
    return content;
}

/**
 * @brief Adds a file to the commit state.
 * @param filename The name of the file.
 * @param content The content of the file.
 */
void CommitState::addFile(const std::string& filename, const FileContent& content) {
    files[filename] = content;
}

/**
 * @brief Gets the content of a specific file.
 * @param filename The name of the file.
 * @return The content of the file.
 */
FileContent CommitState::getFileContent(const std::string& filename) const {
    auto it = files.find(filename);
    if (it != files.end()) {
        return it->second;
    }
    return FileContent{};
}

/**
 * @brief Gets all files in the commit state.
 * @return A map of filenames to their content.
 */
const std::unordered_map<std::string, FileContent>& CommitState::getFiles() const {
    return files;
}

/**
 * @brief Computes the difference between two commits.
 * @param commit1 The first commit.
 * @param commit2 The second commit.
 */
void CommitUtils::diff(const Commit& commit1, const Commit& commit2) {
    const auto& files1 = commit1.state->getFiles();
    const auto& files2 = commit2.state->getFiles();

    // Check for deleted or modified files
    for (const auto& [filename, content1] : files1) {
        auto it = files2.find(filename);
        if (it == files2.end()) {
            std::cout << "File deleted: " << filename << std::endl;
        } else {
            const auto& content2 = it->second;
            const auto& lines1 = content1.getAllLines();
            const auto& lines2 = content2.getAllLines();

            // Check for modified lines
            for (const auto& [lineNumber, line1] : lines1) {
                auto lineIt = lines2.find(lineNumber);
                if (lineIt == lines2.end()) {
                    std::cout << "Line " << lineNumber << " deleted in file: " << filename << std::endl;
                } else if (lineIt->second != line1) {
                    std::cout << "Line " << lineNumber << " modified in file: " << filename << std::endl;
                }
            }

            // Check for added lines
            for (const auto& [lineNumber, line2] : lines2) {
                if (lines1.find(lineNumber) == lines1.end()) {
                    std::cout << "Line " << lineNumber << " added in file: " << filename << std::endl;
                }
            }
        }
    }

    // Check for added files
    for (const auto& [filename, content2] : files2) {
        if (files1.find(filename) == files1.end()) {
            std::cout << "File added: " << filename << std::endl;
        }
    }
}