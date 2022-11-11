# Copyright (C) 2022 Valentin-Ioan VINTILA (313CA / 2021-2022).
# All rights reserved.

# Final archive configuration
ARCHIVE=313CA_VintilaValentinIoan_SpamDetector.zip

# Compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -lm -g # -ggdb3 -DDEBUG # Uncomment while testing

# Valgrind setup
VG=valgrind
VFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes \
       --verbose --log-file=VG_LOG.txt

# Targets (.o files)
TARGETS=sd_date.o sd_errors.o sd_hash.o sd_input.o sd_keywords.o sd_kmp.o \
sd_memory.o sd_parse.o sd_score.o sd_tasks.o sd_utilities.o

# Testing targets (.o files)
TTARGETS=sd_test.o

# Building process (used in production)
build: clean $(TARGETS)
	$(CC) $(CFLAGS) -o spam_detector $(TARGETS) spam_detector.c -lm
	rm -f $(TARGETS)

# Building process (used when testing)
build-test: clean test-setup $(TARGETS) $(TTARGETS)
	$(CC) $(CFLAGS) -o spam_detector $(TARGETS) $(TTARGETS) spam_detector.c -lm
	rm -f $(TARGETS) $(TTARGETS)

# Valgrind manual testing
run-vg: build-test
	$(VG) $(VFLAGS) ./spam_detector

# General setup when NOT in production
test-setup:
	$(eval CFLAGS += -DSD_LOG_FULL)

# Building process > sd_date
sd_date.o: sd_date.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_errors
sd_errors.o: sd_errors.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_input
sd_input.o: sd_input.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_hash
sd_hash.o: sd_hash.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_keywords
sd_keywords.o: sd_keywords.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_kmp
sd_kmp.o: sd_kmp.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_memory
sd_memory.o: sd_memory.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_parse
sd_parse.o: sd_parse.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_score
sd_score.o: sd_score.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_tasks
sd_tasks.o: sd_tasks.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_test
sd_test.o: sd_test.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Building process > sd_utilities
sd_utilities.o: sd_utilities.c
	$(CC) $(CFLAGS) -c -o $@ $^

# Cleaning process
clean:
	rm -f $(TARGETS) $(TTARGETS) spam_detector

# Packing process (to .ZIP)
pack:
	rm -f log0.txt $(ARCHIVE)
	touch log0.txt
	zip -FSr $(ARCHIVE) README Makefile *.c *.h in_* log0.txt
