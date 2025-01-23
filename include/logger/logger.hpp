#ifndef LOGGER_HPP
#define LOGGER_HPP

/**
 * @def WARN(msg)
 * @brief Logs a warning message.
 * @param msg The warning message to log.
 */
#define WARN(msg) std::cout << "[WARNING] " << msg << std::endl;

/**
 * @def INFO(msg)
 * @brief Logs an informational message.
 * @param msg The informational message to log.
 */
#define INFO(msg) std::cout << "[STASH] " << msg << std::endl;

/**
 * @def ERROR(msg)
 * @brief Logs an error message.
 * @param msg The error message to log.
 */
#define ERROR(msg) std::cout << "[ERROR] " << msg << std::endl;

#endif // LOGGER_HPP