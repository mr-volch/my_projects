#include <stdio.h>

double*
find_max(double* double_mas, int len_mas)
{
    double max_elem;
    double* max_ind;
    if (len_mas == 0){
        return(NULL);
    }
    max_elem = double_mas[0];
    max_ind = double_mas;
    for (int i = 1; i < len_mas; i++){
        //printf("%lf\n", *(double_mas + i));
        if (double_mas[i] > max_elem){
            max_elem = double_mas[i];
            max_ind = double_mas + i;
        }
    }
    return(max_ind);
}

void
exchange(double* double_one, double* double_two)
{
    double tmp;
    tmp = *double_one;
    *double_one = *double_two;
    *double_two = tmp;
}

void
sort_puz(double* double_mas, int len_mas)
{
    double* cur_max;
    for(int i = 0; i < len_mas; i++){
        cur_max = find_max((double_mas + i), (len_mas - i));
        //printf("%lf\n", *cur_max);
        exchange((double_mas + i), cur_max);
    }
}

int
main(void)
{
    int st_len=0;
    double cur_double;
    scanf("%d", &st_len);
    double main_mas[st_len];
    for(int i = 0; i < st_len; i++){
        scanf("%lf", &cur_double);
        main_mas[i] = cur_double;
        //printf("%d\n", i);
    }
    
    //for(int i = 0; i < st_len; i++){
    //    printf("%lf\n", main_mas[i]);
    //}
    if (st_len >= 18){
        printf("%lf\n", *find_max((main_mas + 4), 14));
        //printf("%s\n", "bingo");
    }
    sort_puz(main_mas, st_len);
    for(int i = 0; i < st_len; i++){
        printf("%lf\n", main_mas[i]);
    }
    return(0);
}


