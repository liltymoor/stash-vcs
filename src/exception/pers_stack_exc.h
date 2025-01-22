/**
 * @file pers_stack_exc.h
 * @brief Header file for custom exceptions related to branch and commit operations.
 */

#ifndef ITMO_VCS_PERS_STACK_EXC_H
#define ITMO_VCS_PERS_STACK_EXC_H

#include <stdexcept>
#include <string>

/**
 * @class BranchAlreadyExistsException
 * @brief Exception thrown when attempting to create a branch that already exists.
 */
class BranchAlreadyExistsException : public std::invalid_argument {
public:
    /**
     * @brief Constructs a new BranchAlreadyExistsException object.
     * @param branch_name The name of the branch that already exists.
     */
    explicit BranchAlreadyExistsException(const std::string& branch_name)
            : std::invalid_argument("Branch with name " + branch_name + " already exists") {}
};

/**
 * @class BranchNotFoundException
 * @brief Exception thrown when a branch is not found.
 */
class BranchNotFoundException : public std::invalid_argument {
public:
    /**
     * @brief Constructs a new BranchNotFoundException object.
     * @param branch_name The name of the branch that was not found.
     */
    explicit BranchNotFoundException(const std::string& branch_name)
            : std::invalid_argument("Branch with name " + branch_name + " not found") {}
};

/**
 * @class CommitNotFoundException
 * @brief Exception thrown when a commit is not found.
 */
class CommitNotFoundException : public std::out_of_range {
public:
    /**
     * @brief Constructs a new CommitNotFoundException object.
     * @param hash The hash of the commit that was not found.
     */
    explicit CommitNotFoundException(const std::string& hash)
            : std::out_of_range("Commit with hash " + hash + " not found") {}
};

/**
 * @class NoCommitsLeftException
 * @brief Exception thrown when there are no more commits to traverse.
 */
class NoCommitsLeftException : public std::out_of_range {
public:
    /**
     * @brief Constructs a new NoCommitsLeftException object.
     */
    NoCommitsLeftException() : std::out_of_range("No commits back left") {}
};

#endif // ITMO_VCS_PERS_STACK_EXC_H