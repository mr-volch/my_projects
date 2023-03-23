#include <iostream>
#include <cstdio>

using namespace std;

class Arr{
    int *arr;
    int size;
    public:

    Arr(int s, int date = 0): size(s){
        arr = new int[s];
        for (int i = 0; i < size; arr[i++] = date);
    }

    Arr(const Arr &a){
        size = a.size;
        arr = new int[size];
        for (int i = 0; i < size; i++){
            arr[i] = a.arr[i];
        }
    }

    Arr(Arr &&a){
        size = a.size;
        arr = a.arr;

        a.arr = NULL;
    }

    Arr operator=(Arr &&a){
        if (&a == this){
            return *this;
        }

        delete []arr;
        arr = a.arr;
        size = a.size;

        a.arr = NULL;
        return *this;
    }

    Arr operator=(const Arr &a){
        if (&a == this){
            return *this;
        }

        delete []arr;

        arr = new int[a.size];

        for (int i = 0; i < a.size; i++){
            arr[i] = a.arr[i];
        }

        size = a.size;
        return *this;
    }

    Arr operator!(){
        Arr new_a = Arr(size, 0);
        for (int i = 0; i < size; i++){
            new_a.arr[i] = -arr[i];
        }

        return new_a;
    }

    Arr operator&(const Arr &y)const{
        int min_size = size;

        if (y.size < size){
            min_size = y.size;
        }

        Arr new_a(min_size, 0);

        for (int i = 0; i < min_size; i++){
            new_a.arr[i] = ((arr[i]) & (y.arr[i]));
        }

        return new_a;
    }

    friend ostream& operator<<(ostream &out, const Arr &y);

    int& operator[](int i){
        if ((i < 0) || (i >= size)){
            cout << "index out of range" << endl;
            return size;
        }
        return arr[i];
    }

    ~Arr(){
        delete[]arr;
    }
};


//тело функции-друга (перегрузка <<)

ostream& operator<<(ostream& out, const Arr &y){
    out << "printing Arr" << endl;
    for (int i = 0; i < y.size; i++){
        out << "index: " << i << ", elem: " << y.arr[i] << endl;
    }
    return out;
}

int main(){
    Arr a1(5), a2(10,1), a3(4,2);
    const Arr a4(5);
    cout << a1 << a2 << a3 << a4;
    cout << endl << endl << endl;
    a2 = a2 & 2;
    a3 = (!a2) & a3;
    a1 = a4 & a3;
    a1[1] = 100;
    cout << a1 << a2 << a3 << a4;
}
