#include "repository.hpp"
#include "metadata.hpp"
#include "../exception/pers_stack_exc.h"
#include <variant>
#include "../logger/logger.hpp"

/**
 * @brief Constructor for the Commit class.
 * @param metaData The metadata for the commit.
 * @param commit_files The files associated with the commit.
 */
Commit::Commit(std::map<std::string, std::string>& metaData, const std::unordered_map<std::string, File>& commit_files) {
    message = metaData[META_COMMIT_MSG];
    hash = metaData[META_COMMIT_HASH];
    branch = metaData[META_COMMIT_BRANCH];

    prev = nullptr;

    // Initialize commit files
    state = std::make_shared<CommitState>();

    for (const auto &[filename, file] : commit_files)
        state->addFile(filename, file);
}

/**
 * @brief Default constructor for the PersistenceStack class.
 */
PersistenceStack::PersistenceStack() {}

/**
 * @brief Constructor for the PersistenceStack class with a current branch.
 * @param currentBranch The current branch.
 */
PersistenceStack::PersistenceStack(std::string currentBranch) {
    if (exists(Stash::getStashPath() / META_BRANCH_FOLDER) && !currentBranch.empty()) {
        try {
            this->currentBranch = currentBranch;
            for (const auto &entry : std::filesystem::directory_iterator(Stash::getStashPath() / META_BRANCH_FOLDER)) {
                if (entry.is_directory()) {
                    init_branch(entry.path().filename());
                }
            }
            // Initialize head after the branches
            head = branches[currentBranch];
        } catch (const std::filesystem::filesystem_error &e) {
            std::__throw_bad_variant_access(e.what());
        }
    }
}

/**
 * @brief Returns the current branch.
 * @return The current branch.
 */
std::string PersistenceStack::getCurrentBranch() const {
    return currentBranch;
}

/**
 * @brief Migrates to a new branch.
 * @param branch_name The name of the branch to migrate to.
 */
void PersistenceStack::migrateBranch(const std::string &branch_name) {
    if (branches.find(branch_name) == branches.end()) {
        throw BranchNotFoundException(branch_name);
    }

    head = branches[branch_name];
    currentBranch = branch_name;
}

/**
 * @brief Checks if the persistence stack is valid.
 * @return true if valid, otherwise false.
 */
bool PersistenceStack::isValid() const {
    return branches.empty();
}

/**
 * @brief Saves metadata to the stash.
 */
void PersistenceStack::stashMeta() const {
    std::map<std::string, std::string> metadataBranch;

    if (head == nullptr) {
        // TODO: Unsafe place
        return;
    }

    metadataBranch[META_CURRENT_HEAD] = head->hash;
    if (head->prev != nullptr) {
        std::map<std::string, std::string> metadataFirstCommit;
        metadataFirstCommit[META_COMMIT_HASH] = head->hash;
        metadataFirstCommit[META_COMMIT_PREV] = head->prev->hash;
        metadataFirstCommit[META_COMMIT_MSG] = head->message;
        metadataFirstCommit[META_COMMIT_BRANCH] = head->branch;

        MetadataHandler::save((Repo::getBranchesPath() / currentBranch / head->hash / META_FILENAME).c_str(),
                              metadataFirstCommit);

        Commit prev = *head->prev;
        while (prev.prev != nullptr) {
            std::map<std::string, std::string> metadataCommit;
            metadataCommit[META_COMMIT_HASH] = prev.hash;
            metadataCommit[META_COMMIT_PREV] = prev.prev->hash;
            metadataCommit[META_COMMIT_MSG] = prev.message;
            metadataCommit[META_COMMIT_BRANCH] = prev.branch;

            MetadataHandler::save((Repo::getBranchesPath() / currentBranch / prev.hash / META_FILENAME).c_str(),
                                  metadataCommit);
            prev = *prev.prev;
        }

        std::map<std::string, std::string> metadataLastCommit;
        metadataLastCommit[META_COMMIT_HASH] = prev.hash;
        metadataLastCommit[META_COMMIT_PREV] = "NULL";
        metadataLastCommit[META_COMMIT_MSG] = prev.message;
        metadataLastCommit[META_COMMIT_BRANCH] = prev.branch;

        MetadataHandler::save((Repo::getBranchesPath() / currentBranch / prev.hash / META_FILENAME).c_str(),
                              metadataLastCommit);
    } else {
        std::map<std::string, std::string> metadataLastCommit;
        metadataLastCommit[META_COMMIT_HASH] = head->hash;
        metadataLastCommit[META_COMMIT_PREV] = "NULL";
        metadataLastCommit[META_COMMIT_MSG] = head->message;
        metadataLastCommit[META_COMMIT_BRANCH] = head->branch;

        MetadataHandler::save((Repo::getBranchesPath() / currentBranch / head->hash / META_FILENAME).c_str(),
                              metadataLastCommit);
    }

    MetadataHandler::save((Repo::getBranchesPath() / currentBranch / META_FILENAME).c_str(), metadataBranch);
}

