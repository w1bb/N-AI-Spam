// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_KEYWORDS_H
#define SD_KEYWORDS_H

// This library creates the means to store keywords and apply operations on
// strings present in emails.

// This is the maximum number of strings in a keywords collection.
#define MAX_COLLECTION_SIZE 1024

// This is the maximum size of a keyword that can be found in such a collection.
#define MAX_WORD_SIZE 64

// This is the default buffer size for any character array input.
#define BUFFER_SIZE 1024

// A keywords collection
typedef struct {
	char associated_filename[BUFFER_SIZE]; // used in error logs
	int count; // the ammount of elements in the collection
	char keywords[MAX_COLLECTION_SIZE][MAX_WORD_SIZE]; // keywords themselves
	int lps[MAX_COLLECTION_SIZE][MAX_WORD_SIZE]; // lps for said keywords
	int kw_size[MAX_COLLECTION_SIZE]; // the length of each keyword
	double values[MAX_COLLECTION_SIZE]; // the multiplier for each keyword
} kw_collection_t;

// General header file
#include "spam_detector.h"

// This function reads the content of a file and stores the given information in
// a kw_collection_t element.
// - - - - -
// char[]            filename = The name of the file that has to be parsed.
// double          multiplier = If > 0, this will be the value used for every
//                              "values" component instead of reading a new one.
// kw_collection_t        *kw = This is where the resulting collection will be
//                              returned.
// - - - - -
// Return: error_t = The function will report CRITICAL_INPUT_KW in case the
//                   input file could not be opened.
extern error_t kw_collection_read(char *filename,
								  double multiplier,
								  kw_collection_t *kw);

// This function computes the score associated between a collection of keywords
// and an email.
// - - - - -
// kw_collection_t        *kw = The collection of keywords.
// email_t             *email = The email that should be scored.
// double          multiplier = In addition to the values present in kw, the
//                              result is multiplied by multiplier.
// bool                header = True if the email's header should be examined.
// bool               use_log = True if the score should use the log function.
// double             *result = This is where the resulting score will be
//                              returned.
// - - - - -
// Return: error_t = The function will report a warning in case the collection
//                   was empty.
extern error_t kw_collection_apply(kw_collection_t *kw,
								   email_t *email,
								   double multiplier,
								   bool header,
								   bool use_log,
								   double *result);

#endif // SD_KEYWORDS_H
