#include <cstdio>
#include <cstddef>
#include <iostream>
using namespace std;

class A{
    int a;
    ~A(){}
    public:

    A(int a = 0): a(a){}

    //статический метод для
    //уничтожения объекта класса,
    //хранящегося по указателю
    static void
    destruct(A *c){
        delete c;
    }

    //статический метод дляинициализации
    //указателя на объект класса
    static A*
    init(A* cur, int i){
        cur = new A(i);
        return cur;
    }

    //метод выделения памяти под массив
    static A*
    make_arr(int N){
         return new A[N];
    }

    //метод для инициализации массива значениями (j)
    static A*
    init_arr(A*arr, int N, int j){
        for (int i = 0; i < N; i++){
            arr[i].a = j;
        }
        return arr;
    }

    //метод для уничтожения объектов в массиве
    static void
    del_arr(A* arr){
        delete []arr;
    }

    A(A &b){
        if (&b == this){
            return;
        }
        a = b.a;
        b.~A();
    }

    A& operator=(A &b){
        if (&b == this){
            return *this;
        }

        a = b.a;
        b.~A();
    }

    void
    print(){
        cout << a  << endl;
        return;
    }
};


int
main(){
    //демонстрация возможности
    //создания и уничтожения объекта
    A *c = NULL;
    c = A::init(c,3);
    c->print();
    A::destruct(c);

    //демонстрация работы с массивом
    int N = 10;

    A *arr = A::make_arr(N);
    arr = A::init_arr(arr, N, 5);
    arr[2].print();
    A::del_arr(arr);
}
