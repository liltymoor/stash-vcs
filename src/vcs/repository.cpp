//
// Created by Timmie on 20.01.2025.
//

#include "repository.hpp"

#include "metadata.hpp"
#include "../exception/pers_stack_exc.h"
#include <variant>
#include "../logger/logger.hpp"

PersistenceStack::PersistenceStack()
{
    // TODO Init branches and repo from .stash sources

    if (exists(Stash::getStashPath() / BRANCHES_FOLDER_NAME))
    {
        try {
            for (const auto& entry : std::filesystem::directory_iterator(Stash::getStashPath() / BRANCHES_FOLDER_NAME)) {
                if (entry.is_directory()) {
                    INFO("Branch: " << entry.path().filename());
                    init_branch(entry.path().filename());
                }
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::__throw_bad_variant_access(e.what());
        }
    }

}

std::string PersistenceStack::getCurrentBranch()
{
    return currentBranch;
}

bool PersistenceStack::isValid() const
{
    return branches.empty();
}

std::map<std::string, std::string> RepoSettings::map_settings() const
{
    std::map<std::string, std::string> map;
    map["RepositoryName"] = str_repoName;
    map["StartBranchName"] = str_startBranchName;
    return map;
};
PersistenceStack::PersistenceStack(const std::string &startBranchName) :
head(nullptr)
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
}

void PersistenceStack::stage(const std::string &files)
// TODO feature version should decline this way to stage files because it's very heavy
// Better way is to store meta-data like file_path, update_time and hash (sha1), than the right order to stage will be
// update_time not equal -> file hash not equal -> than we can mark the file for staging
{
    if (files.empty())
        throw std::invalid_argument("Files can not be empty");

    File::move_files(files, Repo::getBranchesPath() / currentBranch / STAGE_FOLDER_NAME, File::isRegexp(files));
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
    create_directories(Repo::getBranchesPath() / branch_name / "staged");
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

void PersistenceStack::init_branch(const std::string &branch_name)
{
    if (branch_name.empty())
    {
        throw std::invalid_argument("Name can not be empty");
    }

    if (branches.find(branch_name) != branches.end())
    {
        throw BranchAlreadyExistsException(branch_name);
    }

    branches[branch_name] = nullptr;
    //TODO init commits state here
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
    currentBranch = branch_name;
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
    if (std::filesystem::exists(Stash::getStashPath() / METADATA_FILE_NAME))
    {
        std::map<std::string, std::string> metadata = MetadataHandler::load(Stash::getStashPath() / METADATA_FILE_NAME);
        // TODO validate metadata
        repoName = metadata[META_REPO_NAME];
        branchStack.checkout_branch(metadata[META_CURRENT_BRANCH]);

    }
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
void Repo::stashMeta()
{
    std::map<std::string, std::string> metadata;
    metadata[META_REPO_NAME] = repoName;
    metadata[META_CURRENT_BRANCH] = branchStack.getCurrentBranch();

    MetadataHandler::save((Stash::getStashPath() / METADATA_FILE_NAME).c_str(), metadata);
}

void Repo::stashMeta(const RepoSettings &settings)
{
    MetadataHandler::save((Stash::getStashPath() / METADATA_FILE_NAME).c_str(), settings.map_settings());
}

PersistenceStack & Repo::getRepoStack()
{
    return this->branchStack;
}

Repo &Repo::getInstance()
{
    if (!Stash::stashExists())
        throw std::runtime_error("Stash does not exist. You should initialize it with command \"stash init\"");
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