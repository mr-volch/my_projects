#include <cstring>
#include <iostream>
#include <cstdio>
#include <stdlib.h>

using namespace std;

enum type_of_lex
{
    LEX_NULL, /*0*/
    LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, /*9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_BREAK,
    LEX_WHILE, LEX_GOTO, LEX_FOR, LEX_WRITE, /*18*/
    LEX_FIN, /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, /*35*/
    LEX_NUM, /*36*/
    LEX_STR,
    LEX_ID, /*37*/
    LEX_POINTER,
    LEX_OPEN, LEX_CLOSE,
    POLIZ_LABEL, /*38*/
    POLIZ_ADDRESS, /*39*/
    POLIZ_GO, /*40*/
    POLIZ_FGO
}; /*41*/


class Lex
{
    const char *to_print[43] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct",
    "not", "or", "program", "read", "true", "break", "while", "goto",
    "for", "write", "fin", ";", ",", ":", ".", "=", "(", ")", "==", "<",
    ">", "+", "-", "*", "/", "<=", "!=", ">=", "num", "str", "id", "pointer", "{", "}"};
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
    int last_goto;
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
        if (buf_top == 79){
            throw "too long id";
        }
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
        last_goto = 0;
        CS = H;
        clear();
        gc();
    }

    ~Scanner(){
        pclose(fp);
    }
};



const char *Scanner::TW[] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct", "not", "or", "program", "read", "true", "break", "while", "goto", "for", "write", NULL};
const char *Scanner::TD[] = {"", "@", ";", ",", ":", ".", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{", "}", NULL};

Lex_arr lex_collection;

Tabl_ident TID(100);
Tabl_ident TPD(100);

type_of_lex Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, LEX_NOT,
    LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_BREAK, LEX_WHILE, LEX_GOTO, LEX_FOR, LEX_WRITE, LEX_NULL};
type_of_lex Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN,
    LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_OPEN, LEX_CLOSE, LEX_NULL};


