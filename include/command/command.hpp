#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>
#include "command_args.hpp"

/**
 * @class Command
 * @brief Base class for creating commands.
 *
 * The Command class provides an interface for creating commands with a name, description,
 * and an action that is executed when the command is invoked.
 */
class Command {
private:
    const char* name;        ///< The name of the command.
    const char* description; ///< The description of the command.

protected:
    /**
     * @brief The action performed by the command.
     * @param args Arguments passed to the command.
     * @return A pointer to the result of the command execution.
     */
    virtual void* action(ParsedArgs args) const;

    CommandArgs* expected_args; ///< Expected arguments for the command.

public:
    /**
     * @brief Constructor for the Command class.
     * @param name The name of the command.
     * @param description The description of the command.
     */
    Command(const char* name, const char* description);

    /// Destructor for the Command class.
    ~Command();

    /**
     * @brief Executes the command.
     * @param argc The number of arguments.
     * @param argv The array of arguments.
     * @return A pointer to the result of the command execution.
     */
    void* execute(int argc, char* argv[]) const;

    /**
     * @brief Returns the name of the command.
     * @return The name of the command.
     */
    const char* get_name() const;

    /**
     * @brief Returns the description of the command.
     * @return The description of the command.
     */
    const char* get_desc() const;
};

#endif