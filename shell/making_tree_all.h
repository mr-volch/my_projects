#include "lexems1.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>


//функция, формирующая массив из дескрипторов
//вводного и выводного файлов (если их нет,
//то соотв. элемент массива равен -1)
int* file_proc(struct comand_info* cur_cmd);
//основная функция выполнения: выполняет нужный код возврата
int main_proc(struct comand_info* cur_cmd);
//функция, содержащая основной шаг цикла выполнения
int* main_step_proc(struct comand_info* cur_cmd);
//функция, реализующая конвейер. Возвращает массив пидов сыновей
int* pipe_proc(struct comand_info* cur_cmd);
//подфункция pipe_proc: возвращает дескриптор чтения из последнего пайпа
int link_proc(struct comand_info* cur_cmd, int fd_in, int **pid_arr);
