#include "command.hpp"

Command::Command(const char *name, const char *description)
{
    this->name = name;
    this->description = description;
    this->expected_args = new CommandArgs();

    this->action = [](ParsedArgs args) {
        if (args.hasArg("help"))
        { // если help есть
            std::cout << "Sorry, I don't know what to do" << std::endl;
            return nullptr;
        }

        if (args.hasArg("verbose"))
        { // если verbose есть
            std::cout << "Verbose mode is on" << std::endl;
            return nullptr;
        }

        std::cout << "Hello world!" << std::endl;

        return nullptr;
    };
}

Command::~Command()
{
    delete this->expected_args;
}


void Command::execute(int argc, char *argv[])
{
    std::vector<std::string> const args = std::vector<std::string>(argv + 1, argv + argc); //игнорируем имя команды
    ParsedArgs const parsed_args = this->expected_args->parseArgs(args);

    this->action(parsed_args);
}
