#ifndef REPO_HPP
#define REPO_HPP


#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include "commit_state.hpp"
#include "stash.hpp"

#include <filesystem>

#define META_FILENAME "metadata"
#define META_STAGE_FOLDER "staged"
#define META_BRANCH_FOLDER "branches"
#define META_COMMIT_FILES_FOLDER "files"

#define META_REPO_NAME "RepoName"
#define META_CURRENT_BRANCH "CurrentBranch"
#define META_CURRENT_HEAD "CurrentHead"
#define META_REPO_CORE_BRANCH "RepoStartBranch"

#define META_COMMIT_MSG "CommitMessage"
#define META_COMMIT_PREV "PrevCommit"
#define META_COMMIT_HASH "CommitHash"
#define META_COMMIT_BRANCH "CommitBranch"

struct Commit {
    std::string message;
    std::string hash;
    std::string branch;

    std::shared_ptr<Commit> prev;
    std::shared_ptr<CommitState> state;

    Commit(std::string msg, std::string h, std::string b, std::shared_ptr<Commit> p = nullptr)
            : message(std::move(msg)),
              hash(std::move(h)),
              branch(b),
              prev(std::move(p)),
              state(std::make_shared<CommitState>())
    {
    }

    Commit(std::map<std::string, std::string>& metaData, const std::unordered_map<std::string, File>& commit_files);
};


class PersistenceStack {
private:
    std::shared_ptr<Commit> head = nullptr;
    std::string currentBranch;
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches;
    std::unordered_map<std::string, std::shared_ptr<Commit>> commits; // <hash, commit>

    std::string generate_hash(const std::string& message);
    void move_branch_files(const std::string &branch_name);

public:
    PersistenceStack();
    PersistenceStack(std::string currentBranch);

    std::string getCurrentBranch() const;
    void migrateBranch(const std::string& branch_name);

    void commit(const std::string& message);
    void commit(const Commit &commit);
    void stage(const std::string& files);
    void revert_previous();
    // TODO To think of...
    void revert_to(const std::string &hash);
    void init_branch(const std::string& branch_name);
    void init_commit(const std::filesystem::path& commit_hash);
    void create_branch(const std::string& branch_name);
    void checkout_branch(const std::string& branch_name);
    void merge(const std::string& branch_name);
    void list_branches() const;
    void list_commits() const;

    bool isValid() const;
    void stashMeta() const;
    void log() const;
};

struct RepoSettings {
    std::string str_repoName;
    std::string str_startBranchName;

    std::map<std::string, std::string> map_settings() const;
};


class Repo {
private:
    std::string repoName;
    PersistenceStack branchStack;
    static Repo *stashRepository;
    inline const static std::filesystem::path branchesPath = Stash::getStashPath() / "branches";

    Repo();
    explicit Repo(const RepoSettings& settings);
public:
    void initRepository(const RepoSettings& settings);
    void stashMeta(const RepoSettings& settings);
    void stashMeta() const;
    PersistenceStack& getRepoStack();

    static Repo &getInstance();
    static std::filesystem::path getBranchesPath();
    static bool IsEmpty();
};

#endif