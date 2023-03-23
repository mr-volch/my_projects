#include "making_tree_all.h"

int
main(int agc, char **argv)
{

    //получаем строку
    char *main_ll;    
    main_ll = get_string();
    
    //если ввели пустую строку
    if (main_ll == NULL){
        return 1;
    }
    
    //memmove(main_ll, "(sleep 5 ; ls) ; sleep 5 ; who;\0", (strlen("(sleep 5 ; ls) & sleep 5 ; who;\0") + 1));
    
    //memmove(main_ll, "pwd | (ls|cat) > fffff | cat < fffff\0", (strlen("pwd | (ls|cat) > fffff | cat < fffff\0") + 1));
    //запуск парсинга
    struct comand_info* main_graph = parse_input(main_ll);
    free(main_ll);
    
    //если надо посмотреть граф:
    //print_comand_info(main_graph, 0);
    
    //инициализируем exit_code
    
    int exit_code = 1;
    
    //если нет синтаксич. ошибок, запуск выполнения
    if (main_graph != NULL){
        exit_code = main_proc(main_graph);
    }
    
    free_comand_info(main_graph);
    return exit_code;
}     
