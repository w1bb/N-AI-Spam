// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_hash.h"

// This function generates a "unique" hash for a given string.
// - - - - -
// char *s = The string that will be hashed.
// - - - - -
// Return: unsigned long = The hashed value.
// - - - - -
// Further reading: https://cp-algorithms.com/string/string-hashing.html
unsigned long hash_compute(char *s)
{
	const unsigned long p = 31;
	const unsigned long mod = 1e9 + 9;

	unsigned long result = 0;
	unsigned long p_pow = 1;

	for (int i = 0; s[i]; ++i) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			result = (result + (s[i] - 'a' + 1) * p_pow) % mod;
			p_pow = (p_pow * p) % mod;
		}
	}

	return result;
}

// This function checks if two string have the same hash and that their
// corresponding strings are indeed similar. It is used in qsort.
// - - - - -
// const void *h1 = The first hash.
// const void *h2 = The second hash.
// - - - - -
// Return: int = The return value required by qsort.
int hash_compare(const void *h1, const void *h2)
{
	char *s1 = ((hash_t *)h1)->s;
	char *s2 = ((hash_t *)h2)->s;
	unsigned long x1 = ((hash_t *)h1)->hash;
	unsigned long x2 = ((hash_t *)h2)->hash;
	if (x1 != x2)
		return (x1 < x2) ? -1 : 1;
	for (int i = 0, j = 0; ; ++i, ++j) {
		while (s1[i] && (s1[i] < 'a' || s1[i] > 'z'))
			++i;
		while (s2[j] && (s2[j] < 'a' || s2[j] > 'z'))
			++j;
		if (s1[i] != s2[j])
			return (s1[i] < s2[j]) ? -1 : 1;
		if (s1[i] == '\0')
			return 0;
	}
	// Should never be reached
	return -1;
}
