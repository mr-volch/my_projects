#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree { char c;
              int n;
              struct tree* left;
              struct tree* right;
              struct tree* parent;
};

struct tree* add_to_tree(struct tree*, struct tree*, char);

struct tree*
add_to_tree(struct tree* cur, struct tree* prev, char char_in)
{
    if (cur == NULL){
        cur = (struct tree*)malloc(sizeof(struct tree));
        if (cur == NULL){
            return NULL;
        }
        cur->c = char_in;
        cur->n = 1;
        cur->left = NULL;
        cur->right = NULL;
        cur->parent = prev;
        return cur;
    } else{
        if (char_in > cur->c){
            struct  tree *tmp;
            tmp = add_to_tree(cur->right, cur, char_in);
            if (tmp == NULL){
                return NULL;
            }
	    cur->right = tmp;
        } else if (char_in < cur->c){
            struct tree *tmp;
            tmp = add_to_tree(cur->left,cur,char_in);
            if (tmp == NULL){
                return NULL;
            cur->left = tmp;
            }
        } else{
            (cur->n)++;
            return cur;
        }
    }
    return cur;
}


void
delete_whole_tree(struct tree* cur)
{
    struct tree* p = cur;
    struct tree* q;
    while (p!=NULL){
        while ((p->left != NULL) || (p->right != NULL)){
            if (p->left != NULL){
                p = (p->left);
            } else{
                p = (p->right);
            }
        }
        if (p->parent != NULL){
            q = p->parent;
        }
        free(p);
        //whyyy not?
        p=q;
    }
    return;
}

void
check_print(struct tree* cur)
{
    if (cur != NULL){
        printf("%c ", cur->c);
        printf("%d\n", cur->n);
        check_print(cur->left);
        check_print(cur->right);
        }
    return;
}

int
main(void)
{
    char str_test[102];
    int len_test;
    struct tree* main_tree = NULL;
    struct tree* tmp;
    if (fgets(str_test, 102, stdin) == 0){
        return(1);
    }
    len_test = strlen(str_test) - 1;
    str_test[len_test] = '\0';
    //printf("%s\n", "here");
    for(int i = 0; i < len_test; i++){
        //printf("%d\n", i);
        tmp = add_to_tree(main_tree, NULL,  str_test[i]);
        if (tmp == NULL){
            delete_whole_tree(main_tree);
            fprintf(stderr, "dynamic memory has run out");
            return 1;
        }
        main_tree = tmp;
    }
    check_print(main_tree);
    printf("%s\n", "fuck");
    delete_whole_tree(main_tree);
    printf("%s\n", "fuck");
    check_print(main_tree);
    return 0;
}
