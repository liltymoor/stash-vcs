/**
 * @file command_args.h
 * @brief Header file for the Arg, ParsedArgs, and CommandArgs classes.
 */

#ifndef COMMAND_ARGS_H
#define COMMAND_ARGS_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>

/**
 * @class Arg
 * @brief Represents a command-line argument.
 *
 * This class defines a command-line argument with its name, description, and possible definitions.
 */
class Arg {
private:
    std::string str_name;          ///< The name of the argument.
    std::string str_description;   ///< A brief description of the argument.
    std::string str_short_name;    ///< The short name of the argument (usually the first character of the name).

    bool b_required;               ///< Whether the argument is required.

    std::vector<std::string> definitions; ///< Possible definitions (e.g., "--name", "-n").

public:
    /**
     * @brief Constructs a new Arg object.
     * @param name The name of the argument.
     * @param desc A brief description of the argument.
     */
    Arg(const char* name, const char* desc);

    /**
     * @brief Constructs a new Arg object with custom definitions.
     * @param name The name of the argument.
     * @param desc A brief description of the argument.
     * @param definitions A list of custom definitions for the argument.
     */
    Arg(const char* name, const char* desc, std::vector<std::string> definitions);

    /**
     * @brief Constructs a new Arg object with a required flag.
     * @param name The name of the argument.
     * @param desc A brief description of the argument.
     * @param required Whether the argument is required.
     */
    Arg(const char* name, const char* desc, bool required);

    /**
     * @brief Constructs a new Arg object with custom definitions and a required flag.
     * @param name The name of the argument.
     * @param desc A brief description of the argument.
     * @param definitions A list of custom definitions for the argument.
     * @param required Whether the argument is required.
     */
    Arg(const char* name, const char* desc, std::vector<std::string> definitions, bool required);

    /**
     * @brief Checks if the argument is required.
     * @return True if the argument is required, false otherwise.
     */
    bool isRequired() const;

    /**
     * @brief Gets the name of the argument.
     * @return The name of the argument.
     */
    const char* getName() const;

    /**
     * @brief Gets the description of the argument.
     * @return The description of the argument.
     */
    const char* getDescription() const;

    /**
     * @brief Parses a string to check if it matches any of the argument's definitions.
     * @param str The string to parse.
     * @return True if the string matches any definition, false otherwise.
     */
    bool parse(const char* str) const;

    /**
     * @brief Compares two Arg objects for equality.
     * @param other The other Arg object to compare with.
     * @return True if the arguments have the same name, false otherwise.
     */
    bool operator==(const Arg& other) const;
};

/**
 * @struct ParsedArgs
 * @brief Represents the parsed command-line arguments.
 */
struct ParsedArgs {
    std::unordered_set<std::string> parsed_args; ///< A set of parsed argument names.

    /**
     * @brief Checks if a specific argument was parsed.
     * @param str The name of the argument to check.
     * @return True if the argument was parsed, false otherwise.
     */
    bool hasArg(const char* str) const;
};

/**
 * @class CommandArgs
 * @brief Represents a collection of expected command-line arguments.
 */
class CommandArgs {
private:
    std::vector<Arg*> expected_args; ///< A list of expected arguments.

public:
    /**
     * @brief Constructs a new CommandArgs object with default arguments.
     */
    CommandArgs();

    /**
     * @brief Constructs a new CommandArgs object with custom arguments.
     * @param args A list of expected arguments.
     */
    CommandArgs(std::vector<Arg*> args);

    /**
     * @brief Parses a list of command-line arguments.
     * @param args The list of arguments to parse.
     * @return A ParsedArgs object containing the parsed arguments.
     */
    virtual ParsedArgs parseArgs(const std::vector<std::string> args) const;

    /**
     * @brief Destructor for the CommandArgs object.
     */
    virtual ~CommandArgs();
};

#endif // COMMAND_ARGS_H