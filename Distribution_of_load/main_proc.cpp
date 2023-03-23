#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "xml_worker.h"
#include <time.h>


//глобальная переменная, хранящая "уровень параллелизма"
//алгоритм будет запущен на равном ей количестве потоков
//по умолчанию: 1
int parallel_level = 1;


using namespace std;


//функция, осуществляющая параллельный алгоритм
int *
find_best_parallel(int n_proc, int *proc_load, int n_prog, int *prog_load, int **intensity){

    //заводим 4 неименованных канала

    //"семафор" - будет использоваться для синхронизации
    int fd_semoph[2];
    if (pipe(fd_semoph) == -1){
        return NULL;
    }

    //в этом канале в конце алгоритма будет записано общее число итераций во всех процессах
    int fd_iterations[2];
    if (pipe(fd_iterations) == -1){
        return NULL;
    }

    //здесь будет храниться текущее, наилучшее(для всех процессов) значение целевой функции
    int fd_value[2];
    if (pipe(fd_value) == -1){
        return NULL;
    }

    //здесь будет храниться вектор, соответсвующий наилучшему решению
    int fd_res[2];
    if (pipe(fd_res) == -1){
        return NULL;
    }

    //заполняем неименованные каналы начальными значениями
    int beg = 0;
    //в "семафоре" просто должно что-то лежать,
    // будем хранить в нем теущее число безрезультат. итераций
    // а изнач. число итераций - ноль
    write(fd_semoph[1], &beg, sizeof(int));
    write(fd_iterations[1], &beg, sizeof(int));

    //формируем макс. теоретическое значение целевой функции (про то, почеу именно так см README)
    for (int i = 0; i < n_prog-1; i++){
        for (int j = 0; j < n_prog-1-i; j++){
            beg += intensity[i][j];
        }
    }

    //записываем в соотв. канал найденное значение
    write(fd_value[1], &beg, sizeof(int));

    //изначальный вектор заполняем -1 (т.к. если алгоритм не преуспеет,
    //то мы не будем менять вектор, а просто вернем вектор из -1
    beg = -1;
    for (int i = 0; i < n_prog; i++){
        write(fd_res[1], &beg, sizeof(int));
    }

    //сзапускаем столько процессов, сколько сказано в parallel_level
    for (int j = 0; j < parallel_level; j++){
        if (fork() == 0){
            //под текущий вектор
            int *cur_x = new int[n_prog];
            //под вектор, который будем читать из канала
            int *cur_best_res = new int[n_prog];
            //под число итераций в этом потоке
            int n_try = -1;
            while(1){
                n_try++;

                //формируем случайное решение
                for (int i = 0; i < n_prog; i++){
                    cur_x[i] = rand() % n_proc;
                }
                //вычисляем значение целевой функции (про то, почеу именно так см README)
                int cur_F= 0;

                for (int i = 0; i < n_prog-1; i++){
                    for (int j = i+1; j < n_prog; j++){
                        if (cur_x[i] != cur_x[j]){
                            cur_F += intensity[i][j-i-1];
                        }
                    }
                }

                //проверяем корректность:
                //не нарушены ли ограничения по нагрузке на процессоры

                //формируем пустой вектор, каждый элемент которого будет соотв. некому процессору
                int cur_load[n_proc];
                for (int i = 0; i < n_proc; i++){
                    cur_load[i] = 0;
                }

                //рассчитываем получившеюся нагрузку на все процессоры
                for (int i = 0;i < n_prog; i++){
                    cur_load[cur_x[i]] += prog_load[i];
                }

                //проверяем не превышена ли нагрузка на процессоры
                //если превышена хоть на один, то flag = 1
                int flag = 0;
                for (int i = 0;i < n_proc; i++){
                    if (cur_load[i] >= proc_load[i]){
                        flag = 1;
                        break;
                    }
                }

                int sem;
                //опускаем семафор: вход в крит. секцию

                //все промежуточные выводы остались закомментированы:
                //если их раскомментировать, то будет хорошо видна
                //корректность работы алгоритма с точки зрения
                //строго последовательного доступа процессов к разделяемым ресурсам

                read(fd_semoph[0], &sem, sizeof(int));
                //cout << "process " << getpid() << " entered sec " << sem << endl;

                //если число безрезультатных попыток превысило 5000: заканчиваем алгоритм
                if (sem >= 5000){
                    //cout << "process " << getpid() << " escaping sec " << sem << endl << endl;
                    write(fd_semoph[1], &sem, sizeof(int));
                    break;
                }

                //если текущее решение оказалось некорректным, то увеличиваем семафор на 1
                //и выходим из критической секции
                if (flag){
                    sem++;
                    //cout << "process " << getpid() << " escaping sec " << sem << endl << endl;
                    write(fd_semoph[1], &sem, sizeof(int));
                    continue;
                }

                //читаем текущее наилучшее значение целевой функции
                int cur_max_F;
                read(fd_value[0], &cur_max_F, sizeof(int));

                //cout << "process " << getpid() << " read val " << cur_max_F << " cur val " << cur_F << endl;

                //если значение оказалось 0, но заканчиваем алгоритм
                if (cur_max_F == 0){
                    write(fd_value[1], &cur_max_F, sizeof(int));
                    //cout << "process " << getpid() << " escaping sec " << sem << endl << endl;
                    write(fd_semoph[1], &sem, sizeof(int));
                    break;
                }

                //если новое значение целевой функции не лучшее, записываем все данные обратно
                //в каналы и выходим из критич. секцмм
                if (cur_max_F <= cur_F){
                    sem++;
                    write(fd_value[1], &cur_max_F, sizeof(int));
                    //cout << "process " << getpid() << " escaping sec " << sem << endl << endl;
                    write(fd_semoph[1], &sem, sizeof(int));
                    continue;
                }

                //к этому моменту мы нашли новое наилучшее корректное решение

                //меняем лучшее значение и лучшее решение
                read(fd_res[0], cur_best_res, n_prog*sizeof(int));

                for (int i = 0; i < n_prog; i++){
                    cur_best_res[i] = cur_x[i];
                    //cout << "process: " << getpid() << " elem " << i << " val " << cur_best_res[i] << endl;
                }

                //записываем все данные в каналы
                write(fd_res[1], cur_best_res, n_prog*sizeof(int));
                write(fd_value[1], &cur_F, sizeof(int));

                //выход из критической секции: поднимаем семафор
                sem = 0;
                //cout << "process " << getpid() << " escaping sec " << sem << endl << endl;
                write(fd_semoph[1], &sem, sizeof(int));
            }

            //вышли из цикла
            //завершаем процесс
            //закрываем  дескрипторы каналов
            close(fd_semoph[0]);
            close(fd_semoph[1]);
            close(fd_res[0]);
            close(fd_res[1]);
            close(fd_value[0]);
            close(fd_value[1]);

            //заносим сведения о числе итераций в соотв. канал
            int cur_try;
            read(fd_iterations[0], &cur_try, sizeof(int));
            cur_try += n_try;
            write(fd_iterations[1], &cur_try, sizeof(int));

            //закрываем последний незакрытый канал
            close(fd_iterations[0]);
            close(fd_iterations[1]);

            //чистим динамическую память
            delete []cur_x;
            delete []cur_best_res;

            //чистим наследие от родителся
            delete []proc_load;
            delete []prog_load;
            for (int i = 0; i < n_prog-1; i++){
                delete []intensity[i];
            }
            delete []intensity;

            //завершаемся
            exit(0);
        }
    }

    //ВСЕ СЫНОВЬЯ СОЗДАНЫ
    //закрывваем дескрипторы, которые нам более не нужны в родителе
    close(fd_semoph[0]);
    close(fd_semoph[1]);
    close(fd_res[1]);
    close(fd_value[1]);
    close(fd_iterations[1]);

    //теперь ждем сыновей, после чего смотрим, что они насчитали
    int status;
    while (wait(&status) != -1){};

    //читаем итоговые данные из неименованных каналов и формируем вектор ответов
    //(про структуру вектора ответов см README)
    int *found_x = new int[n_prog + 2];
    read(fd_res[0], found_x + 2, n_prog*sizeof(int));
    read(fd_iterations[0], found_x + 1, sizeof(int));
    read(fd_value[0], found_x, sizeof(int));

    //закрываем оставшиеся десрипторы и возвращаем результат
    close(fd_res[0]);
    close(fd_value[0]);
    close(fd_iterations[0]);

    return found_x;
}


