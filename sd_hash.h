// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_HASH_H
#define SD_HASH_H

// This library provides the hashing capability of the spam detector.

typedef struct {
	unsigned long hash;
	char *s;
	int position;
} hash_t;

// This function generates a "unique" hash for a given string.
// - - - - -
// char *s = The string that will be hashed.
// - - - - -
// Return: unsigned long = The hashed value.
// - - - - -
// Further reading: https://cp-algorithms.com/string/string-hashing.html
extern unsigned long hash_compute(char *s);

// This function checks if two string have the same hash and that their
// corresponding strings are indeed similar. It is used in qsort.
// - - - - -
// const void *h1 = The first hash.
// const void *h2 = The second hash.
// - - - - -
// Return: int = The return value required by qsort.
extern int hash_compare(const void *h1, const void *h2);

#endif // SD_HASH_H
