#include <stdlib.h>
#include <stdio.h>

struct list_bin {
    int n;
    struct list_bin *next;
    struct list_bin *prev;
};

typedef struct {
    struct list_bin *head;
    struct list_bin *tail;
} main_list;
       
void delete_list(main_list*);

int
add_to_list(int num, main_list *list_pointer)
{
    struct list_bin *list = list_pointer->tail;
    if (list == NULL){
        list = (struct list_bin*)malloc(sizeof(struct list_bin));
        if (list == NULL){
            return 1;
        }
        
        list->n = num;
        list->next = NULL;
        list->prev = NULL;
        list_pointer->tail = list;
        list_pointer->head = list;
        return 0;
    }
        
    list->next = (struct list_bin*)malloc(sizeof(struct list_bin));
    if (list->next == NULL){
        return 1;
    }
    
    list->next->n = num;
    list->next->next = NULL;
    list->next->prev = list;
    list_pointer->tail = list->next;
    return 0;
}


int
invert_file(FILE *f, main_list *list_to_read)
{
    int a;
    struct list_bin* p;
    if (fseek(f,0,SEEK_SET) != 0){
        exit(1);
    }
    while(fread(&a,sizeof(int),1,f) == 1){
        if (add_to_list(a, list_to_read)){
            delete_list(list_to_read);
            exit(1);
        }
    }
    if (list_to_read->tail != NULL){
        if (fseek(f,0,SEEK_SET) != 0){
            exit(1);
        }
        p = list_to_read->tail;
        while (p != NULL){
            if (fwrite(&(p->n),sizeof(int),1,f) != 1){
                fprintf(stderr, "file end");
                exit(1);
            }
            p = p->prev;
        }
    }
    return 0;
}

void
delete_list(main_list *list)
{
    struct list_bin *tmp = list->head;
    while (tmp != NULL){
        struct list_bin *tmp_tmp;
        tmp_tmp = tmp;
        tmp = tmp->next;
        free(tmp_tmp);
    }
    list->head=NULL;
    list->tail=NULL;
}        
        
int
main(void)
{
    FILE *f;
    main_list m_list = {NULL, NULL};
    
    if ((f=fopen("aaa","w+")) == NULL){
        exit(1);
    }
    for (int i = 1; i < 10; i++){
        if (fwrite(&i,sizeof(int),1,f) != 1){
            fprintf(stderr, "file end");
            exit(1);
        }
    }
    invert_file(f, &m_list);
    int a; 
    if (fseek(f, 0, SEEK_SET) != 0){
        exit(1);
    }
    while(fread(&a,sizeof(int),1,f)){
        printf("%d\n", a);
    }
    delete_list(&m_list);
    return 0;
}
