/**
 * @file repository.cpp
 * @brief Implementation of the PersistenceStack and Repo classes.
 */

#include "repository.hpp"
#include "../exception/pers_stack_exc.h"

/**
 * @brief Default constructor for PersistenceStack.
 */
PersistenceStack::PersistenceStack() {}

/**
 * @brief Checks if the persistence stack is valid.
 * @return True if the stack is valid (branches are empty), false otherwise.
 */
bool PersistenceStack::isValid() const {
    return branches.empty();
}

/**
 * @brief Constructs a new PersistenceStack object with a starting branch.
 * @param startBranchName The name of the starting branch.
 */
PersistenceStack::PersistenceStack(const std::string& startBranchName) : head(nullptr) {
    create_branch(startBranchName);
}

/**
 * @brief Creates a new commit.
 * @param message The commit message.
 * @throws std::invalid_argument If the message is empty.
 */
void PersistenceStack::commit(const std::string& message) {
    if (message.empty())
        throw std::invalid_argument("Message can not be empty");

    const auto newCommit = std::make_shared<Commit>(message, generate_hash(message), head);
    head = newCommit;
    // TODO Commit being produced not to the actual branch but in commit-stack.
}

/**
 * @brief Creates a new branch.
 * @param branch_name The name of the new branch.
 * @throws std::invalid_argument If the branch name is empty.
 * @throws BranchAlreadyExistsException If the branch already exists.
 */
void PersistenceStack::create_branch(const std::string& branch_name) {
    if (branch_name.empty()) {
        throw std::invalid_argument("Name can not be empty");
    }

    if (branches.find(branch_name) != branches.end()) {
        throw BranchAlreadyExistsException(branch_name);
    }

    branches[branch_name] = head;
    // TODO Track current branch
}

/**
 * @brief Reverts to the previous commit.
 * @throws NoCommitsLeftException If there are no commits to revert to.
 */
void PersistenceStack::revert_previous() {
    if (head->prev == nullptr) {
        throw NoCommitsLeftException();
    }

    head = head->prev;
}

// TODO To think of...
// void PersistenceStack::revert_to(const std::string &hash)
// {
//
// }

/**
 * @brief Switches to a different branch.
 * @param branch_name The name of the branch to switch to.
 * @throws BranchNotFoundException If the branch does not exist.
 */
void PersistenceStack::checkout_branch(const std::string& branch_name) {
    if (branches.find(branch_name) == branches.end()) {
        throw BranchNotFoundException(branch_name);
    }

    head = branches[branch_name];
}

/**
 * @brief Merges a branch into the current branch.
 * @param branch_name The name of the branch to merge.
 * @throws BranchNotFoundException If the branch does not exist.
 * @throws std::runtime_error If the branches have no common commit.
 */
void PersistenceStack::merge(const std::string& branch_name) {
    // Can be merged only if branches have at least 1 shared commit.
    // Maybe not...

    if (branches.find(branch_name) == branches.end()) {
        throw BranchNotFoundException(branch_name);
    }

    auto targetBranch = branches[branch_name];
    auto currentBranch = head;

    bool b_sharedCommitExists = false;
    std::shared_ptr<Commit> targetCommit = targetBranch;
    std::shared_ptr<Commit> currentCommit = currentBranch;

    while (targetCommit != nullptr && currentCommit != nullptr) {
        if (targetCommit == currentCommit) {
            b_sharedCommitExists = true;
            break;
        }
        targetCommit = targetCommit->prev;
        currentCommit = currentCommit->prev;
    }

    if (b_sharedCommitExists) {
        // Merge
    } else {
        throw std::runtime_error("Branches have no common commit and cannot be merged");
    }
}

/**
 * @brief Generates a hash for a commit.
 * @param branch_name The name of the branch.
 * @return The generated hash.
 */
std::string PersistenceStack::generate_hash(const std::string& branch_name) {
    auto timestamp = time(0);
    return std::to_string(std::hash<std::string>{}(branch_name + std::to_string(timestamp)));
}

/**
 * @brief Default constructor for Repo.
 */
Repo::Repo() : repoName("~none"), branchStack(PersistenceStack("core")) {}

/**
 * @brief Constructs a new Repo object with settings.
 * @param settings The repository settings.
 */
Repo::Repo(const RepoSettings& settings)
        : repoName(settings.str_repoName), branchStack(PersistenceStack(settings.str_startBranchName)) {}

/**
 * @brief Initializes the repository with the given settings.
 * @param settings The repository settings.
 */
void Repo::initRepository(const RepoSettings& settings) {
    // TODO Validate
    this->repoName = settings.str_repoName;
    this->branchStack = PersistenceStack(settings.str_startBranchName);
}

/**
 * @brief Gets the singleton instance of the repository.
 * @return Reference to the repository instance.
 */
Repo& Repo::getInstance() {
    static Repo stashRepository = Repo();
    return stashRepository;
}

/**
 * @brief Checks if the repository is empty.
 * @return True if the repository is empty, false otherwise.
 */
bool Repo::IsEmpty() {
    Repo stashRepository = Repo::getInstance();
    return stashRepository.branchStack.isValid();
}