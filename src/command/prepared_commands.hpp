/**
 * @file prepared_commands.hpp
 * @brief Header file for predefined commands and their arguments.
 */

#ifndef PREPARED_COMMANDS_HPP
#define PREPARED_COMMANDS_HPP

#include "command.hpp"

/**
 * @def DEFINE_CMND(x)
 * @brief Macro to define a command class and its corresponding arguments class.
 *
 * This macro creates two classes:
 * - `x##Command`: A command class that inherits from `Command`.
 * - `x##Args`: A class for command arguments that inherits from `CommandArgs`.
 */
#define DEFINE_CMND(x) \
    class x##Command : public Command { \
    public: \
        x##Command(); \
        void* action(ParsedArgs) const override; \
    }; \
    class x##Args : public CommandArgs { \
    public: \
        x##Args(); \
    }

// Define commands using the macro
DEFINE_CMND(Init);    ///< Command for initializing the Stash.
DEFINE_CMND(Commit);  ///< Command for creating a commit.
DEFINE_CMND(Checkout);///< Command for switching branches.
DEFINE_CMND(Merge);   ///< Command for merging branches.

#endif // PREPARED_COMMANDS_HPP