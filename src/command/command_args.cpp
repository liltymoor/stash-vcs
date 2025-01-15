#include "command_args.hpp"

//
// ARGS
//

#define FULL_ARG(arg) std::string("--") + arg
#define SHORT_ARG(arg) std::string("-") + arg

Arg::Arg(const char* name, const char* desc) {
    str_name = name;
    str_description = desc;
    str_short_name = str_name.at(0);

    b_required = false;

    definitions = std::vector<std::string>();

    definitions.push_back(FULL_ARG(name));
    definitions.push_back(SHORT_ARG(name));
}

Arg::Arg(const char* name, const char* desc, std::vector<std::string> definitions)
: Arg::Arg(name, desc)
{
    this->definitions.insert(
        this->definitions.end(), definitions.begin(), definitions.end()
    );
}

bool Arg::isRequired() const {
    return b_required;
}

const char* Arg::getName() const {
    return str_name.c_str();
}

const char* Arg::getDescription() const {
    return str_description.c_str();
}

bool Arg::parse(const char* str) const {
    if (str == NULL)
        return false;

    for (const auto& definition : definitions) {
        if (str == definition)
            return true;
    }

    return false;
}

bool Arg::operator==(const Arg& other) const {
    return str_name == other.str_name;
}

//
// COMMAND ARGS
//

CommnandArgs::CommnandArgs() {
    expected_args = std::vector<Arg*>();
    expected_args.push_back(new Arg("verbose", "Do explicit prints to console"));
    expected_args.push_back(new Arg("help", "Print help message to console"));
}

CommnandArgs::CommnandArgs(std::vector<Arg*> args)
: CommnandArgs::CommnandArgs()
{
    expected_args.insert(expected_args.end(), args.begin(), args.end());
}

CommnandArgs::~CommnandArgs() {
    for (auto arg : expected_args) {
        delete arg;
    }
}

bool ParsedArgs::hasArg(const char* str) const {
    return parsed_args.find(str) != parsed_args.end();
}
