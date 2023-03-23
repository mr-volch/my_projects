#include "lexems1.h"


char*
get_string(void)
{
    char *ret_str = NULL;
    int ret_len = 0;
    char c;
    
    //пока не дошли до ЕОФ, читаем символы и записываем их в строку    
    while ((c = getchar()) != EOF){
        ret_len++;
        ret_str = realloc(ret_str, ret_len * sizeof(char));
        ret_str[ret_len - 1] = c;
    }
    
    if (ret_str == NULL){
        return NULL;
    }
    //помещаем \0 в конец строки и возвращаем указатель
    // на сформированную строку
    ret_len++;
    ret_str = realloc(ret_str, ret_len * sizeof(char));
    ret_str[ret_len - 1] = '\0';
    
    return ret_str;
}
    
    

int
is_divider(char in_char)
{
    //все возможные разделители: служебные символы и пробельные
    if ((in_char == ';') || (in_char == '(') || (in_char == ')') || (in_char == '|') || (in_char == '&') || (in_char == '>') || (in_char == '<') || (isspace(in_char))){
        return 1;
    }
    return 0;
}


struct token
get_next_token(char *input_str, int *input_ind)
{
    //по умолчанию: токен конца
    struct token tmp;
    tmp.typ = 'e';
    tmp.lex = NULL;
    
    //пропуск пробельных символов
    while (((*input_ind) < strlen(input_str)) && (isspace(input_str[*input_ind]))){
        *input_ind = *input_ind + 1;
    }
    
    //если дошли до конца, то возвращаем токен конца
    if ((*input_ind) >= strlen(input_str)){
        return tmp;
    }
    
    //иначе анализируем токен
    
    if (input_str[*input_ind] == '|'){
        tmp.typ = '|';
    }
    
    else if (input_str[*input_ind] == '&'){
        tmp.typ = '&';
    }
    
    else if (input_str[*input_ind] == ';'){
        tmp.typ = ';';
    }
    
    else if (input_str[*input_ind] == '<'){
        tmp.typ = '<';
    }
    
    else if (input_str[*input_ind] == '>'){
        tmp.typ = '>';
        if (((*input_ind + 1) < strlen(input_str)) && ((input_str[*input_ind + 1] == '>'))){
            *input_ind = *input_ind + 1;
            //тип для >>
            tmp.typ = 'd';
        }
    }
    
    else if (input_str[*input_ind] == '('){
        tmp.typ = '(';
    }
    
    else if (input_str[*input_ind] == ')'){
        tmp.typ = ')';
    }
    
    //если столкнулись с символьным именем, то также
    //заполням поле lex, в котором и будет сама по себе
    //строка из символов
    else{
        tmp.typ = 'a';
        int cur_size = 0;
        while (((*input_ind) < strlen(input_str)) && (is_divider(input_str[*input_ind]) == 0)){
            cur_size++;
            tmp.lex = realloc(tmp.lex, cur_size * sizeof(char));
            tmp.lex[cur_size-1] = input_str[*input_ind];
            *input_ind = *input_ind + 1;
        }
        
        *input_ind = *input_ind - 1;
        cur_size++;
        tmp.lex = realloc(tmp.lex, cur_size * sizeof(char));
        tmp.lex[cur_size-1] = '\0';
    }
    
    //увеличиваем индекс на 1    
    *input_ind = *input_ind + 1;
    
    //возвращаем сформированный токен
    return tmp;
}


void
init_comand_info(struct comand_info* in_s)
{
    //забиваем все поля структуры
    //нулевыми занчениями
    in_s->argv = NULL;
    in_s->arg_amount = 0;
    in_s->infile = NULL;
    in_s->outfile = NULL;
    in_s->if_append = 0;
    in_s->backgrnd = 0;
    in_s->is_error = 0;
    in_s->psubcmd = NULL;
    in_s->pipe = NULL;
    in_s->next = NULL;
}         


