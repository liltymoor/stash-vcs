/**
 * @file stash.hpp
 * @brief Header file for the Stash class.
 */

#ifndef STASH_HPP
#define STASH_HPP

#include <filesystem>
#include <iostream>
#include "../vcs/repository.hpp"

/**
 * @class Stash
 * @brief Manages the .stash directory and repository initialization.
 *
 * This class is responsible for creating and managing the `.stash` directory,
 * which contains all the commits and branches of the repository.
 */
class Stash {
private:
    inline const static std::filesystem::path stash_path = "./.stash"; ///< Path to the .stash directory.

    /**
     * @brief Private constructor to enforce singleton pattern.
     */
    Stash();

public:
    /**
     * @brief Gets the singleton instance of the Stash class.
     * @return Reference to the Stash instance.
     */
    static Stash& getInstance();
};

#endif // STASH_HPP