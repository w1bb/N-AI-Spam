// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_kmp.h"

// This function computes the lps array for a given pattern string.
// Note that lps means longest proper prefix which is also suffix.
// - - - - -
// char  *pattern = The string that needs attention.
// int  pattern_n = The size of said string.
// int       *lps = The array that will return the lps of pattern.
void kmp_compute_lps(char *pattern, int pattern_n,
					 int *lps)
{
	int prev_len = 0, i = 1;
	lps[0] = 0;
	while (i < pattern_n) {
		if (pattern[i] == pattern[prev_len]) {
			++prev_len;
			lps[i] = prev_len;
			++i;
		} else {
			if (prev_len) {
				prev_len = lps[prev_len - 1];
			} else {
				lps[i] = 0;
				++i;
			}
		}
	}
}

// This function counts the number of occurences of a pattern in another string.
// - - - - -
// char     *text = The text that should be searched.
// int     text_n = The size of said text.
// char  *pattern = The pattern that should be counted.
// int  pattern_n = The size of said pattern.
// int       *lps = The lps array associated with pattern.
// - - - - -
// Return: int = The number of occurences of pattern in text.
int kmp_search_utility(char *text, int text_n,
					   char *pattern, int pattern_n,
					   int *lps)
{
	int i = 0, j = 0, result = 0;
	while (i < text_n) {
		if (text[i] == pattern[j]) {
			++i;
			++j;
		}
		if (j == pattern_n) {
			j = lps[j - 1];
			++result;
		} else if (i < text_n && text[i] != pattern[j]) {
			if (j)
				j = lps[j - 1];
			else
				++i;
		}
	}
	return result;
}
