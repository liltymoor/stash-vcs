/**
 * @file prepared_commands.cpp
 * @brief Implementation of predefined commands and their arguments.
 */

#include "prepared_commands.hpp"
#include "../logger/logger.hpp"

//
// INIT COMMAND
//

/**
 * @brief Constructs a new InitCommand object.
 */
InitCommand::InitCommand() : Command("init", "Initial command to create Stash") {}

/**
 * @brief Executes the initialization command.
 * @param args The parsed arguments passed to the command.
 * @return A pointer to the result of the action (nullptr by default).
 */
void *InitCommand::action(ParsedArgs args) const {
    INFO("Init command");

    if (args.hasArg("description"))
        INFO("Пока здесь нет описания");

    return nullptr;
}

/**
 * @brief Constructs a new InitArgs object.
 */
InitArgs::InitArgs() : CommandArgs() {
    // No additional arguments needed for initialization
}

//
// COMMIT COMMAND
//

/**
 * @brief Constructs a new CommitCommand object.
 */
CommitCommand::CommitCommand() : Command("commit", "Create commit command") {}

/**
 * @brief Executes the commit command.
 * @param args The parsed arguments passed to the command.
 * @return A pointer to the result of the action (nullptr by default).
 */
void *CommitCommand::action(ParsedArgs args) const {
    return nullptr;
}

/**
 * @brief Constructs a new CommitArgs object.
 */
CommitArgs::CommitArgs() : CommandArgs() {
    // No additional arguments needed for commit
}

//
// CHECKOUT COMMAND
//

/**
 * @brief Constructs a new CheckoutCommand object.
 */
CheckoutCommand::CheckoutCommand() : Command("checkout", "Switch branch command") {}

/**
 * @brief Executes the checkout command.
 * @param args The parsed arguments passed to the command.
 * @return A pointer to the result of the action (nullptr by default).
 */
void *CheckoutCommand::action(ParsedArgs args) const {
    return nullptr;
}

/**
 * @brief Constructs a new CheckoutArgs object.
 */
CheckoutArgs::CheckoutArgs() : CommandArgs() {
    // No additional arguments needed for checkout
}

//
// MERGE COMMAND
//

/**
 * @brief Constructs a new MergeCommand object.
 */
MergeCommand::MergeCommand() : Command("merge",
                                       "Merge branches command. This command requires branches to have at least 1 shared commit.") {}

/**
 * @brief Executes the merge command.
 * @param args The parsed arguments passed to the command.
 * @return A pointer to the result of the action (nullptr by default).
 */
void *MergeCommand::action(ParsedArgs args) const {
    return nullptr;
}

/**
 * @brief Constructs a new MergeArgs object.
 */
MergeArgs::MergeArgs() : CommandArgs() {
    // No additional arguments needed for merge
}