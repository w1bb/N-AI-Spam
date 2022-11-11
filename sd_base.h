// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_BASE_H
#define SD_BASE_H

// This header provides an unedrstanding for the common mail.

// This is the default size of an email address: 255.
// Further reading: https://stackoverflow.com/questions/386294/
// However, in the implementation, BUFFER_SIZE was used instead, in order to
// accomodate for the name itself.

// These are the characteristics of an email:
// > date = The specified date inside an email.
// > subject = The subject of an email.
// > subject_n = The length of the subject (in characters).
// > from = This email comes from this address.
// > content = The body of an email.
// > content_n = The length of the body (in characters).
// > uppercase_letters = The ammount of uppercase letters.
// > uppercase_letters_h = The ammount of uppercase letters in the subject.
// > exclamation_marks = The ammount of exclamation marks.
// > exclamation_marks_h = The ammount of exclamation marks in the subject.
// > question_marks_h = The ammount of question marks in the subject.
// > asterisks = The maximum ammount of consecutive asterisks.
// > dollar_signs = The ammount of dollar signs.
// > dollar_signs_h = The ammount of dollar signs in the subject.
// > word_count = The number of words in the email.
// > nonspace_count = The ammount of characters that are not spaces.
// > characters = The ammount of characters in total.
// > punctuation_chars = The ammount of punctuation characters.
// > found_emails = The ammount of emails found in the content of an email.
// > found_links = The ammount of links found in the content of an email.
// > numbers_long = The ammount of long numbers in a given email.
// > numbers_short = The ammount of short numbers in a given email.
// > numbers_money = The ammount of numbers that could be $ in a given email.
typedef struct {
	datetime_t date;
	char subject[BUFFER_SIZE];
	char from[BUFFER_SIZE];
	char *content;
	int content_n, subject_n;
	int uppercase_letters;
	int uppercase_letters_h;
	int exclamation_marks;
	int exclamation_marks_h;
	int question_marks_h;
	int asterisks;
	int dollar_signs;
	int dollar_signs_h;
	int word_count, nonspace_count;
	int characters;
	int punctuation_chars;
	int found_emails;
	int found_links;
	int numbers_long;
	int numbers_short;
	int numbers_money;
} email_t;

// General header file
#include "spam_detector.h"

#endif // SD_BASE_H
