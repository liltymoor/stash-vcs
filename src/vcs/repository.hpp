#ifndef REPO_HPP
#define REPO_HPP


#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

struct Commit
{
    std::string message;
    std::string hash;
    std::shared_ptr<Commit> prev;

    Commit(std::string msg, std::string h, std::shared_ptr<Commit> p = nullptr)
        : message(std::move(msg)), hash(std::move(h)), prev(std::move(p))
    {
    }
};


class PersistenceStack
{
private:
    std::shared_ptr<Commit> head;
    std::unordered_map<std::string, std::shared_ptr<Commit>> branches;
    static std::string generate_hash(const std::string &message);

public:
    explicit PersistenceStack(const std::string &startBranchName);
    void commit(const std::string &message);
    void revert_previous();
    void revert_to(const std::string &hash);
    void create_branch(const std::string &branch_name);
    void checkout_branch(const std::string &branch_name);
    void merge(const std::string &branch_name);
    //геттеры
    std::shared_ptr<Commit> getHead() const;
    const std::unordered_map<std::string, std::shared_ptr<Commit>> &getBranches() const;
};


struct RepoSettings
{
    std::string str_repoName;
    std::string str_startBranchName;
};


class Repo
{
private:
    RepoSettings settings;
    PersistenceStack branchStack;

    // Приватный конструктор и копирующие операции
    explicit Repo(const RepoSettings &settings);
    Repo(const Repo &) = delete;
    Repo &operator=(const Repo &) = delete;

    static std::unique_ptr<Repo> instance;
//    friend std::unique_ptr<Repo> std::make_unique<Repo>(const RepoSettings &);

public:
    static Repo &getInstance(const RepoSettings &settings);
    bool fnIsEmpty() const;
    bool branchStackIsEmpty() const;
    void create_branch(const std::string& branch_name);

};

#endif