/**
 * @file command_args.cpp
 * @brief Implementation of the Arg, ParsedArgs, and CommandArgs classes.
 */

#include "command_args.hpp"

//
// ARG
//

#define FULL_ARG(arg) std::string("--") + arg
#define SHORT_ARG(arg) std::string("-") + arg

/**
 * @brief Constructs a new Arg object.
 * @param name The name of the argument.
 * @param desc A brief description of the argument.
 */
Arg::Arg(const char *name, const char *desc) {
    str_name = name;
    str_description = desc;
    str_short_name = str_name.at(0);

    b_required = false;

    definitions = std::vector<std::string>();

    definitions.push_back(FULL_ARG(str_name));
    definitions.push_back(SHORT_ARG(str_short_name));
}

/**
 * @brief Constructs a new Arg object with custom definitions.
 * @param name The name of the argument.
 * @param desc A brief description of the argument.
 * @param definitions A list of custom definitions for the argument.
 */
Arg::Arg(const char *name, const char *desc, std::vector<std::string> definitions)
        : Arg::Arg(name, desc) {
    this->definitions.insert(
            this->definitions.end(), definitions.begin(), definitions.end()
    );
}

/**
 * @brief Checks if the argument is required.
 * @return True if the argument is required, false otherwise.
 */
bool Arg::isRequired() const {
    return b_required;
}

/**
 * @brief Gets the name of the argument.
 * @return The name of the argument.
 */
const char *Arg::getName() const {
    return str_name.c_str();
}

/**
 * @brief Gets the description of the argument.
 * @return The description of the argument.
 */
const char *Arg::getDescription() const {
    return str_description.c_str();
}

/**
 * @brief Parses a string to check if it matches any of the argument's definitions.
 * @param str The string to parse.
 * @return True if the string matches any definition, false otherwise.
 */
bool Arg::parse(const char *str) const {
    if (str == NULL)
        return false;

    for (const auto &definition: definitions) {
        if (str == definition)
            return true;
    }

    return false;
}

/**
 * @brief Compares two Arg objects for equality.
 * @param other The other Arg object to compare with.
 * @return True if the arguments have the same name, false otherwise.
 */
bool Arg::operator==(const Arg &other) const {
    return str_name == other.str_name;
}

//
// COMMAND ARGS
//

/**
 * @brief Constructs a new CommandArgs object with default arguments.
 */
CommandArgs::CommandArgs() {
    expected_args = std::vector<Arg *>();
    expected_args.push_back(new Arg("verbose", "Do explicit prints to console"));
    expected_args.push_back(new Arg("description", "Print command description to console"));
}

/**
 * @brief Constructs a new CommandArgs object with custom arguments.
 * @param args A list of expected arguments.
 */
CommandArgs::CommandArgs(std::vector<Arg *> args)
        : CommandArgs::CommandArgs() {
    expected_args.insert(expected_args.end(), args.begin(), args.end());
}

/**
 * @brief Destructor for the CommandArgs object.
 */
CommandArgs::~CommandArgs() {
    for (auto arg: expected_args) {
        delete arg;
    }
}

/**
 * @brief Parses a list of command-line arguments.
 * @param args The list of arguments to parse.
 * @return A ParsedArgs object containing the parsed arguments.
 */
ParsedArgs CommandArgs::parseArgs(const std::vector<std::string> args) const {
    ParsedArgs result;

    for (const auto &token: args) {
        for (const auto &arg: expected_args) {
            if (arg->parse(token.c_str())) {
                result.parsed_args.insert(arg->getName());
                break;
            }
        }
    }
    return result;
}

/**
 * @brief Checks if a specific argument was parsed.
 * @param str The name of the argument to check.
 * @return True if the argument was parsed, false otherwise.
 */
bool ParsedArgs::hasArg(const char *str) const {
    return parsed_args.find(str) != parsed_args.end();
}