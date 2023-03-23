#include <stdio.h>
#include <stdlib.h>


//объявление структур
 
//заводим структура для хранения списка смежности каждой вершины     
struct list
{   
    //номер вершины истока
    long v_from;
    //номер концевой вершины
    long v_to;
    //вес ребра
    long w;
    //указатель на следующее ребро
    struct list* next;
};


//структура-очередь для организации очереди приоритетов алгоритма Дейкстры
struct queue{
    //указатель на первый элемент очереди
    struct list *head;
    //указатель на последний элемент очереди
    struct list *tail;
};


//стрктура для хранения графа
struct graph
{
    //кол-во вершин
    long l;
    //массив вершин(каждая вершина характеризуется ее списком смежности)
    struct list** main_arr;
    //служебный массив для перевода реального номера вершины
    //в ее индекс в основном массиве вершин
    long* vertexes;
};


//прототипы функций


//функция инициализации графа(выделяет в динамической памяти место под граф)
//на вход принимает кол-во вершин для нового графа
//возвращает указатель на созданную структуру struct graph
//если граф создать не удалось, возвращает NULL
struct graph* init_graph(long vertex_amount);

//функция деинициализации графа(полностью удаляет граф из динамической памяти)
//на вход принимает указатель на структуру struct graph
void deinit_graph(struct graph* g);

//функция добавления ребра в граф
//на вход принимает: 1) указатель на структуру struct graph
//                   2)номер исходящей вершины
//                   3)номер концевой вершины
//                   4)вес нового ребра
//возвращает успешность добавления(0-успешно; 1-не успешно)
int add_edge(struct graph* g, long ver_from, long ver_to, long weight);

//функция удаления ребра из графа
//на вход принимает: 1) указатель на структуру struct graph
//                   2) номер исходящей вершины
//                   3) номер концевой вершины
void delete_edge(struct graph* g, long n_from, long n_to);


//функция добавления вершины в граф
//на вход принимает: 1) указатель на структура struct graph
//                   2) номер новой вершины
//возвращает успещность добавления(0-успешно, 1-не успешно)
int add_vertex(struct graph *g, long v);

//функция удаления вершины
//на воход принимает: 1) указатель на структуру struct graph
//                    2) номер удаляемой вершины
void delete_vertex(struct graph *g, long v);

//служебная функция удаления структуры типа list из динамической памяти
void delete_list(struct list *list);

//функция осуществляющая вывод графа.
//На вход принимает указатель на структуру struct graph
void print_graph(struct graph* g);

//служебная функция перевода номера вершины в ее индекс в основном массиве
long v_to_n(struct graph* g, long v);

//функция, реализующая основное тело алгоритма Дейкстры
//на вход принимает: 1)указатель на структуру struct graph
//                   2)номер вершины, от которой считаются кратчайшие пути
//возвращает массив чисел с информацией о кратчайших расстояниях
//и информацией о последнем предке для каждой вершины на пути до нее
long* dijkstra_main(struct graph *g, long root_vertex);

//функция, выводящая результат выполнения алгоритма Дейкстры
//на вход принимает: 1)указатель на структуру struct graph
//                   2)указатель на начало массива с информацией о путях
//                   3)номер вершины от которой считаются пути
void print_dijkstra(struct graph *g, long* arr_out, long root_vertex);

//служебная функция удаления первого элемента из очереди
void pop_first(struct queue* l);

//служебная функция добавления в конец очереди элементов поданного списка 
int add_to_queue(struct queue *q, struct list *list);


//реализация функций


int
main(void)
{
    struct graph* graph = init_graph(6);
    //print_graph(graph);
    
    add_edge(graph, 0, 5, 14);
    add_edge(graph, 0, 1, 7);
    add_edge(graph, 0, 2, 9);
    add_edge(graph, 1, 0, 7);
    add_edge(graph, 1, 3, 15);
    add_edge(graph, 1, 2, 10);
    add_edge(graph, 2, 0, 9);
    add_edge(graph, 2, 1, 10);
    add_edge(graph, 2, 5, 2);
    add_edge(graph, 2, 3, 11);
    add_edge(graph, 3, 1, 15);
    add_edge(graph, 3, 2, 11);
    add_edge(graph, 3, 4, 6);
    add_edge(graph, 4, 3, 6);
    add_edge(graph, 4, 5, 9);
    add_edge(graph, 5, 4, 9);
    add_edge(graph, 5, 0, 14);
    add_edge(graph, 5, 2, 2);
    delete_edge(graph,5,2);
    delete_edge(graph,2,5);
    //delete_vertex(graph,4);
    //delete_vertex(graph,2);
    print_graph(graph);
    long *dist_arr = dijkstra_main(graph, 0);
    free(dist_arr);
    deinit_graph(graph);
    return 0;
}
     
     
     
