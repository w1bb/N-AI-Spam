// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_KMP_H
#define SD_KMP_H

// This header provides an efficient way to compute the number of occurences of
// a string in another.
// Further reading: https://en.wikipedia.org/wiki/Knuth-Morris-Pratt_algorithm
// Note: aho corasick is a total overkill for such a simple task!

// General header file
#include "spam_detector.h"

// This function computes the lps array for a given pattern string.
// Note that lps means longest proper prefix which is also suffix.
// - - - - -
// char  *pattern = The string that needs attention.
// int  pattern_n = The size of said string.
// int       *lps = The array that will return the lps of pattern.
extern void kmp_compute_lps(char *pattern, int pattern_n,
							int *lps);

// This function counts the number of occurences of a pattern in another string.
// - - - - -
// char     *text = The text that should be searched.
// int     text_n = The size of said text.
// char  *pattern = The pattern that should be counted.
// int  pattern_n = The size of said pattern.
// int       *lps = The lps array associated with pattern.
// - - - - -
// Return: int = The number of occurences of pattern in text.
extern int kmp_search_utility(char *text, int text_n,
							  char *pattern, int pattern_n,
							  int *lps);

#endif // SD_KMP_H
