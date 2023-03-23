first: mpi_vers omp_vers simple_vers

simple_vers: my_func.o simple_vers.o
	gcc simple_vers.o my_func.o -lm -o simple_solution
	
mpi_vers: mpi_vers.o my_func.o
	mpicc mpi_vers.o my_func.o -lm -o mpi_solution

omp_vers: omp_vers.o my_func.o
	gcc -fopenmp omp_vers.o my_func.o -lm -o omp_solution
	
simple_vers.o :simple_vers.c my_func.h
	gcc simple_vers.c -c -lm

mpi_vers.o: my_func.h mpi_vers.c
	mpicc mpi_vers.c -c -lm

omp_vers.o: my_func.h omp_vers.c
	gcc -fopenmp omp_vers.c -c -lm	

my_func.o: my_func.c my_func.h
	gcc my_func.c -c -lm
	
clean:
	rm -f simple_solution omp_solution mpi_solution my_func.o simple_vers.o omp_vers.o mpi_vers.o
