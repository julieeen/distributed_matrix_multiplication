/*
 ============================================================================
 Name        : help.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : some functions 5 getting work done
 ============================================================================
 */

#include <time.h>
#include <stdio.h>
#include "help.h"
#include <sys/time.h>

int isANumber(char value) {
	// ASCII : 0 = 48, 9 = 57
	if ((47 < value) && (value < 58)) {
		return 1;
	}
	return 0;
}

int isPartOfNumber(char value) {
	if ((47 < value) && (value < 58)) {	// ASCII : 0 = 48, 9 = 57
		return 1;
	}
	if (value == 46) {			// Char is a point
		return 1;
	}
	return 0;
}

/*
 ***************** Implements a small timestop function with milliseconds ***********
 */
struct timeval start,stop;
/*
 * start a clock
 * clock() falsified  the results by multithreading
 */
void run_timer() {
    gettimeofday(&start, NULL);
//	time(&start);
}

/*
 * reset timer and return bygone time
 */
void stop_timer() {
    gettimeofday(&stop, NULL);
    last_measured_time = difftime(stop.tv_sec, start.tv_sec) 
			+ (difftime(stop.tv_usec,start.tv_usec)/1000000);
}

