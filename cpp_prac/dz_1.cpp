#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;

class order{
    char *firm;
    char *tele;
    static int amount;
    int num;

    public:
    order(){
        firm = NULL;
        tele = NULL;
        num = 0;
    }

    order(char *firm_in, char *tele_in){
        firm = NULL;
        if (firm_in != NULL){
            firm = new char[strlen(firm_in) + 1];
            memmove(firm, firm_in, strlen(firm_in) + 1);
        }

        tele = NULL;
        if (tele_in != NULL){
            tele = new char[strlen(tele_in) + 1];
            memmove(tele, tele_in, strlen(tele_in) + 1);
        }

        amount++;
        num = amount;
    }

    order(const order &ord){
        num = ord.num;
        firm = new char[strlen(ord.firm) + 1];
        memmove(firm, ord.firm, strlen(ord.firm) + 1);
        tele = new char[strlen(ord.tele) + 1];
        memmove(tele, ord.tele, strlen(ord.tele) + 1);
   }


    void
    check_print(){
        cout << "firm: " << firm << '\n';
        cout << "tele: " << tele << '\n';
        cout << "amount: " << amount << '\n';
        cout << "num: " << num << '\n';
    }

    order
    operator=(const order &ord){

        if (&ord == this){
            return *this;
        }

        delete []firm;
        delete []tele;

        if (ord.firm == NULL){
            firm = NULL;
        }

        if (ord.tele == NULL){
            tele = NULL;
        }

        firm = new char[strlen(ord.firm) + 1];
        memmove(firm, ord.firm, strlen(ord.firm) + 1);
        tele = new char[strlen(ord.tele) + 1];
        memmove(tele, ord.tele, strlen(ord.tele) + 1);

        num = ord.num;
        return *this;
    }

    ~order(){
        delete []firm;
        delete []tele;
    }
};


int order :: amount = 0;


struct Node{
    order cur;
    Node *next;
    Node *prev;
};


class List{
    Node *head;
    Node *tail;

    public:

    List(){
        head = NULL;
        tail = NULL;
    }

    List(order &ord){
        Node* N = new Node;
        N->cur = ord;
        N->next = NULL;
        N->prev = NULL;
        head = N;
        tail = N;
    }

    void
    push_front(order &ord){

        Node *N = new Node;
        N->cur = ord;
        N->next = head;
        N->prev = NULL;

        if ((tail == head) || (tail == NULL)){
            tail = N;
        }

        head = N;
        return;
    }

    void
    push_back(order &ord){

        Node *N = new Node;
        N->cur = ord;
        N->next = head;
        N->prev = NULL;

        if ((tail == head) || (tail == NULL)){
            tail = N;
        }

        head = N;
        return;
    }


    order
    front(){
        return head->cur;
    }

    order
    back(){
        return tail->cur;
    }

    void
    pop_front(){
        if (head == NULL){
            cout << "list is empty, cant delete first element" << endl;
            return;
        }

        Node *temp = head->next;
        temp->prev = NULL;
        delete head;
        head = temp;
        return;
    }

    void
    pop_back(){
        if (tail == NULL){
            cout << "list is empty, cant delete first element" << endl;
            return;
        }

        cout << "ok" <<endl;
        Node *temp = tail->prev;
        temp->next = NULL;
        cout << "ok" <<endl;
        delete tail;
        cout << "ok" <<endl;
        tail = temp;
        return;
    }

    void
    print(){
        if (head == NULL){
            cout << "list is empty" << endl;
            return;
        }
        Node *temp = head;
        int i = 1;
        while (temp != NULL){
            cout << "list element number:" << i << endl;
            temp->cur.check_print();
            i++;
            temp = temp->next;
        }
        return;
    }
};


int
main()
{
    char main_str[100];
    char main_tele[20];
    List l1;
    while (1){
        cin.getline(main_str,100);
        cin.getline(main_tele,20);
        order ord1(main_str, main_tele);
        ord1.check_print();
        if (main_tele[0] == '0'){
            break;
        }

        l1.push_front(ord1);
    }

    l1.pop_back();
    l1.print();
}

