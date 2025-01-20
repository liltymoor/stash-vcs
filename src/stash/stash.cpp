#include "./stash.hpp"
#include <fstream>
#include <iostream>
#include "../logger/logger.hpp"

namespace fs = std::filesystem;


RepoSettings ask_repo_stuff() {
    RepoSettings settings;

    std::cout << "Please specify repository name:" << std::endl;
    std::cin >> settings.str_repoName;

    std::cout << "Please specify start branch name:" << std::endl << "[core] ";
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

        bool b_isSubCreated = create_directory(stash_path / "branches");
        if (!b_isSubCreated)
        {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Subdirectory \"branches\" created");
    }



    if (vcs_repo.fnIsEmpty())
    {
        // Init repo
        init_repo();
    }
}

Stash::Stash(const RepoSettings& settings) {
    // If repo is already exists
    if (!vcs_repo.fnIsEmpty()) return;
    
    vcs_repo = Repo(settings);
}

void Stash::init_repo()
{
    if (vcs_repo.fnIsEmpty())
        vcs_repo = Repo(ask_repo_stuff());
}

void Stash::init_repo(const RepoSettings &settings)
{
    if (vcs_repo.fnIsEmpty())
        vcs_repo = Repo(settings);
}

Repo Stash::get_repo()
{
    if (!vcs_repo.fnIsEmpty()) return vcs_repo;
    init_repo();
    return vcs_repo;
}