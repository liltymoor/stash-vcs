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
