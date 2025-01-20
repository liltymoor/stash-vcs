//
// Created by Timmie on 20.01.2025.
//

#include "repository.hpp"

PersistenceStack::PersistenceStack(const std::string &startBranchName) : head(nullptr)
{
    create_branch(startBranchName);
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
        // Invalid name
        return;
    }

    if (branches.find(branch_name) != branches.end())
    {
        // Branch already exists
        return;
    }

    branches[branch_name] = head;
}

void PersistenceStack::revert_previous()
{
    if (head->prev == nullptr)
    {
        // No commits back left
        return;
    }

    head = head->prev;
}

// TODO To think of...
// void PersistenceStack::revert_to(const std::string &hash)
// {
//
// }

void PersistenceStack::checkout_branch(const std::string &branch_name)
{
    if (branches.find(branch_name) == branches.end())
    {
        // Branch not found
        return;
    }

    head = branches[branch_name];
}

void PersistenceStack::merge(const std::string &branch_name)
{
    // Can be merged only if branches have at least 1 shared commit.
    // Maybe not...

    if (branches.find(branch_name) == branches.end())
    {
        // Branch not found
        return;
    }

    auto targetBranch = branches[branch_name];
    auto currentBranch = head;

    bool b_sharedCommitExists = false;

    if (b_sharedCommitExists)
    {
        // Merge
    }
    else
    {
        // Branches are different
        return;
    }
}










