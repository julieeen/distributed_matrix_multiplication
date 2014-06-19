/*
 ============================================================================
 Name        : stack.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct node {
	double data;
	struct node* next;
}node;

void init(node * head);

node * push(node* head, double data);

node * pop(node *head, double * element);

#endif /* STACK_H_ */
