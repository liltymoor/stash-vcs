/**
 * @file invoker.cpp
 * @brief Implementation of the CommandInvoker class.
 */

#include "invoker.hpp"
#include "../logger/logger.hpp"
#include <unordered_map>
#include "prepared_commands.hpp"

/**
 * @brief Constructs a new CommandInvoker object.
 */
CommandInvoker::CommandInvoker() {}

/**
 * @brief Invokes a command based on the provided arguments.
 * @param argc The number of arguments.
 * @param argv The array of argument strings.
 * @return True if the command was successfully invoked, false otherwise.
 */
bool CommandInvoker::invoke_command(int argc, char* argv[]) {
    if (argc < 2) {
        INFO("stash usage example:\t\"stash --help\" to get help");
        return false;
    }

    std::string const command_name(argv[1]);

    if (commands.find(command_name) == commands.end()) {
        INFO("command not found");
        return false;
    }

    char** command_args = argc > 2 ? (argv + 2) : nullptr;
    commands[command_name]->execute(argc, command_args);

    return true;
}

/**
 * @brief Registers a command with the invoker.
 * @param command A pointer to the command to register.
 */
void CommandInvoker::init_command(const Command* command) {
    commands[command->get_name()] = command;
}