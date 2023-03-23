#include <cstdio>
#include <cstddef>
#include <iostream>
#include <vector>
#include <list>
using namespace std;

template <class C>
void print_front_and_back(C &c){
    typename C::iterator first = c.begin();
    typename C::iterator last = c.end();
    while (first != last){
        cout << *first << endl;
        first++;
    }

    typename C::reverse_iterator rlast = c.rend();
    typename C::reverse_iterator rfirst = c.rbegin();
    while (rfirst != rlast){
        cout << *rfirst << endl;
        rfirst++;
    }

    return;
}

template <class C, class D>
void add_list_to_vector(C & c, D & d){
    typename C::iterator vector_i = c.begin();
    typename D::iterator list_i = d.begin();
    typename C::iterator vector_last = c.end();
    typename D::iterator list_last = d.end();
    int flag = 1;

    while ((vector_i != vector_last) && (list_i != list_last)){
        vector_i = c.insert(vector_i, *list_i);
        vector_i++;
        vector_i++;
        list_i++;
        if ((vector_i == vector_last) || (list_i == list_last)){
            flag = 0;
            break;
        }
        vector_i++;
        list_i++;
    }

    if (flag){
        c.insert(vector_i, *list_i);
    }
    return;
}

template <class C>
void double_odd(C & c){
    typename C::iterator i = c.begin();
    typename C::iterator j = c.end();

    while (i != j){
        i++;
        if (i == j){
            break;
        }
        i = c.insert(i, *i);
        i++;
        if (i == j){
            break;
        }
        i++;
    }

    return;
}


int
main(){
    vector <int> a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(7);

    list <int> b;
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    b.push_back(8);

    print_front_and_back(a);

    cout << endl;

    print_front_and_back(b);

    //cout << endl;
    //add_list_to_vector(a,b);
    //print_front_and_back(a);

    cout << endl;
    double_odd(b);
    print_front_and_back(b);

    cout << endl;
    double_odd(a);
    print_front_and_back(a);
    return 0;
}
