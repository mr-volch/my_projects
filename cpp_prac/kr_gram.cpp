#include <cstdio>
#include <iostream>

using namespace std;

char c;

void gc() {cin >> c;}

bool
scan(){
    enum states {H, AB, BS, S, ER};
    states CS;
    CS = H;
    do{
        gc ();
        switch (CS){
            case H:
                if ( c == 'a' ){
                    CS = AB;
                }

                else{
                    CS = ER;
                }

                break;
            case AB:
                if ( c =='a' ){
                    CS = AB;
                }
                else if ( c == 'b' ){
                    CS = BS;
                }
                else{
                    CS = ER;
                }
                break;
            case BS:
                if (c == 'b'){
                    CS = BS;
                }

                else if (c == '|'){
                    CS = S;
                }

                else{
                    CS = ER;
                }

                break;
        }
    } while (CS != S && CS != ER);

    return CS == S; // true, если CS != ER, иначе false
}

int
main(int argc, char **argv){
    if (scan()){
        cout << "принадлежит" << endl;
        return 0;
    }

    cout << "не принадлежит" << endl;
    return 0;
}

