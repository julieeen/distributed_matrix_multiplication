/*
 ============================================================================
 Name        : project1.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : Matrix Multiplikation
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

#include "matrix.h"
#include "openMP.h"
#include "pThreads.h"
#include "openMPI.h"

#include <mpi.h>

/*
 * make a small benchmark testcase and bench our 4 implementations
 */
int main( int argc, char* argv[] ){

	
	// Initialize the MPI environment
        MPI_Init(&argc, &argv);
	
	// run all testcases with all 4 implementations
	test(1);
	test(2);
	test(3);
	test(4);



// decalre some easy problemsets 4 benching all implemenations & run
	int problemset_easy[5] = { 10, 50, 100, 500, 750};

	matrix m1;
	int i, size;
	for(i=0; i < 5; i++) {
		size = problemset_easy[i];
		printf(" ================================================ \n");
		printf(" ==========\t  Problemsize : %d  \t========= \n",size);
		printf(" ================================================ \n");
		createRandomMatrix(&m1,size,size);
		bench(&m1,1);
		bench(&m1,2);
		bench(&m1,3);
		bench(&m1,4);
	}

	// declare some hard problemsets 4 the parallel methods & run	
	int problemset_hard[2] = { 2000, 5000};
	for(i=0; i < 2; i++) {
		size = problemset_hard[i];
		printf(" ================================================ \n");
		printf(" ==========\t  Problemsize : %d  \t========= \n",size);
		printf(" ================================================ \n");
		createRandomMatrix(&m1,size,size);
		bench(&m1,2);
		bench(&m1,3);
		bench(&m1,4);
	}




	MPI_Finalize();
	destroyMatrix(&m1);

	return EXIT_SUCCESS;
}


