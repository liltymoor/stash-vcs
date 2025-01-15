#include <iostream>
#include <functional>
#include "command_args.hpp"

class Command {
private:
    const char* name;
    const char* description;

    std::function<void(const CommnandArgs* expected_args)> action;

public:
    Command(const char* name, const char* description);


};
