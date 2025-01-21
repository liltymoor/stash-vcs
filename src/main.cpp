#include "stash.hpp"

#include <iostream>
#include "command/command.hpp"

int main(int argc, char *argv[])
{
    Stash::getInstance();
    Command* command = new Command("test", "Test command");
    command->execute(argc, argv);
}
