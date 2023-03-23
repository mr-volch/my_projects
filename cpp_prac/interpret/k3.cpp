#include  <cstdio>
#include <iostream>
using namespace std;

class A {
     const int i;
public:
     A():i(1){}
     //A& operator=(const A& ra) {return *this;}
};

int main() {
    A a; A b=a; a=b;
    return 0;
}
