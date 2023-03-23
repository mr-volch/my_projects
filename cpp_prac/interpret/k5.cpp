#include<iostream>

using namespace std;

class A { public:

    A(int){cout<<"A(int)\n";}

    operator char*(){

        cout<<"A::operator char*\n";

        return "";}

};

class B: public A { public:

    B(char):A(1){cout<<"B(char)\n";}

    B(double):A(1){cout<<"B(double)\n";}

    operator void*(){

    cout<<"A::operator void*\n";

    return this;}

};

void f(A*){cout<<"f(A*)\n";}

void f(B){cout<<"f(B)\n";}

char f(void*){

    cout<<"f(void*)\n";

    return 'a';

}

int main(){ f(1); }
