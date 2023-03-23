#include <cstring>
#include <iostream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

enum type_of_lex
{
    LEX_NULL, /*0*/
    LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, /*9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE,
    LEX_WHILE, LEX_GOTO, LEX_FOR, LEX_WRITE, /*18*/
    LEX_FIN, /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, /*35*/
    LEX_NUM, /*36*/
    LEX_STR,
    LEX_ID, /*37*/
    LEX_OPEN, LEX_CLOSE
}; /*41*/


class Lex
{
    const char *to_print[41] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct",
    "not", "or", "program", "read", "true", "while", "goto",
    "for", "write", "fin", ";", ",", ":", ".", "=", "(", ")", "==", "<",
    ">", "+", "-", "*", "/", "<=", "!=", ">=", "num", "str", "id", "{", "}"};
    type_of_lex t_lex;
    int v_lex;
    char *str_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0, char *s = NULL): t_lex (t), v_lex (v), str_lex(s) {}
    Lex (const Lex &l){
        t_lex = l.t_lex;
        v_lex = l.v_lex;
        str_lex = NULL;
        if (l.str_lex != NULL){
            str_lex = new char[v_lex];
            memmove(str_lex, l.str_lex, v_lex);
        }
    }

    Lex& operator =(const Lex &l){
        t_lex = l.t_lex;
        v_lex = l.v_lex;
        if (this == &l){
            return *this;
        }
        delete []str_lex;
        str_lex = NULL;

        if (l.str_lex == NULL){
            return *this;
        }

        str_lex = new char[v_lex];
        memmove(str_lex, l.str_lex, v_lex);

        return *this;
    }
    type_of_lex  get_type ()  { return t_lex; }
    int get_value () { return v_lex; }
    char * get_str_value () { return str_lex; }
    friend ostream & operator<< (ostream &s, Lex l){
        if (l.str_lex != NULL){
            s << '(' << l.to_print[l.t_lex] << ',' << l.v_lex << ',' << l.str_lex << ");";
            return s;
        }
        s << '(' << l.to_print[l.t_lex] << ',' << l.v_lex << ");" ;
        return s;
    }

    ~Lex(){
        if (str_lex != NULL){
            delete []str_lex;
        }
    }
};

class
Lex_arr
{
    Lex *arr;
    int size;
    int cur_i;
public:
    Lex_arr(int s = 1000): size(s), cur_i(0) {arr = new Lex[size];}

    void
    append_arr(Lex l){
        cur_i++;
        if ((cur_i % 1000) == 0){
            Lex *tmp = new Lex[cur_i + 1000];
            for (int i=0;i < size;i++){
                tmp[i] = arr[i];
            }
            size+=1000;
            delete []arr;
            arr = tmp;
       }
       arr[cur_i-1] = l;
    }

    void
    print_arr(){
        cout << "l: " << cur_i << "s: " << size << endl;
        for (int i = 0; i < cur_i; i++){
            cout << arr[i] << endl;
        }
    }

    ~Lex_arr(){
        delete []arr;
    }
};


class Ident
{
     char *name;
     bool declare;
     type_of_lex  type;
     bool assign;
     int value;
     char *str_val;
public:
    Ident() { declare = false; assign = false; str_val = NULL; name = NULL;}
    char * get_name () {return name;}
    void put_name (const char *n){
        name = new char [strlen(n)+1];
        strcpy(name,n);
    }
    bool get_declare () { return declare; }
    void put_declare () { declare = true; }
    type_of_lex get_type() { return type; }
    void put_type(type_of_lex t) { type = t; }
    bool get_assign() { return assign; }
    void put_assign(){ assign = true; }
    int get_value() { return value; }
    void put_value (int v){ value = v; }

    char * get_str_value(){return str_val;}
    void change_str_value(int i, char v){
        if ((i<value) && (i>=0) && (str_val != NULL)){
            str_val[i] = v;
        }
    }

    ~Ident(){
        if (str_val != NULL){
            delete []str_val;
        }

        delete []name;
    }
};


class Tabl_ident{
    Ident *p;
    int size;
    int top;
public:
    Tabl_ident ( int max_size ){
        p = new Ident [ size = max_size ];
        top = 0;
    }
    ~Tabl_ident () { delete []p; }
    int get_top() {return top;}
    Ident & operator[] ( int k ) { return p[k]; }
    int put ( const char *buf );
};

int Tabl_ident::put ( const char *buf ){
    for ( int j = 0; j < top; j++ ){
        if ( !strcmp ( buf, p[j].get_name() ) ){
            return j;
        }
    }
    p[top].put_name(buf);
    ++top;
    return top-1;
}

class Scanner
{
    enum state { H, IDENT, NUMB, PCOM, COM, STR, ALE, DELIM, NEQ};
    static const char *TW[];
    static type_of_lex  words[];
    static const char *TD[];
    static type_of_lex  dlms  [];
    state CS;
    FILE * fp;
    char c;
    char buf [80];
    int  buf_top;
    void clear(){
        buf_top = 0;
        for ( int j = 0; j < 80; j++ ){
             buf[j] = '\0';
        }
    }

