#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>


struct comand_info{
    char **argv; //имя команды и аргументы
    int arg_amount; //длина argv
    char *infile; //имя входного файла
    char *outfile; //имя выходного файла
    int if_append; //флаг дозаписи в конец
    int backgrnd; //флаг фона
    int is_error; //флаг синтаксической ошибки
    struct comand_info *psubcmd; //указатель на первую команду в скобках
    struct comand_info *pipe; //указатель на след. команду в конвейере
    struct comand_info *next; //указатель на следующую команду (после ; или &)
};    
            
             
struct token
{
    char typ; //тип токена
    char *lex; //актуально только для токенв типа 'a': сама п себе лексемма    
};

//функция получения строки из stdin
char* get_string(void);
//служебная функции проверки символа на разделитель
int is_divider(char in_char);
//функция получения следующего токена в строке
struct token get_next_token(char *input_str, int *input_ind);
//иницализация структуры
void init_comand_info(struct comand_info* in_s);
//функция для пользователя: начать парсинг строки
struct comand_info* parse_input(char *in_str);

//5 функция рекурсивного спуска
//на каждом этапе возвращается сформированное
//к текущему моенту дерево
struct comand_info* shell_comand(struct comand_info* in_s, struct token cur, char *main_str, int *main_ind);
struct comand_info* comand_list(struct comand_info* in_s, struct token cur, char *main_str, int *main_ind);
struct comand_info* pipe_comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind);
struct comand_info* comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind);
struct comand_info* simple_comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind);

//служебные функции печати и освобождения дин. памяти
//печать использовалась при отладке
void print_comand_info(struct comand_info *in_s, int k);
void free_comand_info(struct comand_info *in_s);
