#include "invoker.hpp"
#include "prepared_commands.hpp"
#include "stash.hpp"

#include <iostream>
#include "command/command.hpp"
#include "logger/logger.hpp"

int main(int argc, char *argv[])
{
    Stash::getInstance();
    const Command testCommand("test", "Test command");
    const InitCommand initCommand;
    const CommitCommand commitCommand;
    const CheckoutCommand checkoutCommand;
    const MergeCommand mergeCommand;

    CommandInvoker invoker;

    invoker.init_command(&initCommand);
    invoker.init_command(&commitCommand);
    invoker.init_command(&checkoutCommand);
    invoker.init_command(&mergeCommand);
    invoker.init_command(&testCommand);

    if (!invoker.invoke_command(argc, argv)) ERROR("Command failed. Check output above to see more detailed info.");
}

/*

Way to operate file contents

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

*/
