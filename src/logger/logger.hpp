/**
 * @file logger.hpp
 * @brief Header file for logging macros.
 */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>

/**
 * @def WARN(msg)
 * @brief Logs a warning message to the standard output.
 * @param msg The warning message to log.
 */
#define WARN(msg) std::cout << "[WARNING] " << msg << std::endl;

/**
 * @def INFO(msg)
 * @brief Logs an informational message to the standard output.
 * @param msg The informational message to log.
 */
#define INFO(msg) std::cout << "[STASH] " << msg << std::endl;

/**
 * @def ERROR(msg)
 * @brief Logs an error message to the standard output.
 * @param msg The error message to log.
 */
#define ERROR(msg) std::cout << "[ERROR] " << msg << std::endl;

#endif // LOGGER_HPP