Lex Scanner::get_lex (){

    int d, j;
    char *cur;
    CS = H;
    do{
      switch(CS){
          case H:
              if (last_goto != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t'){
                  //cout << "MODIFIED-" << endl;
                  last_goto--;
              }

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
              if ( isalpha(c) || isdigit(c) || (c == ':') ){
                  if (c == ':'){
                      gc();
                      if (j = look(buf, TW)){
                          throw ("Error: code word as a pointer");
                      }

                      else{
                          j = TPD.put(buf);
                          return Lex (LEX_POINTER, j);
                      }
                  }

                  add();
                  gc();
              }
              else if (j = look (buf, TW)){
                  if (!strcmp(buf, "goto")){
                      last_goto = 2;
                      //cout << "LAST GOTO : " << last_goto << endl;
                  }
                  return Lex (words[j], j);
              }
              else{
                  //cout << "CUR GOTO" << last_goto << endl;
                  if (last_goto != 0){
                      j = TPD.put(buf);
                      return Lex(LEX_POINTER, j);
                  }
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


class Poliz
{
         Lex        * p;
         int          size;
         int          free;
public:
                      Poliz (int max_size)
                      {
                        p = new Lex [size = max_size];
                        free = 0;
                      }
                     ~Poliz() { delete [] p; }
         void         put_lex ( Lex l )
                      {
                        p [free] = l;
                        free++;
                      }
         void         put_lex ( Lex l, int place) {
            p [place] = l;
            cout << place;
         }
         void         blank    () { free++; }
         int          get_free () { return free; }
         Lex        & operator[] (int index)
                      {
                        if ( index > size )
                          throw "POLIZ:out of array";
                        else
                          if ( index > free )
                            throw "POLIZ:indefinite element of array";
                          else
                            return p[index];
                      }
         void         print ()
                      {
                        for ( int i = 0; i < free; ++i )
                          cout << p[i];
                      }
};

template < class T, int max_size >
class Stack
{
         T            s [max_size];
         int          top;
public:
                      Stack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( T i );
         T            pop ();
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == max_size; }
};
 
template < class T, int max_size >
void Stack < T, max_size > :: push (T i)
{
  if ( !is_full() )
    s [top++] = i;
  else
    throw "Stack_is_full";
}
 
template <class T, int max_size >
T Stack < T, max_size > :: pop ()
{
  if ( !is_empty() )
    return s[--top];
  else
    throw "stack_is_empty";
}

class Parser
{
         Lex          curr_lex;
         type_of_lex  c_type;
         int          c_val;
         int          in_while;
         Scanner      scan;
         Stack < int, 100 > st_int;
         Stack < type_of_lex, 100 >  st_lex;
         void         P();
         void         D1();
         int          D2();
         void         V(type_of_lex);
         void         B();
         int         S();
         void          E();
         void         E1();
         void         T();
         void         F();
         void         dec ( type_of_lex type);
         void         check_id ();
         void         check_op ();
         void         check_not ();
         void         eq_type ();
         void         eq_bool ();
         void         check_id_in_read ();
         void         gl ()
                      {
                        curr_lex = scan.get_lex();
                        cout << curr_lex << endl;
                        c_type = curr_lex.get_type();
                        c_val = curr_lex.get_value();
                      }
public:
         Poliz        prog;
         Parser (const char *program ) : scan (program),prog (1000) {in_while = 0;}
         void         analyze();
};


void Parser::analyze ()
{
  gl();
  P();
  //prog.print();
  cout << endl << "Yes!!!" << endl;
}


void Parser::P ()
{
    if (c_type == LEX_PROGRAM){
        gl();
    }
    else{
        throw curr_lex;
    }

    if (c_type == LEX_OPEN){
        gl();
    }

    else{
        throw curr_lex;
    }

    D1();
    cout << "launching operators" << endl;
    B();

    if (c_type == LEX_CLOSE){
        gl();
    }

    else{
        throw curr_lex;
    }

    if (c_type == LEX_FIN){
        gl();
    }

    else{
        throw curr_lex;
    }
}


void Parser::D1 ()
{

    while (D2()){
        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
        }
    }
}


int Parser::D2 ()
{
    st_int.reset();
    type_of_lex type_cur_0 = LEX_NULL;

    if ((c_type == LEX_INT) || (c_type == LEX_STRING) || (c_type == LEX_BOOL)){
        type_cur_0 = c_type;
        gl();
    }

    else{
        return 0;
    }

    V(type_cur_0);

    while (c_type == LEX_COMMA){
        gl();
        V(type_cur_0);
    }

    dec(type_cur_0);
    return 1;
}


void Parser::V (type_of_lex type_cur_0){
    if (c_type != LEX_ID){
        throw curr_lex;
    }

    else{
        st_int.push(c_val);
        Lex cur_p = curr_lex;
        gl();
        

        if (c_type == LEX_ASSIGN){
            prog.put_lex(Lex(POLIZ_ADDRESS, cur_p.get_value()));
            gl();
            prog.put_lex(curr_lex);

            if (type_cur_0 == LEX_INT){
                if (c_type == LEX_NUM){
                    prog.put_lex(Lex (LEX_ASSIGN));
                }

                else{
                    throw "wrong types are in =";
                }
            }
            
            else if (type_cur_0 == LEX_STRING){
                if (c_type == LEX_STR){
                    prog.put_lex(Lex (LEX_ASSIGN));
                }

                else{
                    throw "wrong types are in =";
                }
            }

            else if (type_cur_0 == LEX_BOOL){
                if ((c_type == LEX_TRUE) || (c_type == LEX_FALSE)){
                    prog.put_lex(Lex (LEX_ASSIGN));
                }

                else{
                    throw "wrong types are in =";
                }
            }

            gl();
        }
    }
}



void Parser::B ()
{
    while (S()){}
}

 
int Parser::S ()
{
    int pl0, pl1, pl2, pl3;
 
    if (c_type == LEX_IF){
        gl();
        if (c_type == LEX_LPAREN){
            gl();
        }
        else{
            throw curr_lex;
        }

        E();
        eq_bool();
        pl2 = prog.get_free ();
        prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));

        if (c_type == LEX_RPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }

        S();
        pl3 = prog.get_free();
        prog.blank();
        prog.put_lex (Lex(POLIZ_GO));
        prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl2);

        if (c_type == LEX_ELSE){
            gl();
            S();
            prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl3);
        }

        else{
            throw curr_lex;
        }
    }//end if

    else if (c_type == LEX_WHILE){
        in_while++;
        pl0=prog.get_free();
        gl();

        if (c_type == LEX_LPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }

        E();
        eq_bool();
        pl1=prog.get_free(); prog.blank();
        prog.put_lex (Lex(POLIZ_FGO));

        if (c_type == LEX_RPAREN){
           gl();
        }

        else{
            throw curr_lex;
        }

        S();
        prog.put_lex (Lex (POLIZ_LABEL, pl0));
        prog.put_lex (Lex ( POLIZ_GO));
        prog.put_lex (Lex ( POLIZ_LABEL, prog.get_free()), pl1);
        in_while--;
    }//end while

    else if (c_type == LEX_READ){
        gl();

        if (c_type == LEX_LPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }

        if (c_type == LEX_ID) {
            check_id_in_read();
            prog.put_lex (Lex ( POLIZ_ADDRESS, c_val) );
            gl();
        }

        else{
            throw curr_lex;
        }

        if ( c_type == LEX_RPAREN ){
            gl();
        }

        else{
            throw curr_lex;
        }

        if ( c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
        }

        prog.put_lex (Lex (LEX_READ));
    }//end read

    else if (c_type == LEX_WRITE){
        gl();
        if (c_type == LEX_LPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }

        E();

        while (c_type == LEX_COMMA){
            gl();
            E();
        }

        if (c_type == LEX_RPAREN){
            gl();
            prog.put_lex (Lex(LEX_WRITE));
        }

        else{
            throw curr_lex;
        }

        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
        }
    }//end write

    else if (c_type == LEX_BREAK){
        if (in_while == 0){
            throw "break out of while";
        }

        //часть для полиза
        gl();
        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
        }
    }

    else if (c_type == LEX_FOR){
        in_while++;

        //c полизом - потом

        gl();
        if (c_type == LEX_LPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }

        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            E();

            if (c_type == LEX_SEMICOLON){
                gl();
            }

            else{
                throw curr_lex;
            }
        }

        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            E();
            eq_bool();

            if (c_type == LEX_SEMICOLON){
                gl();
            }

            else{
                throw curr_lex;
            }
        }

        if (c_type == LEX_RPAREN){
            gl();
        }

        else{
            E();

            if (c_type == LEX_RPAREN){
                gl();
            }

            else{
                throw curr_lex;
            }
        }

        S();
    }

    else if (c_type == LEX_GOTO){
        //сделать полиз!
        gl();

        if (c_type == LEX_POINTER){
            gl();
        }

        else{
            throw curr_lex;
        }

        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
        }
    }

    else if (c_type == LEX_OPEN){
        gl();
        B();

        if (c_type == LEX_CLOSE){
            gl();
        }

        else{
            throw curr_lex;
        }
    }

    else if (c_type == LEX_ID){
        check_id();
        prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
        gl();

        if (c_type == LEX_ASSIGN){
            gl();
            E();
            eq_type();
            prog.put_lex(Lex(LEX_ASSIGN));
        }

        else{
            throw curr_lex;
        }
    }

    else if (c_type == LEX_POINTER){
        //сделать полиз
        //check_pointer();
        gl();

        if (c_type == LEX_COLON){
            gl();
            S();
        }

        else{
            throw curr_lex;
        }
    }

    else{
        return 0;
    }

    return 1;
}


