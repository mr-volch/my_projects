#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdio>
using namespace std;


//класс заказ
class order{
    //название фирмы
    char *firm;
    //телефон
    char *tele;
    //статическая переменная - общее колво
    //зарегистрированных заказов
    static int amount;
    //номер заказа
    int num;

    public:

    //конструктор умолчания
    order(){
        cout << endl << "default costructor" << endl;
        firm = NULL;
        tele = NULL;
        num = 0;
    }

    //конструктор с двумя параметрами
    order(char *firm_in, char *tele_in){
        cout << endl << "constructor with two params" << endl;
        firm = NULL;

        //выделяем место под члены класса и заполняем его
        if (firm_in != NULL){
            firm = new char[strlen(firm_in) + 1];
            memmove(firm, firm_in, strlen(firm_in) + 1);
        }

        tele = NULL;
        if (tele_in != NULL){
            tele = new char[strlen(tele_in) + 1];
            memmove(tele, tele_in, strlen(tele_in) + 1);
        }

        //увеличиваем статическую переменную
        //и присваиваем номер заказа
        amount++;
        num = amount;
    }

    //конструктор копирования
    order(const order &ord){
        cout << endl << "copy constructor" << endl;
        num = ord.num;
        firm = new char[strlen(ord.firm) + 1];
        memmove(firm, ord.firm, strlen(ord.firm) + 1);
        tele = new char[strlen(ord.tele) + 1];
        memmove(tele, ord.tele, strlen(ord.tele) + 1);
   }

    //функция проверочной печати
    void
    check_print(){
        cout << "firm: " << firm << endl;
        cout << "tele: " << tele << endl;
        cout << "num: " << num << endl;
    }

    //перегруженный оператор присваивания
    order
    operator=(const order &ord){
        cout << endl << "overloaded asignment operator" << endl;
        if (&ord == this){
            return *this;
        }

        //удаляем старые данные из динамической памяти
        //и заполняем новыми значениями
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

    //дуструктор
    ~order(){
        cout << endl << "destructor" << endl;
        delete []firm;
        delete []tele;
    }
};

//инициализация статической переменной
int order :: amount = 0;


//вспомогательная структура: узел списка
struct Node{
    order cur;
    Node *next;
    Node *prev;
};


//класс список
class List{
    //будем хранить указатели на
    //первый, последний элементы списка,
    // а также его длину
    Node *head;
    Node *tail;
    int len;

    public:

    //конструктор умолчания
    List(){
        head = NULL;
        tail = NULL;
        len = 0;
    }

    //конструктор с одним параметром
    List(order &ord){
        //выделяем место под новый узел списка
        Node* N = new Node;
        //заполняем поля структуры узла
        N->cur = ord;
        N->next = NULL;
        N->prev = NULL;

        //присваиваем переменным - членам класса
        //соответсвующие значения
        head = N;
        tail = N;
        len = 1;
    }

    //функция добавления в начало
    void
    push_front(order &ord){

        //создаем новый узел
        //и заполняем его
        Node *N = new Node;
        N->cur = ord;
        N->next = head;
        N->prev = NULL;

        //если список не пуст, новый
        //элемент станет предыдущим для головы
        if (head != NULL){
            head->prev = N;
        }

        //меняем голову, если надо - хвост
        head = N;

        if (tail == NULL){
            tail = N;
        }

        //увеличиваем длину
        len++;
        return;
    }

    //функция добавления в конец
    //аналогично
    void
    push_back(order &ord){

        Node *N = new Node;
        N->cur = ord;
        N->prev = tail;
        N->next = NULL;

        if (tail != NULL){
            tail->next = N;
        }

        tail = N;

        if (head == NULL){
            head = N;
        }

        len++;
        return;
    }

    //функция удаления из начала
    void
    pop_front(){
        Node *P;

        //попытка удаления из пустого списка - ошибка
        if (head == NULL){
            cout << "error: deleting from empty list" << endl;
            return;
        }

        //меняем голову, в Р созраняем
        //текущую голову
        P = head;
        head = head->next;

        //если теперь список не пуст,
        //присваиваем голове NULL в поле
        //предыдущего элемента
        if (head != NULL){
            head->prev = NULL;
        }

        //обнуляем хвост, если теперь
        //список пуст
        if (P == tail){
            tail = NULL;
        }

        //удаляем сохраненную голову
        //и уменьшаем длину
        delete(P);
        len--;
        return;
    }

    //удаление из конца
    //аналогично
    void
    pop_back(){
        Node *P;

        if (tail == NULL){
            cout << "error: deleting from empty list" << endl;
            return;
        }

        P = tail;
        tail = tail->prev;

        if (tail != NULL){
            tail->next = NULL;
        }

        if (P == head){
            head = NULL;
        }

        delete(P);
        len--;
        return;
    }

    //вставка в позицию р
    void
    insert(int p, order &x){

        if (p <= 0){
            cout << "error: insert out of range" << endl;
            return;
        }

        //ищем элемент в р-той позиции
        Node *elem = head;

        int i = 1;
        while ((elem != NULL) && (i < p-1)){
            elem = elem->next;
            i++;
        }

        //если не нашли, то ошибка
        if (elem == NULL){
            cout << "error: insert out of range" << endl;
            return;
        }

        //создаем новую структуру в динамической памяти
        Node *in_elem = new Node;

        //заполняем ее
        in_elem->cur = x;
        in_elem->prev = elem;
        in_elem->next = elem->next;

        if (elem->next != NULL){
            elem->next->prev = in_elem;
        }

        elem->next = in_elem;

        //если добавили голову или хвост - меняем их
        if (elem->prev == NULL){
            head = elem;
        }

        if (elem->next == NULL){
            tail = elem;
        }

        //увеличиваем длину
        len++;
        return;
    }

