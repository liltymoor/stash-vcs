#ifndef INVOKER_HPP
#define INVOKER_HPP

#include "command.hpp"
#include <iostream>

/**
 * @class CommandInvoker
 * @brief Manages and invokes commands.
 * 
 * The CommandInvoker class is responsible for registering and executing commands.
 */
class CommandInvoker {
private:
    std::unordered_map<std::string, const Command*> commands; ///< Map of registered commands.

public:
    /// Default constructor for the CommandInvoker class.
    CommandInvoker();

    /**
     * @brief Registers a command.
     * @param command The command to register.
     */
    void init_command(const Command* command);

    /**
     * @brief Invokes a command.
     * @param argc The number of arguments.
     * @param argv The array of arguments.
     * @return true if the command was successfully invoked, otherwise false.
     */
    bool invoke_command(int argc, char* argv[]);
};

#endif // INVOKER_HPP