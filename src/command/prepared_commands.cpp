//
// Created by Timmie on 21.01.2025.
//

#include "prepared_commands.hpp"
#include "../logger/logger.hpp"

//
// INIT COMMAND
//


InitCommand::InitCommand() : Command("init", "Initial command to create Stash") {}

void* InitCommand::action(ParsedArgs args) const
{
    INFO("Init command");

    if (args.hasArg("description"))
        INFO("Пока здесь нет описания");

    return nullptr;
}

InitArgs::InitArgs() : CommandArgs() // Init default args like verbose or description
{
    // No need in arguments here
}

//
// COMMIT COMMAND
//

CommitCommand::CommitCommand() : Command("commit", "Create commit command") {}
void* CommitCommand::action(ParsedArgs args) const
{
    return nullptr;
}

CommitArgs::CommitArgs() : CommandArgs() // Init default args like verbose or description
{

}

//
// CHECKOUT COMMAND
//

CheckoutCommand::CheckoutCommand() : Command("checkout", "Switch branch command") {}
void* CheckoutCommand::action(ParsedArgs args) const
{
    return nullptr;
}

CheckoutArgs::CheckoutArgs() : CommandArgs() // Init default args like verbose or description
{

}

//
// MERGE COMMAND
//

MergeCommand::MergeCommand() : Command("merge", "Merge branches command. This command requires branches to have at least 1 shared commit.") {}
void* MergeCommand::action(ParsedArgs args) const
{
    return nullptr;
}

MergeArgs::MergeArgs() : CommandArgs() // Init default args like verbose or description
{

}