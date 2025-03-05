#ifndef REPO_HPP
#define REPO_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "commit_state.hpp"
#include "stash.hpp"
#include "state_diff.hpp"
#include <filesystem>

// Metadata constants
#define META_FILENAME "metadata" ///< Filename for metadata.
#define META_STAGE_FOLDER "staged" ///< Folder for staged files.
#define META_BRANCH_FOLDER "branches" ///< Folder for branches.
#define META_COMMIT_FILES_FOLDER "files" ///< Folder for commit files.

#define META_REPO_NAME "RepoName" ///< Metadata key for repository name.
#define META_CURRENT_BRANCH "CurrentBranch" ///< Metadata key for current branch.
#define META_CURRENT_HEAD "CurrentHead" ///< Metadata key for current head commit.
#define META_REPO_CORE_BRANCH "RepoStartBranch" ///< Metadata key for the core branch.

#define META_STASH_USER_SIGN "UserSignedCommits" ///< Metadata key for b_userSignedCommits
#define META_STASH_USERNAME "UsernameConfig" ///< Metadata key for storing username
#define META_STASH_EMAIL "EmailConfig" ///< Metadata key for storing email

#define META_COMMIT_MSG "CommitMessage" ///< Metadata key for commit message.
#define META_COMMIT_PREV "PrevCommit" ///< Metadata key for previous commit.
#define META_COMMIT_HASH "CommitHash" ///< Metadata key for commit hash.
#define META_COMMIT_BRANCH "CommitBranch" ///< Metadata key for commit branch.
#define META_COMMIT_USER "CommitUser" ///< Metadata key for storing commit owner's username

#define META_STAGE_FILES_TO_UNTRACK "Files2Untrack" ///< Metadata key for files being marked as untracked

/**
 * @struct Commit
 * @brief Represents a commit in the version control system.
 */
struct Commit {
    std::string message; ///< The commit message.
    std::string hash; ///< The commit hash.
    std::string branch; ///< The branch associated with the commit.

    std::shared_ptr<Commit> prev; ///< Pointer to the previous commit.
    std::shared_ptr<CommitState> state; ///< Pointer to the commit state.

    /**
     * @brief Constructor for Commit.
     * @param msg The commit message.
     * @param h The commit hash.
     * @param b The branch name.
     * @param p Pointer to the previous commit.
     */
    Commit(std::string msg, std::string h, std::string b, std::shared_ptr<Commit> p = nullptr)
            : message(std::move(msg)),
              hash(std::move(h)),
              branch(b),
              prev(std::move(p)),
              state(std::make_shared<CommitState>()) {}

    /**
     * @brief Constructor for Commit that initializes from metadata and files.
     * @param metaData The metadata for the commit.
     * @param commit_files The files associated with the commit.
     */
    Commit(std::map<std::string, std::string>& metaData, const std::unordered_map<std::string, File>& commit_files);

    Commit(const std::string& hash);

    void set_commit_files(const std::unordered_map<std::string, File>& commit_files);
    void set_commit_data(std::map<std::string, std::string>& metaData, const std::unordered_map<std::string, File>& commit_files);
    bool is_valid() const;
};

/**
 * @class PersistenceStack
 * @brief Manages the persistence stack for commits and branches.
 */
class PersistenceStack {
private:
    std::shared_ptr<Commit> head = nullptr; ///< Pointer to the head commit.
    std::string currentBranch; ///< The current branch.
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches; ///< Map of branch names to commits.
    std::unordered_map<std::string, std::shared_ptr<Commit>> commits; ///< Map of commit hashes to commits.

    /**
     * @brief Generates a hash for a commit message.
     * @param message The commit message.
     * @return The generated hash.
     */
    std::string generate_hash(const std::string& message);

    /**
     * @brief Moves branch files to a new location.
     * @param branch_name The name of the branch.
     */
    void move_branch_files(const std::string &branch_name);

public:
    /// Default constructor.
    PersistenceStack();

    /**
     * @brief Constructor that initializes the persistence stack with a current branch.
     * @param currentBranch The current branch.
     */
    PersistenceStack(std::string currentBranch);

    /**
     * @brief Retrieves the current branch.
     * @return The current branch.
     */
    std::string getCurrentBranch() const;

    /**
     * @brief Migrates to a new branch.
     * @param branch_name The name of the branch to migrate to.
     */
    void migrateBranch(const std::string& branch_name);

    /**
     * @brief Creates a new commit with a message.
     * @param message The commit message.
     */
    void commit(const std::string& message, const bool& verbose = false, const bool& empty = false);

