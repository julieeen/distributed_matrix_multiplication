/*
 ============================================================================
 Name        : matrix.c
 Author      : Julian Fietkau
 Copyright   : Julian Fietkau
 Description : matrix datatype and standard functionality
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "stack.h"
#include <string.h>
#include "help.h"

/*
 * getter for value (x,y) of committed matrix
 */
double getValue(matrix * matrix, int x, int y) {
	if ((x < 0) || (y < 0)) {
		perror("Matrix index starts @ 0!\n");
		return 0.0;
	}
	if (x > matrix->rows) {
		perror("out of bounds!");
		printf("[%d][%d]\n",x,y);

		return 0.0;

	}
	if (y > matrix->columns) {
		perror("out of bounds!\n");
		printf("[%d][%d]\n",x,y);

		return 0.0;

	}
	return matrix->values[(x * (matrix->columns)) + y];
}

/*
 * setter for value (x,y) of committed matrix
 */
int setValue(matrix * matrix, double value, int x, int y) {
	if ((x < 0) || (y < 0)) {
		perror("Matrix index starts @ 0!\n");
		printf("[%d][%d]\n",x,y);

		return 0;
	}
	if (x > matrix->rows) {
		perror("out of bounds!\n");
		printf("[%d][%d]\n",x,y);

		return 0;

	}
	if (y > matrix->columns) {
		perror("out of bounds!\n");
		return 0;

	}
	matrix->values[(matrix->columns * x) + y] = value;
	return 1;
}

double absolut(double value){
    	if( value > 0) return value;
	if( value < 0) return -1*value;
	if( value == 0) return 0;
}


/*
 * Defines equality for matrices.
 * Matrix is individual via size and by values.
 */
int equals(matrix * m1, matrix * m2){

	if(m1->columns != m2->columns){
		return 0;
	}
	if(m1->rows != m2->rows){
		return 0;
	}

	int x = 0; int y = 0; float E = 0.0000000001;
	while(x < m1->rows){
		while(y < m1->columns){	  
			// Comparing 2 double values, by compare the diff with a super-small number E
			if(!(absolut(getValue(m1,x,y) - getValue(m2,x,y)) <= E)){
				return 0;
			}
			y++;
		}
		y = 0;
		x++;
	}

	return 1;
}


/*
 * create a matrix with dimention row x column
 */
void createMatrix(matrix * m1, int rows, int column) {

	m1->rows = rows;
	m1->columns = column;

	double * array = malloc(m1->rows * m1->columns * sizeof(double));
	
	int i = 0;
	for(i = 0; i < m1->rows * m1->columns; i++){	// Array init with zeros
		array[i] = 0.0;	
	}

	if (array == NULL ) {
		perror("createMatrix() : out of memory");
	}
	m1->values = array;
}

/*
 * destroy matrix and free() the allocated memory
 */
int destroyMatrix(matrix * m1) {
	free(m1->values);
	return 1;
}


/*
 * prints matrix on an natural way
 */
void printMatrix(matrix * m1) {

	if(0){
		perror("Matrix is empty.");
	}

	printf("\n");

	int x = 0;
	int y = 0;
	while (x < m1->rows) {
		while (y < m1->columns) {
			printf("%f", getValue(m1, x, y));
			if ((y + 1 <= m1->columns)) {
				printf(" \t ");
			}
			y++;
		}
		printf("\n");
		y = 0;
		x++;
	}
	printf("\n");

}

/*
 * prints matrix as row
 */
void printMatrixRow(matrix * m1) {
	printf("[");

	int x = 0;
	int y = 0;
	while (x < m1->rows) {
		while (y < m1->columns) {
			printf("%f", getValue(m1, x, y));
			if ((y + 1 < m1->columns)) {
				printf(", ");
			}
			y++;
		}
		if( x + 1 < m1->rows){
			printf("; ");
		}
		y = 0;
		x++;
	}
	printf("]\n");

}

/*
 * Matrix multiplication implementation without parallelism
 * Check for : (a x b) * (b x c)
 * Calculation: Sum( ROW * COLUMN )
 */
