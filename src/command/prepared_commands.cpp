#include "prepared_commands.hpp"
#include "command_args.hpp"
#include "invoker.hpp"
#include "stash.hpp"
#include "repository.hpp"
#include "../logger/logger.hpp"
#include <string>

/**
 * @brief Constructor for the InitCommand class.
 */
InitCommand::InitCommand()
        : Command("init", "Initial command to create Stash") {
    expected_args = new InitArgs();
}

/**
 * @brief The action performed by the InitCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* InitCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Init command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    Stash::getInstance(); // Stash initialization

    return nullptr;
}

/**
 * @brief Constructor for the InitArgs class.
 */
InitArgs::InitArgs()
        : CommandArgs() // Init default args like verbose or description
{
    // No need in arguments here
}

/**
 * @brief Constructor for the AddCommand class.
 */
AddCommand::AddCommand()
        : Command("add", "Command to stage changes into stash") {
    expected_args = new AddArgs();
}

/**
 * @brief The action performed by the AddCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* AddCommand::action(ParsedArgs args) const {

    const bool verbose = args.hasArg("verbose"); 
    if (verbose)
        INFO("Add command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("files")) {
        std::string value = args.getArgValue("files");
        if (value == "*") {
            value = "^[^\\.].*";
        }
        // TODO: Verify value

        Repo::getInstance().getRepoStack().stage(value, verbose);
        Repo::getInstance().stashMeta();
    }

    return nullptr;
}

/**
 * @brief Constructor for the AddArgs class.
 */
AddArgs::AddArgs()
        : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("files",
                                    "Argument to specify files that you want to add",
                                    true,
                                    true));
}

/**
 * @brief Constructor for the CommitCommand class.
 */
CommitCommand::CommitCommand()
        : Command("commit", "Create commit command") {
    expected_args = new CommitArgs();
}

/**
 * @brief The action performed by the CommitCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* CommitCommand::action(ParsedArgs args) const {
    const bool verbosedCommand = args.hasArg("verbose");
    if (verbosedCommand)
        INFO("Commit command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("message")) {
        // TODO: Verify value
        INFO("Commiting...")
        auto msg = args.getArgValue("message");
        const bool emptyCommit = args.hasArg("empty");
        
        Repo::getInstance().getRepoStack().commit(msg, verbosedCommand, emptyCommit);
        Repo::getInstance().stashMeta();
    }

    if (args.hasArg("list")) {
        Repo::getInstance().getRepoStack().list_commits();
    }

    return nullptr;
}

/**
 * @brief Constructor for the CommitArgs class.
 */
CommitArgs::CommitArgs()
        : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("message",
                                    "Argument to specify commit message",
                                    false,
                                    true));
    expected_args.push_back(new Arg("list",
                                    "List current branch commits",
                                    false,
                                    false));
    expected_args.push_back(new Arg("empty",
                                    "For empty commits",
                                    false,
                                    false));
}

/**
 * @brief Constructor for the CheckoutCommand class.
 */
CheckoutCommand::CheckoutCommand()
        : Command("checkout", "Switch branch command") {
    expected_args = new CheckoutArgs();
}

/**
 * @brief The action performed by the CheckoutCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* CheckoutCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Checkout command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("branch")) {
        std::string value = args.getArgValue("branch");

        Repo::getInstance().getRepoStack().checkout_branch(value);
        Stash::getInstance().stashMeta();
    }

    if (args.hasArg("list")) {
        Repo::getInstance().getRepoStack().list_branches();
    }

    return nullptr;
}

/**
 * @brief Constructor for the CheckoutArgs class.
 */
CheckoutArgs::CheckoutArgs()
        : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("branch",
                                    "Argument to specify branch name you want checkout to",
                                    false,
                                    true));

    expected_args.push_back(new Arg("list",
                                    "List all the branches you have",
                                    false,
                                    false));
}

/**
 * @brief Constructor for the MergeCommand class.
 */
MergeCommand::MergeCommand()
        : Command("merge", "Merge branches command. This command requires branches to have at least 1 shared commit.") {
    expected_args = new MergeArgs();
}

/**
 * @brief The action performed by the MergeCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* MergeCommand::action(ParsedArgs args) const {
    bool verbose = args.hasArg("verbose"); 
    if (verbose)
        INFO("Merge command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("branch")) {
        std::string branchToMerge = args.getArgValue("branch");
        Repo::getInstance().getRepoStack().merge(branchToMerge, verbose);
        Repo::getInstance().stashMeta();
    }    

    return nullptr;
}

/**
 * @brief Constructor for the MergeArgs class.
 */
