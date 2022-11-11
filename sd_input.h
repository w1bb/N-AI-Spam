// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_INPUT_H
#define SD_INPUT_H

// This libray handles the input requirements for the emails.

// General header file
#include "spam_detector.h"

// This function counts the number of files in a given folder.
// - - - - -
// char *path = The path to said folder.
// - - - - -
// Return: int = The number of files in the given folder. If 0, this directory
//               is considered a critical problem!
extern int count_files(char *path);

// This function inserts new information inside the content of an email,
// accomodating the new space requirements of said info.
// - - - - -
// email_t         *email = The email that receives an update.
// char           *buffer = The string that will be injected.
// int     *content_total = The true size of email->content.
// - - - - -
// Return: error_t = Could be CRITICAL_SAFE_REALLOC.
extern error_t inject_in_email(email_t *email,
							   char *buffer,
							   int *content_total);

// This function reads an email, given a filename and its category.
// - - - - -
// char[]     filename = The path where the email is stored.
// email_t      *email = The content of an email that will be read.
// bool    is_personal = In all given tests it is false, but this flag is set to
//                       true in some personal tests.
// - - - - -
// Return: error_t = The function can detect any failure inside of it.
extern error_t read_email(char filename[BUFFER_SIZE],
						  email_t *email,
						  bool is_personal);

// This function reads all the emails in a given folder.
// - - - - -
// char *emails_path = The folder that contains all the emails.
// bool  is_personal = In all given tests it is false, but this flag is set to
//                       true in some personal tests.
// int            *n = The number of emails.
// email_t  **emails = Pointer to the array of emails.
// - - - - -
// Return: error_t = In case something unconventional happens, better be
//                   prepared!
extern error_t read_all_emails(char *emails_path, bool is_personal,
							   int *n, email_t **emails);

#endif // SD_INPUT_H