/**
 * @brief Moves branch files to a new location.
 * @param branch_name The name of the branch.
 */
void PersistenceStack::move_branch_files(const std::string &branch_name) {
    if (branches.find(branch_name) == branches.end()) {
        throw BranchNotFoundException(branch_name);
    }

    if (branches[branch_name] == nullptr) {
        throw std::runtime_error("Tried to transfer files from branch to core dir, but branch has no commits");
    }

    std::shared_ptr<Commit> branchHead = branches[branch_name];
    std::string originBranch = currentBranch;

    // Delete files that are committed anywhere in Stash
    for (const auto &entry : std::filesystem::directory_iterator(".")) {
        if (!entry.is_regular_file()) continue;

        // Contains in current branch HEAD
        auto currentBranchFiles = branches[currentBranch]->state->getFiles();
        if (currentBranchFiles.find(entry.path().filename()) != currentBranchFiles.end()) {
            std::filesystem::remove(entry.path());
            continue;
        }

        // Contains in next branch HEAD
        auto nextBranchFiles = branches[branch_name]->state->getFiles();
        if (nextBranchFiles.find(entry.path().filename()) != nextBranchFiles.end()) {
            std::filesystem::remove(entry.path());
            continue;
        }
    }

    if (branchHead->branch != currentBranch) { // Commit takes its origin from another branch
        originBranch = branchHead->branch;
    }

    auto path = std::filesystem::current_path();
    std::filesystem::current_path(Repo::getBranchesPath() / originBranch / branches[branch_name]->hash / META_COMMIT_FILES_FOLDER);
    File::copy_files(".*", path.c_str(), true);
    std::filesystem::current_path(path);
}

/**
 * @brief Converts the repository settings to a map of key-value pairs.
 * @return A map of settings.
 */
std::map<std::string, std::string> RepoSettings::map_settings() const {
    std::map<std::string, std::string> map;
    map["RepositoryName"] = str_repoName;
    map["StartBranchName"] = str_startBranchName;
    return map;
}

/**
 * @brief Creates a new commit with a message.
 * @param message The commit message.
 */
void PersistenceStack::commit(const std::string &message) {
    if (message.empty())
        throw std::invalid_argument("Message cannot be empty");

    if (File::getFilesFromDir(Repo::getBranchesPath() / currentBranch / META_STAGE_FOLDER).empty()) {
        throw std::runtime_error("No changes staged");
    }

    const auto newCommit = std::make_shared<Commit>(message, generate_hash(message), currentBranch, head);
    create_directories(Repo::getBranchesPath() / currentBranch / newCommit->hash / META_COMMIT_FILES_FOLDER);

    // It could be that head is empty (fresh branch with no commits)
    auto statedFiles = head == nullptr ? std::unordered_map<std::string, File>() : head->state->getFiles();
    auto stagedFiles = File::getFilesFromDir(Repo::getBranchesPath() / currentBranch / META_STAGE_FOLDER);

    for (const auto &[filename, file] : statedFiles) {
        newCommit->state->addFile(filename, file);
        File::copy_files(filename,
                         Repo::getBranchesPath() / currentBranch / newCommit->hash / META_COMMIT_FILES_FOLDER,
                         false);
    }

    for (const auto &[filename, file] : stagedFiles) {
        newCommit->state->addFile(filename, file);
        File::copy_files(filename,
                         Repo::getBranchesPath() / currentBranch / newCommit->hash / META_COMMIT_FILES_FOLDER,
                         false);
    }

    File::clean_dir(Repo::getBranchesPath() / getCurrentBranch() / META_STAGE_FOLDER);

    head = newCommit;
    branches[currentBranch] = head;

    INFO("Commited " << stagedFiles.size() << " files");

    INFO("New commit HEAD is " << ((head->prev == nullptr) ? "NULL" : head->prev->hash) << "--->" << head->hash);
}

