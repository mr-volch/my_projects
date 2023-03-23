#include "my_func.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

//function that change the function for
// which the integral is calculated
void
modify_file(char *new_str)
{
    int ds = open("my_func.c", O_CREAT|O_TRUNC|O_WRONLY);
    write(ds, "#include \"my_func.h\"\n\ndouble\nmy_func(double x)\n{\n  return ", 60);
    write(ds, new_str, strlen(new_str));
    write(ds, ";\n}\n", 4);
    close(ds);
    return;
}

int
main(int argc, char **argv)
{
    //default number of iterations
    long int n = 1000000;
    //default segment ends
    double x0 = -10;
    double x1 = 10;
    
    //parsing command line: first is number of iterations
    //second and third are segment end and fourth is
    //function, if the user want to change it
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
        modify_file(argv[4]);
        printf("You have set a new function ");
        printf("Please call make again and than run your programm\n");
        return 1;
    }

    //sequntial algorithm with loging the results into base file
    double res = 0;
    double h = (x1-x0)/n;
    double cur_x = x0 + h/2;
    struct timeval time_start, time_finish;
    gettimeofday(&time_start, NULL);
    for (int i = 0; i < n-1; i++)
    {
        res += my_func(cur_x);
        cur_x += h;
    }
    gettimeofday(&time_finish, NULL);
    float res_sec = (time_finish.tv_sec-time_start.tv_sec);
    float res_usec = (float)(time_finish.tv_usec-time_start.tv_usec);
    float res_time = res_sec + res_usec / 1000000;
    printf("result: %e has been achieved in %f s\n", res*h, res_time);
    FILE *f;
    f = fopen("base", "a");
    fprintf(f, "%ld %f\n", n, res_time);
    fclose(f);
    return 0;
}
