#include "invoker.hpp"
#include "../logger/logger.hpp"
#include <memory>
#include <unordered_map>
#include "command.hpp"
#include "prepared_commands.hpp"

/**
 * @brief Default constructor for the CommandInvoker class.
 */
CommandInvoker::CommandInvoker() {}

std::shared_ptr<CommandInvoker> CommandInvoker::invoker = std::make_shared<CommandInvoker>(CommandInvoker());

/**
 * @brief Invokes a command with the provided arguments.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return true if the command was successfully invoked, otherwise false.
 */
bool CommandInvoker::invoke_command(int argc, char* argv[]) {
    if (argc < 2) {
        INFO("stash usage example:\t\"stash --help\" to get help");
        return false;
    }

    std::string const command_name(argv[1]);

    if (commands.find(command_name) == commands.end()) {
        ERROR("Command not found");
        return false;
    }
    char** command_args = argc > 2 ? (argv + 2) : nullptr;
    commands[command_name]->execute(argc, command_args);

    return true;
}

/**
 * @brief Initializes a command in the invoker.
 * @param command The command to initialize.
 */
void CommandInvoker::init_command(const Command* command) {
    commands[command->get_name()] = command;
}

void CommandInvoker::print_desc() const {
    for (const auto&[name, command]: commands) {
        INFO(name + " - " + command->get_desc());
    }
};

std::shared_ptr<CommandInvoker> CommandInvoker::getInstance() {
    return invoker;
};