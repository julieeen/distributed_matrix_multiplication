/*
 ============================================================================
 Name        : help.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Matrix Multiplikation with pThreads
 ============================================================================
 */

#include "matrix.h"
#include <error.h>
#include <pthread.h>
#include <stdio.h>

// datastruct for share data with threads
struct thread_data {
	int row;
	matrix * result;
	matrix * m1;
	matrix * m2;
};

//#define MAX_THREADS 4

// thread method : run on every WORKER
void * calculateRow(void * threadarg) {
	struct thread_data * my_data;
	my_data = (struct thread_data *) threadarg;
	int x, y, m;
	x = my_data->row;
	//printf("Thread %d working for Row %d\n", my_data->row, my_data->row);
	for (y = 0; y < my_data->result->columns; y++) {
		for (m = 0; m < my_data->m1->columns; m++) {
			double temp = getValue(my_data->result, x, y) + getValue(my_data->m1, x, m) * getValue(my_data->m2, m, y);
			setValue(my_data->result, temp, x, y);
		}
	}
	pthread_exit((void*) 0);
}

/*
 *  Matrix multiplication implementation with pThreads standard
 *  finding help at https://computing.llnl.gov/tutorials/pthreads
 */
int pThreadsMultiplication(matrix * result, matrix * m1, matrix * m2) {
	if (m1->columns != m2->rows) {
		perror("Spaltenanzahl m1 muss gleich Zeilenanzahl von m2 sein!\n");
		return 0;
	}

	createMatrix(result, m1->rows, m2->columns);
	pthread_t threads[result->rows];
	struct thread_data thread_data[result->rows];
	void * status;

	int x;
	/*
	 * Better than spawning n new threads and destroy them is using a thread pool
	 * --> Queuing the work and work only with a number of threads, but here not necessary
	 */
	for (x = 0; x < result->rows; x++) {
		// pack all necessary data for thread
		thread_data[x].m1 = m1;
		thread_data[x].m2 = m2;
		thread_data[x].result = result;
		thread_data[x].row = x;

		/*
		 * spawning a new thread should mean: get work from queue and deploy it to the threads
		 */
		if (pthread_create(&threads[x], NULL, calculateRow, (void*) &thread_data[x])) {
			printf("ERROR @ pthread_create()");
		}
	}

	/* Wait for all threads at the end*/
	int i;
	for (i = 0; i < result->rows; i++) {
		pthread_join(threads[i], &status);
	}
	return 1;
}
