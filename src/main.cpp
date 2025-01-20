#include <iostream>
#include "command/command.hpp"

int main(int argc, char *argv[])
{
    std::vector<std::string> args = std::vector<std::string>(argv, argv + argc);
    Command* command = new Command("test", "Test command");
    command->execute(argc, argv);
}
