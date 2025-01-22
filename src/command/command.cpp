#include "command.hpp"

Command::Command(const char* name, const char* description) {
    this->name = name;
    this->description = description;
    this->expected_args = new CommandArgs();
}

Command::~Command() {
    delete this->expected_args;
}


void* Command::execute(int argc, char* argv[]) const {

    if (argv == nullptr)
    {
        ParsedArgs const parsed_args;
        return this->action(parsed_args);
    }

    std::vector<std::string> const args = std::vector<std::string>(argv, argv + (argc - 2));
    ParsedArgs const parsed_args = this->expected_args->parseArgs(args);

    return this->action(parsed_args);
}

void* Command::action(ParsedArgs const args) const
{
    if (args.hasArg("description")) { // если description есть
        std::cout << description << std::endl;
        return nullptr;
    }

    if (args.hasArg("verbose")) { // если verbose есть
        std::cout << "Verbose mode is on" << std::endl;
    }

    std::cout << "Hello world!" << std::endl;

    return nullptr;
}

const char * Command::get_name() const
{
    return this->name;
}

const char * Command::get_desc() const
{
    return this->description;
}