struct graph*
init_graph(long vertex_amount)
{
    //выделяем в памяти место под саму struct graph
    struct graph* g = malloc(sizeof(struct graph));
    //в случае неудачи возвращаем NULL
    if (g == NULL){
       return NULL;
    }
    
    //ничинаем инициализировать граф
    
    //под поле main_arr выделяем в динамической памяти место
    //для массива вершин(элемент массива - список смежности вершины) 
    g->main_arr = malloc(vertex_amount * sizeof(struct list*));
    //в случае неудачи чистим уже выделенную память и возвращаем NULL
    if (g->main_arr == NULL){
        free(g);
        return NULL;
    } 
    
    //под поле vertexes выделяем в динамической памяти место для
    //служебного массива, синхронизирующего индекс вершины и ее реальный номер
    g->vertexes = malloc(vertex_amount * sizeof(long));
    //в случае неудачи чистим уже выделенную память и возвращаем NULL
    if (g->vertexes == NULL){
        free(g->main_arr);
        free(g);
        return NULL;
    }
    
    //после успешного выделения памяти забиваем массивы дефолтными значениями
    for (long i = 0; i < vertex_amount; i++){
            g->main_arr[i] = NULL;
            g->vertexes[i] = i;
    }
    
    //в поле l кладем поданное кол-во вершин
    g->l = vertex_amount; 
    
    return g;
}



long
v_to_n(struct graph* g, long v)
{
    //бежим по служебному массиву с номерами вершин
    //пока не дойдем до вершины с поданным номером
    for(long i = 0; i < g->l; i++){
        if (g->vertexes[i] == v){
            //возвращаем индекс найденной вершины
            return i;
        }
    }
    //если вершины с поданным номером не найдена возвращаем -1
    return -1;
}


     
int
add_edge(struct graph* g, long ver_from, long ver_to, long weight)
{
    //переводим поданны номера вершин в их индексы
    long n_from = v_to_n(g, ver_from);
    //long n_to = v_to_n(g, ver_to);
    
    //выделяем место под новое ребро
    struct list* new = malloc(sizeof(struct list));    
    //если выделить память не получилось, ругаемся
    if (new == NULL){
        printf("run out of memory");
        return 1;
    }
    
    //заполняем новое ребро
    new->v_from = ver_from;
    new->v_to = ver_to;
    new->w = weight;
    
    //добавляем ребро в список смежности вершины истока
    
    struct list *place = g->main_arr[n_from];
    //если список смежности пуст, значит созданный элемент будет первым
    if (place == NULL){
        new->next = NULL;
        g->main_arr[n_from] = new;
        return 0;
    }
    struct list *prev = place;
    //поддерживаем список смежности упорядоченным по увеличению веса ребер
    //ищем место для вставки нового ребра в список
    while ((place != NULL) && ((place->w) < weight)){
        prev = place;
        place = place -> next;
    }
    //если мы должны вставить ребро в самое начало, прийдется
    //поменять указатель на список смежности
    if (place == prev){
        new -> next = g->main_arr[n_from];
        g->main_arr[n_from] = new;
        return 0;
    }
    //иначе просто вставляем новое ребро в список
    new->next = place;
    prev->next = new;
    return 0;
}

void
delete_edge(struct graph* g, long ver_from, long ver_to)
{
    //переводим поданныем номера вершин в их индексы
    long n_from = v_to_n(g, ver_from);
    //long n_to = v_to_n(g, ver_to);
    
    //удаляем ребро из списка смежности
    struct list* old = g->main_arr[n_from];
    struct list* cur = old;
    
    //ищем нужное ребро
    while ((old -> v_to) != ver_to){
        cur = old;
        old = old->next;
    }
    
    //если удаляем первый элемент списка, прийдется
    //поменять указатель на список смежности
    if (old == cur){
        cur = old->next;
        free(g->main_arr[n_from]);
        g->main_arr[n_from] = cur;
        return;
    }
    
    //иначе просто удаляем элемент
    cur->next = old->next;
    free(old);
    return;
}

