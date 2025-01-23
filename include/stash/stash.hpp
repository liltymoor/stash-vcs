#ifndef STASH_HPP
#define STASH_HPP

#include <filesystem>

/**
 * @class Stash
 * @brief Manages the stash (storage) for commits and repository metadata.
 *
 * The Stash class is responsible for managing the directory structure for commits
 * and providing functionality to create and access commits.
 */
class Stash {
private:
    inline const static std::filesystem::path stash_path = "./.stash"; ///< Path to the stash directory.
    inline static bool isExists = false; ///< Flag indicating whether the stash exists.

    /**
     * @brief Private constructor for the Stash class.
     */
    Stash();

public:
    /**
     * @brief Returns the singleton instance of the Stash class.
     * @return Reference to the Stash instance.
     */
    static Stash& getInstance();

    /**
     * @brief Checks if the stash directory exists.
     * @return true if the stash exists, otherwise false.
     */
    static bool stashExists();

    /**
     * @brief Returns the path to the stash directory.
     * @return The path to the stash directory.
     */
    static const std::filesystem::path& getStashPath();
};

#endif // STASH_HPP