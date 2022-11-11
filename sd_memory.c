// Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
// All rights reserved.

// Include the asscociated header file
#include "sd_memory.h"

// This function allocates memory safely (it verifies that said memory does
// indeed get allocated and retries for a few times)
// - - - - -
// int     n = The number of bytes that should be allocated
// int  line = The line number where the safe_malloc was called
// int retry = The number of times the function should retry to allocate said
//			   memory
// - - - - -
// Return: void * = Pointer to the allocated memory (or NULL)
void *safe_malloc_utility(int n, int line, int retry)
{
	static long total_allocation;
	void *p = malloc(n);
	if (!p) {
		if (retry != 0)
			return safe_malloc_utility(n, line, retry - 1);

		// Error message
		sd_log_error(CRITICAL_SAFE_MALLOC);
		sd_log(LOG_DEBUG, "Totally allocated (using safe_malloc): %d",
			   total_allocation);
		sd_log(LOG_INFO, "More info could appear bellow:");
		return NULL;
	}
	total_allocation += n;
	return p;
}

// This function allocates memory safely (it verifies that said memory does
// indeed get allocated and retries for a few times)
// - - - - -
// int n = The number of elements that should be allocated
// int size = The number of bytes in an element
// int  line = The line number where the safe_calloc was called
// int retry = The number of times the function should retry to allocate said
//			   memory
// - - - - -
// Return: void * = Pointer to the allocated memory (or NULL)
void *safe_calloc_utility(int n, int size, int line, int retry)
{
	static long total_allocation;
	void *p = calloc(n, size);
	if (!p) {
		if (retry != 0)
			return safe_calloc_utility(n, size, line, retry - 1);

		// Error message
		sd_log_error(CRITICAL_SAFE_CALLOC);
		sd_log(LOG_DEBUG, "Totally allocated (using safe_calloc): %d",
			   total_allocation);
		sd_log(LOG_INFO, "More info could appear bellow:");
		return NULL;
	}
	total_allocation += n * size;
	return p;
}

// This function reallocates memory safely (it verifies that said memory does
// indeed get reallocated and retries for a few times)
// - - - - -
// int     n = The number of bytes that should be reallocated
// int  line = The line number where the safe_realloc was called
// int retry = The number of times the function should retry to reallocate said
//			   memory
// - - - - -
// Return: void * = Pointer to the reallocated memory
void *safe_realloc_utility(void *ptr, int n, int line, int retry)
{
	static long total_reallocation;
	void *p = realloc(ptr, n);
	if (!p) {
		if (retry != 0)
			return safe_realloc_utility(ptr, n, line, retry - 1);

		// Error message
		sd_log_error(CRITICAL_SAFE_REALLOC);
		sd_log(LOG_DEBUG, "Totally reallocated (using safe_realloc): %d",
			   total_reallocation);
		sd_log(LOG_INFO, "More info could appear bellow:");
		return NULL;
	}
	total_reallocation += n;
	return p;
}
