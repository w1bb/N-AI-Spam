// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_SCORE_H
#define SD_SCORE_H

// This header contains many different ways to score a given email as spam or
// ham.

// General header file
#include "spam_detector.h"

// After this value, an email is a little suspicious
#define SPAM_LOW_SUSPICION 35.0

// After this value, an email is really suspicious
#define SPAM_HIGH_SUSPICION 42.0

// After this value, an email is considered spam.
#define SPAM_THRESHOLD 50.0

// This function checks if a given email is a response to another.
// - - - - -
// email_t *email = The email to be checked.
// - - - - -
// Return: bool = true, if the email was a response.
extern bool is_response(email_t *email);

// If enough "> " substrings are found, the email could be indeed ham. This is
// a metric similar to the response one.
// - - - - -
// email_t *email = The email to be checked.
// - - - - -
// Return: bool = true, if the email might have been a forward.
extern bool is_great(email_t *email);

// This function scores an email based on the ammount of uppercase letters
// present in its original content.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
extern void score_uppercase(double *score, email_t *email);

// This function scores an email based on its header.
// Note: This function is currently DISABLED!
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
extern void score_header(double *score, email_t *email);

// This function scores an email based on the ammount of consecutive consonants.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
extern void score_consonants(double *score, email_t *email);

// This function scores an email based on its punctuation.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
extern void score_punctuation(double *score, email_t *email);

// This function scores an email based on the 'data/spammers' file.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
// - - - - -
// Return: error_t = Could be CRITICAL_INPUT_SP in something wrong happened
//                   when trying to input from data/spammers.
extern error_t score_known_spammers(double *score, email_t *email);

// This functions scores the first 100 characters of the email. If these are
// "spamy", the whole email has a way higher chance of also being spam.
// - - - - -
// double            *score = The end result.
// email_t           *email = The email to be scored.
// kw_collection_t *kw_a_kw = The spam words collection.
// double          *k_score = The original score that will be updated.
extern void score_first_100(double *score, email_t *email,
							kw_collection_t *kw_a_kw, double *k_score);

// This function scores how many connections can be found in a given email.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
extern void score_connectivity(double *score, email_t *email,
							   kw_collection_t *kw_connectivity);

// This function scores an email in general.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
// - - - - -
// Return: error_t = In case something wrong happens.
extern error_t score_all(double *score, email_t *email,
						 kw_collection_t *kw_a_kw,
						 kw_collection_t *kw_connectivity,
						 kw_collection_t *kw_news,
						 kw_collection_t *kw_positive_kw);

// This function takes into consideration duplicate spam emails.
// - - - - -
// int          n = The number of emails.
// hash_t *hashed = The hashes for said emails.
// double  *score = The scores that will be updated.
extern void post_score(int n, hash_t *hashes, double *scores);

#endif // SD_SCORE_H