/**
 * @brief Creates a new commit from an existing Commit object.
 * @param commit The Commit object.
 */
void PersistenceStack::commit(const Commit &commit) {
    std::string msg = "Reverted to commit " + commit.hash;

    const auto newCommit = std::make_shared<Commit>(msg, generate_hash(msg), currentBranch, head);
    create_directories(Repo::getBranchesPath() / currentBranch / newCommit->hash / META_COMMIT_FILES_FOLDER);

    // It could be that head is empty (fresh branch with no commits)
    auto statedFiles = head == nullptr ? std::unordered_map<std::string, File>() : commit.state->getFiles();

    for (const auto &[filename, file] : statedFiles) {
        // TODO: Unsafe place (produces new commit with old paths to data)
        newCommit->state->addFile(filename, file);
        File::copy_files(Repo::getBranchesPath() / commit.branch / commit.hash / META_COMMIT_FILES_FOLDER / filename,
                         Repo::getBranchesPath() / newCommit->branch / newCommit->hash / META_COMMIT_FILES_FOLDER,
                         false);
    }

    head = newCommit;
    branches[currentBranch] = head;

    INFO("Commited " << statedFiles.size() << " files");

    INFO("New commit HEAD is " << ((head->prev == nullptr) ? "NULL" : head->prev->hash) << "--->" << head->hash);
}

/**
 * @brief Stages files for the next commit.
 * @param files The files to stage.
 */
void PersistenceStack::stage(const std::string &files) {
    if (files.empty())
        throw std::invalid_argument("Files cannot be empty");

    auto stage_dir = Repo::getBranchesPath() / currentBranch / META_STAGE_FOLDER;

    INFO("Already staged:");
    for (const auto& entry : std::filesystem::directory_iterator(stage_dir)) {
        if (is_regular_file(entry)) {
            INFO(entry.path().filename() << " : " << File::file_time_to_string(entry.last_write_time()));
        }
    }
    INFO("=============");
    uint32_t files_staged = File::copy_files(files, stage_dir, File::isRegexp(files));

    if (files_staged == 0) {
        INFO("No files to stage were found");
    } else {
        INFO("Files staged: " << files_staged);
    }
}

/**
 * @brief Creates a new branch.
 * @param branch_name The name of the branch.
 */
void PersistenceStack::create_branch(const std::string &branch_name) {
    if (branch_name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }

    if (branches.find(branch_name) != branches.end()) {
        throw BranchAlreadyExistsException(branch_name);
    }

    branches[branch_name] = head;
    // TODO: Make staged stores not the actual files but their filepaths
    create_directories(Repo::getBranchesPath() / branch_name / "staged");
    currentBranch = branch_name;

    INFO("Branch " << branch_name << " created");
}

/**
 * @brief Reverts to the previous commit.
 */
void PersistenceStack::revert_previous() {
    if (head->prev == nullptr) {
        throw NoCommitsLeftException();
    }

    head = head->prev;
}

/**
 * @brief Initializes a new branch.
 * @param branch_name The name of the branch.
 */
void PersistenceStack::init_branch(const std::string &branch_name) {
    if (branch_name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }

    if (branches.find(branch_name) != branches.end()) {
        throw BranchAlreadyExistsException(branch_name);
    }

    // TODO: Initialize commits state here
    auto meta = MetadataHandler::load(Repo::getBranchesPath() / branch_name / META_FILENAME);
    try {
        for (const auto &entry : std::filesystem::directory_iterator(Repo::getBranchesPath() / branch_name)) {
            if (entry.is_directory() && entry.path().filename() != "staged") {
                init_commit(entry.path());
            }
        }
        branches[branch_name] = commits[meta[META_CURRENT_HEAD]];
    } catch (const std::filesystem::filesystem_error &e) {
        std::__throw_bad_variant_access(e.what());
    }
}

