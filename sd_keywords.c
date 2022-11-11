// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_keywords.h"

// This function reads the content of a file and stores the given information in
// a kw_collection_t element.
// - - - - -
// char[]            filename = The name of the file that has to be parsed.
// double          multiplier = If > 0, this will be the value used for every
//                              "values" component instead of reading a new one.
// kw_collection_t        *kw = This is where the resulting collection will be
//                              returned
// - - - - -
// Return: error_t = The function will report CRITICAL_INPUT_KW in case the
//                   input file could not be opened.
error_t kw_collection_read(char *filename,
						   double multiplier,
						   kw_collection_t *kw)
{
	FILE *fin = fopen(filename, "rt+");
	if (!fin) {
		sd_log_error(CRITICAL_INPUT_KW);
		sd_log(LOG_DEBUG, "Could not open '%s'.", filename);
		return CRITICAL_INPUT_KW;
	}

	strcpy(kw->associated_filename, filename);
	fscanf(fin, "%d\n", &kw->count);
	for (int i = 0; i < kw->count; ++i) {
		if (multiplier < 0)
			fscanf(fin, "%lf", &kw->values[i]);
		else
			kw->values[i] = multiplier;
		fgets(kw->keywords[i], MAX_WORD_SIZE, fin);

		// Defensive programming
		kw->keywords[i][MAX_WORD_SIZE - 1] = '\0';
		// Remove '\n':
		if (multiplier < 0) {
			kw->kw_size[i] = strlen(kw->keywords[i]) - 2;
			for (int j = 0; j < kw->kw_size[i]; ++j)
				kw->keywords[i][j] = kw->keywords[i][j + 1];
			kw->keywords[i][kw->kw_size[i]] = '\0';
			if (kw->keywords[i][kw->kw_size[i] - 1] == '\n')
				kw->keywords[i][kw->kw_size[i] - 1] = '\0';
		} else {
			kw->keywords[i][MAX_WORD_SIZE - 1] = '\0';
			// Remove '\n':
			kw->kw_size[i] = strlen(kw->keywords[i]) - 1;
			if (kw->keywords[i][kw->kw_size[i]] == '\n')
				kw->keywords[i][kw->kw_size[i]] = '\0';
		}

		kmp_compute_lps(kw->keywords[i], kw->kw_size[i], kw->lps[i]);
	}
	fclose(fin);
	return NO_ERROR;
}

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
error_t kw_collection_apply(kw_collection_t *kw,
							email_t *email,
							double multiplier,
							bool header,
							bool use_log,
							double *result)
{
	if (kw->count <= 0) {
		sd_log(LOG_DEBUG,
			   "The '%s' kw collection is empty",
			   kw->associated_filename);
		return WARNING_KW_COLLECTION_EMPTY;
	}

	double aux_score;
	*result = 0;
	for (int i = 0; i < kw->count; ++i) {
		aux_score = (double)kmp_search_utility(email->content, email->content_n,
											   kw->keywords[i], kw->kw_size[i],
											   kw->lps[i]);
		if (aux_score > 0.001 && use_log)
			aux_score = log(1.76 * aux_score + 0.96);
		aux_score *= multiplier * kw->values[i];
		*result += aux_score;
		if (aux_score > 0.001) {
			sd_log(LOG_DEBUG,
				   "| | | [%s] B-Found '%s', contributing %.2lf point(s).",
				   kw->associated_filename,
				   kw->keywords[i], aux_score);
		}
		if (!header)
			continue;
		aux_score = (double)kmp_search_utility(email->subject, email->subject_n,
											   kw->keywords[i], kw->kw_size[i],
											   kw->lps[i]) * multiplier;
		if (aux_score > 0.001 && use_log)
			aux_score = log(1.76 * aux_score + 0.96);
		aux_score *= multiplier * kw->values[i];
		*result += aux_score;
		if (aux_score > 0.001) {
			sd_log(LOG_DEBUG,
				   "| | | [%s] S-Found '%s', contributing %.2lf point(s).",
				   kw->associated_filename,
				   kw->keywords[i], aux_score);
		}
	}
	if (*result > 0.01)
		sd_log(LOG_DEBUG,
			   "| | ^-- Collection '%s' might contribute %.2lf point(s).",
			   kw->associated_filename, *result);
	return NO_ERROR;
}
