#include <stdio.h>
#include <string.h>

int
check_equal(char str_one[], char str_two[])
{
    int len_one, len_two, i, j = 0;
    char *pointer_one = str_one, *pointer_two = str_two;
    len_one = strlen(str_one);
    len_two = strlen(str_two);
    pointer_one = str_one;
    pointer_two = str_two;
    for(i=0; i < len_one; i++){
        if (*(pointer_one + i) != ' '){
            while ((*(pointer_two + j) == ' ') && (j < len_two)){
                j++;
            }
            if (*(pointer_one + i) != *(pointer_two + j)){
                return(0);
            }
            if (j != len_two){
                j++;
            }
        } 
    }       
    while ((*(pointer_two + j) == ' ') && (j < len_two)){
        j++;
    }
    if (j != len_two){
        return(0);
    }
    return(1);
}


int
main(void)
{
    char s_one[82];
    char s_two[82];
    int res, len_one, len_two;
    if (fgets(s_one, 82, stdin) == 0){
        return(1);
    }
    if (fgets(s_two, 82, stdin) == 0){
        return(1);
    }
    len_one = strlen(s_one) - 1;
    len_two = strlen(s_two) - 1;
    s_one[len_one] = '\0';
    s_two[len_two] = '\0';
    res = check_equal(s_one, s_two);
    if (res == 1){
        printf("%s\n", "YES");
    }  else{
        printf("%s\n", "NO");
    }
}


