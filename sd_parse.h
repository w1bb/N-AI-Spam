// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_PARSE_H
#define SD_PARSE_H

// This header specifies the required functions that will parse an email in a
// given way.

// General header file
#include "spam_detector.h"

// This function counts:
//   > uppercase_letters
//   > uppercase_letters_h
//   > exclamation_marks
//   > exclamation_marks_h
//   > question_marks_h
//   > asterisks
//   > dollar_signs
//   > dollar_signs_h
//   > word_count
//   > characters
//   > punctuation_chars
// ... while also lowering the uppercase letters of a given email.
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_parse_count_and_lower_email(email_t *email);

// This function removes the excess spaces. It also makes the whole email just a
// long sentence (a single line). Words are recounted.
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_parse_spaces(email_t *email);

// This function removes all spaces and punctuation. It isn't currently used.
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_keep_nonspaces(email_t *email);

// This function counts are removes all numbers in a given email.
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_parse_numbers(email_t *email);

// This function clears a known email found using sd_parse_links(). It shouldn't
// really be used anywhere else, but it is present in .h anyway.
// - - - - -
// email_t *email = The email that will be parsed.
// int          i = The current position in email->content.
// int        len = The length of the prefix that has to be removed.
extern void sd_parse_clear_links(email_t *email, int i, int len);

// This function counts all links and email addresses and removes them.
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_parse_links(email_t *email);

// This function tries to find hidden letters in an email's content
// - - - - -
// email_t *email = The email that will be parsed.
extern void sd_parse_hidden_letters_email(email_t *email);

#endif // SD_PARSE_H
