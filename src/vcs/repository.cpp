//
// Created by Timmie on 20.01.2025.
//

#include "repository.hpp"
#include "../exception/pers_stack_exc.h"
#include <chrono>
#include <random>

PersistenceStack::PersistenceStack(const std::string &startBranchName) : head(nullptr)
{
    create_branch(startBranchName);
    commit("Initial commit"); // Это обновит head для новой ветки
}

void PersistenceStack::commit(const std::string &message)
{
    const auto newCommit = std::make_shared<Commit>(message, generate_hash(message), head);
    head = newCommit;
}

void PersistenceStack::create_branch(const std::string &branch_name)
{
    if (branch_name.empty())
    {
        throw std::invalid_argument("Name can not be empty");
    }

    if (branches.find(branch_name) != branches.end())
    {
        throw BranchAlreadyExistsException(branch_name);
    }

    branches[branch_name] = head;
}

void PersistenceStack::revert_previous()
{
    if (head->prev == nullptr)
    {
        throw NoCommitsLeftException();
    }

    head = head->prev;
}

void PersistenceStack::checkout_branch(const std::string &branch_name)
{
    if (branches.find(branch_name) == branches.end())
    {
        throw BranchNotFoundException(branch_name);
    }

    head = branches[branch_name];
}

void PersistenceStack::merge(const std::string &branch_name)
{
    if (branches.find(branch_name) == branches.end())
    {
        throw BranchNotFoundException(branch_name);
    }

    auto targetBranch = branches[branch_name];
    auto currentBranch = head;

    bool b_sharedCommitExists = false;
    std::shared_ptr<Commit> targetCommit = targetBranch;
    std::shared_ptr<Commit> currentCommit = currentBranch;

    while (targetCommit != nullptr && currentCommit != nullptr)
    {
        if (targetCommit == currentCommit)
        {
            b_sharedCommitExists = true;
            break;
        }
        targetCommit = targetCommit->prev;
        currentCommit = currentCommit->prev;
    }

    if (b_sharedCommitExists)
    {
        head = targetBranch;
        std::cout << "Merged branch: " << branch_name << "\n";
    }
    else
    {
        throw std::runtime_error("Branches have no common commit and cannot be merged");
    }
}

std::string PersistenceStack::generate_hash(const std::string &message)
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::random_device random_device;
    std::mt19937 gen(random_device());
    std::uniform_int_distribution<> dis(0, 1000000);
    std::string const unique_input = message + std::to_string(timestamp) + std::to_string(dis(gen));
    return std::to_string(std::hash<std::string>{}(unique_input));
}

void PersistenceStack::revert_to(const std::string &hash)
{
    std::shared_ptr<Commit> current = head;

    while (current != nullptr)
    {
        if (current->hash == hash)
        {
            head = current;
            std::cout << "Reverted to commit: " << hash << "\n";
            return;
        }
        current = current->prev;
    }

    throw CommitNotFoundException(hash);
}

std::shared_ptr<Commit> PersistenceStack::getHead() const
{
    return head;
}

const std::unordered_map<std::string, std::shared_ptr<Commit>> &PersistenceStack::getBranches() const
{
    return branches;
}

Repo::Repo(const RepoSettings &settings) : settings(settings), branchStack(settings.str_startBranchName)
{
}

std::unique_ptr<Repo> Repo::instance = nullptr;

Repo &Repo::getInstance(const RepoSettings &settings)
{
    if (!instance)
    {
        instance.reset(new Repo(settings));
    }
    return *instance;
}

bool Repo::fnIsEmpty() const
{
    return branchStackIsEmpty();
}

bool Repo::branchStackIsEmpty() const
{
    return branchStack.getBranches().empty();
}
void Repo::create_branch(const std::string &branch_name)
{
    branchStack.create_branch(branch_name);
}