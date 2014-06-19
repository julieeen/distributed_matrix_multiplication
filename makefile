
project1: project1.c
	mpicc -fopenmp -pthread  *.c -o project1

run: project1
	mpirun project1

clean: project1
	rm project1
