/**
 * @file repository.hpp
 * @brief Header file for the repository and commit management classes.
 */

#ifndef REPO_HPP
#define REPO_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "commit_state.hpp"

/**
 * @struct Commit
 * @brief Represents a commit in the version control system.
 */
struct Commit {
    std::string message; ///< The commit message.
    std::string hash;    ///< The commit hash.
    std::shared_ptr<Commit> prev; ///< Pointer to the previous commit.
    std::shared_ptr<CommitState> state; ///< The state of the commit (files and their content).

    /**
     * @brief Constructs a new Commit object.
     * @param msg The commit message.
     * @param h The commit hash.
     * @param p Pointer to the previous commit (default is nullptr).
     */
    Commit(std::string msg, std::string h, std::shared_ptr<Commit> p = nullptr)
            : message(std::move(msg)),
              hash(std::move(h)),
              prev(std::move(p)),
              state(std::make_shared<CommitState>()) {}
};

/**
 * @class PersistenceStack
 * @brief Manages the history of commits and branches.
 */
class PersistenceStack {
private:
    std::shared_ptr<Commit> head; ///< Pointer to the latest commit.
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches; ///< Map of branch names to their latest commit.

    /**
     * @brief Generates a hash for a commit.
     * @param message The commit message.
     * @return The generated hash.
     */
    std::string generate_hash(const std::string& message);

public:
    /**
     * @brief Constructs a new PersistenceStack object with a starting branch.
     * @param startBranchName The name of the starting branch.
     */
    PersistenceStack(const std::string& startBranchName);

    /**
     * @brief Default constructor.
     */
    PersistenceStack();

    /**
     * @brief Creates a new commit.
     * @param message The commit message.
     */
    void commit(const std::string& message);

    /**
     * @brief Reverts to the previous commit.
     * @throws NoCommitsLeftException If there are no commits to revert to.
     */
    void revert_previous();

    /**
     * @brief Creates a new branch.
     * @param branch_name The name of the new branch.
     * @throws std::invalid_argument If the branch name is empty.
     * @throws BranchAlreadyExistsException If the branch already exists.
     */
    void create_branch(const std::string& branch_name);

    /**
     * @brief Switches to a different branch.
     * @param branch_name The name of the branch to switch to.
     * @throws BranchNotFoundException If the branch does not exist.
     */
    void checkout_branch(const std::string& branch_name);

    /**
     * @brief Merges a branch into the current branch.
     * @param branch_name The name of the branch to merge.
     * @throws BranchNotFoundException If the branch does not exist.
     * @throws std::runtime_error If the branches have no common commit.
     */
    void merge(const std::string& branch_name);

    /**
     * @brief Checks if the persistence stack is valid.
     * @return True if the stack is valid, false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Logs the commit history.
     */
    void log() const;
};

/**
 * @struct RepoSettings
 * @brief Represents the settings for initializing a repository.
 */
struct RepoSettings {
    std::string str_repoName; ///< The name of the repository.
    std::string str_startBranchName; ///< The name of the starting branch.
};

/**
 * @class Repo
 * @brief Manages the repository and its persistence stack.
 */
class Repo {
private:
    std::string repoName; ///< The name of the repository.
    PersistenceStack branchStack; ///< The persistence stack for managing commits and branches.
    static Repo* stashRepository; ///< Singleton instance of the repository.

    /**
     * @brief Default constructor.
     */
    Repo();

    /**
     * @brief Constructs a new Repo object with settings.
     * @param settings The repository settings.
     */
    explicit Repo(const RepoSettings& settings);

public:
    /**
     * @brief Initializes the repository with the given settings.
     * @param settings The repository settings.
     */
    void initRepository(const RepoSettings& settings);

    /**
     * @brief Gets the singleton instance of the repository.
     * @return Reference to the repository instance.
     */
    static Repo& getInstance();

    /**
     * @brief Checks if the repository is empty.
     * @return True if the repository is empty, false otherwise.
     */
    static bool IsEmpty();
};

#endif // REPO_HPP