//
// Created by Ivanov Ilya on 21.01.2025.
//

#include "commit_state.hpp"
#include "repository.hpp"
#include "../logger/logger.hpp"
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

/**
 * @brief Constructor for the File class.
 * @param filepath The path to the file.
 */
File::File(const std::filesystem::path &filepath)
        : filepath(filepath) {}

/**
 * @brief Returns the path of the file.
 * @return The path of the file.
 */
std::filesystem::path File::get_path() const {
    return filepath;
}

/**
 * @brief Moves the file to a new location.
 * @param to The new path for the file.
 */
void File::move(const std::filesystem::path &to) {
    if (!exists(filepath)) {
        throw std::runtime_error("Source file does not exist: " + filepath.string());
    }

    std::filesystem::path target_path = to;
    if (is_directory(to)) {
        target_path /= filepath.filename();
    }

    try {
        if (exists(target_path)) {
            std::filesystem::remove(target_path);
        }

        std::filesystem::rename(filepath, target_path);
        filepath = target_path;

    } catch (const std::filesystem::filesystem_error& e) {
        if (e.code() == std::errc::cross_device_link) {
            copy(filepath, target_path,
                 std::filesystem::copy_options::overwrite_existing
            );
            std::filesystem::remove(filepath);
            filepath = target_path;
        } else {
            throw;
        }
    }
}

/**
 * @brief Retrieves the content of the file.
 * @return The content of the file.
 */
FileContent File::get_content() {
    if (cachedContent.isEmpty())
        cachedContent = FileContent(filepath);
    return cachedContent;
}

/**
 * @brief Reads the content of a file from a given path.
 * @param filepath The path to the file.
 * @return The content of the file as a string.
 */
std::string File::read(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

/**
 * @brief Writes content to a file at a given path.
 * @param filepath The path to the file.
 * @param content The content to write.
 */
void File::write(const std::string &filepath, const std::string &content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    file << content;
}

/**
 * @brief Checks if a string is a regular expression.
 * @param str The string to check.
 * @return true if the string is a regular expression, otherwise false.
 */
bool File::isRegexp(const std::string &str) {
    const std::string regex_chars = ".*+?[]()|{}^$\\";
    return str.find_first_of(regex_chars) != std::string::npos;
}

/**
 * @brief Compares filename with pattern.
 * @param filename Filename to compare.
 * @param pattern The actual pattern.
 * @param isRegexp Whether to use regex for comparing files.
 * @return Matches or not.
 */
bool File::matchesPattern(const std::string& filename, const std::string& pattern, const bool& isRegexp) {
    if (isRegexp) {
        try {
            std::regex re(pattern);
            return std::regex_match(filename, re);
        } catch (const std::regex_error&) {
            return false;
        }
    }
    return filename == pattern;
}

/**
 * @brief Copies files from a source pattern to a target directory.
 * @param source_pattern The source pattern for files.
 * @param target_dir The target directory.
 * @param use_regex Whether to use regex for matching files.
 * @return The number of files copied.
 */
uint32_t File::copy_files(const std::string &source_pattern, const std::string &target_dir, bool use_regex) {
    try {
        fs::create_directories(target_dir);

        fs::path current_dir = fs::current_path();

        std::vector<fs::path> files_to_move;

        if (use_regex) {
            std::regex pattern(source_pattern);
            for (const auto& entry : fs::directory_iterator(current_dir)) {
                if (fs::is_regular_file(entry)) {
                    std::string filename = entry.path().filename().string();
                    if (std::regex_match(filename, pattern)) {
                        files_to_move.push_back(entry.path());
                    }
                }
            }
        } else {
            fs::path source_path = current_dir / source_pattern;
            if (fs::exists(source_path)) {
                files_to_move.push_back(source_path);
            } else {
                WARN("File not found");
            }
        }

        uint32_t file_counter = 0;
        for (const auto& file : files_to_move) {
            fs::path target_path = fs::path(target_dir) / file.filename();

            if (exists(target_path)) {
                fs::remove(target_path);
            }
            copy(file, target_path);
            file_counter++;
        }
        return file_counter;

    } catch (const std::exception& ex) {
        ERROR("Error while transferring files: ");
        ERROR(ex.what());
        return 0;
    }
}

/**
 * @brief Cleans a directory by removing all files.
 * @param dir_path The path to the directory.
 */
void File::clean_dir(const std::string &dir_path) {
    fs::path directory(dir_path);

    if (!fs::exists(directory)) {
        return;
    }

    if (!fs::is_directory(directory)) {
        throw std::invalid_argument("Path is not a directory: " + dir_path);
    }

    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(directory)) {
        fs::remove_all(entry.path(), ec);

        if (ec) {
            throw std::runtime_error(
                    "Failed to remove " + entry.path().string() +
                    ": " + ec.message()
            );
        }
    }
}

/**
 * @brief Converts file time to a string representation.
 * @param ft The file time to convert.
 * @return The string representation of the file time.
 */
