#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>
#include "command_args.hpp"

class Command {
private:
    const char* name;
    const char* description;

    CommandArgs* expected_args;

protected:
    virtual void* action(ParsedArgs args) const;
public:
    Command(const char* name, const char* description);
    ~Command();

    void* execute(int argc, char* argv[]) const;

    const char* get_name() const;
};

#endif
