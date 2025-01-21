//
// Created by Timmie on 21.01.2025.
//

#ifndef INVOKER_HPP
#define INVOKER_HPP
#include "command.hpp"

#include <iostream>

class CommandInvoker
{
private:
    std::unordered_map<std::string, const Command*> commands;
public:
    CommandInvoker();
    void init_command(const Command* command);
    bool invoke_command(int argc, char* argv[]);
};

#endif //INVOKER_HPP