    void add(){
        buf [ buf_top++ ] = c;
    }

    int look( const char *buf, const char **list ){
        int i = 0;
        while (list[i]){
            if ( !strcmp(buf, list[i]) ){
                return i;
            }
            ++i;
        }
        return 0;
    }

    void gc (){
        c = fgetc (fp);
        if (c == EOF){
            c = '@';
        }
    }

public:
    Lex get_lex();

    Scanner ( const char * program ){
        fp = fopen ( program, "r" );
        CS = H;
        clear();
        gc();
    }

    ~Scanner(){
        pclose(fp);
    }
};



const char *Scanner::TW[] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct", "not", "or", "program", "read", "true", "while", "goto", "for", "write", NULL};
const char *Scanner::TD[] = {"", "@", ";", ",", ":", ".", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{", "}", NULL};

Lex_arr lex_collection;

Tabl_ident TID(100);
type_of_lex Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, LEX_NOT,
    LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_GOTO, LEX_FOR, LEX_WRITE, LEX_NULL};
type_of_lex Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN,
    LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_OPEN, LEX_CLOSE, LEX_NULL};


Lex Scanner::get_lex (){

    int d, j;
    char *cur;
    CS = H;
    do{
      switch(CS){
          case H:
              if ( c==' ' || c == '\n' || c== '\r' || c == '\t' ){
                  gc();
              }
              else if ( isdigit(c) ){
                  d = c - '0';
                  gc();
                  CS = NUMB;
              }
              else if ( c == '"' ){
                  d = 0;
                  cur = new char[256];
                  gc();
                  CS = STR;
              }
              else if ( isalpha(c) ){
                  clear();
                  add();
                  gc();
                  CS = IDENT;
              }

              else if (c== '<' || c== '>' || c == '='){
                  clear();
                  add();
                  gc();
                  CS = ALE;
              }
              else if (c == '@'){
                  return Lex(LEX_FIN);
              }

              else if (c == '/'){
                  clear();
                  add();
                  gc();
                  CS = PCOM;
              }
              else if (c == '!') {
                  clear();
                  add();
                  gc();
                  CS = NEQ;
              }
              else{
                  CS = DELIM;
              }
              break;
          case IDENT:
              if ( isalpha(c) || isdigit(c) ){
                  add();
                  gc();
              }
              else if (j = look (buf, TW)){
                  return Lex (words[j], j);
              }
              else{
                  j = TID.put(buf);
                  return Lex (LEX_ID, j);
              }
              break;
          case NUMB:
              if ( isdigit(c) ){
                  d = d * 10 + (c - '0'); gc();
              }
              else{
                  return Lex ( LEX_NUM, d);
              }
              break;
          case STR:
              if (c == '"'){
                  gc();
                  cur[d] = '\0';
                  d++;
                  return Lex ( LEX_STR, d, cur);
              }

              else if (c == '@'){
                  delete []cur;
                  throw c;
              }
              if (d == 256){
                  delete []cur;
                  throw '"';
              }
              cur[d] = c;
              d++;
              gc();
              break;
          case PCOM:
              if (c == '*'){
                  gc();
                  CS = COM;
              }

              else{
                  CS = H;
                  j = look (buf, TD);
                  return Lex (LEX_SLASH, j);
              }
              break;
          case COM:
              if (c == '*'){
                  gc();
                  if (c == '/'){
                      gc();
                      CS = H;
                  }
                  else{
                      gc();
                  }
              }

              else if (c == '@'){
                  throw c;
              }

              else{
                  gc();
              }
              break;
          case ALE:
              if ( c== '='){
                  add();
                  gc();
                  j = look ( buf, TD );
                  return Lex ( dlms[j], j);
              }
              else{
                  j = look ( buf, TD );
                  return Lex ( dlms[j], j );
              }
              break;
          case NEQ:
              if (c == '='){
                  add();
                  gc();
                  j = look ( buf, TD );
                  return Lex ( LEX_NEQ, j );
              }
              else{
                  throw '!';
              }
              break;
          case DELIM:
              clear();
              add();
              if (j = look ( buf, TD)){
                  gc();
                  return Lex ( dlms[j], j );
              }
              else{
                  throw c;
              }
              break;
      }//end switch
    } while (true);
}

void
make_lex_analyze(Scanner &scan){
    Lex cur_lex;
    while (cur_lex.get_type() != LEX_FIN){
        cur_lex = scan.get_lex();
        //cout << cur_lex << endl;
        lex_collection.append_arr(cur_lex);
    }
}

int
main(int argc, char **argv)
{
    Scanner scan(argv[1]);
    try{
        make_lex_analyze(scan);

        cout << "printing lexems: " << endl;
        lex_collection.print_arr();
        cout << "printing idents" << endl;
        for (int i = 0; i < (TID.get_top()); i++){
            cout << TID[i].get_name() << endl;
        }
    }
    catch(char c){
        cout << "error\n" << c << endl;
    }
    return 0;
}
