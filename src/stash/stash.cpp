/**
 * @file stash.cpp
 * @brief Implementation of the Stash class.
 */

#include "./stash.hpp"
#include <fstream>
#include <iostream>
#include "../logger/logger.hpp"

namespace fs = std::filesystem;

/**
 * @brief Prompts the user to input repository settings.
 * @return A RepoSettings object containing the repository name and start branch name.
 */
RepoSettings ask_repo_stuff() {
    RepoSettings settings;

    std::cout << "Please specify repository name:" << std::endl;
    std::cin >> settings.str_repoName;

    std::cout << "Please specify start branch name:" << std::endl;
    std::cin >> settings.str_startBranchName;
    settings.str_startBranchName = settings.str_startBranchName.empty() ? "core" : settings.str_startBranchName;

    return settings;
}

/**
 * @brief Constructs a new Stash object.
 *
 * This constructor initializes the `.stash` directory and its subdirectories.
 * It also initializes the repository if it doesn't already exist.
 */
Stash::Stash() {
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

        // Create repository
        Repo::getInstance().initRepository(ask_repo_stuff());

        if (Repo::IsEmpty()) {
            ERROR("Repo wasn't initialized");
            return;
        }

        INFO("Repository initialized");
    }
}

/**
 * @brief Gets the singleton instance of the Stash class.
 * @return Reference to the Stash instance.
 */
Stash& Stash::getInstance() {
    static Stash instance;
    return instance;
}