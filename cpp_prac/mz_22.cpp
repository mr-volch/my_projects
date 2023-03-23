#include <cstddef>
#include <iostream>
#include <cstdio>
using namespace std;

class A{
    int j;
    public:
    //конструктор с одним параметром
    A(int a){
        j = a;
    }

    //перегружаем операторы , [] и ()
    A operator,(int k){
        cout<<","<<endl;
        this->j++;
        return *this;
    }

    A operator[](A cur){
        cout << "[]" << endl;
        this->j = this->j + 2;
        return *this;
    }

    int operator()(A cur, int k){
        this->j = -this->j;
        return this->j;
    }

    int f1(){
        A i(3);
        return (i,j)[i,j](i,j);
    }
};

int
main(){
    A cur(1);
    cout << cur.f1() << endl;
    return 0;
}


