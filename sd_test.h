// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_TEST_H
#define SD_TEST_H

// This header is only used to log if a given set of emails is correctly
// labeled. Obviously, THIS IS NOT USED in the final output, only in the log and
// only OUTSIDE production.

// General header file
#include "spam_detector.h"

// The maximum ammount of emails in a set.
#define MAX_EMAILS 8192

// This function sets an array according to the emails' true nature if and only
// if SD_LOG_FULL is defined
// - - - - -
// bool  is_private = Usually false, true only for testing purposes.
// int[]  true_spam = The array that will be returned.
#ifdef SD_LOG_FULL
extern void get_true_spam(bool is_private,
						  int true_spam[MAX_EMAILS]);
#endif

#endif // SD_TEST_H
