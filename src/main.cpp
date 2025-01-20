#include "command/command.hpp"
#include "repository.hpp"
#include "stash.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    Command *command = new Command("test", "Test command");
    command->execute(argc, argv);


    Stash stash;

    // Получаем репозиторий
    Repo &repo = stash.get_repo();

    // Проверяем, инициализирован ли репозиторий
    if (!repo.fnIsEmpty())
    {
        std::cout << "Repository is initialized!" << std::endl;
    }
    else
    {
        std::cout << "Repository is not initialized!" << std::endl;
    }

}
