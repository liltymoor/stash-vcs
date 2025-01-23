//
// Created by Ivanov Ilya on 21.01.2025.
//

#include "commit_state.hpp"
#include "repository.hpp"
#include "../logger/logger.hpp"

#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

File::File(const std::filesystem::path &filepath)
    : filepath(filepath)
{
}

std::filesystem::path File::get_path() const
{
    return filepath;
}

void File::move(const std::filesystem::path &to)
{
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

FileContent File::get_content()
{
    if (cachedContent.isEmpty())
        cachedContent = FileContent(filepath);
    return cachedContent;
}

std::string File::read(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

void File::write(const std::string &filepath, const std::string &content) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    file << content;
}

bool File::isRegexp(const std::string &str)
{
    const std::string regex_chars = ".*+?[]()|{}^$\\";
    return str.find_first_of(regex_chars) != std::string::npos;
}

uint32_t File::copy_files(const std::string &source_pattern, const std::string &target_dir, bool use_regex)
{
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
            }
            else
            {
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
        ERROR("Error while transfering files: ");
        ERROR(ex.what());
        return 0;
    }
}

void File::clean_dir(const std::string &dir_path)
{
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

std::string File::file_time_to_string(const std::filesystem::file_time_type &ft)
{
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ft - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
    );

    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::unordered_map<std::string, File> File::getFilesFromDir(const std::filesystem::path &dir)
{
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

FileContent::FileContent(const std::map<int, std::string> &lines) {
    for (const auto &[lineNumber, line]: lines) {
        addLine(lineNumber, line);
    }
}

FileContent::FileContent(const std::filesystem::path& filePath)
{
    readFromFile(filePath);
}

void FileContent::addLine(int lineNumber, const std::string &line) {
    lines[lineNumber] = line;
}

void FileContent::readFromFile(const std::filesystem::path& filePath)
{
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

std::string FileContent::getLine(int lineNumber) const {
    auto it = lines.find(lineNumber);
    if (it != lines.end()) {
        return it->second;
    }
    return "";
}

const std::map<int, std::string> &FileContent::getAllLines() const {
    return lines;
}

std::string FileContent::getFullContent() const {
    std::string content;
    for (const auto &[lineNumber, line]: lines) {
        content += line + "\n";
    }
    if (!content.empty()) {
        content.pop_back(); // Убираем последний символ новой строки
    }
    return content;
}

bool FileContent::isEmpty() const
{
    return lines.empty();
}

void CommitState::addFile(const std::string &filename, const File &content) {
    files[filename] = content;
}

FileContent CommitState::getFileContent(const std::string &filename) {
    auto it = files.find(filename);
    if (it != files.end()) {
        return it->second.get_content();
    }
    return FileContent{};
}

const std::unordered_map<std::string, File> &CommitState::getFiles() {
    return files;
}

void CommitUtils::diff(const Commit &commit1, const Commit &commit2) {
    auto files1 = commit1.state->getFiles();
    auto files2 = commit2.state->getFiles();

    for (auto &[filename, file1]: files1) {
        auto it = files2.find(filename);
        if (it == files2.end()) {
            INFO("File deleted: " << filename);
        } else {
            auto &file2 = it->second;
            const auto &lines1 = file1.get_content().getAllLines();
            const auto &lines2 = file2.get_content().getAllLines();

            for (const auto &[lineNumber, line1]: lines1) {
                auto lineIt = lines2.find(lineNumber);
                if (lineIt == lines2.end()) {
                    INFO("Line " << lineNumber << " deleted in file: " << filename);
                } else if (lineIt->second != line1) {
                    INFO("Line " << lineNumber << " modified in file: " << filename);
                }
            }

            for (const auto &[lineNumber, line2]: lines2) {
                if (lines1.find(lineNumber) == lines1.end()) {
                    INFO("Line " << lineNumber << " added in file: " << filename);
                }
            }
        }
    }

    for (const auto &[filename, content2]: files2) {
        if (files1.find(filename) == files1.end()) {
            INFO("File added: " << filename);
        }
    }
}