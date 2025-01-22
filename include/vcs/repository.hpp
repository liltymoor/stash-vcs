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

#define STAGE_FOLDER_NAME "staged"
#define METADATA_FILE_NAME "metadata"
#define BRANCHES_FOLDER_NAME "branches"

#define META_REPO_NAME "RepoName"
#define META_CURRENT_BRANCH "CurrentBranch"
#define META_REPO_CORE_BRANCH "RepoStartBranch"

struct Commit {
    std::string message;
    std::string hash;
    std::shared_ptr<Commit> prev;
    std::shared_ptr<CommitState> state;

    Commit(std::string msg, std::string h, std::shared_ptr<Commit> p = nullptr)
            : message(std::move(msg)),
              hash(std::move(h)),
              prev(std::move(p)),
              state(std::make_shared<CommitState>())
    {
    }
};


class PersistenceStack {
private:
    std::shared_ptr<Commit> head;
    std::string currentBranch;
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches;

    std::string generate_hash(const std::string& message);

public:
    PersistenceStack(const std::string &startBranchName);
    PersistenceStack();

    std::string getCurrentBranch();

    void commit(const std::string& message);
    void stage(const std::string& files);
    void revert_previous();
    // TODO To think of...
    //void revert_to(const std::string &hash);
    void init_branch(const std::string& branch_name);
    void create_branch(const std::string& branch_name);
    void checkout_branch(const std::string& branch_name);
    void merge(const std::string& branch_name);

    bool isValid() const;
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
    void stashMeta();
    PersistenceStack& getRepoStack();

    static Repo &getInstance();
    static std::filesystem::path getBranchesPath();
    static bool IsEmpty();
};

#endif