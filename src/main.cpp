#include "invoker.hpp"
#include "prepared_commands.hpp"
#include "stash.hpp"

#include <iostream>
#include <filesystem>
#include "command.hpp"
#include "logger.hpp"

int main(int argc, char *argv[])
{
    if (std::filesystem::exists(".stash"))
        Stash::getInstance();

    const Command testCommand("test", "Test command");
    const InitCommand initCommand;
    const AddCommand addCommand;
    const CommitCommand commitCommand;
    const CheckoutCommand checkoutCommand;
    const MergeCommand mergeCommand;
    const RevertToCommand revertToCommand;
    const StatusCommand statusCommand;

    CommandInvoker invoker;

    invoker.init_command(&initCommand);
    invoker.init_command(&addCommand);
    invoker.init_command(&commitCommand);
    invoker.init_command(&checkoutCommand);
    invoker.init_command(&mergeCommand);
    invoker.init_command(&testCommand);
    invoker.init_command(&revertToCommand);
    invoker.init_command(&statusCommand);

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
