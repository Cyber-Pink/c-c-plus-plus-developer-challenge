#ifndef LOG_H
#define LOG_H

#include <stdio.h>

/**
 * @brief Initialize the logging system, opening the log file.
 * Call this once at the start of the program.
 * @return 0 if success, non-zero if error.
 */
int init_log();

/**
 * @brief Write a message to the log file.
 * This uses printf-style formatting.
 * @param fmt Format string
 * @param ... Arguments for the format string
 */
void log_message(const char *fmt, ...);

#endif // LOG_H
