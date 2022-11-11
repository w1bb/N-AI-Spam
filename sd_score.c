// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_score.h"

// This function checks if a given email is a response to another.
// - - - - -
// email_t *email = The email to be checked.
// - - - - -
// Return: bool = true, if the email was a response.
bool is_response(email_t *email)
{
	if (email->subject_n < 4)
		return false;
	if (email->subject[0] == 'r' && email->subject[1] == 'e' &&
		email->subject[2] == ':' && email->subject[3] == ' ')
		return true;
	if (email->subject_n < 5)
		return false;
	if (email->subject[0] == 'f' && email->subject[1] == 'w' &&
		email->subject[2] == 'd' && email->subject[3] == ':' &&
		email->subject[2] == ' ')
		return true;
	return false;
}

// If enough "> " substrings are found, the email could be indeed ham. This is
// a metric similar to the response one.
// - - - - -
// email_t *email = The email to be checked.
// - - - - -
// Return: bool = true, if the email might have been a forward.
bool is_great(email_t *email)
{
	int count = 0;
	for (int i = 1; i < email->content_n; ++i) {
		if (email->content[i - 1] == '>' && email->content[i] == ' ')
			++count;
	}
	return (count >= 3);
}

// This function scores an email based on the ammount of uppercase letters
// present in its original content.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
void score_uppercase(double *score, email_t *email)
{
	if (email->nonspace_count) {
		*score = 100.0 * (double)email->uppercase_letters /
				(double)email->nonspace_count;
		if (*score >= 17.9) {
			if (*score >= 19.9) {
				if (*score >= 21.9)
					*score = 17.0 + (*score - 21.0) * (*score - 21.0) * 2;
				else // < 21.9
					*score = (*score - 20) * 5.18 + 7.0;
			} else { // < 19.9
				*score /= 3.0616;
			}
		} else if (*score < 2) {
			*score = (2 - *score) * (2 - *score) + 0.67;
		}
		sd_log(LOG_DEBUG, "| U: Uppercase score: %lf", *score);
	} else {
		*score = 0;
		sd_log(LOG_DEBUG, "| U: Uppercase score UNAVAILABLE (forced 0)");
	}
	*score *= 2;
}

// This function scores an email based on its header.
// Note: This function is currently DISABLED!
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
void score_header(double *score, email_t *email)
{
	*score = 0;
	if (email->uppercase_letters_h * 2 > email->subject_n)
		*score += 10.0;
	// *score = 30.0 * (double)email->uppercase_letters_h /
	//		 (double)email->subject_n;
	// if (email->question_marks_h >= 1)
		// *score += 4.0 * email->question_marks_h;
	if (email->exclamation_marks_h >= 1)
		*score += 3.0 * email->exclamation_marks_h;
	// if (email->dollar_signs_h >= 1)
	//	*score += 6.0 * email->dollar_signs_h;

	// This function has to be enabled - comment the following line:
	// *score = 0;

	if (*score > 0.1)
		sd_log(LOG_DEBUG, "| H: Header score: %lf", *score);
}

// This function scores an email based on the ammount of consecutive consonants.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
void score_consonants(double *score, email_t *email)
{
	int consec_cons6 = 0, consec_cons7 = 0, cc = 0;

	for (int i = 0; i < email->content_n; ++i) {
		if (email->content[i] >= 'a' && email->content[i] <= 'z' &&
			!strchr("aeiou", email->content[i])) { // maybe y?
			++cc;
		} else {
			if (cc == 6)
				++consec_cons6;
			else if (cc >= 7)
				++consec_cons7;
			cc = 0;
		}
	}
	*score = (double)consec_cons6 * 2.0 + (double)consec_cons7 * 9.0;
	sd_log(LOG_DEBUG, "| C: Consonants score: %lf", *score);
}

// This function scores an email based on its punctuation.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
void score_punctuation(double *score, email_t *email)
{
	*score = 0;
	if (email->asterisks) {
		*score += 6.0;
		if (email->asterisks >= 4)
			*score += 15.0 * (double)(email->asterisks - 2.0);
		else
			*score += (email->asterisks - 1) * 8.0;
	}
	if (email->dollar_signs) {
		*score += 15;
		*score += 7 * (double)(email->dollar_signs - 1.0);
	}
	if (email->exclamation_marks) {
		*score += 0.8;
		*score += 2.4 * (double)(email->exclamation_marks - 1.0);
	}
	*score *= 1000.0 / (double)email->characters;

	if (email->punctuation_chars * 20 < email->word_count) {
		*score += pow((double)email->punctuation_chars / 20.0, 1.3);
		if (email->punctuation_chars * 30 < email->word_count) {
			*score += pow((double)email->punctuation_chars / 30.0, 1.5);
			if (email->punctuation_chars * 40 < email->word_count)
				*score += pow((double)email->punctuation_chars / 40.0, 1.8);
		}
	}

	sd_log(LOG_DEBUG, "| P: Unbalanced punctuation score: %lf", *score);
}