void Parser::E () 
{
  E1();
  if ( c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
       c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
  {
    st_lex.push (c_type);
    gl(); 
    E1(); 
    check_op();
  }
}
 
void Parser::E1 ()
{
  T();
  while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR)
  {
    st_lex.push (c_type);
    gl();
    T();
    check_op();
  }
}
 
void Parser::T ()
{
  F();
  while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND)
  {
    st_lex.push (c_type);
   gl();
   F();
   check_op();
  }
}
 
void Parser::F () 
{
  if ( c_type == LEX_ID ) 
  {
    check_id();
    prog.put_lex (Lex (LEX_ID, c_val));
    gl();
  }
  else if ( c_type == LEX_NUM ) 
  {
    st_lex.push ( LEX_INT );
    prog.put_lex ( curr_lex );
    gl();
  }
  else if (c_type == LEX_STR){
    st_lex.push (LEX_STR);
    prog.put_lex ( curr_lex );
    gl();
  }
  else if ( c_type == LEX_TRUE ) 
  {
    st_lex.push ( LEX_BOOL );
    prog.put_lex (Lex (LEX_TRUE, 1) );
    gl();
  }
  else if ( c_type == LEX_FALSE)
  {
    st_lex.push ( LEX_BOOL );
    prog.put_lex (Lex (LEX_FALSE, 0) );
    gl();
  }
  else if (c_type == LEX_NOT) 
  {
    gl(); 
    F(); 
    check_not();
  }
  else if ( c_type == LEX_LPAREN ) 
  {
    gl(); 
    E();
    if ( c_type == LEX_RPAREN)
      gl();
    else 
      throw curr_lex;
  }
  else 
    throw curr_lex;
}
 
