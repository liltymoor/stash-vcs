//
// Created by Ivanov Ilya on 21.01.2025.
//

#ifndef ITMO_VCS_PERS_STACK_EXC_H
#define ITMO_VCS_PERS_STACK_EXC_H
#include <stdexcept>
#include <string>

class BranchAlreadyExistsException : public std::invalid_argument
{
public:
    explicit BranchAlreadyExistsException(const std::string &branch_name)
        : std::invalid_argument("Branch with name " + branch_name + " already exists")
    {
    }
};

class BranchNotFoundException : public std::invalid_argument
{
public:
    explicit BranchNotFoundException(const std::string &branch_name)
        : std::invalid_argument("Branch with name " + branch_name + " not found")
    {
    }
};

class CommitNotFoundException : public std::out_of_range
{
public:
    explicit CommitNotFoundException(const std::string &hash)
        : std::out_of_range("Commit with hash " + hash + " not found")
    {
    }
};

class NoCommitsLeftException : public std::out_of_range
{
public:
    NoCommitsLeftException() : std::out_of_range("No commits back left")
    {
    }
};
#endif //ITMO_VCS_PERS_STACK_EXC_H
