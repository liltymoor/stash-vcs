//
// Created by Timmie on 21.01.2025.
//

#include "prepared_commands.hpp"

#include "stash.hpp"
#include "repository.hpp"
#include "../logger/logger.hpp"

//
// INIT COMMAND
//


InitCommand::InitCommand()
    : Command("init", "Initial command to create Stash")
{
    expected_args = new InitArgs();
}

void *InitCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Init command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    Stash::getInstance(); // Stash initialization

    return nullptr;
}

InitArgs::InitArgs()
    : CommandArgs() // Init default args like verbose or description
{
    // No need in arguments here
}

//
// ADD COMMAND
//


AddCommand::AddCommand()
    : Command("add", "Command to stage changes into stash")
{
    expected_args = new AddArgs();
}

void *AddCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Add command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    if (args.hasArg("files"))
    {
        std::string value = args.getArgValue("files");
        // TODO verify value

        Repo::getInstance().getRepoStack().stage(value);
    }

    return nullptr;
}

AddArgs::AddArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("files",
                                    "Argument to specify files that you want to add",
                                    true,
                                    true)
        );
}

//
// COMMIT COMMAND
//

CommitCommand::CommitCommand()
    : Command("commit", "Create commit command")
{
    expected_args = new CommitArgs();
}

void *CommitCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Commit command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    if (args.hasArg("message"))
    {
        // TODO verify value
        //if (args.getArgValue("message").empty())
        INFO("Commiting...")
        Repo::getInstance().getRepoStack().commit(args.getArgValue("message"));
    }

    return nullptr;
}

CommitArgs::CommitArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("message",
                                "Argument to specify commit message",
                                true,
                                true)
    );
}

//
// CHECKOUT COMMAND
//

CheckoutCommand::CheckoutCommand()
    : Command("checkout", "Switch branch command")
{
    expected_args = new CheckoutArgs();
}

void *CheckoutCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Checkout command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    return nullptr;
}

CheckoutArgs::CheckoutArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("branch",
                                    "Argument to specify branch name you want checkout to",
                                    true,
                                    true)
        );
}

//
// MERGE COMMAND
//

MergeCommand::MergeCommand()
    : Command("merge", "Merge branches command. This command requires branches to have at least 1 shared commit.")
{
    expected_args = new MergeArgs();
}

void *MergeCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Merge command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    return nullptr;
}

MergeArgs::MergeArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("branch",
                                "Argument to specify branch name you want to merge to (current->specified)",
                                true,
                                true)
    );
}