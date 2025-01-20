#include "./stash.hpp"
#include <fstream>
#include <iostream>

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
    // If repo is already exists
    if (!vcs_repo.fnIsEmpty()) return;

    
    if (!fs::exists(Stash::stash_path)) return;

    // Init repo
    RepoSettings settings = ask_repo_stuff();

    vcs_repo = Repo(settings);
}

Stash::Stash(const RepoSettings& settings) {
        // If repo is already exists
    if (!vcs_repo.fnIsEmpty()) return;
    
    vcs_repo = Repo(settings);
}