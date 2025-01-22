/**
 * @file command.h
 * @brief Header file for the Command class.
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>
#include "command_args.hpp"

/**
 * @class Command
 * @brief Represents a command that can be executed with arguments.
 *
 * This class provides functionality to define and execute commands with optional arguments.
 */
class Command {
private:
    const char *name;         ///< The name of the command.
    const char *description;  ///< A brief description of the command.
    CommandArgs *expected_args; ///< Expected arguments for the command.

protected:
    /**
     * @brief The action to be performed when the command is executed.
     * @param args The parsed arguments passed to the command.
     * @return A pointer to the result of the action (nullptr by default).
     */
    virtual void *action(ParsedArgs args) const;

public:
    /**
     * @brief Constructs a new Command object.
     * @param name The name of the command.
     * @param description A brief description of the command.
     */
    Command(const char *name, const char *description);

    /**
     * @brief Destructor for the Command object.
     */
    ~Command();

    /**
     * @brief Executes the command with the provided arguments.
     * @param argc The number of arguments.
     * @param argv The array of argument strings.
     * @return A pointer to the result of the execution.
     */
    void *execute(int argc, char *argv[]) const;

    /**
     * @brief Gets the name of the command.
     * @return The name of the command.
     */
    const char *get_name() const;
};

#endif