#ifndef REPO_HPP
#define REPO_HPP


#include <unordered_map>
#include <iostream>
#include <memory>
#include <string>

struct Commit {
    std::string message;
    std::string hash;
    std::shared_ptr<Commit> prev;

    Commit(const std::string& msg, const std::string& h, std::shared_ptr<Commit> p = nullptr)
        : message(msg), hash(h), prev(p) {}
};


class PersistenceStack {
private:
    std::shared_ptr<Commit> head;
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches;

    std::string generate_hash(const std::string& message);

public:
    PersistenceStack(const std::string &startBranchName);

    void commit(const std::string& message);
    void revert_previous();
    // TODO To think of...
    //void revert_to(const std::string &hash);
    void create_branch(const std::string& branch_name);
    void checkout_branch(const std::string& branch_name);
    void merge(const std::string& branch_name);
    void log() const;
};


struct RepoSettings {
    std::string str_repoName;
    std::string str_startBranchName;
};


class Repo {
private:
    RepoSettings settings;
    PersistenceStack branchStack;
public:
    explicit Repo(const RepoSettings& settings);

    bool fnIsEmpty();
};

#endif