// This function scores an email based on the 'data/spammers' file.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
// - - - - -
// Return: error_t = Could be CRITICAL_INPUT_SP in something wrong happened
//                   when trying to input from data/spammers.
error_t score_known_spammers(double *score, email_t *email)
{
	FILE *fspam = fopen("data/spammers", "rt+");
	if (!fspam) {
		sd_log_error(CRITICAL_INPUT_SP);
		return CRITICAL_INPUT_SP;
	}

	char spammer[BUFFER_SIZE];
	int spammers_n;
	double rv;
	*score = 0;

	fscanf(fspam, "%d", &spammers_n);
	for (int i = 0; i < spammers_n; ++i) {
		fscanf(fspam, "%s", spammer);
		fscanf(fspam, "%lf", &rv);
		rv *= SPAM_THRESHOLD / 35.0; // Update to proprietary scoring system
		if (strcmp(spammer, email->from) == 0) {
			*score += rv;
			fclose(fspam);
			sd_log(LOG_DEBUG, "| S: Spam database score: %lf", *score);
			return NO_ERROR;
		}
	}

	// Not found
	fclose(fspam);
	return NO_ERROR;
}

// This functions scores the first 100 characters of the email. If these are
// "spamy", the whole email has a way higher chance of also being spam.
// - - - - -
// double            *score = The end result.
// email_t           *email = The email to be scored.
// kw_collection_t *kw_a_kw = The spam words collection.
// double          *k_score = The original score that will be updated.
void score_first_100(double *score, email_t *email,
					 kw_collection_t *kw_a_kw, double *k_score)
{
	int aux_content_n = email->content_n;
	char aux_chr;
	if (aux_content_n > 100) {
		aux_chr = email->content[100];
		email->content[100] = 0;
		if (email->content_n > 100)
			email->content_n = 100;
	}
	kw_collection_apply(kw_a_kw, email, 1.35, true, true, score);
	if (aux_content_n > 100) {
		email->content[100] = aux_chr;
		email->content_n = aux_content_n;
	}

	if (*score >= 3.5)
		if (*k_score)
			*k_score = (*k_score + 2) * 3.0;
}

// This function scores how many connections can be found in a given email.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
void score_connectivity(double *score, email_t *email,
						kw_collection_t *kw_connectivity)
{
	kw_collection_apply(kw_connectivity, email, 1, true, true, score);
	*score += 2.0 * email->found_links;
	*score += 1.2 * email->found_emails;
}

