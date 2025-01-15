#include <iostream>
#include <functional>
#include "command_args.hpp"

class Command {
private:
    const char* name;
    const char* description;

    CommnandArgs* expected_args;

    std::function<void*(ParsedArgs)> action;

public:
    Command(const char* name, const char* description);
    ~Command();

    void execute(int argc, char* argv[]);
};
