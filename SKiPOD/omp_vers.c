#include <omp.h>
#include <stdio.h>
#include "my_func.h"


int
main(int argc, char **argv)
{
    //default number of iterations
    long int n = 1000000;
    //default values of segment ends
    double x0 = -10;
    double x1 = 10;
    //default thread number
    int num = 100;
    
    //parsing the command line:
    //first is number of iterations second and
    //third are segment ends and fourth is amount of threads
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
    if (argc >= 5)
    {
        num = atoi(argv[4]);
    }

    //making all variables we need
    //calculation result
    double res = 0;
    //the size of one segment of the partition
    double h = (x1-x0)/n;
    //starting point for calculating the function: implement the method of middle rectangles
    double cur_x = x0 + h/2;
    //variables under time: start, end and difference.
    double time_start, time_finish, res_time;
    //launching parallel area with the reduction operation
    //to calculate max time of thread being in main loop
    //variables time_start and time_finish are private,
    //all the other - shared
    #pragma omp parallel default(none) private(time_start, time_finish) shared(x1, h, cur_x, n, res) reduction(max: res_time) num_threads(num)
    {
        time_start = omp_get_wtime();
        //launching main loop distribution between threads
        //with reduction operation to summarize the results
        //writing nowait, to measure maximum time of working
        #pragma omp for reduction(+ : res) nowait
        for (int i = 0; i < n; i += 1)
        {
            res += my_func(cur_x + i*h);
        }
        time_finish = omp_get_wtime();
        res_time = time_finish-time_start;
    }
    //parallel area has been done
    //showing result, and writing it down to omp_stats file
    printf("result: %e\n had been achieved in %e sec", res*h, res_time);
    FILE *f;
    f = fopen("omp_stats", "a");
    fprintf(f,"%d %ld %f %f\n", num, n, (float)res*h, (float)res_time);
    fclose(f);
    return 0;
}
