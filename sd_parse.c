// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_parse.h"

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
void sd_parse_count_and_lower_email(email_t *email)
{
	email->uppercase_letters = 0;
	email->uppercase_letters_h = 0;
	email->exclamation_marks = 0;
	email->exclamation_marks_h = 0;
	email->question_marks_h = 0;
	email->asterisks = 0;
	email->dollar_signs = 0;
	email->dollar_signs_h = 0;
	email->word_count = 0;
	email->characters = 0;
	email->punctuation_chars = 0;

	int i;

	for (i = 0; i < email->content_n; ++i) {
		if (email->content[i] == '*' &&
			i + 1 < email->content_n && email->content[i + 1] == '*') {
			++email->asterisks;
		} else if (email->content[i] == '!') {
			++email->exclamation_marks;
		} else if (email->content[i] == '$' && i + 1 < email->content_n &&
				   email->content[i + 1] >= '0' &&
				   email->content[i + 1] <= '9') {
			++email->dollar_signs;
		} else if (email->content[i] >= 'A' && email->content[i] <= 'Z') {
			++email->uppercase_letters;
			email->content[i] += 32; // to lowercase
		}
		if (email->content[i] != ' ' && email->content[i] != '\n') {
			++email->nonspace_count;
			if (!isalnum(email->content[i]))
				++email->punctuation_chars;
			if (i && isalnum(email->content[i - 1]))
				email->word_count++;
		}
		++email->characters;
	}
	if (i && isalnum(email->content[i - 1]))
		email->word_count++;

	for (i = 0; i < email->subject_n; ++i) {
		if (email->subject[i] == '*' &&
			i + 1 < email->subject_n && email->subject[i + 1] == '*') {
			++email->asterisks;
		} else if (email->subject[i] == '!') {
			++email->exclamation_marks;
			++email->exclamation_marks_h;
		}  else if (email->subject[i] == '?') {
			++email->question_marks_h;
		} else if (email->subject[i] == '$' && i + 1 < email->subject_n &&
				   email->subject[i + 1] >= '0' &&
				   email->subject[i + 1] <= '9') {
			++email->dollar_signs;
			++email->dollar_signs_h;
		} else if (email->subject[i] >= 'A' && email->subject[i] <= 'Z') {
			++email->uppercase_letters;
			++email->uppercase_letters_h;
			email->subject[i] += 32; // to lowercase
		}
		++email->characters;
	}
}

// This function removes the excess spaces. It also makes the whole email just a
// long sentence (a single line). Words are recounted.
// - - - - -
// email_t *email = The email that will be parsed.
void sd_parse_spaces(email_t *email)
{
	email->word_count = 0;
	int j = 0, i;
	bool was_an = false;
	for (i = 0; i < email->content_n; ++i) {
		if (isalnum(email->content[i])) {
			email->content[j] = email->content[i];
			++j;
			was_an = true;
		} else {
			if (was_an) {
				++email->word_count;
				email->content[j] = ' ';
				++j;
				was_an = false;
			}
		}
	}
	email->word_count += was_an;
	email->content[j] = '\0';
	email->content_n = j;

	for (was_an = false, j = 0, i = 0; i < email->subject_n; ++i) {
		if (isalnum(email->subject[i])) {
			email->subject[j] = email->subject[i];
			++j;
			was_an = true;
		} else {
			if (was_an) {
				++email->word_count;
				email->subject[j] = ' ';
				++j;
				was_an = false;
			}
		}
	}
	email->word_count += was_an;
	if (!email->word_count) // defensive programming
		email->word_count = 1;
	email->subject[j] = '\0';
	email->subject_n = j;
}

// This function removes all spaces and punctuation. It isn't currently used.
// - - - - -
// email_t *email = The email that will be parsed.
void sd_keep_nonspaces(email_t *email)
{
	int j = 0, i;
	for (i = 0; i < email->content_n; ++i) {
		if (isalnum(email->content[i])) {
			email->content[j] = email->content[i];
			++j;
		}
	}
	email->content[j] = '\0';
	email->content_n = j;
}

