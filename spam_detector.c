// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.
#define WORK_ON_PRIVATE 0

#include "spam_detector.h"

int main(void)
{
	if (sd_tasks((bool)WORK_ON_PRIVATE) != NO_ERROR)
		exit(EXIT_FAILURE);
	return 0;
}