    /**
     * @brief Creates a new commit from an existing Commit object.sign
     * @param commit The Commit object.
     */
    void commit(const Commit &commit, const bool& verbose = false);

    /**
     * @brief Stages files for the next commit.
     * @param files The files to stage.
     */
    void stage(const std::string& files, const bool& verbose = false);

    /**
     * @brief Stages files for the next commit.
     * @param files The files to stage.
     */
     void stageCommit(std::shared_ptr<Commit> commit, const bool& verbose = false);

    /**
     * @brief Stages file for the next commit.
     * @param filename The file to stage.
     * @param changes File changes to stage.
     * @return Files staged count (1 by default)
     */
     uint32_t stageFile(const std::string& filename,  const FileDiff changes, const bool& isRegexp = false);

    /**
     * @brief Reverts to the previous commit.
     */
    void revert_previous();

    /**
     * @brief Reverts to a specific commit by hash.
     * @param hash The hash of the commit to revert to.
     */
    void revert_to(const std::string &hash);

    void reset_to(const std::string& hash);

    /**
     * @brief Initializes a new branch.
     * @param branch_name The name of the branch.
     */
    void init_branch(const std::string& branch_name);

    /**
     * @brief Initializes a commit from a given hash.
     * @param commit_hash The hash of the commit.
     */
    void init_commit(const std::filesystem::path& commit_hash);

    /**
     * @brief Creates a new branch.
     * @param branch_name The name of the branch.
     */
    void create_branch(const std::string& branch_name);

    /**
     * @brief Checks out a branch.
     * @param branch_name The name of the branch.
     */
    void checkout_branch(const std::string& branch_name);

    /**
     * @brief Merges a branch into the current branch.
     * @param branch_name The name of the branch to merge.
     */
    void merge(const std::string& branch_name, const bool& verbose = false);

        /**
     * @brief Shows current branch status.
     */
     void status();

    /**
     * @brief Lists all branches.
     */
    void list_branches() const;

    /**
     * @brief Lists all commits.
     */
    void list_commits() const;

    /**
     * @brief Checks if the persistence stack is valid.
     * @return true if valid, otherwise false.
     */
    bool isValid() const;

    /**
     * @brief Saves metadata to the stash.
     */
    void stashMeta() const;

    std::shared_ptr<Commit> predefineCommit(const std::string hash);

    /**
     * @brief Logs the current state of the persistence stack.
     */
    void log() const;
};

/**
 * @struct RepoSettings
 * @brief Represents settings for a repository.
 */
struct RepoSettings {
    std::string str_repoName; ///< The name of the repository.
    std::string str_startBranchName; ///< The name of the starting branch.
    bool userSignedCommits;

    /**
     * @brief Converts the settings to a map of key-value pairs.
     * @return A map of settings.
     */
    std::map<std::string, std::string> map_settings() const;
};

/**
 * @class Repo
 * @brief Represents a repository in the version control system.
 */
class Repo {
private:
    std::string repoName; ///< The name of the repository.
    PersistenceStack branchStack; ///< The persistence stack for branches.
    bool signCommits;

    static Repo *stashRepository; ///< Pointer to the singleton repository instance.
    inline const static std::filesystem::path branchesPath = Stash::getStashPath() / "branches"; ///< Path to the branches folder.

    /// Private constructor.
    Repo();

    /**
     * @brief Constructor that initializes the repository with settings.
     * @param settings The repository settings.
     */
    explicit Repo(const RepoSettings& settings);

public:
    /**
     * @brief Initializes the repository with settings.
     * @param settings The repository settings.
     */
    void initRepository(const RepoSettings& settings);

    /**
     * @brief Saves metadata to the stash.
     * @param settings The repository settings.
     */
    void stashMeta(const RepoSettings& settings);

    /**
     * @brief Saves metadata to the stash.
     */
    std::map<std::string, std::string> stashMeta() const;

    /**
     * @brief Retrieves the persistence stack for the repository.
     * @return Reference to the persistence stack.
     */
    PersistenceStack& getRepoStack();

    bool isSignRequired() const;

    /**
     * @brief Retrieves the singleton instance of the repository.
     * @return Reference to the repository instance.
     */
    static Repo &getInstance();

    /**
     * @brief Retrieves the path to the branches folder.
     * @return The path to the branches folder.
     */
    static std::filesystem::path getBranchesPath();

    /**
     * @brief Checks if the repository is empty.
     * @return true if the repository is empty, otherwise false.
     */
    static bool IsEmpty();
};

#endif // REPO_HPP