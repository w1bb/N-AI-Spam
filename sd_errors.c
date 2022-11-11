// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_errors.h"

// This function tests if an error is critical or not.
// - - - - -
// error_t e = A given error that should be checked.
// - - - - -
// Return: bool = true if the error was critical.
bool is_critical(error_t e)
{
    // All critical errors are negative! It's as if this was done by choice...
	return (e < NO_ERROR);
}

// This function provides a safe way to output information to a log file.
// - - - - -
// log_t type = The message will be prefixed with a string based on this option
// The next arguments are similar to a printfs'
void sd_log(log_t type, const char *format, ...)
{
	static int log_counter;
	static char log_path[32];
	static bool first_time = true;
	FILE *flog;
	if (first_time) {
		sprintf(log_path, "log%d.txt", log_counter);
		flog = fopen(log_path, "wt+");
	} else {
		flog = fopen(log_path, "at+");
	}
	if (!flog) { // This is a huge problem!
		fprintf(stderr, "[FATAL ERROR] - Cannot write to log file\n");
		exit(EXIT_FAILURE);
	}
	if (first_time) {
		fprintf(flog, "- - - Log %d started - - -\n", log_counter);
		first_time = false;
	}
	if (type == LOG_CLOSE_SIGNAL) {
		fprintf(flog, "\n\n- - - Log %d ended - - -\n", log_counter);
		first_time = true;
		++log_counter;
	} else {
		va_list argptr;
		va_start(argptr, format);
		if (type == LOG_DEBUG) {
#ifdef SD_LOG_DEBUG
			fprintf(flog, "\n[d]: ");
			vfprintf(flog, format, argptr);
#endif
		} else if (type == LOG_INFO) {
			fprintf(flog, "\n[i]: ");
			vfprintf(flog, format, argptr);
		} else if (type == LOG_WARN) {
			fprintf(flog, "\n[warn]: ");
			vfprintf(flog, format, argptr);
		} else if (type == LOG_ERROR) {
#ifdef SD_LOG_FULL
			fprintf(stderr, "\n[ERROR]: ");
			vfprintf(stderr, format, argptr);
#endif
			fprintf(flog, "\n[ERROR]: ");
			vfprintf(flog, format, argptr);
		} else if (type == LOG_GENERAL) {
			vfprintf(flog, format, argptr);
		}
		va_end(argptr);
	}
	fclose(flog);
}

// This function is a faster way to output a default log message for an error.
// - - - - -
// error_t e = The error based on which a message shall be written.
void sd_log_error(error_t e)
{
	if (e == CRITICAL_OUTPUT_FILE)
		sd_log(LOG_ERROR, "Could not open one of the output files!");
	else if (e == CRITICAL_INPUT_SP)
		sd_log(LOG_ERROR, "Could not open input file 'spammers'!");
	else if (e == CRITICAL_INPUT_KW)
		sd_log(LOG_ERROR, "Could not open 'keywords' / one 'in_' file!");
	else if (e == CRITICAL_INPUT_DIR)
		sd_log(LOG_ERROR, "Could not open the input directory file!");
	else if (e == CRITICAL_INPUT_FILE)
		sd_log(LOG_ERROR, "Could not open the one of the emails!");
	else if (e == CRITICAL_SAFE_REALLOC)
		sd_log(LOG_ERROR, "safe_realloc() failed!");
	else if (e == CRITICAL_SAFE_MALLOC)
		sd_log(LOG_ERROR, "safe_malloc() failed!");
	else if (e == CRITICAL_SAFE_CALLOC)
		sd_log(LOG_ERROR, "safe_calloc() failed!");
	else if (e == CRITICAL_UNKNOWN)
		sd_log(LOG_ERROR, "Unknown critical error occured!");
	else if (e == NO_ERROR)
		sd_log(LOG_DEBUG, "No error occured.");
	else if (e == WARNING_UNKNOWN)
		sd_log(LOG_WARN, "Unknown warning occured.");
	else if (e < 0)
		sd_log(LOG_ERROR, "Even sd_log_error() failed! Error code: %d", e);
	else
		sd_log(LOG_WARN, "Even sd_log_error() failed! Warning code: %d", e);
}

// This function sends the closing signal to the log.
void sd_log_close(void)
{
	sd_log(LOG_CLOSE_SIGNAL, "");
}