int
add_vertex(struct graph *g, long v)
{   
    struct list **tmp_one;
    
    //увеличиваем место под основной массив
    tmp_one = realloc(g->main_arr, ((g->l)+1)*(sizeof(struct list*)));
    //в случае неудачи возвращаем 1
    if (tmp_one == NULL){
        return 1;
    }
    
    long *tmp_two;
    
    //увеличиваем местро под вспомогательный массив
    tmp_two = realloc(g->vertexes, ((g->l)+1)*(sizeof(long)));
    //в случае неудачи возвращаем 1
    if (tmp_two == NULL){
        return 1;
    }
    //если место успешно выделено, инициализируем новую вершину новыми
    //значениями и записываем ее в основной массив
    g->main_arr = tmp_one;
    g->vertexes = tmp_two;
    g->main_arr[g->l] = NULL;
    g->vertexes[g->l] = v;
    g->l += 1;
    return 0;
}

void
delete_vertex(struct graph *g, long v)
{
    //удаляем список смежности поданной вершины
    delete_list(g->main_arr[v_to_n(g,v)]);
    
    //свдигаем основной и вспомогательный массивы на 1 влево
    //начиная с индекса поданной вершины
    for (long i = v_to_n(g,v); i < (g->l)-1; i++){
        (g->main_arr[i]) = (g->main_arr[i+1]);
        g->vertexes[i] = g->vertexes[i+1];
    }
    
    //теперь последний элемент массива пустой(или в нем удаляемая вершина)
    //уменьшаем размеры основных массивов нв 1 элемент
    g->vertexes = realloc(g->vertexes, ((g->l)-1)*(sizeof(long)));
    g->main_arr = realloc(g->main_arr, ((g->l)-1)*(sizeof(struct list *)));
    g->l -= 1;
    
    //удаляем из всех списков смежности ребра,
    //инциндентные с удаляемой вершиной
    for(long i=0; i<(g->l); i++){
        struct list *tmp = g->main_arr[i];
        struct list *prev = tmp;
        //ищем соответсвующее ребро
        while ((tmp != NULL) && (tmp->v_to != v)){
            prev = tmp;
            tmp = tmp->next;
        }
        //если нашли, то удаляем его
        if (tmp != NULL){
            //если удаляемое ребро первое в списке, прийдется поменять
            //указатель на начало списка
            if (prev == tmp){
                g->main_arr[i] = g->main_arr[i]->next;
            //иначе просто перевешеваем соответсвующий указатель next
            } else{
                prev->next=tmp->next;
            }
            //чистим память
            free(tmp);
        }
    }
    return;    
}
     
void
print_graph(struct graph* g)
{
    for (long i = 0; i < (g->l); i++){
        struct list* tmp = g->main_arr[i];
        //выводим сообщение о том, соседей какой вершины сейчас выведем
        printf("neighbours of vertex %ld\n ", g->vertexes[i]);
        while (tmp != NULL){
            //каждого соседа выводим в формате v:n w:m
            //где n - номер вершины-соседа
            //    m - длина ребра в него
            printf("v: %ld w: %ld ", tmp->v_to, tmp->w);
            tmp = tmp->next;
        }
        printf("\n");
    }
    return;
}

void
deinit_graph(struct graph* g)
{
    //чистим списки смежности
    for(long i = 0; i < (g->l); i++){
        delete_list(g->main_arr[i]);
    }
    
    //чистим основной и вспомогательный массивы
    free(g->main_arr);
    free(g->vertexes);
    
    //чистим сам граф
    free(g);
}

void 
delete_list(struct list *list)
{
    while (list != NULL){
        struct list *tmp = list -> next;
        free(list);
        list = tmp;
    }
}


int
add_to_queue(struct queue *q, struct list *list)
{
    struct list *l = list;
    //если нам подали не пустой список
    if (l != NULL){
        //если очередь пуста, добавляем в нее первый элемент
        if ((q -> head == NULL) && (q -> tail == NULL)){
        
            //создаем новый элемент
            q->tail = malloc(sizeof(struct list));
            //если не получилось, возвращаем 1
            if (q->tail == NULL){
                return 1;
            }
            //иначе инициализируем новый элемент соответствующими значениями
            q->tail->v_to = l->v_to;
            q->tail->v_from = l->v_from;
            q->tail->w = l->w;
            q->tail->next = l->next;
            q->head = q->tail;
            //делаем шаг по списку
            l = l->next;
        }
        //аналогично добавляем элементы в уже непустую очередь
        //до тех пор пока поданный список не закончится
        while (l != NULL){
            struct list *tmp = malloc(sizeof(struct list));
            if (tmp == NULL){
                return 1;
            }
            tmp->v_to = l->v_to;
            tmp->v_from = l->v_from;
            tmp->w = l->w;
            tmp->next = l->next;
            q->tail->next = tmp;
            q->tail = tmp;
            l = l->next;
        }
    }
    return 0;
}
    
