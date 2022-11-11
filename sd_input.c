// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_input.h"

// This function counts the number of files in a given folder.
// - - - - -
// char *path = The path to said folder.
// - - - - -
// Return: int = The number of files in the given folder. If 0, this directory
//               is considered a critical problem!
int count_files(char *path)
{
	int counter = 0;
	DIR *dir;
	struct dirent *entry;
	dir = opendir(path);
	if (!dir)
		return 0; // this will throw a critical error later
	while ((entry = readdir(dir))) {
		if (entry->d_type == DT_REG)
			++counter;
	}
	closedir(dir);
	return counter;
}

// This function inserts new information inside the content of an email,
// accomodating the new space requirements of said info.
// - - - - -
// email_t         *email = The email that receives an update.
// char           *buffer = The string that will be injected.
// int     *content_total = The true size of email->content.
// - - - - -
// Return: error_t = Could be CRITICAL_SAFE_REALLOC.
error_t inject_in_email(email_t *email,
						char *buffer,
						int *content_total)
{
	if (buffer[0] == '\n' || buffer[0] == '\0')
		return NO_ERROR;
	buffer[BUFFER_SIZE - 1] = '\0'; // defensive programming
	int buffer_size = strlen(buffer);

	while (email->content_n + buffer_size >= *content_total) {
		(*content_total) <<= 1; // double the capacity
		void *aux = safe_realloc(email->content,
								(*content_total) * sizeof(char));
		if (!aux) {
			sd_log(LOG_DEBUG,
				   "Could not reallocate %d * %d (= %d) bytes for %s",
				   (*content_total), sizeof(char),
				   (*content_total) * sizeof(char),
				   "email->content");
			free(email->content);
			return CRITICAL_SAFE_REALLOC;
		}
		email->content = aux;
	}
	for (int i = 0; i < buffer_size; ++i) {
		email->content[email->content_n] = buffer[i];
		email->content_n++;
	}
	email->content[email->content_n] = '\0';
	return NO_ERROR;
}

// This function reads an email, given a filename and its category.
// - - - - -
// char[]     filename = The path where the email is stored.
// email_t      *email = The content of an email that will be read.
// bool    is_personal = In all given tests it is false, but this flag is set to
//                       true in some personal tests.
// - - - - -
// Return: error_t = The function can detect any failure inside of it.
error_t read_email(char filename[BUFFER_SIZE],
				   email_t *email,
				   bool is_personal)
{
	FILE *fin = fopen(filename, "rt+");
	if (!fin) {
		sd_log(LOG_DEBUG,
			   "Could not read file '%s' in read_email()",
			   filename);
		return CRITICAL_INPUT_FILE;
	}
	static error_t e;
	static char buffer[BUFFER_SIZE];

    // Date and time
	if (is_personal)
		strcpy(buffer, "Wed, 11 Dec 2002 10:30:00 +2000\n");
	else
		fgets(buffer, BUFFER_SIZE, fin);
	datetime_extract_all(buffer, &email->date);
    // Some preparation
	email->content_n = 0;
	int content_total = BUFFER_SIZE;
	email->content = safe_calloc(content_total, sizeof(char));
	if (!email->content) {
		sd_log(LOG_DEBUG,
			   "Could not allocate %d * %d (= %d) bytes for %s in read_email()",
			   content_total, sizeof(char), content_total * sizeof(char),
			   "email->content");
		fclose(fin);
		return CRITICAL_SAFE_CALLOC;
	}
    // Subject
	fseek(fin, 9, SEEK_CUR);
	fgets(email->subject, BUFFER_SIZE, fin);
	email->subject[BUFFER_SIZE - 1] = '\0'; // Defensive programming
	email->subject_n = strlen(email->subject);
	if (is_personal) {
		strcpy(email->from, "N/A");
	} else {
		fseek(fin, 6, SEEK_CUR);
		fgets(email->from, BUFFER_SIZE, fin);
		char *left_p = strchr(email->from, '<');
		if (left_p) {
			int i;
			for (i = 0; (left_p + 1)[i]; ++i)
				email->from[i] = (left_p + 1)[i];
			if (email->from[i - 2] == '>')
				email->from[i - 2] = '\0';
			else
				email->from[i - 1] = '\0';
		} else {
			email->from[strlen(email->from) - 1] = '\0';
		}
	}
    // Body
	if (!is_personal)
		fseek(fin, 6, SEEK_CUR);
	while (fgets(buffer, BUFFER_SIZE, fin)) {
		e = inject_in_email(email, buffer, &content_total);
		if (is_critical(e)) {
			sd_log(LOG_DEBUG, "Tokenizing a body caused the program to throw.");
			return e;
		}
	}
    // Final
	sd_parse_count_and_lower_email(email);
	void *aux = safe_realloc(email->content,
							 (email->content_n + 1) * sizeof(char));
	if (!aux) {
		sd_log(LOG_DEBUG, "Adjusting %s to %d bytes was a catastrophe!",
			   "email->content", email->content_n + 1);
		return CRITICAL_SAFE_REALLOC;
	}
	email->content = aux;
	fclose(fin);
	return NO_ERROR;
}

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
error_t read_all_emails(char *emails_path, bool is_personal,
						int *n, email_t **emails)
{
	sd_log(LOG_INFO, "Searching for emails in: %s", emails_path);
	(*n) = count_files(emails_path);
	if (!(*n))
		return CRITICAL_INPUT_DIR;
	sd_log(LOG_INFO, "%d emails were found in %s.", (*n), emails_path);

	(*emails) = safe_calloc((*n), sizeof(email_t));
	if (!(*emails)) {
		sd_log(LOG_DEBUG,
			   "Could not allocate %d * %d (= %d) bytes for %s in %s",
			   (*n), sizeof(email_t), (*n) * sizeof(email_t),
			   "*emails", "read_all_emails()");
		return CRITICAL_SAFE_CALLOC;
	}

	error_t e;
	char buffer[BUFFER_SIZE];
	int emails_path_n = strlen(emails_path);
	strcpy(buffer, emails_path);
	for (int i = 0; i < *n; ++i) {
		sprintf(buffer + emails_path_n, "%d", i); // data/emails/i
		e = read_email(buffer, &((*emails)[i]), is_personal);

		if (is_critical(e)) {
			sd_log(LOG_DEBUG,
				   "Reading email %d (file: %s) triggered a critical error, %d",
				   i, buffer, e);
			for (int j = 0; j < i; ++j)
				free((*emails)[i].content);
			free(*emails);
			return e;
		}
	}
	sd_log(LOG_INFO, "All emails have been read and loaded into the memory.");
	return NO_ERROR;
}