////////////////////////////////////////////////////////////////
 
void Parser::dec ( type_of_lex type ) 
{
  int i;
  while ( !st_int.is_empty()) 
  {
    i = st_int.pop();
    if ( TID[i].get_declare() ) 
      throw "twice";
    else 
    {
      TID[i].put_declare();
      TID[i].put_type(type);
    }
  }
}

void Parser::check_id () 
{
  if ( TID[c_val].get_declare() )
    st_lex.push ( TID[c_val].get_type() );
  else 
    throw "not declared";
}
 
void Parser::check_op () 
{
  type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
 
  t2 = st_lex.pop();
  op = st_lex.pop();
  t1 = st_lex.pop();
  if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH)
    r = LEX_INT;
  if (op == LEX_OR || op == LEX_AND)
    t = LEX_BOOL;
  if (t1 == t2  &&  t1 == t) 
    st_lex.push(r);
  else
    throw "wrong types are in operation";
  prog.put_lex (Lex (op) );
}
 
void Parser::check_not () 
{
  if (st_lex.pop() != LEX_BOOL)
    throw "wrong type is in not";
  else 
  {
    st_lex.push (LEX_BOOL);
    prog.put_lex (Lex (LEX_NOT));
  }
}
 
void Parser::eq_type () 
{
  if (st_lex.pop() != st_lex.pop())
    throw "wrong types are in :=";
}
 
void Parser::eq_bool () 
{
  if ( st_lex.pop() != LEX_BOOL )
    throw "expression is not boolean";
}
 
void Parser::check_id_in_read ()
{
  if ( !TID [c_val].get_declare() )
    throw "not declared";
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
    Parser pars(argv[1]);
    try{
        pars.analyze();


        cout << "pars success" << endl;

        cout << "printing idents" << endl;
        for (int i = 0; i < (TID.get_top()); i++){
            cout << TID[i].get_name() << endl;
        }
        cout << "printing pointers" << endl;
        for (int i = 0; i < (TPD.get_top()); i++){
            cout << TPD[i].get_name() << endl;
        }
    }
    catch(char c){
        cout << "error\n" << c << endl;
    }
    
    catch(char const *c){
        cout << c << endl;
    }
    
    catch(Lex c){
        cout << "error on lexem: " << c << endl;
    }
    
    catch(...){
        cout << "bad error" << endl;
    }
    
    return 0;
}