// This function counts are removes all numbers in a given email.
// - - - - -
// email_t *email = The email that will be parsed.
void sd_parse_numbers(email_t *email)
{
	email->numbers_long = 0;
	email->numbers_short = 0;
	email->numbers_money = 0;
	static char buffer[BUFFER_SIZE];
	bool money = false;
	int at = 0, digits = 0;
	for (int i = 0; i < email->content_n; ++i) {
		buffer[at] = '\0';
		if (email->content[i] == '$') {
			money = true;
			buffer[at] = email->content[i];
			++at;
		} else if (email->content[i] >= '0' && email->content[i] <= '9') {
			buffer[at] = email->content[i];
			++at;
			++digits;
		} else if (isalnum(email->content[i])) {
			if (!money) {
				if (digits >= 9) {
					if (!strstr(buffer, "2002")) {
						sd_log(LOG_DEBUG, "Found big: %s", buffer);
						email->numbers_long++;
					}
					for (int j = i - at; j < i; ++j)
						email->content[j] = ' ';
				} else if (digits >= 3) {
					email->numbers_short++;
				}
			} else if (digits >= 1) {
				email->numbers_money++;
				if (digits >= 3) {
					for (int j = i - at; j < i; ++j)
						email->content[j] = ' ';
				}
			}
			at = 0;
			money = false;
			digits = 0;
		} else if (digits >= 1 &&
				   ((buffer[at - 1] < '0' || buffer[at - 1] > '9') ||
				   email->content[i] == ':')) {
			if (!money) {
				if (digits >= 9) {
					if (buffer[0] != '2' || buffer[1] != '0' ||
						buffer[2] != '0' || buffer[3] != '2') {
						sd_log(LOG_DEBUG, "Found big: %s", buffer);
						email->numbers_long++;
					}
					for (int j = i - at; j < i; ++j)
						email->content[j] = ' ';
				} else if (digits >= 3) {
					email->numbers_short++;
				}
			} else if (digits >= 1) {
				email->numbers_money++;
				if (digits >= 3) {
					for (int j = i - at; j < i; ++j)
						email->content[j] = ' ';
				}
			}
			at = 0;
			money = false;
			digits = 0;
		} else if (digits >= 1) {
			buffer[at] = email->content[i];
			++at;
		}
	}
	if (digits >= 9)
		email->numbers_long++;
	else if (digits >= 3)
		email->numbers_short++;
	if (digits >= 1 && money)
		email->numbers_money++;

	sd_log(LOG_DEBUG, "| N: Found %d long, %d short and %d money numbers",
		   email->numbers_long, email->numbers_short, email->numbers_money);
}

// This function clears a known email found using sd_parse_links(). It shouldn't
// really be used anywhere else, but it is present in .h anyway.
// - - - - -
// email_t *email = The email that will be parsed.
// int          i = The current position in email->content.
// int        len = The length of the prefix that has to be removed.
void sd_parse_clear_links(email_t *email, int i, int len)
{
	while (len > 0) {
		email->content[i - len] = ' ';
		--len;
	}
	for (; i < email->content_n; ++i) {
		if (!isalnum(email->content[i]) &&
			!strchr("/$-_.+!?&*'(),=", email->content[i])) {
			// stop here
			break;
		}
		email->content[i] = ' ';
	}
}

// I'VE HAD ENOUGH OF CS'S STUPID CONCERNS! YEAH, SOMETIMES A FUNCTION IS LONGER
// THAN 80 LINES OF CODE. THIS IS C, NOT PYTHON, SOMETIMES IT CAN'T JUST BE
// SHORTENED WITHOUT TRANSFORMING 81 LINES OF CODE INTO 881! STOP WITH THIS
// NONSENSE AT ONCE. ALSO, A FEW INDENTATIONS TRIGGER CS FOR NO GOOD REASON!
// ENOUGH! THIS IS WHAT YOU GET:
#define sd_parse_links_stupid_cs \
do { \
	if (email->content[i] == '/') { \
		if (in_link <= 2) \
			++in_link; \
		if (in_link == 3) { \
			if (buffer[at - 1] != '/') { \
				++email->found_links; \
				++at; \
				sd_parse_clear_links(email, i, at); \
			} \
			at = 0; \
			in_link = 0; \
		} \
	} \
} while (0)

