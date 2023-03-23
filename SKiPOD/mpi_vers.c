#include <mpi.h>
#include <stdio.h>
#include "my_func.h"


int
main(int argc, char **argv)
{
    //default iterations value
    long int n = 1000000;
    //default values for ends of segment
    double x0 = -10;
    double x1 = 10;
    
    //parsing launch parameters
    //first is number of iterations
    //second and third are ends of segment
    if (argc >= 2)
    {
        n = atoi(argv[1]);
    }
    if (argc >= 3)
    {
        x0 = strtod(argv[2], NULL);
    }
    if (argc >= 4)
    {
        x1 = strtod(argv[3], NULL);
    }

    //making all variables we need
    //calculation result
    double res = 0;
    //size of one segment of the partition
    double h = (x1-x0)/n;
    //initiating point to count function at:
    double cur_x = x0 + h/2;
    //measuring time: start, finish and difference
    double time_start, time_finish, res_time;
    //root-process variables for getting results from MPI_REDUCE
    double my_res, my_time;
    //initializing MPI workspace
    int rank, commSize;
    MPI_Init(&argc, &argv);
    time_start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commSize);
    //scenario of the main "working" processes
    if (rank != 0){
        //every working process making steps of cicle with numbers n,
        //so that: n%(commSize-1) == rank-1
        for (int i = rank-1; i < n; i += commSize-1)
        {
            res += my_func(cur_x + i*h);
        }
        //after making main loop, sending results with MPI_Reduce to
        //the root process, which will count sum of these results
        MPI_Reduce(&res ,&my_res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        //measuring execution time and sending it to root-process
        //wih MPI_Reduce function with max specification
        time_finish = MPI_Wtime();
        res_time = time_finish-time_start;
        MPI_Reduce(&res_time, &my_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    }
    
    //root process scenario. getting values with
    //MPI_Reduce and writing them into "mpi_stats" file
    else
    {
        MPI_Reduce(&res ,&my_res, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&res_time, &my_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
        printf("result: %e had been achieved in %e sec\n", my_res*h, my_time);
        FILE *f;
        f = fopen("mpi_stats", "a");
        fprintf(f,"%d %ld %f %f\n", commSize, n, (float)my_res*h, (float)my_time);
        fclose(f);
    }
    //finishing MPI workspace and leaving
    MPI_Finalize();
    return 0;
}
