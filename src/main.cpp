#include "command/command.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Command *command = new Command("test", "Test command");
    command->execute(argc, argv);
}
