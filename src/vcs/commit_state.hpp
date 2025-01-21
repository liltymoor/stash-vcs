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

class File {
public:
    static std::string read(const std::string &filepath);

    static void write(const std::string &filepath, const std::string &content);
};

class FileContent {
public:
    FileContent() = default;

    explicit FileContent(const std::map<int, std::string> &lines);

    std::string getLine(int lineNumber) const;

    const std::map<int, std::string> &getAllLines() const;

    std::string getFullContent() const;

private:
    void addLine(int lineNumber, const std::string &line);

    std::map<int, std::string> lines;
};

class Commit;

class CommitState {
public:
    void addFile(const std::string &filename, const FileContent &content);

    FileContent getFileContent(const std::string &filename) const;

    const std::unordered_map<std::string, FileContent> &getFiles() const;

private:
    std::unordered_map<std::string, FileContent> files;
};

class CommitUtils {
public:
    static void diff(const Commit &commit1, const Commit &commit2);
};

#endif //ITMO_VCS_COMMITSTATE_H