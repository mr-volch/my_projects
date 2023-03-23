#include "making_tree_all.h"
          
int*
file_proc(struct comand_info* cur_cmd)
{
     //заводим массив под дескрипторы
     int *fd_real = malloc(2*sizeof(int));
     
     //по умолчанию минус еиницы
     fd_real[0] = -1;
     fd_real[1] = -1;
     
     //если есть, то пробуем открыть входной файл
     if (cur_cmd->infile != NULL){
         fd_real[0] = open(cur_cmd->infile, O_RDONLY);
         
         if (fd_real[0] == -1){
             exit(1);
         }
     }
     
     
     //если есть, то пробуем открыть выходной файл
     //(с учетом флага дозаписи в конец)
     if (cur_cmd->outfile != NULL){
         //int fd_2;
         
         if (cur_cmd->if_append == 0){
             fd_real[1] = open(cur_cmd->outfile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
             if (fd_real[1] == -1){
                 exit(1);
             }
         }
         
         else{
             fd_real[1] = open(cur_cmd->outfile, O_WRONLY|O_CREAT|O_APPEND, 0644);
             
             if (fd_real[1] == -1){
                 exit(1);
             }
         }
     }
     
     //возвращаем массив дескрипторов
     return fd_real;
}


int
main_proc(struct comand_info* cur_cmd)
{
    //массив массивов всех сыновей
    int **all_pids = malloc(sizeof(int *));
    int len_all_pids = 0;
    
    //статус - здусь будет статус последнего 
    //завершившегося процесса
    int status = 0;
    
    //вызываем основной шаг цикла и получаем массив созданных сыновей
    int *pid_arr = main_step_proc(cur_cmd);
       
    //если хотим посмотреть на текущую вершину
    //print_comand_info(cur_cmd, 0);
    
    //флаг для того, чтобы понять:
    //следует ли чистить дин. память
    int flag_to_free = 1;
    
    //пока поле next не NULL идем по указателям в нем
    while (cur_cmd->next != NULL){
        //пополнение массива массивов
        len_all_pids++;
        all_pids = realloc(all_pids, len_all_pids * sizeof(int *));
        all_pids[len_all_pids-1] = pid_arr;
        
        //если не & а ;, ждем всех сыновей этого шага
        if (cur_cmd->backgrnd == 0){
            for (int i = 1; i < (pid_arr[0] + 1); i++){
                int cur_one;
                if (pid_arr[i] != -1){
                    if (waitpid(pid_arr[i], &cur_one, 0) != -1){
                        status = cur_one;
                        //после обработки кладем -1
                        pid_arr[i] = -1;
                    }
                }
            }
        }
        
        //цикл, осуществляющий неблокирующее ожидание всех сыновей
        //до ныне запущенных. Для того, чтобы не накапливались зомби
        for (int j = 0; j < len_all_pids; j++){
            for (int k = 1; k < (all_pids[j][0] + 1); k++){
                int cur;
                //если пид равен -1, то либо процесс не запустился,
                //либо уже завершился и был "провейчен"
                if (all_pids[j][k] != -1){
                    if (waitpid(all_pids[j][k], &cur, WNOHANG) > 0){
                        status = cur;
                        //после обработки, кладем -1
                        all_pids[j][k] = -1;
                    }
                } 
            }
        }
        
        //переходим на следующую команду
        cur_cmd = cur_cmd->next;
        
        //если последовательность команд заканчивается на & или ;
        if (cur_cmd->arg_amount == -1){
            flag_to_free = 0;
            break;
        }
        
        //шаг цикла
        pid_arr = main_step_proc(cur_cmd);
    }
    
    //если надо, то чистим последний массив пидов
    if (flag_to_free == 1){
        free(pid_arr);
    }
    
    //ждем всех сыновей       
    int tmp;
    while (wait(&tmp) != -1){
        status = tmp;
    }
    
    //чистим память из массивов пидов
    
    for (int i = 0; i < len_all_pids; i++){
        free(all_pids[i]);
    }
    
    free(all_pids);
    
    //формируем код возврата и возвращаем его
    int res = 0;
    
    if (WIFEXITED(status) == 1){
        res = (WEXITSTATUS(status));
    } else if (WIFSIGNALED(status) == 1){
        res = (128 + WTERMSIG(status));
    }
    
    return res;
}


int*
main_step_proc(struct comand_info* cur_cmd)
{
    int *pid_arr = NULL;

    //если конвейер, то запускаем функцию-обработчик конвейера
    if (cur_cmd->pipe != NULL){
        pid_arr = pipe_proc(cur_cmd);
    }
    
    //иначе просто запускаем команду
    else{
        //создаем массив пидов: первый элемент - кол-во
        //пидов, последующие - собственно пиды
        pid_arr = malloc(2 * sizeof(int));
        pid_arr[0] = 1;
        pid_arr[1] = fork();
        
           
        
        if (pid_arr[1] == 0){
            //в сыне формируем потоки ввода-вывода
            int *fd_real = file_proc(cur_cmd);
        
            if (fd_real[0] != -1){
                dup2(fd_real[0], 0);
                close(fd_real[0]);
            }
        
            if (fd_real[1] != -1){
                dup2(fd_real[1], 1);
                close(fd_real[1]);
            }
            
            //если мы запускаем процесс-скобки,
            // то вызываем main_proc
            
            if(cur_cmd->psubcmd != NULL){
                int cur = main_proc(cur_cmd->psubcmd);                
                //правила возврата из скобок такие же как
                //из всего шелла
                exit (cur);
            }
            
            execvp(cur_cmd->argv[0], cur_cmd->argv);
            //если экзек не сработал
            exit(127);
        }
    }
    
    //возвращаем массив пидов сыновей
    return pid_arr;
}


int*
pipe_proc(struct comand_info* cur_cmd)
{
    //заводим массив пидов под сыновей
    int *pid_arr = malloc(sizeof(int));
    pid_arr[0] = 0;
    
    int **point_pid_arr = malloc(sizeof(int *));
    point_pid_arr[0] = pid_arr;
    
    //значение входного дескриптора по умолчанию: -1
    int fd_in = -1;
    
    //пока есть последующие команды в конвейере
    while (cur_cmd->pipe != NULL){
    
        //вызываем функцию, создающую процесс под конвейер
        fd_in = link_proc(cur_cmd, fd_in, point_pid_arr);
        
        //если вернулись с ошибкой от пайпа, то возвращаем
        //тех сыновей, которых успели сформировать
        if (fd_in == -1){
            return pid_arr;
        }
        
        //движение по указателю
        cur_cmd = cur_cmd->pipe;
    }
    
    //осталась последняя команда в конвейере
    
    int *last_arr = *point_pid_arr;
    
    int to_change = last_arr[0];
    
    //поплняем массив пидов
    last_arr = realloc(last_arr, (to_change + 2) * sizeof(int));
    last_arr[0] = last_arr[0] + 1;
    last_arr[(last_arr[0])] = fork();
    
    //создаем сына
    if (last_arr[(last_arr[0])] == 0){
        
        //free(last_arr);
        
        //сначала перенаправляем ввод-вывод в 
        //соответсвии с конвейером. Так как это
        //менее приоритетно
        dup2(fd_in, 0);
        close(fd_in);
        
        //затем формируем потоки ввода-вывода
        //в соответсвии с непосрдественными указаниями
        int *fd_real = file_proc(cur_cmd);
        
        if (fd_real[0] != -1){
            dup2(fd_real[0], 0);
            close(fd_real[0]);
        }
        
        if (fd_real[1] != -1){
            dup2(fd_real[1], 1);
            close(fd_real[1]);
        }
        
        //если наткунлись на скобки, то 
        //рвызываем main_proc
        if (cur_cmd->psubcmd != NULL){
            main_proc(cur_cmd->psubcmd);
            exit(0);
        }
        
        //иначе exec
        execvp(cur_cmd->argv[0], cur_cmd->argv);
        exit(127);
    }
    
    close(fd_in);
    
    free(point_pid_arr);
    return last_arr;
}

int
link_proc(struct comand_info* cur_cmd, int fd_in, int **point_pid_arr)
{    
    int fd_one[2];
    if (pipe(fd_one) == -1){
        return -1;
    }
    
    //разымненовываем указатель
    int *pid_arr = *point_pid_arr;
    
    //пополняем массив
    int to_change = pid_arr[0];
    pid_arr = realloc(pid_arr, (to_change + 2) * sizeof(int));
    pid_arr[0] = pid_arr[0] + 1;
    pid_arr[(pid_arr[0])] = fork();
    
    if (pid_arr[(pid_arr[0])] == 0){
    
        //free(pid_arr);
        
        //настраиваем каналы ввода-вывода, согласно конвейеру
        dup2(fd_one[1],1);
        if (fd_in != -1){
            dup2(fd_in, 0);
            close(fd_in);
        }
        close(fd_one[1]);
        close(fd_one[0]);
        
        //настраивем каналы ввода-вывода, согласно 
        //непосредственным указаниям файлов
        //(делаем это после конвейерных, т.к. эти
        //перенаправления приоритетнее)
        int *fd_real = file_proc(cur_cmd);
        
        if (fd_real[0] != -1){
            dup2(fd_real[0], 0);
            close(fd_real[0]);
        }
        
        if (fd_real[1] != -1){
            dup2(fd_real[1], 1);
            close(fd_real[1]);
        }
        
        //если скобки
        if (cur_cmd->psubcmd != NULL){
            main_proc(cur_cmd->psubcmd);
            exit(0);
        }
        
        execvp(cur_cmd->argv[0], cur_cmd->argv);
        exit(127);
    }
     
        
    close(fd_one[1]);
    
    if (fd_in != -1){
        close(fd_in);
    }
    
    *point_pid_arr = pid_arr;
    //возвращаем дескриптор для чтения следующей
    //команды конвейера
    return fd_one[0];
}