int multiplication(matrix * result, matrix * m1, matrix * m2) {

	if (m1->columns != m2->rows) {
		perror("Spaltenanzahl m1 muss gleich Zeilenanzahl von m2 sein!\n");
		return 0;
	}

	//printf("--> result with %d x %d :", m1->rows, m2->columns);
	createMatrix(result, m1->rows, m2->columns);

	int x, y, m;
	for (x = 0; x < result->rows; x++) {
		for (y = 0; y < result->columns; y++) {
			for (m = 0; m < m1->columns; m++) {
				double temp = getValue(result,x,y) + getValue(m1,x,m) * getValue(m2,m,y);
				setValue(result,temp,x,y);
			}
		}
	}
	return 1;
}


/*
 * Generate matrix out of a given String
 * Matrix must look like: [double,double;...;double,double]
 * Hint: Because realloc() copy all data to make a new entity of our datastructure,
 * it is more efficient to iterate our data twice and malloc the calculated size .
 * (n * n) > 2 * n
 */
int parse_matrix(matrix * returnMatrix, char * stringData) {

	int i = 1;
	int values = 0;
	int xSize = 1;
	int ySize = 0;

	node * stack = NULL;
	init(stack);

	if (stringData == NULL ) {
		perror("string is empty!\n");
	}
	if (stringData[0] != '[') {
		perror("matrix starts with [\n");
	}
	if (stringData[1] == ']') {
		perror("matrix is empty!\n");
	}

	/*
	 * Iterate string, parse values and stack 'em.
	 */
	while (i < strlen(stringData) - 1) {

		// Numbers, Columns separated by ,
		if (stringData[i] == ',') {
			i++;

			if (ySize == 0) {
				xSize++;
			}
			continue;
		}
		// Rows separated by ;
		if (stringData[i] == ';') {
			i++;
			ySize++;
			continue;
		}

		// Converting string vales into real double
		if (isANumber(stringData[i])) {
			char * numberString = calloc(16, sizeof(char));	// Double has max. of 15 digites + '.'
			int iterateNumberString = 0;
			while (isPartOfNumber(stringData[i])) {		// itererate the whole number_string
				numberString[iterateNumberString] = stringData[i];
				iterateNumberString++;
				i++;
			}
			double number = atof(numberString);	// Converting with c_standard function atof()
			stack = push(stack, number);		// notice the number for allocation
			values++;
			continue;
		}
		perror("Matrix contains restricted sign(s)!\n");
		break;
	}

	if (stringData[i] != ']') {
		perror("matrix ! ends with ]\n");
	}

	/*
	 * Allocate matrix and fill with numbers
	 */
	//printf("INPUT: Allocate matrix with dimension of %d x %d\n", ySize + 1, xSize);
	returnMatrix->rows = ySize + 1;
	returnMatrix->columns = xSize;
	createMatrix(returnMatrix, returnMatrix->rows, returnMatrix->columns);
	int x = returnMatrix->rows-1, y = returnMatrix->columns-1;
	double value;
	while (x >= 0) {
		while (y >= 0) {
			stack = pop(stack, &value);		// remember the number (LIFO)
			setValue(returnMatrix,value,x,y);	// save data at his specific place
			y--;
		}
		y = returnMatrix->columns-1;
		x--;
	}
	return 1;
}

/*
 * generate identity matrix for problem size n
 */
int createIdentityMatrix(matrix * returnMatrix, int n){
	createMatrix(returnMatrix,n,n);
	int x = 0;
	while(x < n){
		if(!setValue(returnMatrix,1.0,x,x)){
			return 0;
		}
		x++;
	}
	return 1;
}

/*
 * Generate a random number
 */
double randomDouble(){
	return ((double) 10*rand()/(double)RAND_MAX);
}

/*
 * Generate a n x m matrix with random numbers
 */
int createRandomMatrix(matrix * returnMatrix, int n, int m){
	createMatrix(returnMatrix,n,m);
	int x = 0;
	int y = 0;
	while(y < m){
		while(x < n){
			if(!setValue(returnMatrix,randomDouble(),x,y)){
				return 0;
			}
			x++;
		}
		x = 0;
		y++;
	}
	return 1;
}

