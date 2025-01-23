#include "command_args.hpp"

// Macros for full and short argument names
#define FULL_ARG(arg) std::string("--") + arg
#define SHORT_ARG(arg) std::string("-") + arg

/**
 * @brief Constructor for the Arg class.
 * @param name The name of the argument.
 * @param desc The description of the argument.
 */
Arg::Arg(const char* name, const char* desc) {
    str_name = name;
    str_description = desc;
    str_short_name = str_name.at(0);

    b_required = false;

    definitions = std::vector<std::string>();

    definitions.push_back(FULL_ARG(str_name));
    definitions.push_back(SHORT_ARG(str_short_name));
}

/**
 * @brief Constructor for the Arg class with custom definitions.
 * @param name The name of the argument.
 * @param desc The description of the argument.
 * @param definitions A list of custom definitions for the argument.
 */
Arg::Arg(const char* name, const char* desc, std::vector<std::string> definitions)
        : Arg::Arg(name, desc) {
    this->definitions.insert(
            this->definitions.end(), definitions.begin(), definitions.end()
    );
}

/**
 * @brief Constructor for the Arg class with required and valuable flags.
 * @param name The name of the argument.
 * @param desc The description of the argument.
 * @param required Whether the argument is required.
 * @param valuable Whether the argument requires a value.
 */
Arg::Arg(const char* name, const char* desc, bool required, bool valuable)
        : Arg(name, desc) {
    b_required = required;
    b_valuable = valuable;
}

/**
 * @brief Constructor for the Arg class with custom definitions, required, and valuable flags.
 * @param name The name of the argument.
 * @param desc The description of the argument.
 * @param definitions A list of custom definitions for the argument.
 * @param required Whether the argument is required.
 * @param valuable Whether the argument requires a value.
 */
Arg::Arg(const char* name, const char* desc, std::vector<std::string> definitions, bool required, bool valuable)
        : Arg(name, desc, definitions) {
    b_required = required;
    b_valuable = valuable;
}

/**
 * @brief Checks if the argument is required.
 * @return true if the argument is required, otherwise false.
 */
bool Arg::isRequired() const {
    return b_required;
}

/**
 * @brief Checks if the argument requires a value.
 * @return true if the argument requires a value, otherwise false.
 */
bool Arg::isValuable() const {
    return b_valuable;
}

/**
 * @brief Returns the name of the argument.
 * @return The name of the argument.
 */
const char* Arg::getName() const {
    return str_name.c_str();
}

/**
 * @brief Returns the description of the argument.
 * @return The description of the argument.
 */
const char* Arg::getDescription() const {
    return str_description.c_str();
}

/**
 * @brief Parses the argument to check if it matches any definition.
 * @param str The argument string to parse.
 * @return true if the argument matches any definition, otherwise false.
 */
bool Arg::parse(const char* str) const {
    if (str == NULL)
        return false;

    for (const auto& definition : definitions) {
        if (str == definition)
            return true;
    }

    return false;
}

/**
 * @brief Compares two Arg objects for equality.
 * @param other The other Arg object to compare with.
 * @return true if the arguments are equal, otherwise false.
 */
bool Arg::operator==(const Arg& other) const {
    return str_name == other.str_name;
}

/**
 * @brief Default constructor for the CommandArgs class.
 */
CommandArgs::CommandArgs() {
    expected_args = std::vector<Arg*>();
    expected_args.push_back(new Arg("verbose", "Do explicit prints to console"));
    expected_args.push_back(new Arg("description", "Print command description to console"));
}

/**
 * @brief Constructor for the CommandArgs class with additional arguments.
 * @param args A list of additional arguments.
 */
CommandArgs::CommandArgs(std::vector<Arg*> args)
        : CommandArgs::CommandArgs() {
    expected_args.insert(expected_args.end(), args.begin(), args.end());
}

/**
 * @brief Destructor for the CommandArgs class.
 */
CommandArgs::~CommandArgs() {
    for (auto arg : expected_args) {
        delete arg;
    }
}

/**
 * @brief Parses the provided arguments.
 * @param args The arguments to parse.
 * @return A ParsedArgs object containing the parsed arguments.
 */
ParsedArgs CommandArgs::parseArgs(const std::vector<std::string> args) const {
    ParsedArgs result;

    for (int i = 0; i < args.size(); i++) {
        for (const auto& arg : expected_args) {
            if (arg->parse(args[i].c_str())) { // Argument found
                if (arg->isValuable()) {
                    if (args.size() <= i + 1) {
                        // TODO: Throw value for arg expected
                    }

                    std::string value = args[++i];
                    result.parsed_args[arg->getName()] = value;
                    break;
                }
                result.parsed_args[arg->getName()] = std::string();
                break;
            }

            if (arg->isRequired()) {
                // TODO: Throw arg expected
            }
        }
    }
    return result;
}

/**
 * @brief Checks if an argument exists in the parsed arguments.
 * @param str The name of the argument.
 * @return true if the argument exists, otherwise false.
 */
bool ParsedArgs::hasArg(const char* str) const {
    return parsed_args.find(str) != parsed_args.end();
}

/**
 * @brief Retrieves the value of an argument.
 * @param arg_name The name of the argument.
 * @return The value of the argument.
 */
std::string ParsedArgs::getArgValue(const std::string& arg_name) {
    if (this->hasArg(arg_name.c_str())) {
        return parsed_args[arg_name];
    }
    return {};
}