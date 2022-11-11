// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_tasks.h"

// This function solves all the tasks.
// - - - - -
// bool is_private = Usually false, sometimes true for testing purposes.
// - - - - -
// Return: error_t = The return value of sd_task_1() and/or sd_task_2().
error_t sd_tasks(bool is_private)
{
	error_t e;
	char emails_path[BUFFER_SIZE];

	if (is_private)
		strcpy(emails_path, "data/personal_test/");
	else
		strcpy(emails_path, "data/emails/");

	sd_log(LOG_INFO, "Emails folder set to '%s'", emails_path);
	int n;
	email_t *emails;

	e = read_all_emails(emails_path, is_private, &n, &emails);
	if (is_critical(e)) {
		sd_log(LOG_DEBUG,
			   "Reading all emails in sd_tasks() raised signal %d.", e);
	    sd_log_close();
		return e;
	}

	e = sd_task_1(n, emails);
	if (is_critical(e)) {
		sd_log(LOG_DEBUG,
			   "Solving task 1 in sd_tasks() raised signal %d.", e);
		for (int i = 0; i < n; ++i)
			free(emails[i].content);
		free(emails);
	    sd_log_close();
		return e;
	}

	e = sd_task_2(n, emails, is_private);
	if (is_critical(e)) {
		sd_log(LOG_DEBUG,
			   "Solving task 2 in sd_tasks() raised signal %d.", e);
		for (int i = 0; i < n; ++i)
			free(emails[i].content);
		free(emails);
	    sd_log_close();
		return e;
	}

	for (int i = 0; i < n; ++i)
		free(emails[i].content);
	free(emails);
	sd_log_close();
	return NO_ERROR;
}

// This function solves the first task.
// - - - - -
// int           n = The number of emails.
// email_t *emails = The emails found in a given directory.
// - - - - -
// Return: error_t = In case something wrong happens when allocating memory,
//                   when an output file is opened etc.
error_t sd_task_1(int n, email_t *emails)
{
	sd_log(LOG_GENERAL, "\n\n= = =   TASK 1   = = =");

	sd_log(LOG_INFO, "Reading keywords collection...");
	kw_collection_t kw;
	error_t e = kw_collection_read("data/keywords", 1, &kw);
	if (is_critical(e)) {
		sd_log(LOG_DEBUG, "Reading the keywords in sd_task_1() failed");
		return e;
	}

	FILE * fout = fopen("statistics.out", "wt+");
	if (!fout) {
		sd_log_error(CRITICAL_OUTPUT_FILE);
		sd_log(LOG_DEBUG, "Could not open 'statistics.out' in sd_task_1()");
		return CRITICAL_OUTPUT_FILE;
	}

	int *counts = safe_malloc(n * sizeof(int));
	if (!counts) {
		sd_log_error(CRITICAL_SAFE_MALLOC);
		sd_log(LOG_DEBUG,
			   "Could not allocate %d * %d (= %d) bytes for %s in sd_task_1()",
			   n, sizeof(int), n * sizeof(int), "counts");
		fclose(fout);
		return CRITICAL_SAFE_MALLOC;
	}

	sd_log(LOG_INFO, "Computing occurences...");
	// FILE * fkw = fopen("in_a_kw_2", "wt+");
	// fprintf(fkw, "%d\n", kw.count);
	for (int i = 0; i < kw.count; ++i) {
		int total_count = 0;
		double sq_sum = 0;
		bool first_occur = true;
		for (int j = 0; j < n; ++j) {
			counts[j] = kmp_search_utility(emails[j].content,
										   emails[j].content_n,
										   kw.keywords[i], kw.kw_size[i],
										   kw.lps[i]);

			if (counts[j]) {
				if (first_occur) {
					sd_log(LOG_INFO, "| Findings: [(in %d: %d)", j, counts[j]);
					first_occur = false;
				} else {
					sd_log(LOG_GENERAL, ", (in %d: %d)", j, counts[j]);
				}
			}
			total_count += counts[j];
		}
		if (!first_occur)
			sd_log(LOG_GENERAL, "]");
		double m_a = (double)total_count / (double)n;
		for (int j = 0; j < n; ++j)
			sq_sum += ((double)counts[j] - m_a) * ((double)counts[j] - m_a);
		double result = sqrt(sq_sum / (double)n);
		sd_log(LOG_INFO,
			   "^--- KW#%03d ('%s') occured %d times. Score %.6f was reported.",
			   i, kw.keywords[i], total_count, result);
		fprintf(fout, "%s %d %.6f\n", kw.keywords[i], total_count, result);
		// fprintf(fkw, "%.6f %s\n", result, kw.keywords[i]);
	}

	sd_log(LOG_GENERAL, "\n\n==> TASK 1 COMPLETE");
	free(counts);
	fclose(fout);
	// fclose(fkw);
	return NO_ERROR;
}

