// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_UTILITIES_H
#define SD_UTILITIES_H

// This is a collection of functions that didn't really fit anywhere else, but
// they were also important in this code's execution.

// General header file
#include "spam_detector.h"

// This function converts all the uppercase characters in a string to lowercase
// letters and leaves everything else untouched.
// - - - - -
// char *s = The string to be updated
extern void strlow(char *s);

// This function checks if a given string is a number.
// - - - - -
// char       *s = The string that should be checked
// int  max_size = The maximum length upon the string will be checked. If < 0,
//                 the string will be checked until '\0' is reached.
// - - - - -
// Return: bool = true only if the string was a number
extern bool isnint(char *s, int max_size);

// This function checks if a character is a vowel
// - - - - -
// char c = The character that should be checked
// - - - - -
// Return: bool = true only if said character is a vowel
extern bool isvow(char c);

#endif // SD_UTILITIES_H
