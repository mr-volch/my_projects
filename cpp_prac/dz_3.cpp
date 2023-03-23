#include <cstdio>
#include <cstddef>
#include <iostream>
#include <cstring>

using namespace std;

//абстрактный класс - случайная величина
class random_value{
    public:
    //параметры под распределения с.в.
    double a;
    double b;
    //методы подсчета мат.ожидания. и дисперсии
    virtual double count_expect()=0;
    virtual double count_variance()=0;
    //виртуальный деструктор
    virtual ~random_value(){}
};

//класс, наследующий абстрактный:
//случайная величина с экспоненциальным распределением
class exp_val: public random_value{
    public:

    //у экспоненциального распределения только один параметр
    exp_val(int l_in = 0){
        a = l_in;
    }

    //считаем мат.ож. нашего распределения
    virtual double count_expect(){
        return 1/a;
    }

    //считаем дисперсию нашего распределения
    virtual double count_variance(){
        return 1/(a*a);
    }

    //виртуальный деструктор
    virtual ~exp_val(){}
};

class norm_val: public random_value{
    public:

    //формируем параметры распределения
    norm_val(int m_in = 0, int b2_in = 1){
        a = m_in;
        b = b2_in;
    }

    //считаем мат.ож. для нашего распределения
    virtual double count_expect(){
        return a;
    }

    //считаем дисперсию для нашего расределения
    virtual double count_variance(){
        return b*b;
    }

    //виртуальный деструктор
    virtual ~norm_val(){}
};

class random_value_arr{
    //статич. константа: максимальная возможная длина массива
    static const int len = 100;

    //сам по себе массива и его длина(реальная)
    random_value *arr[len];
    int cur_len;
    public:

    //конструктор умолчания
    random_value_arr(){
        cur_len = 0;
    }

    //конструктор с одним параметром
    random_value_arr(random_value *new_val){
        arr[0] = new_val;
        cur_len = 1;
    }

    //функция добавления нового элемента в массив
    void
    append(random_value *new_val){
        if (cur_len == len){
            cout << "adding element is out of range" << endl;
            return;
        }

        arr[cur_len] = new_val;
        cur_len++;
    }

    //функция удаления элемента с индексом i
    void
    pop(int i){
        if ((i < 0) || (i >= cur_len)){
            cout << "deleting element is out of range" << endl;
            return;
        }

        //удаляем содержимое i-ого элемента и сдвигаем все что справа от него на 1 влево
        delete arr[i];
        for (int j = i; j < cur_len-1; j++){
            arr[j] = arr[j+1];
        }
        cur_len--;
    }

    //функция, печатающая мат.ож. членов массива
    void
    print_expect(){
        cout<<endl;
        cout << "printing math expactations of array members:" << endl;
        for (int i = 0; i < cur_len; i++){
            cout << "math expectation of value №" << i+1 << " equals " << arr[i]->count_expect() << endl;
        }
        cout<<endl;
        return;
    }

    //функция, печатающая дисперсии членов массива
    void
    print_variance(){
        cout<<endl;
        cout << "printing variances of array members:" << endl;
        for (int i = 0; i < cur_len; i++){
            cout << "variance of value №" << i+1 << " equals " << arr[i]->count_variance() << endl;
        }
        cout<<endl;
        return;
    }

    //деструктор: удаляем все что лежит по указателям в массиве
    ~random_value_arr(){
        for(int i = 0; i < cur_len; i++){
            delete arr[i];
        }
    }
};

//int random_value_arr::len = 100;

int
main(){
    random_value_arr arr;
    exp_val *E;
    norm_val *N;

    //демонстрация: добавим 3 элемента
    E = new exp_val(2);
    arr.append(E);

    E = new exp_val(3);
    arr.append(E);

    N = new norm_val(1,2);
    arr.append(N);

    //выведем мат.ож. и дисперсии членов массива
    arr.print_expect();
    arr.print_variance();


    //если надо самому из cin заполнить массив, расскоментировать
    /*
    cout << "insert type of value distribution(E or N) if you want to stop, enter S" << endl;

    char c;
    cin >> c;
    int i = 0;

    while (c != 'S'){
        cout << "insert parametrs for distrubution" << endl;
        if (c == 'N'){
            int a, b;
            cin >> a;
            cin >> b;
            N = new norm_val(a,b);
            arr.append(N);
        }
        else{
            int a;
            cin >> a;
            E = new exp_val(a);
            arr.append(E);
        }
        i++;
        cout << "insert type of value distribution(E or N) if you want to stop, enter S" << endl;
        cin >> c;
    }
    */


    //удалим первый и снова выведем
    arr.pop(0);
    arr.print_expect();
    arr.print_variance();
    return 0;
}