// This function scores an email in general.
// - - - - -
// double  *score = The end result.
// email_t *email = The email to be scored.
// - - - - -
// Return: error_t = In case something wrong happens.
error_t score_all(double *score, email_t *email,
				  kw_collection_t *kw_a_kw,
				  kw_collection_t *kw_connectivity,
				  kw_collection_t *kw_news,
				  kw_collection_t *kw_positive_kw)
{
	bool email_re = is_response(email), email_great = is_great(email);
	error_t e;
	double p_score, u_score, c_score, s_score, k_score, h_score, factor = 2150;
	// sd_log(LOG_DEBUG, "| >>> EMAIL CONTENT: %s", email->content);
	sd_parse_links(email);
	sd_parse_numbers(email);
	sd_parse_spaces(email);
	score_punctuation(&p_score, email);
	score_header(&h_score, email); // DISABLED
	score_uppercase(&u_score, email);
	score_consonants(&c_score, email);
	sd_parse_hidden_letters_email(email);
	sd_parse_spaces(email); // sd_keep_nonspaces(email);
	double a_kw_score, con_score, news_score, p_kw_score, a_top_kw_score;
	kw_collection_apply(kw_a_kw, email, 1.35, true, true, &a_kw_score);
	k_score = a_kw_score;
	sd_log(LOG_DEBUG, "| ^---- K-A => K-Total: %.2lf", k_score);
	score_first_100(&a_top_kw_score, email, kw_a_kw, &k_score);
	sd_log(LOG_DEBUG, "| ^---- K-1 => First 100 score: %.2lf; K-Total: %.2lf",
		   a_top_kw_score, k_score);
	kw_collection_apply(kw_positive_kw, email, 1, false, true, &p_kw_score);
	// p_kw_score = 0; // TODO REMOVE - but should test like this
	k_score = (k_score > p_kw_score) ? (k_score - p_kw_score) : 0;
	sd_log(LOG_DEBUG, "| ^---- K-P => K-Total: %.2lf", k_score);
	kw_collection_apply(kw_news, email, 1, true, false, &news_score);
	if (news_score >= 9.9 && //email->word_count > 500 &&
		news_score * 65.0 > (double)email->word_count) {
		k_score = 0;
	}
	sd_log(LOG_DEBUG, "| ^---- K-N => K-Total: %.2lf", k_score);
	score_connectivity(&con_score, email, kw_connectivity);
	if (con_score < 0.1) {
		k_score /= 3.0;
	} else if (con_score < 2.5) {
		k_score /= 1.2;
	} else if (con_score > 10.9) {
		k_score /= pow(1.03, (con_score - 10.9));
		p_score /= pow(1.02, (con_score - 10.9));
	}
	sd_log(LOG_DEBUG, "| ^---- K-C => Connect. score: %.2lf; K-Total: %.2lf",
		   con_score, k_score);
	sd_log(LOG_DEBUG, "|              New punctuation score: %.2lf", p_score);
	k_score = k_score * factor / (double)(email->word_count);
	if (email->characters <= 20 ||
		email->numbers_short * 5 >= email->word_count) {
		k_score = 60.0;
		sd_log(LOG_DEBUG, "| K-S: Email too short => K-Total: %.2lf", k_score);
	}
	if (email_re)
		k_score /= 2.0;
	if (email_great)
		k_score /= 2.0;
	sd_log(LOG_DEBUG, "| K: re=%d g=%d  ==>  K-Total: %.2lf",
		   email_re, email_great, k_score);
	e = score_known_spammers(&s_score, email);
	if (is_critical(e)) {
		sd_log(LOG_DEBUG, "score_all() couldn't score them known spammers...");
		return e;
	}
	*score = p_score + h_score + u_score + c_score + s_score + k_score;
	if (email->numbers_long) {
		double phone_multiplier = pow(1.5, email->numbers_long);
		sd_log(LOG_DEBUG, "| L: Long (maybe phone) numbers multiplier: %lf",
			   phone_multiplier);
		*score *= phone_multiplier;
	}
	return NO_ERROR;
}

// This function takes into consideration duplicate spam emails.
// - - - - -
// int          n = The number of emails.
// hash_t *hashed = The hashes for said emails.
// double  *score = The scores that will be updated.
void post_score(int n, hash_t *hashes, double *scores)
{
	sd_log(LOG_INFO, "Post score analysis:");
	int consec_eq = 0;
	qsort(hashes, n, sizeof(hash_t), hash_compare);
	for (int i = 1; i <= n; ++i) {
		if ((i == n || hash_compare(&hashes[i], &hashes[i - 1]))) {
			if (scores[hashes[i - 1].position] >= SPAM_LOW_SUSPICION &&
				consec_eq) {
				double to_add = 0.0;
				to_add = pow((double)scores[hashes[i - 1].position] /
						 (double)SPAM_LOW_SUSPICION, consec_eq);
				if (scores[hashes[i - 1].position] >= SPAM_HIGH_SUSPICION) {
					to_add += pow((double)scores[hashes[i - 1].position] /
							  (double)SPAM_HIGH_SUSPICION, consec_eq) * 4.0;
				}

				sd_log(LOG_DEBUG, "| Group detected:");
				sd_log(LOG_DEBUG, "| | [");
				for (int j = i - 1 - consec_eq; j < i - 1; ++j) {
					scores[hashes[j].position] += to_add;
					sd_log(LOG_GENERAL, "%d, ", hashes[j].position);
				}
				scores[hashes[i - 1].position] += to_add;
				sd_log(LOG_GENERAL, "%d]", hashes[i - 1].position);
				sd_log(LOG_DEBUG, "| ^-- Score updated from %.6lf to %.6lf.",
					   hashes[i - 1].position,
					   scores[hashes[i - 1].position] - to_add,
					   scores[hashes[i - 1].position]);
			}
			consec_eq = 0;
		} else {
			++consec_eq;
		}
	}
	sd_log(LOG_GENERAL, "\n");
}
