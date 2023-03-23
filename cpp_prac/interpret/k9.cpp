#include<iostream>

using namespace std;

class Complex {

    double re,im;

public:   

    Complex(double r=0.0,double i=0.0) : re(r),im(i){

        cout<<"Complex("<<re<<",          "<<im<<") ";

    }

    operator double(){

        cout<<"Complex::operator double(). ";

        return re;

    }

};

 

void f(double, Complex){ cout<<"f(double,Complex) ";}

void f(Complex, int){ cout<<"f(Complex, int) ";}

 

int main(){ Complex z;  f(2.0,2); }
