#include "command/command.hpp"
#include "repository.hpp"
#include <iostream>

int main() {
    std::map<int, std::string> initialLines = {
            {1, "Hello"},
            {2, "world"},
            {3, "test"}
    };

    FileContent fileContent(initialLines);

    const auto& allLines = fileContent.getAllLines();
    for (const auto& [lineNumber, line] : allLines) {
        std::cout << "Line " << lineNumber << ": " << line << std::endl;
    }

    std::cout << "Full content:\n" << fileContent.getFullContent() << std::endl;

    return 0;
}