/**
 * @brief Initializes a commit from a given hash.
 * @param commit_hash The hash of the commit.
 */
void PersistenceStack::init_commit(const std::filesystem::path &commit_hash) {
    auto metaCommit = MetadataHandler::load(commit_hash / META_FILENAME);
    std::shared_ptr<Commit> commitPtr = std::make_shared<Commit>(metaCommit, File::getFilesFromDir(commit_hash / META_COMMIT_FILES_FOLDER));
    commits[commitPtr->hash] = commitPtr;

    if (metaCommit[META_COMMIT_PREV] == "NULL") return;

    if (commits.find(metaCommit[META_COMMIT_PREV]) != commits.end()) { // If previous commit was already initialized
        commitPtr->prev = commits[metaCommit[META_COMMIT_PREV]];
    } else {
        init_commit(Repo::getBranchesPath() / metaCommit[META_COMMIT_BRANCH] / metaCommit[META_COMMIT_PREV]);
        commitPtr->prev = commits[metaCommit[META_COMMIT_PREV]];
    }
}

/**
 * @brief Reverts to a specific commit by hash.
 * @param hash The hash of the commit to revert to.
 */
void PersistenceStack::revert_to(const std::string &hash) {
    if (hash.empty())
        throw std::invalid_argument("Hash cannot be empty");

    if (hash == head->hash) {
        ERROR("Can't revert to itself");
        return;
    }

    if (head == nullptr) {
        ERROR("There are no commits in this branch yet");
        return;
    }

    auto commitIter = *head;

    do {
        commitIter = *commitIter.prev;
        if (commitIter.branch != currentBranch) { // No way to revert to another branch commit
            ERROR("Can't revert");
            ERROR("Commit " << hash << " not found in current branch (" << currentBranch << ")");
            return;
        }

        if (commitIter.hash == hash) {
            // Revert to given commit
            commit(commitIter);
            move_branch_files(currentBranch);
            return;
        }
    } while (commitIter.prev != nullptr);

    ERROR("Can't revert");
    ERROR("Commit " << hash << " not found in current branch (" << currentBranch << ")");
}

/**
 * @brief Checks out a branch.
 * @param branch_name The name of the branch.
 */
void PersistenceStack::checkout_branch(const std::string &branch_name) {
    if (branch_name.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }

    if (branches.find(branch_name) == branches.end()) {
        create_branch(branch_name);
    } else {
        if (currentBranch == branch_name)
            throw std::runtime_error("You can't checkout your current branch (" + branch_name + ")");
        INFO("Checkout branch " << currentBranch << "--->" << branch_name);
        move_branch_files(branch_name);
        migrateBranch(branch_name);
    }
}

/**
 * @brief Lists all branches.
 */
void PersistenceStack::list_branches() const {
    int branchCount = 0;
    for (const auto &[branchName, head] : branches) {
        branchCount++;
        INFO("Branch " << branchName << " | HEAD " << head->hash);
    }
    INFO("===========================")
    INFO("Branch count: " << branchCount);
}

/**
 * @brief Lists all commits in the current branch.
 */
void PersistenceStack::list_commits() const {
    if (head == nullptr) {
        ERROR("There are no commits in this branch yet");
        return;
    }

    int commitCount = 1;
    INFO("Branch " << currentBranch << " commits:\n")

    auto commitIter = *head;
    INFO("HEAD");
    do {
        if (commitIter.prev != nullptr)
            INFO(commitIter.prev->hash << "--->" << commitIter.hash)
        else
            INFO(head->hash)
        INFO("Message: " << commitIter.message);
        INFO("Files committed: " << commitIter.state->getFiles().size() << std::endl);
        commitCount++;
        commitIter = *commitIter.prev;
    } while (commitIter.prev != nullptr);
    INFO("===========================")
    INFO("Commit count: " << commitCount);
}

/**
 * @brief Merges a branch into the current branch.
 * @param branch_name The name of the branch to merge.
 */
