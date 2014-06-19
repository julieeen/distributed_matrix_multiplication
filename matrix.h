/*
 ============================================================================
 Name        : matrix.h
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 ============================================================================
 */

#ifndef MATRIX_H_
#define MATRIX_H_

/*
 * Internal representation of used matrices.
 * We have an array of values and calculate every getValue() and setValue() individually.
 * Matrix is uniquely defined by values, rows and columns
 */
typedef struct matrix {
	double * values;
	int rows;
	int columns;
}	matrix;

int equals(matrix * m1, matrix * m2);

double getValue(matrix * matrix, int x, int y);

int setValue(matrix * matrix, double value, int x, int y);

void createMatrix(matrix * m1, int rows, int column);

int destroyMatrix(matrix * m1);

void printMatrix(matrix * m1);

void printMatrixRow(matrix * m1);

int multiplication(matrix * result, matrix * m1, matrix * m2);

int multiplicationPThreads(matrix * result, matrix * m1, matrix * m2);

int parse_matrix(matrix * returnMatrix, char * stringData);

int createIdentityMatrix(matrix * returnMatrix, int n);

int createRandomMatrix(matrix * returnMatrix, int n, int m);

#endif /* MATRIX_H_ */
