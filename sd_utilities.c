// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_utilities.h"

// This function converts all the uppercase characters in a string to lowercase
// letters and leaves everything else untouched.
// - - - - -
// char *s = The string to be updated
void strlow(char *s)
{
	for (int i = 0; s[i]; ++i)
		s[i] = tolower(s[i]);
}

// This function checks if a given string is a number.
// - - - - -
// char       *s = The string that should be checked
// int  max_size = The maximum length upon the string will be checked. If < 0,
//                 the string will be checked until '\0' is reached.
// - - - - -
// Return: bool = true only if the string was a number
bool isnint(char *s, int max_size)
{
	if (max_size > 0) {
		int i = 0;
		if (s[0] == '-' && max_size >= 2 && s[1])
			++i;
		for (; s[i] && i < max_size; ++i) {
			if (!isdigit(s[i]))
				return false;
		}
	} else if (max_size < 0) {
		int i = 0;
		if (s[0] == '-' && s[1])
			++i;
		for (; s[i]; ++i) {
			if (!isdigit(s[i]))
				return false;
		}
	}
	return true;
}

// This function checks if a character is a vowel
// - - - - -
// char c = The character that should be checked
// - - - - -
// Return: bool = true only if said character is a vowel
bool isvow(char c)
{
	if (c >= 'a')
		c -= 32; // to uppercase
	return (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}
