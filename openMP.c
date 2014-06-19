/*
 ============================================================================
 Name        : openMP.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Matrix Multiplikation with OpenMP
 ============================================================================
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

//#define MAX_THREADS 64

/*
 *  Matrix multiplication implementation with openMP
 */
int openMPmultiplication(matrix * result, matrix * m1, matrix * m2) {

	if (m1->columns != m2->rows) {
		perror("Spaltenanzahl m1 muss gleich Zeilenanzahl von m2 sein!\n");
		return 0;
	}
	createMatrix(result, m1->rows, m2->columns);

	/*
	 *  Starting with parallel programming
	 *  shared() : shared data 4 all threads
	 *  local() : only usable 4 a local thread
	 */
#pragma omp parallel shared(result,m1,m2)	// num_threads(MAX_THREADS)
	{
		int tid = omp_get_thread_num();
		int x, y, m = 0;

		if (tid == 0) {	// Masterthread
			//printf("Starting matrix multiplication with %d threads\n", omp_get_num_threads());
		}

		/*
		 * problemsize n = 1000 arguments with 4 threads
		 * nowait : 6 sec
		 * schedule(dynamic,1) collapse(1) : 18 sec
		 * schedule(dynamic,1) collapse(2) : 34 sec		--> more parallism = more overhead = slower ?
		 */
#pragma omp for nowait	// next for() runs parallel
		for (x = 0; x < result->rows; x++) {
			for (y = 0; y < result->columns; y++) {
				//printf("Thread %d working on [%d][%d] \n", omp_get_thread_num(),x, y);
				for (m = 0; m < m1->columns; m++) {
					double temp = getValue(result, x, y)
							+ getValue(m1, x, m) * getValue(m2, m, y);
					setValue(result, temp, x, y);
				}
			}
		}

	}


	return 1;

}
