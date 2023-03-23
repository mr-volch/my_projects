#include <iostream>
#include <vector>
#include <list>
#include <cstddef>
#include <cstdio>

using namespace std;

//предобъявление функции для печати содержимого контейнера
template <class C> void print(C &c);


//класс-функция
class Func{
    public:
    //перегружаем оператор ()
    //семантика функции: меняет местами(если это возможно)
    //два элемента контейнера (i,j - номера элементов. obj - контейнер
    template <class T> T&
    operator()(T &obj, int j, int i){

        //если переставляем элемент на его же мессто, можно ничего не менять
        if (i == j){
            return obj;
        }
        //два итератора на начало:
        //один переведем на i-ый, другой - на j-ый
        typename T::iterator a1 = obj.begin();
        typename T::iterator a2 = obj.begin();
        //итератор на конец - для вычисления длины
        typename T::iterator b = obj.end();

        //вычисляем длину и обнавляем знаечние итератора
        int n = 0;
        while (a1 != b){
            n++;
            a1++;
        }

        a1 = obj.begin();

        //если один из поданных номеров больше длины или меньше равен нулю, ругаемся
        if ((j > n) || (i > n) || (i <= 0) || (j <= 0)){
            cout << "swap out of range" << endl;
            //возвращаем неизмененный контейнер
            return obj;
        }

        //дальнейший алгоритм предполагает, что i должно быть не меньше j
        if (j > i){
            int tmp = j;
            j = i;
            i = tmp;
        }

        //добираемся до i-го элемента (итератор - а1)
        for (int x = 1; x < i; x++){
            a1++;
        }

        //добираемся до j-го элемента (итератор - а2)
        for (int y = 1; y < j; y++){
            a2++;
        }

        //вставляем в i-ую позицию j-ый элемент
        a1 = obj.insert(a1, *a2);

        //снова устанавливаем а2 в j-ую позицию
        a2 = obj.begin();
        for (int y = 1; y < j; y++){
            a2++;
        }
        //сдвигаем а1 (так как без этого он будет на только что вставленном значении)
        a1++;

        //вставляем в j-ую позицию i-ый элемент и сдвигаем а2
        a2 = obj.insert(a2, *a1);
        a2++;

        //стираем старые элементы (стоявшие на i и j)
        obj.erase(a2);
        obj.erase(a1);

        //возвращаем измененный контейнер
        return obj;
    }
};


//шаблонная функция для печати содержимого контейнера
template <class C>
void print(C &c){
    typename C::iterator first = c.begin();
    typename C::iterator last = c.end();
    while (first != last){
        cout << *first << endl;
        first++;
    }

    return;
}

//класс для проверки работы класса-функции с контейнерами, содержащими
//пользовательские типы в качестве своих элементов
class Test{
    int a;
    public:
    Test(int in_a = 0): a(in_a){}

    //перегружаем оператор вывода, чтобы печать работала
    friend ostream& operator<<(ostream &out, const Test &y);
};

ostream& operator <<(ostream &out, const Test &y){
    out << y.a;
    return out;
}

int
main(){
    //заводим объект класса-функции
    Func f;
    //проверка для списка с пользовательским типом данных
    list<Test> L {1,2,3,4,5};
    f(L, 2, 4);
    print(L);


    cout << endl << "list" << endl << endl;

    //проверка отработки ошибки выхода за диапазон
    vector<int> v1 {1,2,3,4,5};
    f(v1,1,6);
    print(v1);

    cout << endl << "vector" << endl << endl;

    //проверка для строки
    string s1 = "abcdef";
    f(s1,6,1);
    print(s1);

    cout << endl << "string" << endl;

    return 0;
}
