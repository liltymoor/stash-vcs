#include "command.hpp"
#include "logger.hpp"

/**
 * @brief Constructor for the Command class.
 * @param name The name of the command.
 * @param description The description of the command.
 */
Command::Command(const char* name, const char* description) {
    this->name = name;
    this->description = description;
    this->expected_args = new CommandArgs();
}

/**
 * @brief Destructor for the Command class.
 */
Command::~Command() {
    delete this->expected_args;
}

/**
 * @brief Executes the command with the provided arguments.
 * @param argc The number of arguments.
 * @param argv The array of arguments.
 * @return A pointer to the result of the command execution.
 */
void* Command::execute(int argc, char* argv[]) const {
    if (argv == nullptr) {
        ParsedArgs const parsed_args;
        return this->action(parsed_args);
    }

    std::vector<std::string> const args = std::vector<std::string>(argv, argv + (argc - 2));
    ParsedArgs const parsed_args = this->expected_args->parseArgs(args);

    return this->action(parsed_args);
}

/**
 * @brief The default action performed by the command.
 * @param args The parsed arguments.
 * @return A pointer to the result of the action.
 */
void* Command::action(ParsedArgs const args) const {
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
 * @brief Returns the name of the command.
 * @return The name of the command.
 */
const char* Command::get_name() const {
    return this->name;
}

/**
 * @brief Returns the description of the command.
 * @return The description of the command.
 */
const char* Command::get_desc() const {
    return this->description;
}

void Command::print_desc() const {
    INFO(this->description);
    INFO("");
    for (const auto arg: expected_args->get_expected_args()) {
        INFO(std::string("[-") + arg->getName()[0] + std::string(" | --") + arg->getName() + std::string("] : ") + arg->getDescription());
    }
}