//
// Created by Ivanov Ilya on 21.01.2025.
//

#include "commitState.hpp"
#include "repository.hpp"

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

FileContent::FileContent(const std::map<int, std::string> &lines) {
    for (const auto &[lineNumber, line]: lines) {
        addLine(lineNumber, line);
    }
}

void FileContent::addLine(int lineNumber, const std::string &line) {
    lines[lineNumber] = line;
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

void CommitState::addFile(const std::string &filename, const FileContent &content) {
    files[filename] = content;
}

FileContent CommitState::getFileContent(const std::string &filename) const {
    auto it = files.find(filename);
    if (it != files.end()) {
        return it->second;
    }
    return FileContent{};
}

const std::unordered_map<std::string, FileContent> &CommitState::getFiles() const {
    return files;
}

void CommitUtils::diff(const Commit &commit1, const Commit &commit2) {
    const auto &files1 = commit1.state->getFiles();
    const auto &files2 = commit2.state->getFiles();

    // Проверяем удаленные или измененные файлы
    for (const auto &[filename, content1]: files1) {
        auto it = files2.find(filename);
        if (it == files2.end()) {
            std::cout << "File deleted: " << filename << std::endl;
        } else {
            const auto &content2 = it->second;
            const auto &lines1 = content1.getAllLines();
            const auto &lines2 = content2.getAllLines();

            // Проверяем измененные строки
            for (const auto &[lineNumber, line1]: lines1) {
                auto lineIt = lines2.find(lineNumber);
                if (lineIt == lines2.end()) {
                    std::cout << "Line " << lineNumber << " deleted in file: " << filename << std::endl;
                } else if (lineIt->second != line1) {
                    std::cout << "Line " << lineNumber << " modified in file: " << filename << std::endl;
                }
            }

            // Проверяем добавленные строки
            for (const auto &[lineNumber, line2]: lines2) {
                if (lines1.find(lineNumber) == lines1.end()) {
                    std::cout << "Line " << lineNumber << " added in file: " << filename << std::endl;
                }
            }
        }
    }

    // Проверяем добавленные файлы
    for (const auto &[filename, content2]: files2) {
        if (files1.find(filename) == files1.end()) {
            std::cout << "File added: " << filename << std::endl;
        }
    }
}