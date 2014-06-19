/*
 ============================================================================
 Name        : test.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : a small test-case 4 developing
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <omp.h>

#include "matrix.h"
#include "openMP.h"
#include "pThreads.h"
#include "openMPI.h"

/*
 * a small test-case 4 developing
 */
int test(int n) {
	if((n < 1) || (n > 4)){
		perror("test() is only defined in range from 1 upto 4.");
		return 0;
	}

	printf("\nRUNNING MATRIX MULTIPLICATION TESTCASE: \n");
	char* m1string = "[1,2;3,4;5,6]";
	char* m2string = "[1,2,3;4,5,6]";
	char* identityMatrix3 = "[1,0,0;0,1,0;0,0,1]";
	char* m1m2string = "[9,12,15;19,26,33;29,40,51]";	// m1 * m2
	char* m2m1string = "[22,28;49,64]";					// m2 * m1

	matrix matrix1;
	matrix matrix2;
	matrix m1m2;
	matrix m2m1;
	matrix identity3;
	matrix identityBig;

	parse_matrix(&matrix1, m1string);
	parse_matrix(&matrix2, m2string);
	parse_matrix(&m1m2, m1m2string);
	parse_matrix(&m2m1, m2m1string);
	parse_matrix(&identity3, identityMatrix3);

	matrix result;
	createMatrix(&result, 0,0);
	/*
	 * CASE 1	A*B
	 */
	printf("Testing A (3x2) * B (2x3) = C (2x2) :");
	destroyMatrix(&result);
	switch (n) {
	case 1:	printf("sequential program");
		multiplication(&result, &matrix1, &matrix2);
		break;
	case 2:
		printf(" with openMP");
		openMPmultiplication(&result, &matrix1, &matrix2);
		break;
	case 3:
		printf(" with pThreads");
		pThreadsMultiplication(&result, &matrix1, &matrix2);
		break;
	case 4:
		printf(" with MPI");
		openMpiMultiplication(&result, &matrix1, &matrix2);
		break;
	}

	if (!equals(&result, &m1m2)) {
		printf(" ... false \n");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);

	/*
	 * CASE 2	B*A
	 */
	printf("Testing B (2x3) * A (3x2) = C (3x3) :");
	destroyMatrix(&result);
	switch (n) {
	case 1:	printf("sequential program");
		multiplication(&result, &matrix2, &matrix1);
		break;
	case 2:
		printf(" with openMP");
		openMPmultiplication(&result, &matrix2, &matrix1);
		break;
	case 3:
		printf(" with pThreads");
		pThreadsMultiplication(&result, &matrix2, &matrix1);
		break;
	case 4:
		printf(" with MPI");
		openMpiMultiplication(&result, &matrix2, &matrix1);
		break;
	}

	if (!equals(&result, &m2m1)) {
		printf(" ... false \n");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);

	/*
	 * CASE 3	E*E
	 */
	printf("Testing E (3x3) * E (3x3) = C (3x3) :");
	destroyMatrix(&result);
	switch (n) {
	case 1:	printf("sequential program");
		multiplication(&result, &identity3, &identity3);
		break;
	case 2:
		printf(" with openMP");
		openMPmultiplication(&result, &identity3, &identity3);
		break;
	case 3:
		printf(" with pThreads");
		pThreadsMultiplication(&result, &identity3, &identity3);
		break;
	case 4:	
		printf(" with MPI");
		openMpiMultiplication(&result, &identity3, &identity3);
		break;
	}
	if (!equals(&result, &identity3)) {
		printf(" ... false \n");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);

	//TODO if big is to high, there is a runtime error : no such file or directory
	int big = 3;
	createIdentityMatrix(&identityBig, big);
	/*
	 * CASE 4	E.big*E.big
	 */
	printf("Testing E (%dx%d) * E (%dx%d) = C %dx%d) :",big,big,big,big,big,big);
	destroyMatrix(&result);
	switch (n) {
	case 1:	printf(" sequential program");
		multiplication(&result, &identityBig, &identityBig);
		break;
	case 2:
		printf(" with openMP");
		openMPmultiplication(&result, &identityBig, &identityBig);
		break;
	case 3:
		printf(" with pThreads");
		pThreadsMultiplication(&result, &identityBig, &identityBig);
		break;
	case 4:
		printf(" with MPI");
		openMpiMultiplication(&result, &identityBig, &identityBig);
		break;
	}
	if (!equals(&result, &identityBig)) {
		printf(" ... false \n");
	}
	else{printf(" ... ok \n");}
	//printMatrix(&result);
	
	destroyMatrix(&result);
	destroyMatrix(&matrix1);
	destroyMatrix(&matrix2);
	destroyMatrix(&m1m2);
	destroyMatrix(&identity3);
	destroyMatrix(&identityBig);

	return 0;
}

#include "help.h"
/*
 * greater testcase 4 benchmark calculation power
 * 	n : 1 for sequential
 * 	n : 2 for p_threads implementation
 * 	n : 3 for opemMP
 * 	n : 4 for ...
 */
void bench(matrix * m1, int n) {
	matrix result;

	run_timer();

	switch (n) {
	case 1:
		printf("Benchmarking sequential program\n");
		multiplication(&result,m1,m1);
		break;
	case 2:
		printf("Benchmarking openMP\n");
		openMPmultiplication(&result, m1, m1);
		break;
	case 3:
		printf("Benchmarking pThreads\n");
		pThreadsMultiplication(&result, m1, m1);
		break;
	case 4:
		printf("Benchmarking MPI \n");
		openMpiMultiplication(&result, m1, m1);
		break;
	}
	stop_timer();
	printf("-> %.3f seconds for benchmark\n", last_measured_time);
	printf("\n");
	destroyMatrix(&result);
}