void PersistenceStack::merge(const std::string &branch_name) {
    // Can be merged only if branches have at least 1 shared commit.
    // Maybe not...

    if (branches.find(branch_name) == branches.end()) {
        throw BranchNotFoundException(branch_name);
    }

    // auto targetBranch = branches[branch_name];
    // auto currentBranch = head;
    //
    // bool b_sharedCommitExists = false;
    // std::shared_ptr<Commit> targetCommit = targetBranch;
    // std::shared_ptr<Commit> currentCommit = currentBranch;
    //
    // while (targetCommit != nullptr && currentCommit != nullptr)
    // {
    //     if (targetCommit == currentCommit)
    //     {
    //         b_sharedCommitExists = true;
    //         break;
    //     }
    //     targetCommit = targetCommit->prev;
    //     currentCommit = currentCommit->prev;
    // }
    //
    // if (b_sharedCommitExists)
    // {
    //     // Merge
    // }
    // else
    // {
    //     throw std::runtime_error("Branches have no common commit and cannot be merged");
    // }
}

/**
 * @brief Generates a hash for a commit message.
 * @param branch_name The name of the branch.
 * @return The generated hash.
 */
std::string PersistenceStack::generate_hash(const std::string &branch_name) {
    auto timestamp = time(0);
    return std::to_string(std::hash<std::string>{}(branch_name + std::to_string(timestamp)));
}

/**
 * @brief Default constructor for the Repo class.
 */
Repo::Repo()
        : repoName("~none") {
    if (exists(Stash::getStashPath() / META_FILENAME)) {
        std::map<std::string, std::string> metadata = MetadataHandler::load(Stash::getStashPath() / META_FILENAME);
        // TODO: Validate metadata
        repoName = metadata[META_REPO_NAME];
        branchStack = PersistenceStack(metadata[META_CURRENT_BRANCH]);
    }
}

/**
 * @brief Constructor for the Repo class with settings.
 * @param settings The repository settings.
 */
Repo::Repo(const RepoSettings &settings)
        : repoName(settings.str_repoName) {
    branchStack.create_branch(settings.str_startBranchName);
}

/**
 * @brief Initializes the repository with settings.
 * @param settings The repository settings.
 */
void Repo::initRepository(const RepoSettings &settings) {
    // TODO: Validate
    this->repoName = settings.str_repoName;
    this->branchStack = PersistenceStack();
    branchStack.create_branch(settings.str_startBranchName);
}

/**
 * @brief Saves metadata to the stash.
 */
void Repo::stashMeta() const {
    std::map<std::string, std::string> metadata;
    metadata[META_REPO_NAME] = repoName;
    metadata[META_CURRENT_BRANCH] = branchStack.getCurrentBranch();

    MetadataHandler::save((Stash::getStashPath() / META_FILENAME).c_str(), metadata);

    branchStack.stashMeta();
}

/**
 * @brief Saves metadata to the stash with settings.
 * @param settings The repository settings.
 */
void Repo::stashMeta(const RepoSettings &settings) {
    MetadataHandler::save((Stash::getStashPath() / META_FILENAME).c_str(), settings.map_settings());
    branchStack.stashMeta();
}

/**
 * @brief Returns the persistence stack for the repository.
 * @return Reference to the persistence stack.
 */
PersistenceStack &Repo::getRepoStack() {
    return this->branchStack;
}

/**
 * @brief Returns the singleton instance of the repository.
 * @return Reference to the repository instance.
 */
Repo &Repo::getInstance() {
    if (!Stash::stashExists())
        throw std::runtime_error("Stash does not exist. You should initialize it with command \"stash init\"");
    static Repo stashRepository = Repo();
    return stashRepository;
}

/**
 * @brief Returns the path to the branches folder.
 * @return The path to the branches folder.
 */
std::filesystem::path Repo::getBranchesPath() {
    return branchesPath;
}

/**
 * @brief Checks if the repository is empty.
 * @return true if the repository is empty, otherwise false.
 */
bool Repo::IsEmpty() {
    Repo stashRepository = Repo::getInstance();
    return stashRepository.branchStack.isValid();
}