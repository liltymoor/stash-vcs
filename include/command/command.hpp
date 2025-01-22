#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <functional>
#include "command_args.hpp"

class Command {
private:
    const char* name;
    const char* description;

protected:
    virtual void* action(ParsedArgs args) const;
    CommandArgs* expected_args;
public:
    Command(const char* name, const char* description);
    ~Command();

    void* execute(int argc, char* argv[]) const;

    const char* get_name() const;
    const char* get_desc() const;
};

#endif
