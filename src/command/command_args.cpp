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

    definitions.push_back(FULL_ARG(str_name));
    definitions.push_back(SHORT_ARG(str_short_name));
}

Arg::Arg(const char* name, const char* desc, std::vector<std::string> definitions)
: Arg::Arg(name, desc)
{
    this->definitions.insert(
        this->definitions.end(), definitions.begin(), definitions.end()
    );
}

Arg::Arg(const char *name, const char *desc, bool required, bool valuable)
    : Arg(name, desc)
{
    b_required = required;
    b_valuable = valuable;
}

Arg::Arg(const char *name, const char *desc, std::vector<std::string> definitions, bool required, bool valuable)
    : Arg(name, desc, definitions)
{
    b_required = required;
    b_valuable = valuable;
}

bool Arg::isRequired() const {
    return b_required;
}

bool Arg::isValuable() const
{
    return b_valuable;
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

CommandArgs::CommandArgs() {
    expected_args = std::vector<Arg*>();
    expected_args.push_back(new Arg("verbose", "Do explicit prints to console"));
    expected_args.push_back(new Arg("description", "Print command description to console"));
}

CommandArgs::CommandArgs(std::vector<Arg*> args)
: CommandArgs::CommandArgs()
{
    expected_args.insert(expected_args.end(), args.begin(), args.end());
}

CommandArgs::~CommandArgs() {
    for (auto arg : expected_args) {
        delete arg;
    }
}

ParsedArgs CommandArgs::parseArgs(const std::vector<std::string> args) const
{
    ParsedArgs result;

    for (int i = 0; i < args.size(); i++)
    {
        for (const auto &arg : expected_args)
        {
            if (arg->parse(args[i].c_str())) // Argument found
            {
                if (arg->isValuable())
                {
                    if (args.size() <= i + 1)
                    {
                        // TODO throw value for arg expected
                    }

                    std::string value = args[++i];
                    result.parsed_args[arg->getName()] = value;
                    break;

                }
                result.parsed_args[arg->getName()] = std::string();
                break;
            }

            if (arg->isRequired())
            {
                // TODO throw arg expected
            }
        }
    }
    return result;
}

bool ParsedArgs::hasArg(const char* str) const {
    return parsed_args.find(str) != parsed_args.end();
}

std::string ParsedArgs::getArgValue(const std::string &arg_name) {
    if (this->hasArg(arg_name.c_str()))
    {
        return parsed_args[arg_name];
    }
    return {};
}
