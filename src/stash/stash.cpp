#include "stash.hpp"
#include "../logger/logger.hpp"
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

RepoSettings ask_repo_stuff()
{
    RepoSettings settings;

    std::cout << "Please specify repository name:" << std::endl;
    std::cin >> settings.str_repoName;

    std::cout << "Please specify start branch name:" << std::endl << "[core] ";
    std::cin >> settings.str_startBranchName;
    settings.str_startBranchName = settings.str_startBranchName.empty() ? "core" : settings.str_startBranchName;

    return settings;
}

Stash::Stash() : vcs_repo(&Repo::getInstance(RepoSettings{"default_repo", "core"})) {
    initialize_stash_directory();
}

Stash::Stash(const RepoSettings &settings) : vcs_repo(&Repo::getInstance(settings)) {
    initialize_stash_directory();

    // Если репозиторий пуст, инициализируем его
    if (vcs_repo->fnIsEmpty()) {
        init_repo(settings);
    }
}

void Stash::initialize_stash_directory() {
    stash_path = fs::current_path() / "stash";

    if (!exists(stash_path)) {
        bool b_isCreated = create_directory(stash_path);
        if (!b_isCreated) {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Stash created");

        bool b_isSubCreated = create_directory(stash_path / "branches");
        if (!b_isSubCreated) {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Subdirectory \"branches\" created");
    }
}

void Stash::init_repo(const RepoSettings &settings) {
    if (vcs_repo->fnIsEmpty()) {
        vcs_repo = &Repo::getInstance(settings);
        vcs_repo->create_branch(settings.str_startBranchName);
        INFO("Repository initialized with name: " << settings.str_repoName);
    }
}

Repo &Stash::get_repo() {
    if (!vcs_repo) {
        std::cout << "no repo" << std::endl;
        RepoSettings settings = ask_repo_stuff();
        vcs_repo = &Repo::getInstance(settings); // Используем getInstance
        vcs_repo->create_branch(settings.str_startBranchName);
    }
    std::cout << "some repo" << std::endl;
    return *vcs_repo;
}