struct comand_info*
parse_input(char *in_str)
{
    //получаем первый токен
    struct token cur;
    int main_ind = 0;
    cur = get_next_token(in_str, &main_ind);
    
    //если ввели строку из пробельных символов
    if (cur.typ == 'e'){
        fprintf(stderr, "ERROR: input string is empty");
        return NULL;
    }
    //создаем первую вершину - структуру(пустую)
    //и запускаем рекурсивный спуск
    struct comand_info* main_struct = malloc(sizeof(struct comand_info));
    init_comand_info(main_struct);
    
    return(shell_comand(main_struct, cur, in_str, &main_ind));

} 
                        
                        
struct comand_info*
shell_comand(struct comand_info* in_s, struct token cur, char *main_str, int *main_ind)
{
    //запускаем следующую функцию
    comand_list(in_s, cur, main_str, main_ind);
    
    //если вернулись с ошибкой, то чистим память и возвращаем NULL
    if (in_s->is_error == 1){
        free_comand_info(in_s);
        return NULL;
    }
    
    //если часть команды корректна, но за ней стоит
    //синтаксически неправильная конструкция
    //(рек. спуск закончился, а строка - нет)
    cur = get_next_token(main_str, main_ind);
    
    if (cur.typ != 'e'){
        //printf("%c\n", cur.typ);
        fprintf(stderr, "\nError: string can not be a shall comand\n");
        free_comand_info(in_s);
        return NULL;
    }
    
    return in_s;
}

struct comand_info*
comand_list(struct comand_info* in_s, struct token cur, char *main_str, int *main_ind)
{
    //запуск след. функции
    pipe_comand(in_s, cur, main_str, main_ind);
    
    //если вернулись с ошибкой, заканчиваем
    if (in_s->is_error == 1){
        return in_s;
    }
    
    //берем следующий токен
    cur = get_next_token(main_str, main_ind);
    
    //если токен окажется & или ;, запустим рекурсивно
    //формирование создание структуры для поля next
    //(предварительно проинициализировав ее
        
    if (cur.typ == '&'){
        //если & то также возводим флаг фона
        in_s->backgrnd = 1;
        struct comand_info *new_one  = malloc(sizeof(struct comand_info));
        init_comand_info(new_one);
        cur = get_next_token(main_str, main_ind);
        in_s->next = comand_list(new_one, cur, main_str, main_ind);
        
        if (in_s->next->is_error == 1){
            in_s->is_error = 1;
            return in_s;
        }
    } 
    
    else if (cur.typ == ';'){
        struct comand_info *new_two  = malloc(sizeof(struct comand_info));
        init_comand_info(new_two);
        
        cur = get_next_token(main_str, main_ind);
        in_s->next = comand_list(new_two, cur, main_str, main_ind);
        
        if (in_s->next->is_error == 1){
            in_s->is_error = 1;
            return in_s;
        }
        
    }
        
    //если мы проанализировали токен, а он оказался "ненужным"
    //то возвращаемся на одну позициб назад
    else if (cur.typ != 'e'){
        *main_ind = *main_ind - 1;
        free(cur.lex);
    }
    
    return in_s;
}


struct comand_info*
pipe_comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind)
{
    //вызов след. функции
    comand(in_s, cur, main_str, main_ind);
    
    if (in_s->is_error == 1){
       return in_s;
    }
    
    //взятие следующего токена и анализ
    cur = get_next_token(main_str, main_ind);
    
    //в этих случаях, пропущена операция
    if ((cur.typ != '|') && (cur.typ != ';') && (cur.typ != '&') && (cur.typ != ')') && (cur.typ != 'e')){
        //printf("%c\n", cur.typ);
        fprintf(stderr, "\nError: missing operation\n");
        in_s->is_error = 1;
        return in_s;
    }
    
    //порождаем структуру для поля pipe и рекурсивно вызываем
    //заполнение это структуры
    if (cur.typ == '|'){
        struct comand_info *new_one  = malloc(sizeof(struct comand_info));
        init_comand_info(new_one);
        cur = get_next_token(main_str, main_ind);
        in_s->pipe = pipe_comand(new_one, cur, main_str, main_ind);
        
        if (in_s->pipe->is_error == 1){
            in_s->is_error = 1;    
            return in_s;
        }
        
        //если после пайпа ничего не оказалось, ошибка
        if (in_s->pipe->arg_amount == -1){
            fprintf(stderr, "\nError: missing comand after pipe\n");
            in_s->pipe->is_error = 1;
            in_s->is_error = 1;
            return in_s;
        }
    }
    
    //в случае "бесполезного" анализа, откат по строке
    else if (cur.typ != 'e'){
        *main_ind = *main_ind - 1;
        free(cur.lex);
    }
    
    return in_s;
}
            