//функция, реализующая однопоточный алгоритм
int *
find_best(int n_proc, int *proc_load, int n_prog, int *prog_load, int **intensity){

    //ищем макс. теоретическое значение целевой функции
    //про то как см. Readme
    int best_F = 0;
    for (int i = 0; i < n_prog-1; i++){
        for (int j = 0; j < n_prog-i-1; j++){
            best_F += intensity[i][j];
        }
    }

    int *best_x = new int[n_prog + 2];

    for (int i = 0;i < n_prog; i++){
        best_x[i] = -1;
    }

    //число безрезультатных попыток
    int n_try = 0;
    //общее число попыток
    int global_try = 0;
    //порождаем место в динамической памяьт под вектор
    //распределения программ по процессорам
    int *cur_x = new int[n_prog];

    //запускаем основной цикл алгоритма
    while(n_try < 5000){

        //генерируем случайный вектор х
        for (int i = 0; i < n_prog; i++){
            cur_x[i] = rand() % n_proc;
        }

        //находим значение целевой функции
        //про то как, см Readme
        int cur_F= 0;

        for (int i = 0; i < n_prog; i++){
            for (int j = i+1; j < n_prog; j++){
                if (cur_x[i] != cur_x[j]){
                    cur_F += intensity[i][j-i-1];
                }
            }
        }

        //если найденное значение оказалось не лучше прежнего, продолжаем поиски
        if (cur_F >= best_F){
            n_try++;
            continue;
        }

        //проверяем корректность:
        //не нарушены ли ограничения по нагрузке на процессоры
        //(аналогично проверке в функции для параллельных процессов)

        int cur_load[n_proc];
        for (int i = 0; i < n_proc; i++){
            cur_load[i] = 0;
        }

        for (int i = 0;i < n_prog; i++){
            cur_load[cur_x[i]] += prog_load[i];
        }

        int flag = 0;
        for (int i = 0;i < n_proc; i++){
            if (cur_load[i] >= proc_load[i]){
                flag = 1;
                break;
            }
        }

        if (flag){
            n_try++;
            continue;
        }

        //если дошли до сюда, то имеем корректное решение,
        //которое наилучшее => меняем наилучшие значение и вектор
        //а также обнуляем счетчик безрезультатных попыток

        best_F = cur_F;
        for (int i = 0; i < n_prog; i++){
            best_x[i+2] = cur_x[i];
        }

        if (best_F == 0){
            break;
        }

        global_try += n_try;
        n_try = 0;
    }

    //формируем итоговый вектор и возвращаем его в качестве рещультата
    global_try += n_try;

    delete []cur_x;
    best_x[0] = best_F;
    best_x[1] = global_try;
    return best_x;
}


