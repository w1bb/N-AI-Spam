// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_TASKS_H
#define SD_TASKS_H

// This library contains the implementations for task 1 and 2.

// General header file
#include "spam_detector.h"

// This function solves all the tasks.
// - - - - -
// bool is_private = Usually false, sometimes true for testing purposes.
// - - - - -
// Return: error_t = The return value of sd_task_1() and/or sd_task_2().
extern error_t sd_tasks(bool is_private);

// This function solves the first task.
// - - - - -
// int           n = The number of emails.
// email_t *emails = The emails found in a given directory
// - - - - -
// Return: error_t = In case something wrong happens when allocating memory,
//                   when an output file is opened etc.
extern error_t sd_task_1(int n, email_t *emails);

// This function logs some information about the i-th email. In case i is
// negative, the function will log the progress it found and then will reset.
// - - - - -
// hash_t    *hashes = The hashes of the emails.
// double    *scores = The score for each email.
// int             i = What email should be logged (or <0 if progress).
// int    *true_spam = This is NULL in production; when testing, it represents
//                     the true nature of emails.
extern void sd_task_2_log_entry(hash_t *hashes,
								double *scores, int i,
								int *true_spam);

// This function solves the second task.
// - - - - -
// int              n = The number of emails.
// email_t    *emails = The emails found in a given directory.
// bool    is_private = Usually false, sometimes true for testing purposes.
// - - - - -
// Return: error_t = In case something wrong happens when allocating memory,
//                   when an output file is opened etc.
extern error_t sd_task_2(int n, email_t *emails, bool is_private);

#endif // SD_TASKS_H
