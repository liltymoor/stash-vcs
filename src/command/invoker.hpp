/**
 * @file invoker.hpp
 * @brief Header file for the CommandInvoker class.
 */

#ifndef INVOKER_HPP
#define INVOKER_HPP

#include "command.hpp"
#include <iostream>
#include <unordered_map>

/**
 * @class CommandInvoker
 * @brief Manages and invokes registered commands.
 *
 * This class is responsible for storing and executing commands based on user input.
 */
class CommandInvoker {
private:
    std::unordered_map<std::string, const Command*> commands; ///< A map of registered commands.

public:
    /**
     * @brief Constructs a new CommandInvoker object.
     */
    CommandInvoker();

    /**
     * @brief Registers a command with the invoker.
     * @param command A pointer to the command to register.
     */
    void init_command(const Command* command);

    /**
     * @brief Invokes a command based on the provided arguments.
     * @param argc The number of arguments.
     * @param argv The array of argument strings.
     * @return True if the command was successfully invoked, false otherwise.
     */
    bool invoke_command(int argc, char* argv[]);
};

#endif // INVOKER_HPP