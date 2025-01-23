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

class FileContent {
public:
    FileContent() = default;
    explicit FileContent(const std::map<int, std::string>& lines);
    explicit FileContent(const std::filesystem::path& filePath);

    std::string getLine(int lineNumber) const;
    const std::map<int, std::string>& getAllLines() const;
    std::string getFullContent() const;
    bool isEmpty() const;

private:
    void addLine(int lineNumber, const std::string& line);
    void readFromFile(const std::filesystem::path& filePath);

    std::map<int, std::string> lines;
};

class File {
private:
    std::filesystem::path filepath;
    FileContent cachedContent;
public:
    File() = default;
    File(const std::filesystem::path& filepath);

    std::filesystem::path get_path() const;
    void move(const std::filesystem::path& to);

    FileContent get_content();


    static std::string read(const std::string &filepath);

    static void write(const std::string &filepath, const std::string &content);

    static bool isRegexp(const std::string &str);
    static void copy_files(const std::string& source_pattern, const std::string& target_dir, bool use_regex = true);
    static void clean_dir(const std::string &str);
    static std::unordered_map<std::string, File> getFilesFromDir(const std::filesystem::path& dir);
};

class Commit;

class CommitState {
public:
    void addFile(const std::string &filename, const File &file);

    FileContent getFileContent(const std::string &filename);

    const std::unordered_map<std::string, File> &getFiles();

private:
    std::unordered_map<std::string, File> files;
};

class CommitUtils {
public:
    static void diff(const Commit &commit1, const Commit &commit2);
};

#endif //ITMO_VCS_COMMITSTATE_H