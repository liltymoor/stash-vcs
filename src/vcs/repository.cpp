//
// Created by Timmie on 20.01.2025.
//

#include "repository.hpp"
#include "../exception/pers_stack_exc.h"

PersistenceStack::PersistenceStack()
{
    // TODO Init branches and repo from .stash sources
}
bool PersistenceStack::isValid() const
{
    return branches.empty();
}

PersistenceStack::PersistenceStack(const std::string &startBranchName) : head(nullptr)
{
    create_branch(startBranchName);
}

void PersistenceStack::commit(const std::string &message)
{
    if (message.empty())
        throw std::invalid_argument("Message can not be empty");

    const auto newCommit = std::make_shared<Commit>(message, generate_hash(message), head);
    head = newCommit;
    branches[currentBranch] = head;
    // TODO Commit being produced not to the actual branch but in commit-stack.
}

void PersistenceStack::stage(const std::string &files)
{
    if (files.empty())
        throw std::invalid_argument("Files can not be empty");

    File::move_files(files, Repo::getBranchesPath() / STAGE_FOLDER_NAME, File::isRegexp(files));
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
    create_directory(Repo::getBranchesPath() / branch_name);
    create_directory(Repo::getBranchesPath() / "staged");
    currentBranch = branch_name;
}

void PersistenceStack::revert_previous()
{
    if (head->prev == nullptr)
    {
        throw NoCommitsLeftException();
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
        throw BranchNotFoundException(branch_name);
    }

    head = branches[branch_name];
}

void PersistenceStack::merge(const std::string &branch_name)
{
    // Can be merged only if branches have at least 1 shared commit.
    // Maybe not...

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
        // Merge
    }
    else
    {
        throw std::runtime_error("Branches have no common commit and cannot be merged");
    }
}

std::string PersistenceStack::generate_hash(const std::string &branch_name)
{
    auto timestamp = time(0);
    return std::to_string(std::hash<std::string>{}(branch_name + std::to_string(timestamp)));
}

Repo::Repo() :
repoName("~none"),
branchStack(PersistenceStack())
{

}

Repo::Repo(const RepoSettings &settings) :
repoName(settings.str_repoName),
branchStack(PersistenceStack(settings.str_startBranchName))
{

}

void Repo::initRepository(const RepoSettings &settings)
{
    // TODO Validate
    this->repoName = settings.str_repoName;
    this->branchStack = PersistenceStack(settings.str_startBranchName);
}

PersistenceStack & Repo::getRepoStack()
{
    return this->branchStack;
}

Repo &Repo::getInstance()
{
    static Repo stashRepository = Repo();
    return stashRepository;
}

std::filesystem::path Repo::getBranchesPath()
{
    return branchesPath;
}

bool Repo::IsEmpty()
{
    Repo stashRepository = Repo::getInstance();
    return stashRepository.branchStack.isValid();
}










