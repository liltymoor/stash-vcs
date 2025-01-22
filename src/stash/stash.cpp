#include "./stash.hpp"

#include "metadata.hpp"

#include <fstream>
#include <iostream>
#include "../vcs/repository.hpp"
#include "../logger/logger.hpp"

namespace fs = std::filesystem;


RepoSettings ask_repo_stuff() {
    RepoSettings settings;

    std::cout << "Please specify repository name:" << std::endl;
    std::cin >> settings.str_repoName;

    std::cout << "Please specify start branch name:" << std::endl;
    std::cin >> settings.str_startBranchName;
    settings.str_startBranchName = settings.str_startBranchName.empty() ? "core" : settings.str_startBranchName;

    return settings;
}

Stash::Stash() {
    if (!exists(stash_path))
    {
        bool b_isCreated = create_directory(stash_path);
        if (!b_isCreated)
        {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Stash created");

        bool b_isSubCreated = create_directory(stash_path / BRANCHES_FOLDER_NAME);
        if (!b_isSubCreated)
        {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Subdirectory \"branches\" created");

        isExists = true;
        // action to make useable Repo::getInstance()

        // Create repository

        RepoSettings settings = ask_repo_stuff();

        Repo::getInstance().initRepository(settings);

        if (Repo::IsEmpty())
        {
            ERROR("Repo wasn't initialized");
            return;
        }

        Repo::getInstance().stashMeta();

        INFO("Repository initialized");
    }
    {
        isExists = true;
        // action to make useable Repo::getInstance()
        Repo::getInstance();
    }
}

Stash& Stash::getInstance()
{
    static Stash instance;
    return instance;
}

bool Stash::stashExists()
{
    return isExists;
}

const std::filesystem::path & Stash::getStashPath()
{
    return stash_path;
}
