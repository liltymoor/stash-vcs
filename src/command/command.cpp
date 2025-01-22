/**
 * @file command.cpp
 * @brief Implementation of the Command class.
 */

#include "command.hpp"

/**
 * @brief Constructs a new Command object.
 * @param name The name of the command.
 * @param description A brief description of the command.
 */
Command::Command(const char *name, const char *description) {
    this->name = name;
    this->description = description;
    this->expected_args = new CommandArgs();
}

/**
 * @brief Destructor for the Command object.
 */
Command::~Command() {
    delete this->expected_args;
}

/**
 * @brief Executes the command with the provided arguments.
 * @param argc The number of arguments.
 * @param argv The array of argument strings.
 * @return A pointer to the result of the execution.
 */
void *Command::execute(int argc, char *argv[]) const {
    if (argv == nullptr) {
        ParsedArgs const parsed_args;
        return this->action(parsed_args);
    }

    std::vector<std::string> const args = std::vector<std::string>(argv, argv + (argc - 2));
    ParsedArgs const parsed_args = this->expected_args->parseArgs(args);

    return this->action(parsed_args);
}

/**
 * @brief The action to be performed when the command is executed.
 * @param args The parsed arguments passed to the command.
 * @return A pointer to the result of the action (nullptr by default).
 */
void *Command::action(ParsedArgs const args) const {
    if (args.hasArg("description")) { // If "description" argument is present
        std::cout << description << std::endl;
        return nullptr;
    }

    if (args.hasArg("verbose")) { // If "verbose" argument is present
        std::cout << "Verbose mode is on" << std::endl;
    }

    std::cout << "Hello world!" << std::endl;

    return nullptr;
}

/**
 * @brief Gets the name of the command.
 * @return The name of the command.
 */
const char *Command::get_name() const {
    return this->name;
}