struct comand_info *
comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind)
{
    //определяем: является ли команда командой в скобках либо же простой
    if (cur.typ == 'a'){
        //если простая
        simple_comand(in_s, cur, main_str, main_ind);
          
        if (in_s->is_error == 1){  
           return in_s;
        }
    }
    
    //если скобки, то рекурсивно заполняем поле psubcmd и 
    //анализируем перенаправления ввода-вывода
    else if (cur.typ == '('){
        struct comand_info *new_one  = malloc(sizeof(struct comand_info));
        init_comand_info(new_one);
        cur = get_next_token(main_str, main_ind);
        in_s->psubcmd = comand_list(new_one, cur, main_str, main_ind);
        if (in_s->psubcmd->is_error == 1){
            in_s->is_error = 1;
            return in_s;
        }
        
        cur = get_next_token(main_str, main_ind);
        
        if (cur.typ != ')'){
            //printf("%c\n", cur.typ);
            //print_comand_info(in_s, 0);
            fprintf(stderr, "\nBalance of brackets is violated\n");
            in_s->is_error = 1;
            return in_s;
        }
        
        cur = get_next_token(main_str, main_ind);
        
        //анализ ввода
        if (cur.typ == '<'){
            cur = get_next_token(main_str, main_ind);
            if (cur.typ == 'a'){
                in_s->infile = malloc((strlen(cur.lex) + 1) * sizeof(char));
                memmove(in_s->infile, cur.lex, (strlen(cur.lex) + 1));
                free(cur.lex);
                cur = get_next_token(main_str, main_ind);
            }
            
            else{
                fprintf(stderr,"\nError: no filename after redirection of standart input\n");
                in_s->is_error = 1;
                return in_s;
            }
        }
        
        //анализ вывода
        if ((cur.typ == '>') || (cur.typ == 'd')){
            int flag = 0;
            if (cur.typ == 'd'){
                flag = 1;
            }
            
            cur = get_next_token(main_str, main_ind);
            if (cur.typ == 'a'){
                in_s->outfile = malloc((strlen(cur.lex) + 1) * sizeof(char));
                memmove(in_s->outfile, cur.lex, (strlen(cur.lex) + 1));
                free(cur.lex);
                in_s->if_append = flag;
                cur = get_next_token(main_str, main_ind);
            }
            
            else{
                fprintf(stderr,"\nError: no filename after redirection of standart output\n");
                in_s->is_error = 1;
                return in_s;
            }
        }
        
        //если не дошли до конца, то возвращаемся на 1 назад
        if (cur.typ != 'e'){
            *main_ind = *main_ind - 1;
        }
        
    }
    
    //случай "пустой" команды:
    //было принято решение, в случае если
    //список команд оканчивается & или ;
    //добавлять в конец структуру с уникальным признаком:
    //-1 в поле arg_amount
    else if ((cur.typ == ')') || (cur.typ == 'e')){
        in_s->arg_amount = -1;
        if (cur.typ != 'e'){
            *main_ind = *main_ind - 1;
        }
        return in_s;
    }
    
    else{
        *main_ind = *main_ind - 1;
        fprintf(stderr, "\nERROR: incorrect comand\n");
        in_s->is_error = 1;
        return in_s;
    }
    
    return in_s;        
}



