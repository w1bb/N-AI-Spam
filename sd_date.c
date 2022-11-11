// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_date.h"

// This function extracts the weekday from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_weekday(char buffer[BUFFER_SIZE],
							  datetime_t *dt)
{
	if (buffer[6] == 'M') {
		dt->weekday = MONDAY;
	} else if (buffer[6] == 'W') {
		dt->weekday = WEDNESDAY;
	} else if (buffer[6] == 'F') {
		dt->weekday = FRIDAY;
	} else if (buffer[6] == 'S') {
		if (buffer[7] == 'a')
			dt->weekday = SATURDAY;
		else
			dt->weekday = SUNDAY;
	} else { // Tue / Thu
		if (buffer[7] == 'u')
			dt->weekday = TUESDAY;
		else
			dt->weekday = THURSDAY;
	}
}

// This function extracts the day from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_day(char buffer[BUFFER_SIZE],
						  datetime_t *dt)
{
	dt->day = (buffer[11] - '0') * 10 + buffer[12] - '0';
}

// This function extracts the month from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_month(char buffer[BUFFER_SIZE],
							datetime_t *dt)
{
	// Select using the last letter
	if (buffer[16] == 'n') {
		if (buffer[15] == 'a')
			dt->month = JAN;
		else
			dt->month = JUN;
	} else if (buffer[16] == 'b') {
		dt->month = FEB;
	} else if (buffer[16] == 'y') {
		dt->month = MAY;
	} else if (buffer[16] == 'l') {
		dt->month = JUL;
	} else if (buffer[16] == 'g') {
		dt->month = AUG;
	} else if (buffer[16] == 'p') {
		dt->month = SEP;
	} else if (buffer[16] == 't') {
		dt->month = OCT;
	} else if (buffer[16] == 'v') {
		dt->month = NOV;
	} else if (buffer[16] == 'c') {
		dt->month = DEC;
	} else {
		if (buffer[15] == 'a')
			dt->month = MAR;
		else
			dt->month = APR;
	}
}

// This function extracts the year from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_year(char buffer[BUFFER_SIZE],
						   datetime_t *dt)
{
	dt->year = ((int)buffer[18] - '0') * 1000 +
		       ((int)buffer[19] - '0') * 100 +
			   ((int)buffer[20] - '0') * 10 +
			   ((int)buffer[21] - '0');
}

// This function extracts the hours, minutes and seconds from a buffer and
// inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_hms(char buffer[BUFFER_SIZE],
						  datetime_t *dt)
{
	dt->h = (buffer[23] - '0') * 10 + buffer[24] - '0';
	dt->m = (buffer[26] - '0') * 10 + buffer[27] - '0';
	dt->s = (buffer[29] - '0') * 10 + buffer[30] - '0';
	dt->GMT = ((int)buffer[33] - '0') * 1000 +
		      ((int)buffer[34] - '0') * 100 +
		      ((int)buffer[35] - '0') * 10 +
		      ((int)buffer[36] - '0');
	if (buffer[32] == '-')
		dt->GMT = -(dt->GMT);
}

// This function extracts everything from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
void datetime_extract_all(char buffer[BUFFER_SIZE],
						  datetime_t *dt)
{
	datetime_extract_weekday(buffer, dt);
	datetime_extract_day(buffer, dt);
	datetime_extract_month(buffer, dt);
	datetime_extract_year(buffer, dt);
	datetime_extract_hms(buffer, dt);
}
