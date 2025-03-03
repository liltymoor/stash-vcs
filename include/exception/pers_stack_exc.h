#ifndef ITMO_VCS_PERS_STACK_EXC_H
#define ITMO_VCS_PERS_STACK_EXC_H

#include <stdexcept>
#include <string>

/**
 * @class BranchAlreadyExistsException
 * @brief Exception thrown when a branch with the same name already exists.
 */
class BranchAlreadyExistsException : public std::invalid_argument {
public:
    /**
     * @brief Constructor for BranchAlreadyExistsException.
     * @param branch_name The name of the branch that already exists.
     */
    explicit BranchAlreadyExistsException(const std::string &branch_name)
            : std::invalid_argument("Branch with name " + branch_name + " already exists") {}
};

/**
 * @class BranchNotFoundException
 * @brief Exception thrown when a branch is not found.
 */
class BranchNotFoundException : public std::invalid_argument {
public:
    /**
     * @brief Constructor for BranchNotFoundException.
     * @param branch_name The name of the branch that was not found.
     */
    explicit BranchNotFoundException(const std::string &branch_name)
            : std::invalid_argument("Branch with name " + branch_name + " not found") {}
};

/**
 * @class CommitNotFoundException
 * @brief Exception thrown when a commit is not found.
 */
class CommitNotFoundException : public std::out_of_range {
public:
    /**
     * @brief Constructor for CommitNotFoundException.
     * @param hash The hash of the commit that was not found.
     */
    explicit CommitNotFoundException(const std::string &hash)
            : std::out_of_range("Commit with hash " + hash + " not found") {}
};

/**
 * @class NoCommitsLeftException
 * @brief Exception thrown when there are no commits left to revert to.
 */
class NoCommitsLeftException : public std::out_of_range {
public:
    /**
     * @brief Constructor for NoCommitsLeftException.
     */
    NoCommitsLeftException() : std::out_of_range("No commits back left") {}
};

/**
 * @class NoCommitsLeftException
 * @brief Exception thrown when there are no commits left to revert to.
 */
class UserNotConfigured : public std::runtime_error {
public:
    /**
     * @brief Constructor for NoCommitsLeftException.
     */
    UserNotConfigured() : std::runtime_error("Your repository requires to sign your commits, but your user not configured. Please configure your user with 'stash config -u username -e email'.") {}
};

#endif // ITMO_VCS_PERS_STACK_EXC_H