struct comand_info*
simple_comand(struct comand_info *in_s, struct token cur, char *main_str, int *main_ind)
{
    int i = 0;
    
    //читаем argv
    if (cur.typ == 'a'){
        i++;
        in_s->argv = malloc(sizeof(char*));
        in_s->arg_amount = i;
        in_s->argv[i-1] = malloc((strlen(cur.lex) + 1) * sizeof(char));
        memmove(in_s->argv[i-1], cur.lex, (strlen(cur.lex) + 1));
        free(cur.lex);
        cur = get_next_token(main_str, main_ind);
    }
    
        
    while (cur.typ == 'a'){
        i++;
        in_s->argv = realloc(in_s->argv, i*sizeof(char *));
        in_s->arg_amount = i;
        in_s->argv[i-1] = malloc((strlen(cur.lex) + 1) * sizeof(char));
        memmove(in_s->argv[i-1], cur.lex, (strlen(cur.lex) + 1));
        free(cur.lex);
        cur = get_next_token(main_str, main_ind);
    }
    
    //кладем NULL в конец массива аргументов
    i++;
    in_s->argv = realloc(in_s->argv, i*sizeof(char *));
    in_s->arg_amount = i;
    in_s->argv[i-1] = NULL;
    
    
    //анализируем ввод
     
    if (cur.typ == '<'){
        //need_back = 0;
        cur = get_next_token(main_str, main_ind);
        if (cur.typ == 'a'){
            in_s->infile = malloc((strlen(cur.lex) + 1) * sizeof(char));
            memmove(in_s->infile, cur.lex, (strlen(cur.lex) + 1));
            free(cur.lex);
            cur = get_next_token(main_str, main_ind);
        }
        
        else{
            fprintf(stderr,"\nError: no filename after redirection of standart input\n");
            in_s->is_error = 1;
            return in_s;
        }
    }
      
    //анализируем вывод
    if ((cur.typ == '>') || (cur.typ == 'd')){
        //need_back = 0;
        int flag = 0;
        if (cur.typ == 'd'){
            flag = 1;
        }
        
        cur = get_next_token(main_str, main_ind);
        if (cur.typ == 'a'){
            in_s->outfile = malloc((strlen(cur.lex) + 1) * sizeof(char));
            memmove(in_s->outfile, cur.lex, (strlen(cur.lex) + 1));
            free(cur.lex);
            in_s->if_append = flag;
            cur = get_next_token(main_str, main_ind);
        }       
        
        else{
            fprintf(stderr,"\nError: no filename after redirection of standart output\n");
            in_s->is_error = 1;
            return in_s;
        }
    }
    
    //если не уперлись в конец, то откатываемся по строку
    if (cur.typ != 'e'){
        *main_ind = *main_ind - 1;
    }
    
    return in_s;
}


void
print_comand_info(struct comand_info *in_s, int k)
{

    if (in_s == NULL){
        return;
    }
    
    printf("comand %d\n", k);
    printf("\nargs\n");
    for (int i = 0; i < in_s -> arg_amount; i++){
        printf("%s ", in_s->argv[i]);
    }
    
    printf("\nargamount\n");
    printf("%d ", in_s->arg_amount);
    
    printf("\ninfile\n");
    if ((in_s->infile) == NULL){
        printf("0");
    } else{
        printf("%s ", in_s->infile);
    }

    printf("\noutfile\n");
    if ((in_s->outfile) == NULL){
        printf("0");
    } else{
        printf("%s ", in_s->outfile);
    }
    
    printf("\nif_append\n");
    printf("%d ", in_s->if_append);
    
    printf("\nbackgrnd\n");
    printf("%d ", in_s->backgrnd);
    
    printf("\ncomand psubcmd%d\n", k);
    print_comand_info(in_s->psubcmd, k+1);
    printf("\ncomand pipe%d\n", k);
    print_comand_info(in_s->pipe, k+1);
    printf("\ncomand next%d\n", k);
    print_comand_info(in_s->next, k+1);
}


void
free_comand_info(struct comand_info *in_s)
{
    if (in_s == NULL){
        return;
    }
    
    for (int i = 0; i < in_s -> arg_amount; i++){
        free(in_s->argv[i]);
    }
    free(in_s->argv);
    free(in_s->infile);
    free(in_s->outfile);
    free_comand_info(in_s->psubcmd);
    free_comand_info(in_s->pipe);
    free_comand_info(in_s->next);
    free(in_s);
}

        

