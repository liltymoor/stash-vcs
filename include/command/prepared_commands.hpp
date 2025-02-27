#ifndef PREPARED_COMMANDS_HPP
#define PREPARED_COMMANDS_HPP

#include "command.hpp"

/**
 * @def DEFINE_CMND(x)
 * @brief Macro for defining a command and its arguments.
 * 
 * This macro creates a command class and an associated arguments class.
 * @param x The name of the command.
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

// Define specific commands
DEFINE_CMND(Init);
DEFINE_CMND(Add);
DEFINE_CMND(Commit);
DEFINE_CMND(Checkout);
DEFINE_CMND(Merge);
DEFINE_CMND(RevertTo);
DEFINE_CMND(Status);

#endif // PREPARED_COMMANDS_HPP