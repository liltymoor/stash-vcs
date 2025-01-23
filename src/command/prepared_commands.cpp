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
        if (value == "*")
        {
            value = "^[^\\.].*";
        }
        // TODO verify value

        Repo::getInstance().getRepoStack().stage(value);
        Repo::getInstance().stashMeta();
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
        Repo::getInstance().stashMeta();
    }

    if (args.hasArg("list"))
    {
        Repo::getInstance().getRepoStack().list_commits();
    }

    return nullptr;
}

CommitArgs::CommitArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("message",
                                "Argument to specify commit message",
                                false,
                                true)
    );
    expected_args.push_back(new Arg("list",
                            "List current branch commits",
                            false,
                            false)
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

    if (args.hasArg("branch"))
    {
        std::string value = args.getArgValue("branch");

        Repo::getInstance().getRepoStack().checkout_branch(value);
        Repo::getInstance().stashMeta();
    }

    if (args.hasArg("list"))
    {
        Repo::getInstance().getRepoStack().list_branches();
    }

    return nullptr;
}

CheckoutArgs::CheckoutArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("branch",
                                    "Argument to specify branch name you want checkout to",
                                    false,
                                    true)
        );

    expected_args.push_back(new Arg("list",
                                "List all the branches you have",
                                false,
                                false)
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

//
// REVERT_TO COMMAND
//

RevertToCommand::RevertToCommand()
    : Command("revert_to", "Reverts to specified commit. Works only in current branch bounds.")
{
    expected_args = new RevertToArgs();
}

void *RevertToCommand::action(ParsedArgs args) const
{
    if (args.hasArg("verbose"))
        INFO("Revert To command");

    if (args.hasArg("description"))
    {
        INFO(get_desc());
        return nullptr;
    }

    if (args.hasArg("hash"))
    {
        std::string value = args.getArgValue("hash");
        Repo::getInstance().getRepoStack().revert_to(value);
        Repo::getInstance().stashMeta();
    }

    return nullptr;
}

RevertToArgs::RevertToArgs()
    : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("hash",
                                "Argument to specify hash, you want to revert to (current->specified)",
                                true,
                                true)
    );
}
