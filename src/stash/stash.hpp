#ifndef STASH_HPP
#define STASH_HPP

#include <filesystem>
#include <iostream>
#include "../vcs/repository.hpp"

class Stash {
    /*
    commits - считывание папки с commits,
    и все папки внутри, которые и являются коммитами
    
    возможность создавать коммиты
    т.е папку с хешем коммита и возможность добавлять туда файлы

    мета-данные о репозитории и о коммитах
    */
private:
    static void init_repo();
    static void init_repo(const RepoSettings&);

    inline const static std::filesystem::path stash_path = "./stash";

    static Repo vcs_repo;

    static Repo get_repo();
public:
    Stash();
    explicit Stash(const RepoSettings&);
};

#endif //STASH_HPP