// This function counts all links and email addresses and removes them.
// - - - - -
// email_t *email = The email that will be parsed.
void sd_parse_links(email_t *email)
{
	email->found_emails = 0;
	email->found_links = 0;
	static char buffer[BUFFER_SIZE];
	int at = 0, dots = 0, in_link = 0, in_email = 0;
	for (int i = 0; i < email->content_n; ++i) {
		buffer[at] = '\0';
		if (isalnum(email->content[i]) || strchr("_-.", email->content[i])) {
			if (email->content[i] == '.') {
				if (buffer[at - 1] == '.') {
					buffer[0] = '.';
					at = 1;
					in_link = 0;
					in_email = 0;
					dots = 0;
				}
				if (in_email == 2) {
					++email->found_emails;
					sd_parse_clear_links(email, i, at);
					in_email = 0;
					at = 0;
				}
				++dots;
			} else {
				if (in_email == 1)
					in_email++;
			}
			if (in_link == 1 || in_link == 2) { // http:a / http:/a
				at = 0;
				in_link = 0;
			}
			buffer[at] = email->content[i];
			++at;
		} else {
			if (in_email) {
				in_email = 0;
			} else if (in_link) {
				sd_parse_links_stupid_cs;
			} else {
				if (email->content[i] == '@') {
					if (at && !strchr(".@-_", buffer[at - 1])) {
						in_email = 1;
						buffer[at] = email->content[i];
						++at;
					} else {
						at = 0;
						dots = 0;
					}
				} else if (email->content[i] == ':') {
					if (!strcmp(buffer, "http") || !strcmp(buffer, "https")) {
						in_link = 1;
						buffer[at] = email->content[i];
						++at;
					}
				} else if (email->content[i] == ' ' ||
						   email->content[i] == '\n' ||
						   email->content[i] == '/' ||
						   email->content[i] == '\"') {
					if (dots >= 1 && at >= 5 &&
						(!strcmp(buffer + at - 4, ".com") ||
						!strcmp(buffer + at - 4, ".org") ||
						!strcmp(buffer + at - 3, ".ru") ||
						!strcmp(buffer + at - 3, ".ie") ||
						!strcmp(buffer + at - 4, ".net") ||
						!strcmp(buffer + at - 4, "o.uk"))) {
						++email->found_links;
						++at;
						sd_parse_clear_links(email, i, at);
					}
					at = 0;
					dots = 0;
				} else {
					at = 0;
					in_email = 0;
					in_link = 0;
					dots = 0;
				}
			}
		}
	}
}

// YEAH, THAT'S RIGHT YOU DON'T GET TO USE IT - HAHAHA!
// WRITE YOUR OWN USELESS MACROS!
#undef sd_parse_links_stupid_cs

// This function tries to find hidden letters in an email's content
// - - - - -
// email_t *email = The email that will be parsed.
void sd_parse_hidden_letters_email(email_t *email)
{
	int i;
	for (i = 0; i < email->content_n; ++i) {
		if (email->content[i] == '@' || email->content[i] == '4')
			email->content[i] = 'a';
		else if (email->content[i] == '8')
			email->content[i] = 'b';
		else if (email->content[i] == '3')
			email->content[i] = 'e';
		else if (email->content[i] == '1')
			email->content[i] = 'i';
		else if (email->content[i] == '0')
			email->content[i] = 'o';
		else if (email->content[i] == '5' || email->content[i] == '$')
			email->content[i] = 's';
		if (!isalnum(email->content[i]))
			email->content[i] = ' ';
	}

	for (i = 0; i < email->subject_n; ++i) {
		if (email->subject[i] == '@' || email->subject[i] == '4')
			email->subject[i] = 'a';
		else if (email->subject[i] == '8')
			email->subject[i] = 'b';
		else if (email->subject[i] == '3')
			email->subject[i] = 'e';
		else if (email->subject[i] == '1')
			email->subject[i] = 'i';
		else if (email->subject[i] == '0')
			email->subject[i] = 'o';
		else if (email->subject[i] == '5' || email->subject[i] == '$')
			email->subject[i] = 's';
		if (!isalnum(email->subject[i]))
			email->subject[i] = ' ';
	}
}