std::string File::file_time_to_string(const std::filesystem::file_time_type &ft) {
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

/**
 * @brief Retrieves all files from a directory.
 * @param dir The path to the directory.
 * @return A map of filenames to File objects.
 */
std::unordered_map<std::string, File> File::getFilesFromDir(const std::filesystem::path &dir) {
    std::unordered_map<std::string, File> files;
    if (!exists(dir)) {
        throw std::runtime_error("Directory does not exist: " + dir.string());
    }

    if (!is_directory(dir)) {
        throw std::runtime_error("Path is not a directory: " + dir.string());
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                const auto& path = entry.path();
                std::string filename = path.filename().string();

                files[filename] = File(path);
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        throw std::runtime_error("Failed to access directory: " + std::string(e.what()));
    }
    return files;
}

/**
 * @brief Constructor for the FileContent class.
 * @param lines A map of line numbers to their content.
 */
FileContent::FileContent(const std::map<int, std::string> &lines) {
    for (const auto &[lineNumber, line]: lines) {
        addLine(lineNumber, line);
    }
}

/**
 * @brief Constructor for the FileContent class that reads from a file.
 * @param filePath The path to the file.
 */
FileContent::FileContent(const std::filesystem::path& filePath) {
    readFromFile(filePath);
}

/**
 * @brief Adds a line to the file content.
 * @param lineNumber The line number.
 * @param line The content of the line.
 */
void FileContent::addLine(int lineNumber, const std::string &line) {
    lines[lineNumber] = line;
}

/**
 * @brief Reads file content from a given path.
 * @param filePath The path to the file.
 */
void FileContent::readFromFile(const std::filesystem::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::string line;
    int lineNumber = 1;
    while (std::getline(file, line)) {
        addLine(lineNumber++, line);
    }
}

/**
 * @brief Retrieves the content of a specific line.
 * @param lineNumber The line number to retrieve.
 * @return The content of the specified line.
 */
std::string FileContent::getLine(int lineNumber) const {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        return it->second;
    }
    return "";
}

/**
 * @brief Retrieves all lines of the file.
 * @return A map of line numbers to their content.
 */
const std::map<int, std::string> &FileContent::getAllLines() const {
    return lines;
}

/**
 * @brief Retrieves the full content of the file as a single string.
 * @return The full content of the file.
 */
std::string FileContent::getFullContent() const {
    std::string content;
    for (const auto &[lineNumber, line]: lines) {
        content += line + "\n";
    }
    if (!content.empty()) {
        content.pop_back(); // Remove the last newline character
    }
    return content;
}

/**
 * @brief Checks if the file content is empty.
 * @return true if the file content is empty, otherwise false.
 */
bool FileContent::isEmpty() const {
    return lines.empty();
}

/**
 * @brief Adds a file to the commit state.
 * @param filename The name of the file.
 * @param content The File object representing the file.
 */
void CommitState::addFile(const std::string &filename, const File &content) {
    files[filename] = content;
}

/**
 * @brief Retrieves the content of a file in the commit state.
 * @param filename The name of the file.
 * @return The content of the file.
 */
FileContent CommitState::getFileContent(const std::string &filename) {
    auto it = files.find(filename);
    if (it != files.end()) {
        return it->second.get_content();
    }
    return FileContent{};
}

/**
 * @brief Retrieves all files in the commit state.
 * @return A map of filenames to File objects.
 */
const std::unordered_map<std::string, File> &CommitState::getFiles() {
    return files;
}

/**
 * @brief Computes the difference between two commits.
 * @param commit1 The first commit.
 * @param commit2 The second commit.
 */
DiffResult CommitUtils::diff(const std::shared_ptr<Commit> commit1, const std::shared_ptr<Commit> commit2) {
    DiffResult result;
    auto files1 = commit1 ? commit1->state->getFiles() : std::unordered_map<std::string, File>{};
    auto files2 = commit2 ? commit2->state->getFiles() : std::unordered_map<std::string, File>{};

    // checking files from commit1
    for (auto& [filename, file1] : files1) {
        FileDiff fileDiff;
        const auto& content1 = file1.get_content();
        const auto& lines1 = content1.getAllLines();

        if (files2.count(filename)) {
            // file exists in first and second commit
            const auto& content2 = files2.at(filename).get_content();
            const auto& lines2 = content2.getAllLines();

            // checking line changes
            for (const auto& [num, line1] : lines1) {
                if (!lines2.count(num)) {
                    fileDiff.changes.push_back({LineDiff::REMOVED, line1, num});
                } else if (lines2.at(num) != line1) {
                    fileDiff.changes.push_back({LineDiff::MODIFIED, line1, num});
                    fileDiff.hasConflicts = true;
                }
            }

            // checking new lines
            for (const auto& [num, line2] : lines2) {
                if (!lines1.count(num)) {
                    fileDiff.changes.push_back({LineDiff::ADDED, line2, num});
                }
            }
        } else {
            // in case the file was deleted in second commit
            for (const auto& [num, line] : lines1) {
                fileDiff.changes.push_back({LineDiff::REMOVED, line, num});
            }
            fileDiff.hasConflicts = true;
            fileDiff.wasDeleted = true;
        }
        
        result[filename] = fileDiff;
    }

    // new commit files checked
    for (auto& [filename, file2] : files2) {
        if (!files1.count(filename)) {
            FileDiff fileDiff;
            const auto& content2 = file2.get_content();
            for (const auto& [num, line] : content2.getAllLines()) {
                fileDiff.changes.push_back({LineDiff::ADDED, line, num});
            }
            result[filename] = fileDiff;
        }
    }

    return result;
}