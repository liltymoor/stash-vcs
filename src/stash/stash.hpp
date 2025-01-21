#ifndef STASH_HPP
#define STASH_HPP

#include <filesystem>

class Stash {
    /*
    commits - считывание папки с commits,
    и все папки внутри, которые и являются коммитами
    
    возможность создавать коммиты
    т.е папку с хешем коммита и возможность добавлять туда файлы

    мета-данные о репозитории и о коммитах
    */
private:
    inline const static std::filesystem::path stash_path = "./.stash";
    Stash();
public:
    static Stash& getInstance();
    static const std::filesystem::path& getStashPath();
};

#endif //STASH_HPP