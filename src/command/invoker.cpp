//
// Created by Timmie on 21.01.2025.
//

#include "invoker.hpp"
#include "../logger/logger.hpp"

#include <unordered_map>
#include "prepared_commands.hpp"

CommandInvoker::CommandInvoker() {}

bool CommandInvoker::invoke_command(int argc, char *argv[])
{
    if (argc < 2)
    {
        INFO("stash usage example:\t\"stash --help\" to get help");
        return false;
    }

    std::string const command_name(argv[1]);

    if (commands.find(command_name) == commands.end())
    {
        INFO("command not found");
        return false;
    }
    char **command_args = argc > 2 ? (argv + 2) : nullptr;
    commands[command_name]->execute(argc, command_args);

    return true;
}

void CommandInvoker::init_command(const Command* command)
{
    commands[command->get_name()] = command;
}