//функция, выводящая резальтат работы алгоритма на экран
void
print_res(int *x, int len_x, long double nano_time, long double sec_time, char *in_file){

    cout << endl << "-------------------------------" << endl;

    if (x[2] == -1){
        cout << "failure: no correct solution has been found in " << x[1] << " iterations" << endl;
        cout << endl << "-------------------------------" << endl;
    }

    else{
        cout << endl << "success" << endl << endl << "the answer was found in " << x[1] << " iterations" << endl;

        cout << endl << "found solution: ";

        for (int i = 0; i < len_x; i++){
            cout << x[i+2] << ' ';
        }

        cout << endl << endl << "found answer: " << x[0] << endl;
    }

    //считаем время
    double global_time = (nano_time / 1000000) + (sec_time * 1000);

    //помимо основного вывода, будем вывыодить колво времени, затраченное на "основной" алгоритм
    cout << endl << "in " << global_time << " ms" << endl;

    //также будем записывать колво потоков, использовавшихся в программе
    //и время в файл, именованный спец. образом (см Readme)
    char file_name[10 + strlen(in_file)];
    memmove(file_name, "log_time_", 9);
    memmove(file_name + 9, in_file, strlen(in_file) + 1);
    int fd = open(file_name, O_WRONLY|O_APPEND|O_CREAT, 0644);

    char tmp;
    write(fd, &global_time, sizeof(double));

    write(fd, &parallel_level, sizeof(int));
    tmp = '\n';
    write(fd, &tmp, sizeof(char));

    close(fd);

    cout << endl << "-------------------------------" << endl;
}


using namespace std;
using namespace xercesc;


int main (int argc, char* argv[]) {

    //для работы нужен входной файл, подающий первым аргументом команды запуска
    if (argc < 2)
    {
        std::cout << "NO INPUT FILE" << std::endl;
        return 1;
    }

    //если подали какое-то число вторым аргументом, то оно будет колвом потоков
    if ((argc > 2) && atoi(argv[2]) && (atoi(argv[2]) > 0)){
        parallel_level = atoi(argv[2]);
    }

    //переменаая с кодом возврата
    int res = 0;

    //основные переменные программы:
    //колво процессоров
    int n_proc;
    //вектор макс. нагрузок на процессоры
    int *proc_load;
    //колво программ
    int n_prog;
    //вектор загрузки от программ
    int *prog_load;
    //"матрица", хранящая интенсивности обмена данными между программами
    //про устройство матрицы: см. Readme
    int **intensity;

    make_parsing(argv[1], res, n_proc, proc_load, n_prog, prog_load, intensity);

    if (res != 0){
        return res;
    }

    //подготавливаем переменные под замер времени работы алгоритма
    struct timespec start_time;
    struct timespec end_time;

    //создаем указатель под ответ
    int *x;

    //в зависимости от указанного числа потоков, вызываем
    //многопоточную или однопоточную функцию
    if (parallel_level == 1){
        clock_gettime(CLOCK_REALTIME, &start_time);
        x = find_best(n_proc, proc_load, n_prog, prog_load, intensity);
        clock_gettime(CLOCK_REALTIME, &end_time);
    }

    else{
        clock_gettime(CLOCK_REALTIME, &start_time);
        x = find_best_parallel(n_proc, proc_load, n_prog, prog_load, intensity);
        clock_gettime(CLOCK_REALTIME, &end_time);
    }

    //печатаем полученный результат
    print_res(x, n_prog, end_time.tv_nsec - start_time.tv_nsec, end_time.tv_sec - start_time.tv_sec, argv[1]);

    //чистим динамическую память и выходим
    delete []x;

    delete []proc_load;
    delete []prog_load;
    for (int i = 0; i < n_prog-1; i++){
        delete []intensity[i];
    }
    delete []intensity;
    return res;
}

