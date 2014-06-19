/*
 ============================================================================
 Name        : openMPI.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Matrix Multiplikation with openMPI
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include <mpi.h>
/*
 * Matrix multiplication implementation with MPI standard
 * http://www.tu-chemnitz.de/informatik/RA/projects/mpihelp/bcast.html
 */
int openMpiMultiplication(matrix * result, matrix * m1, matrix * m2){

	createMatrix(result, m1->rows, m2->columns);

	int pid, processes;

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

// **************   Initialisierung beendet ************
	int x;
	// Parrallelisierung ueber Rows
	for (x = 0; x < result->rows; x++) {

		if (pid == 0) { // i am the master
			// send values to the workers
			MPI_Bcast(m1, 16,MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(m2, 16, MPI_INT, 0, MPI_COMM_WORLD);
			

			MPI_Status status;	// wait 4 workers
		}

		else { // i am the worker
			MPI_Bcast(m1, 16, MPI_INT, 0, MPI_COMM_WORLD);
			MPI_Bcast(m2, 16, MPI_INT, 0, MPI_COMM_WORLD);
			int y,m;
			for (y = 0; y < result->columns; y++) {
				for (m = 0; m < m1->columns; m++) {
					double temp = getValue(result, x, y) + getValue(m1, x, m) * getValue(m2, m, y);
					if(!setValue(result, temp, x, y)){
						perror("ERROR by writing into result!");
						return 0;
					}
				}
			}
			// return calculation back to master
			MPI_Send(result, 16, MPI_INT, 0, result->rows, MPI_COMM_WORLD);
		}
	}


	return 1;
}

