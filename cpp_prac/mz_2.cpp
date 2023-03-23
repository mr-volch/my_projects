#include <cstdio>
#include <cstddef>
#include <iostream>
using namespace std;

class A{
    int i;
    A(int i = 0): i(i){}
    static A* cur;
    public:

    static A*
    init(A *b, int i){
        if (cur != NULL){
            b = cur;
            return cur;
        }
        b = new A(i);
        cur = b;
        return b;
    }

    private:
    A(const A & b){
        if (&b == this){
            return;
        }
        i = b.i;
    }

    public:
    void
    print(){
        cout << i  << endl;
        return;
    }
};

A* A :: cur = NULL;

int
main(){
    A *c = NULL;
    c = A::init(c, 3);
    c->print();
    A *b = NULL;
    b = A::init(b,2);
    b->print();
    delete(c);
}
