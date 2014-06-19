/*
 ============================================================================
 Name        : Stack.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Stack implementation, necessary 4 parsing the matrix
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <error.h>

void init(node* head) {
	head = NULL;
}

/*
 * Stack as linked list, FILO
 */
node* push(node* head, double data) {
	node* tmp = (node*) malloc(sizeof(node));
	if (tmp == NULL ) {
		perror("Not possible to stack matrix elements!");
	}
	tmp->data = data;
	tmp->next = head;
	head = tmp;
	return head;
}

/*
 * Pop out last stacked element
 */
node* pop(node *head, double * element) {
	node* tmp = head;
	*element = head->data;
	head = head->next;
	free(tmp);
	return head;
}