    //функция удаления из позиции р
    void
    erase(int p){

        if (p <= 0){
            cout << "error: erase out of range" << endl;
            return;
        }

        //ищем узел с номером р
        Node *elem = head;
        int i = 0;

        while ((elem != NULL) && (i < p-1)){
            elem = elem->next;
            i++;
        }

        //если не нашли - ошибка
        if (elem == NULL){
            cout << "error: erase out of range" << endl;
            return;
        }

        //если нужно - меняем соответсвующее указатели
        //на предыдущие и следующие элементы
        if (elem->prev != NULL){
            elem->prev->next = elem->next;
        }

        if (elem->next != NULL){
            elem->next->prev = elem->prev;
        }

        //если нужно - меняем голову и хвост
        if (elem->prev == NULL){
            head = elem->next;
        }

        if (elem->next == NULL){
            tail = elem->prev;
        }

        //удаляем элемент из динамической памяти
        delete elem;

        //уменьшаем длину
        len--;
        return;
    }

    //полчаем первый элемент
    //(указатель на него)
    order*
    front(){
        if (head == NULL){
            return NULL;
        }
        return &(head->cur);
    }

    //получаем последнийй элемент
    //(указатель на него)
    order*
    back(){
        if (tail == NULL){
            return NULL;
        }
        return &(tail->cur);
    }

    //проверка на пустоту
    int
    empty(){
        if (len == 0){
            return 1;
        }
        return 0;
    }

    //длина
    int
    size(){
        return len;
    }

    //печать
    void
    print(){
        if (head == NULL){
            cout << "its empty" << endl;
            return;
        }
        Node *temp = head;
        int i = 1;
        while (temp != NULL){
            cout << "element number:" << i << endl;
            temp->cur.check_print();
            i++;
            temp = temp->next;
        }
        cout << endl;
        return;
    }

    //деструктор
    ~List(){
        while (head != NULL){
            Node *P = head;
            head = head->next;
            delete P;
        }
    }
};


class Queue: private List{
    private:
        //из полей данных добавилось только одно: макс. длина
        const int max_len = 1000;
    public:
        Queue(): List(){}

        Queue(order &ord): List(ord){}

        void
        back(order &ord){
            //перед добавлением проверяем: не полна ли очередь
            if (List::size() == max_len){
                cout<<"queue is full"<<endl;
                return;
            }

            List::push_back(ord);
        }

        order*
        front(){
            return(List::front());
        }

        void
        pop(){
            List::pop_front();
        }

        int
        empty(){
            return (List::empty());
        }

        int
        size(){
            return(List::size());
        }

        int
        full(){
            //если длина очередь совпадает с ее макс. длиной, то очередь полна
            if (List::size() == max_len){
                return 1;
            }
            return 0;
        }

        void
        print(){
            List::print();
        }
};


int
main()
{
    //будем вводить название фирмы
    //в перменную main_str
    char main_str[100];

    //будем вводить телефон
    //в переменную main_tele
    char main_tele[20];

    //звели пустой список
    List l1;

    //проверяем работу методов проверки
    //на пустоту и определения числа элементов
    int k1 = l1.empty();
    int k2 = l1.size();
    cout << "is empty: " << k1 << endl;
    cout << "size: " << k2 << endl;

    //добавляем пять элементов
    //от пользователя. Каждый
    //новый заказ - в начало списка
    int i = 0;
    while (i < 5){
        cin.getline(main_str,100);
        cin.getline(main_tele,20);
        order ord1(main_str, main_tele);
        l1.push_front(ord1);
        i++;
    }

    //удаляем последний элемент
    //списка и печатаем его
    l1.pop_back();
    l1.print();

    //проверяем работу методов проверки
    //на пустоту и определения числа элементов
    k1 = l1.empty();
    k2 = l1.size();
    cout << "is empty: " << k1 << endl;
    cout << "size: " << k2 << endl;


    //добавляем пять элементов
    //от пользователя. Каждый
    //новый заказ - в конец списка
    while (i > 0){
        cin.getline(main_str,100);
        cin.getline(main_tele,20);
        order ord1(main_str, main_tele);
        l1.push_back(ord1);
        i--;
    }

    //удаляем первый элемент
    //списка и печатаем его
    l1.pop_front();
    l1.print();

    //печатаем первый и последний элементы списка

    cout << "first" << endl;
    order *first = l1.front();
    first->check_print();

    cout << "last" << endl;
    order *last = l1.back();
    last->check_print();

    //стираем третий элемент списка и
    // дописываем первый элемент на его место
    l1.erase(4);
    l1.insert(4, *first);

    l1.print();

    //отработка ошибок

    cout << endl << "correct reaction on mistakes" << endl;
    l1.erase(100);
    l1.insert(-2, *last);

    List l2;
    if (l2.front() == NULL){
        cout << endl << "NULL" << endl;
    }

    if (l2.back() == NULL){
        cout << endl << "NULL" << endl;
    }

    l2.pop_front();
    l2.pop_back();

    //проверка очереди
    Queue q1;

    cout << "queue demonstration" << endl;

    //добавляем 5 элементов в очередь
    for (int j = 0; j < 5; j++){
        cin.getline(main_str,100);
        cin.getline(main_tele,20);
        order ord_cur(main_str, main_tele);
        q1.back(ord_cur);
    }

    //выводим содержимое очереди
    q1.print();

    //выводим проверку на полную заполненность
    cout << q1.full() << endl;

    //получаем первый элемент и выводим его
    order* tmp = q1.front();
    tmp->check_print();

    //удаляем первый элемент и вывыодим очередь
    q1.pop();
    q1.print();

    return 0;
}
