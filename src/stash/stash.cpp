#include "./stash.hpp"
#include "metadata.hpp"
#include <filesystem>
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

    std::cout << "Commits must be signed [y/n]:" << std::endl;
    std::string answer;
    std::cin >> answer;
    settings.userSignedCommits = answer == "y";

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

        stashMeta();

        INFO("Repository initialized");
    } else {
        isExists = true;
        // Ensure Repo::getInstance() is usable
        loadFromStash();
        checkUserConfigured(true);
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

const std::filesystem::path& Stash::getUserPath() {
    return user_dir_path;
}

void Stash::setStashUsername(const std::string& username) {
    configUsername = username;
};

void Stash::setStashEmail(const std::string& email) {
    configEmail = email;
}

std::string Stash::getConfigUsername() const {
    return configUsername;
}

std::string Stash::getConfigEmail() const {
    return configEmail;
}

void Stash::loadFromStash() {
    if (exists(Stash::getStashPath() / META_FILENAME)) {
        std::map<std::string, std::string> metadata = MetadataHandler::load(Stash::getStashPath() / META_FILENAME);
        // TODO: Validate metadata
        configUsername = metadata[META_STASH_USERNAME]  == "#" ? std::string() : metadata[META_STASH_USERNAME];
        configEmail = metadata[META_STASH_EMAIL] == "#" ? std::string() : metadata[META_STASH_EMAIL];
    }
}

void Stash::stashMeta() const {
    auto metadata = Repo::getInstance().stashMeta();
    metadata[META_STASH_USERNAME] = configUsername.empty() ? "#" : configUsername;
    metadata[META_STASH_EMAIL] = configEmail.empty() ? "#" : configEmail;
    MetadataHandler::save((Stash::getStashPath() / META_FILENAME).c_str(), metadata);
}

bool Stash::checkUserConfigured(const bool& verbose) const {
    if (configUsername.empty() || configEmail.empty()) {
        if (verbose) WARN("Stash doesn't configured with username or email. This may lead to problems if your repository needs to be user-signed.");
        return false;
    }

    return true;
}