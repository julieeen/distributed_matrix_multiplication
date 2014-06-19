/*
 ============================================================================
 Name        : help.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */

#ifndef HELP_H_
#define HELP_H_

int isANumber(char value);

int isPartOfNumber(char value);

double last_measured_time;

void run_timer();

void stop_timer();

#endif /* HELP_H_ */
