// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

#ifndef SPAM_DETECTOR_H
#define SPAM_DETECTOR_H

// This file contains everything that has to be included in order to provide the
// needed functionality. This file also contains important macro definitions.

// This is required for count_files - it makes sure DT_REG is defined.
#define _GNU_SOURCE

// This is the default buffer size for any character array input.
#define BUFFER_SIZE 1024

// Standard library dependencies
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <dirent.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Spam Detector
#include "sd_errors.h"
#include "sd_kmp.h"
#include "sd_memory.h"
#include "sd_hash.h"
#include "sd_date.h"
#include "sd_base.h"
#include "sd_utilities.h"
#include "sd_keywords.h"
#include "sd_parse.h"
#include "sd_input.h"
#include "sd_test.h"
#include "sd_score.h"
#include "sd_tasks.h"

#endif // SPAM_DETECTOR_H