MergeArgs::MergeArgs()
        : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("branch",
                                    "Argument to specify branch name you want to merge to (current->specified)",
                                    true,
                                    true));
}

/**
 * @brief Constructor for the RevertToCommand class.
 */
RevertToCommand::RevertToCommand()
        : Command("revert_to", "Reverts to specified commit. Works only in current branch bounds.") {
    expected_args = new RevertToArgs();
}

/**
 * @brief The action performed by the RevertToCommand.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* RevertToCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Revert To command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("hash")) {
        std::string value = args.getArgValue("hash");
        Repo::getInstance().getRepoStack().revert_to(value);
        Repo::getInstance().stashMeta();
    }

    return nullptr;
}

/**
 * @brief Constructor for the RevertToArgs class.
 */
RevertToArgs::RevertToArgs()
        : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg("hash",
                                    "Argument to specify hash, you want to revert to (current->specified)",
                                    true,
                                    true));
}

/**
 * @brief Constructor for the RevertToCommand class.
 */
 StatusCommand::StatusCommand()
 : Command("status", "Shows current branch status. (e.g. modified files)") {
    expected_args = new StatusArgs();
}

/**
* @brief The action performed by the Status.
* @param args The parsed arguments.
* @return A pointer to the result of the action.
*/
void* StatusCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Status command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    Repo::getInstance().getRepoStack().status();

    return nullptr;
}

/**
* @brief Constructor for the RevertToArgs class.
*/
StatusArgs::StatusArgs()
 : CommandArgs() // Init default args like verbose or description
{

}

/**
 * @brief Constructor for the RevertToCommand class.
 */
 HelpCommand::HelpCommand()
 : Command("help", "Shows description for all the commands") {
    expected_args = new HelpArgs();
}

/**
* @brief The action performed by the Status.
* @param args The parsed arguments.
* @return A pointer to the result of the action.
*/
void* HelpCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Status command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    CommandInvoker::getInstance()->print_desc();

    return nullptr;
}

/**
* @brief Constructor for the RevertToArgs class.
*/
HelpArgs::HelpArgs()
 : CommandArgs() // Init default args like verbose or description
{

}

/**
 * @brief Constructor for the ConfigCommand class.
 */
 ConfigCommand::ConfigCommand()
 : Command("config", "Edit local stash config") {
    expected_args = new ConfigArgs();
}

/**
* @brief The action performed by the Config.
* @param args The parsed arguments.
* @return A pointer to the result of the action.
*/
void* ConfigCommand::action(ParsedArgs args) const {
    if (args.hasArg("verbose"))
        INFO("Status command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    std::string username = args.hasArg("user") ? args.getArgValue("user") : std::string();
    std::string email = args.hasArg("email") ? args.getArgValue("email") : std::string();

    if (!username.empty()) {
        Stash::getInstance().setStashUsername(username);
        INFO("Username set");
    }

    if (!email.empty()) {
        Stash::getInstance().setStashEmail(email);
        INFO("Email set");
    }

    Stash::getInstance().stashMeta();

    return nullptr;
}

/**
* @brief Constructor for the ConfigArgs class.
*/
ConfigArgs::ConfigArgs()
 : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg(
        "user",
        "Argument to specify username in local stash config",
        false,
        true
    ));

    expected_args.push_back(new Arg(
        "email",
        "Argument to specify email in local stash config",
        false,
        true
    ));
}

/**
 * @brief Constructor for the ConfigCommand class.
 */
 ResetCommand::ResetCommand()
 : Command("reset", "Reset your branch state. You can set your HEAD to special commit or discard changes.") {
    expected_args = new ResetArgs();
}

/**
* @brief The action performed by the Config.
* @param args The parsed arguments.
* @return A pointer to the result of the action.
*/
void* ResetCommand::action(ParsedArgs args) const {
    const bool verbose = args.hasArg("verbose"); 
    if (verbose)
        INFO("Reset command\n");

    if (args.hasArg("description")) {
        print_desc();
        return nullptr;
    }

    if (args.hasArg("hash")) {
        const std::string commitHash = args.getArgValue("hash");

        Repo::getInstance().getRepoStack().reset_to(commitHash);
        Repo::getInstance().stashMeta();
    }

    return nullptr;
}

/**
* @brief Constructor for the ConfigArgs class.
*/
ResetArgs::ResetArgs()
 : CommandArgs() // Init default args like verbose or description
{
    expected_args.push_back(new Arg(
        "hash",
        "Argument to specify commit hash. This hash will be used to reset your branch state.",
        false,
        true
    ));
}