// This function logs some information about the i-th email. In case i is
// negative, the function will log the progress it found and then will reset.
// - - - - -
// hash_t    *hashes = The hashes of the emails.
// double    *scores = The score for each email.
// int             i = What email should be logged (or <0 if progress).
// int    *true_spam = This is NULL in production; when testing, it represents
//                     the true nature of emails.
void sd_task_2_log_entry(hash_t *hashes,
						 double *scores, int i,
						 int *true_spam)
{
#ifdef SD_LOG_FULL
	static int count_ok, count_nok;
#else
	true_spam = true_spam; // useless warning
#endif
	if (i < 0) {
#ifdef SD_LOG_FULL
		sd_log(LOG_GENERAL, "\nCurrent F1: %lf\n", (double)count_ok /
			   (double)((double)count_ok + 0.5 * (double)count_nok));
		printf("\nCurrent F1: %lf\n", (double)count_ok /
			   (double)((double)count_ok + 0.5 * (double)count_nok));
		count_ok = 0;
		count_nok = 0;
#endif
	} else {
		sd_log(LOG_INFO, "> Email #%03d (hash: %lu) scored %.6f",
			   i, hashes[i].hash, scores[i]);
		if (scores[i] >= SPAM_THRESHOLD) {
#ifdef SD_LOG_FULL
			if (true_spam[i]) {
				sd_log(LOG_INFO, "  Email correctly labeled as spam");
				++count_ok;
			} else {
				sd_log(LOG_INFO, "  EMAIL WAS HAM (FALSE POSITIVE)");
				++count_nok;
			}
#else
			sd_log(LOG_INFO, "  Email marked as SPAM");
#endif
		} else {
#ifdef SD_LOG_FULL
			if (true_spam[i]) {
				sd_log(LOG_INFO, "  EMAIL WAS SPAM");
				++count_nok;
			} else {
				sd_log(LOG_INFO, "  Email correctly labeled as ham");
			}
#else
			if (scores[i] >= SPAM_LOW_SUSPICION) {
				if (scores[i] >= SPAM_HIGH_SUSPICION)
					sd_log(LOG_INFO, "  Email marked as suspicious HAM (SHAM)");
				else
					sd_log(LOG_INFO, "  Email marked as probably HAM (PHAM)");
			} else {
				sd_log(LOG_INFO, "  Email marked as definitely HAM (DHAM)");
			}
#endif
		}
		sd_log(LOG_GENERAL, "\n");
	}
}

// This function solves the second task.
// - - - - -
// int              n = The number of emails.
// email_t    *emails = The emails found in a given directory.
// bool    is_private = Usually false, sometimes true for testing purposes.
// - - - - -
// Return: error_t = In case something wrong happens when allocating memory,
//                   when an output file is opened etc.
// - - - - -
// Return: error_t = In case something wrong happens when allocating memory,
//                   when an output file is opened etc.
error_t sd_task_2(int n, email_t *emails, bool is_private)
{
	sd_log(LOG_GENERAL, "\n\n= = =   TASK 2   = = =");
	FILE *fout = fopen("prediction.out", "wt+");
	if (!fout) {
		sd_log_error(CRITICAL_OUTPUT_FILE);
		sd_log(LOG_DEBUG, "Could not open 'prediction.out' in sd_task_2()");
		return CRITICAL_OUTPUT_FILE;
	}
	kw_collection_t kw_a_kw, kw_connectivity, kw_news, kw_positive_kw;
	error_t e;
	if (is_critical(kw_collection_read("in_a_kw", -1, &kw_a_kw)) ||
		is_critical(kw_collection_read("in_con", 1, &kw_connectivity)) ||
		is_critical(kw_collection_read("in_news", -1, &kw_news)) ||
		is_critical(kw_collection_read("in_p_kw", -1, &kw_positive_kw))) {
		sd_log(LOG_DEBUG,
			   "One of the keyword collections could not be read in %s",
			   "sd_task_2()");
		return CRITICAL_INPUT_KW;
	}
#ifdef SD_LOG_FULL
	int true_spam[MAX_EMAILS];
	get_true_spam(is_private, true_spam);
#else
	int *true_spam = NULL;
	is_private = is_private;
#endif
	double *scores = safe_calloc(n, sizeof(double));
	if (!scores) {
		sd_log_error(CRITICAL_SAFE_CALLOC);
		sd_log(LOG_DEBUG,
			   "Could not allocate %d * %d (= %d) bytes for %s in sd_task_2()",
			   n, sizeof(double), n * sizeof(double), "scores");
		return CRITICAL_SAFE_CALLOC;
	}
	hash_t *hashes = safe_calloc(n, sizeof(hash_t));
	if (!hashes) {
		sd_log_error(CRITICAL_SAFE_CALLOC);
		sd_log(LOG_DEBUG,
			   "Could not allocate %d * %d (= %d) bytes for %s in sd_task_2()",
			   n, sizeof(hash_t), n * sizeof(hash_t), "hashes");
		free(scores);
		return CRITICAL_SAFE_CALLOC;
	}
	for (int i = 0; i < n; ++i) {
		sd_log(LOG_INFO, "Logging Email - %03d:", i);
		e = score_all(&scores[i], &emails[i],
					  &kw_a_kw, &kw_connectivity, &kw_news, &kw_positive_kw);
		if (is_critical(e)) {
		    sd_log(LOG_DEBUG,
				   "Something very mysterious happened in score_all()");
			free(scores);
			free(hashes);
			return e;
		}
		hashes[i].hash = hash_compute(emails[i].content);
		hashes[i].s = emails[i].content;
		hashes[i].position = i;
		sd_task_2_log_entry(hashes, scores, i, true_spam);
	}
	sd_task_2_log_entry(hashes, scores, -1, true_spam);
	// post_score(n, hashes, scores);
	for (int i = 0; i < n; ++i) {
		if (scores[i] >= SPAM_THRESHOLD)
			fprintf(fout, "1\n");
		else
			fprintf(fout, "0\n");
		// sd_task_2_log_entry(hashes, scores, i, true_spam);
	}
	// sd_task_2_log_entry(hashes, scores, -1, true_spam);
	sd_log(LOG_GENERAL, "\n==> TASK 2 COMPLETE");
	free(hashes);
	free(scores);
	fclose(fout);
	return NO_ERROR;
}
