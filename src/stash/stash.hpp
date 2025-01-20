#ifndef STASH_HPP
#define STASH_HPP

#include "../vcs/repository.hpp"
#include <filesystem>
#include <iostream>

class Stash
{
    /*
    commits - считывание папки с commits,
    и все папки внутри, которые и являются коммитами
    
    возможность создавать коммиты
    т.е папку с хешем коммита и возможность добавлять туда файлы

    мета-данные о репозитории и о коммитах
    */
private:
    std::filesystem::path stash_path;
    Repo* vcs_repo;

public:
    Stash();
    void initialize_stash_directory();
    explicit Stash(const RepoSettings &settings);
    void init_repo(const RepoSettings &settings);
    Repo &get_repo(); // Возвращаем ссылку на синглтон

};

#endif //STASH_HPP