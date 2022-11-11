// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SD_DATE_H
#define SD_DATE_H

// This library presents a way to obtain the date and time when an email was
// sent. In hindsight, this is a little useless. However, I could figure out
// some interesting facts about mail in general: most mail comes when you should
// rest, that is, Friday!

// It’s Friday then... it’s Saturday, Sunday what??
typedef enum {
	UNKNOWN_DAY = 0,
	MONDAY = 1,
	TUESDAY = 2,
	WEDNESDAY = 3,
	THURSDAY = 4,
	FRIDAY = 5,
	SATURDAY = 6,
	SUNDAY = 7
} day_t;

// The Gregorian calendar was introduced in 1582 by Pope Gragory XIII as a
// modification of, and replacement for, the Julian calendar!
typedef enum {
	UNKNOWN_MONTH = 0,
	JAN = 1, FEB = 2, MAR = 3,
	APR = 4, MAY = 5, JUN = 6,
	JUL = 7, AUG = 8, SEP = 9,
	OCT = 10, NOV = 11, DEC = 12
} month_t;

// This structure defines what info a date&time combo should contain.
typedef struct {
	day_t weekday;
	int day;
	month_t month;
	int year;
	int h, m, s;
	int GMT;
} datetime_t;

// General header file
#include "spam_detector.h"

// This function extracts the weekday from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_weekday(char buffer[BUFFER_SIZE],
									 datetime_t *dt);

// This function extracts the day from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_day(char buffer[BUFFER_SIZE],
								 datetime_t *dt);

// This function extracts the month from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_month(char buffer[BUFFER_SIZE],
								   datetime_t *dt);

// This function extracts the year from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_year(char buffer[BUFFER_SIZE],
								  datetime_t *dt);

// This function extracts the hours, minutes and seconds from a buffer and
// inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_hms(char buffer[BUFFER_SIZE],
								 datetime_t *dt);

// This function extracts everything from a buffer and inserts it into dt.
// - - - - -
// char[]     buffer = The input buffer.
// datetime_t    *dt = The resulting date (filled with said information).
extern void datetime_extract_all(char buffer[BUFFER_SIZE],
								 datetime_t *dt);

#endif // SD_DATE_H
