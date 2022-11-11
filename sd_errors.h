// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_ERRORS_H
#define SD_ERRORS_H

// This header contains lots of info about possible errrors, as well as a great
// logging function, custom-made for this project!

// Include detailed debugging information. This is optional.
#define SD_LOG_DEBUG

// If in a private environment, always debug.
#ifdef SD_LOG_FULL
#define SD_LOG_DEBUG
#endif

// These are the different types of errors that can be returned in case
// something very wrong happens
typedef enum {
    // Critical errors are negative :(
	CRITICAL_OUTPUT_FILE = -9,
	CRITICAL_INPUT_SP = -8,
	CRITICAL_INPUT_KW = -7,
	CRITICAL_INPUT_DIR = -6,
	CRITICAL_INPUT_FILE = -5,
	CRITICAL_SAFE_REALLOC = -4,
	CRITICAL_SAFE_CALLOC = -3,
	CRITICAL_SAFE_MALLOC = -2,
	CRITICAL_UNKNOWN = -1,
    // No error, no problem! We got you covered ;)
	NO_ERROR = 0,
    // Warnings, also known as "who cares" errors
	WARNING_KW_COLLECTION_EMPTY = 1,
	WARNING_UNKNOWN = 2
} error_t;

// There are quite a few ways to write a log entry:
typedef enum {
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_GENERAL,
	LOG_CLOSE_SIGNAL
} log_t;

// General header file
#include "spam_detector.h"

// This function tests if an error is critical or not.
// - - - - -
// error_t e = A given error that should be checked.
// - - - - -
// Return: bool = true if the error was critical.
extern bool is_critical(error_t e);

// This function provides a safe way to output information to a log file.
// - - - - -
// log_t type = The message will be prefixed with a string based on this option
// The next arguments are similar to a printfs'
extern void sd_log(log_t type, const char *format, ...);

// This function is a faster way to output a default log message for an error.
// - - - - -
// error_t e = The error based on which a message shall be written.
extern void sd_log_error(error_t e);

// This function sends the closing signal to the log.
extern void sd_log_close(void);

#endif // SD_ERRORS_H
