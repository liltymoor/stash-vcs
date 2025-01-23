#include "./stash.hpp"
#include "metadata.hpp"
#include <fstream>
#include <iostream>
#include "../vcs/repository.hpp"
#include "../logger/logger.hpp"

namespace fs = std::filesystem;

/**
 * @brief Prompts the user to specify repository settings.
 * @return A RepoSettings object containing the user-provided settings.
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
 * @brief Constructor for the Stash class.
 *
 * Initializes the stash directory and repository if it doesn't already exist.
 */
Stash::Stash() {
    if (!exists(stash_path)) {
        bool b_isCreated = create_directory(stash_path);
        if (!b_isCreated) {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Stash created");

        bool b_isSubCreated = create_directory(stash_path / META_BRANCH_FOLDER);
        if (!b_isSubCreated) {
            ERROR("Can't create stash directory");
            return;
        }

        INFO("Subdirectory \"branches\" created");

        isExists = true;

        // Create repository
        RepoSettings settings = ask_repo_stuff();

        Repo::getInstance().initRepository(settings);

        if (Repo::IsEmpty()) {
            ERROR("Repo wasn't initialized");
            return;
        }

        Repo::getInstance().stashMeta();

        INFO("Repository initialized");
    } else {
        isExists = true;
        // Ensure Repo::getInstance() is usable
        Repo::getInstance();
    }
}

/**
 * @brief Returns the singleton instance of the Stash class.
 * @return Reference to the Stash instance.
 */
Stash& Stash::getInstance() {
    static Stash instance;
    return instance;
}

/**
 * @brief Checks if the stash directory exists.
 * @return true if the stash exists, otherwise false.
 */
bool Stash::stashExists() {
    return isExists;
}

/**
 * @brief Returns the path to the stash directory.
 * @return The path to the stash directory.
 */
const std::filesystem::path& Stash::getStashPath() {
    return stash_path;
}