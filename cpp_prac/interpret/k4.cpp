#include<iostream>

using namespace std;

class A { public:

    A(int){cout<<"A(int) ";}

    A(char){cout<<"A(char) ";}

    A(const char*){cout<<"A(const char*) ";}

    A(float){cout<<"A(float) ";}

};

void f(A){cout<<"f(A) ";}

char f(const char*){

    cout<<"f(const char*) ";

    return 'a';

}

int main(){ f(""); }
