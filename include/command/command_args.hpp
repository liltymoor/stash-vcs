#ifndef COMMAND_ARGS_H
#define COMMAND_ARGS_H

#include <iostream>
#include <vector>
#include <unordered_map>

/**
 * @class Arg
 * @brief Represents a command argument.
 *
 * The Arg class contains information about the argument's name, description, short name,
 * and flags indicating whether the argument is required and whether it requires a value.
 */
class Arg {
private:
    std::string str_name;           ///< The name of the argument.
    std::string str_description;    ///< The description of the argument.
    std::string str_short_name;     ///< The short name of the argument.

    bool b_valuable;                ///< Flag indicating whether the argument requires a value.
    bool b_required;                ///< Flag indicating whether the argument is required.

    std::vector<std::string> definitions; ///< List of valid values for the argument.

public:
    /**
     * @brief Constructor for the Arg class.
     * @param name The name of the argument.
     * @param desc The description of the argument.
     */
    Arg(const char* name, const char* desc);

    /**
     * @brief Constructor for the Arg class with valid values.
     * @param name The name of the argument.
     * @param desc The description of the argument.
     * @param definitions List of valid values.
     */
    Arg(const char* name, const char* desc, std::vector<std::string> definitions);

    /**
     * @brief Constructor for the Arg class with flags.
     * @param name The name of the argument.
     * @param desc The description of the argument.
     * @param required Flag indicating whether the argument is required.
     * @param valuable Flag indicating whether the argument requires a value.
     */
    Arg(const char* name, const char* desc, bool required, bool valuable);

    /**
     * @brief Constructor for the Arg class with valid values and flags.
     * @param name The name of the argument.
     * @param desc The description of the argument.
     * @param definitions List of valid values.
     * @param required Flag indicating whether the argument is required.
     * @param valuable Flag indicating whether the argument requires a value.
     */
    Arg(const char* name, const char* desc, std::vector<std::string> definitions, bool required, bool valuable);

    /**
     * @brief Checks if the argument is required.
     * @return true if the argument is required, otherwise false.
     */
    bool isRequired() const;

    /**
     * @brief Checks if the argument requires a value.
     * @return true if the argument requires a value, otherwise false.
     */
    bool isValuable() const;

    /**
     * @brief Returns the name of the argument.
     * @return The name of the argument.
     */
    const char* getName() const;

    /**
     * @brief Returns the description of the argument.
     * @return The description of the argument.
     */
    const char* getDescription() const;

    /**
     * @brief Parses the argument value.
     * @param str The value to parse.
     * @return true if the value is valid, otherwise false.
     */
    bool parse(const char* str) const;

    /**
     * @brief Compares two Arg objects for equality.
     * @param other The other Arg object to compare with.
     * @return true if the arguments are equal, otherwise false.
     */
    bool operator==(const Arg& other) const;
};

/**
 * @struct ParsedArgs
 * @brief Represents parsed command arguments.
 */
struct ParsedArgs {
    std::unordered_map<std::string, std::string> parsed_args; ///< Map of parsed arguments.

    /**
     * @brief Checks if an argument exists.
     * @param str The name of the argument.
     * @return true if the argument exists, otherwise false.
     */
    bool hasArg(const char* str) const;

    /**
     * @brief Returns the value of an argument.
     * @param str The name of the argument.
     * @return The value of the argument.
     */
    std::string getArgValue(const std::string& str);
};

/**
 * @class CommandArgs
 * @brief Represents a collection of expected command arguments.
 */
class CommandArgs {
protected:
    std::vector<Arg*> expected_args; ///< List of expected arguments.

public:
    /// Default constructor for the CommandArgs class.
    CommandArgs();

    /**
     * @brief Constructor for the CommandArgs class with arguments.
     * @param args List of expected arguments.
     */
    CommandArgs(std::vector<Arg*> args);

    /**
     * @brief Parses the provided arguments.
     * @param args The arguments to parse.
     * @return A ParsedArgs object containing the parsed arguments.
     */
    virtual ParsedArgs parseArgs(const std::vector<std::string> args) const;

    std::vector<Arg*> get_expected_args() const;

    /// Destructor for the CommandArgs class.
    virtual ~CommandArgs();
};

#endif // COMMAND_ARGS_H