//    

void
pop_first(struct queue* q)
{   
    struct list *tmp;
    tmp = q->head;
    q->head = q->head->next;
    free(tmp);
}


long*
dijkstra_main(struct graph *g, long root_vertex)
{
    long num = g->l;
    //заводим основной массив(в нем должны быть расстояния и последние
    //предки на кратчайшем пути к каждой вершине,а также признак конца массива
    long *dist_arr = malloc((2*num + 1)*sizeof(long));
    
    //если памяти не хватило, возвращаем NULL
    if (dist_arr == NULL){
        return NULL;
    }
    
    //инициализируем массив дефолтными значениями: 
    //все ячейки - нулевые, последняя(признак конца) -1
    for (long i = 0; i < num; i++){
        dist_arr[i] = 0;
        dist_arr[i+num] = 0;
    }
    dist_arr[2*num] = -1;
    
    //выделяем место под очередь приоритетов
    struct queue *priority = malloc(sizeof(struct queue));
    //если памяти не хватило, чистим уже заведенный массив и возвращаем NULL
    if (priority == NULL){
        free(dist_arr);
        return NULL;
    }
    
    //инициализируем очередь нулевыми указателями
    priority->head = NULL;
    priority->tail = NULL;
    
    //добавялем в очередь список смежности вершины-корня
    //если памяти не хватило, чистим все ранее заведенные объекты
    //в динамической памяти и возвращаем NULL
    if (add_to_queue(priority, g->main_arr[v_to_n(g, root_vertex)]) == 1){
        free(dist_arr);
        while (priority != NULL){
            pop_first(priority);
        }
        free(priority);
        return NULL;
    }
    
    //алгоритм работает до тех пор, пока очередь не пуста
    while (priority->head != NULL){
    
        //заводим переменные, хранящие информацию из первого элемента очереди
        long cur_from = v_to_n(g, priority->head->v_from);
        long cur_to = v_to_n(g, priority->head->v_to);
        long cur_l = priority->head->w;
        
        //если кратчайшее найденное ранее расстояние до концевой вершины
        //рассматриваемого ребра больше длины текущего ребра в сумме
        //с кратчайшим найденным ранее расстоянием до начальной вершины
        //обновляем массив расстояний и предков,
        // и добавляем новые ребра в очередь
        if ((dist_arr[cur_to] == 0)||((cur_l + dist_arr[cur_from]) < dist_arr[cur_to])){
        
            dist_arr[cur_to] = cur_l + dist_arr[cur_from];
            dist_arr[cur_to + num] = cur_from;
            //добавялем в очередь список смежности концевой вершины
            //если памяти не хватило, чистим все ранее заведенные объекты
            //в динамической памяти и возвращаем NULL
            if (add_to_queue(priority, g->main_arr[cur_to]) == 1){
                free(dist_arr);
                while (priority != NULL){
                    pop_first(priority);
                }
                free(priority);
                return NULL;
            }
        }
        //двигаемся по очереди
        pop_first(priority);
    }
    //когда основной цикл завершен, выводим результат
    print_dijkstra(g, dist_arr, root_vertex);
    //в конце алгоритма очередь уже пуста, осталось почистить место
    //выделенное под нее саму
    free(priority);
    //возвращаем массив с выводом алгоритма дейкстры.
    //дальнейший контроль области памяти , выделенной под массив
    //возлагается на пользователя библиотеки
    return dist_arr;
}        

void
print_dijkstra(struct graph *g, long* arr_out, long root_vertex)
{
    for(long i = 0; i < g->l; i++){
    
        //выводим расстояние до текущей рассматриваемой вершины
        //если в вершину попасть нельзя, будет выведен ноль
        printf("distance from your root to vertex %ld equals %ld\n", g->vertexes[i], arr_out[i]);
        
        //если в вершину можно попасть, выводим путь до нее в формате
        //current_vertex <- n1 <- n2 ... <- nk <- root
        //где n1, .., nk - вершины кратчайшего пути
        if (arr_out[i] != 0){
            printf("shortest way, presented as list of vertexes: %ld <- ", g->vertexes[i]);
            long j = i;
            while(arr_out[j + g->l] != v_to_n(g, root_vertex)){
                printf("%ld <- ", g->vertexes[arr_out[j + g->l]]);
                j = arr_out[j + g->l];
            }
            printf("%ld\n", root_vertex);
        }
    }
}
        
             
            
