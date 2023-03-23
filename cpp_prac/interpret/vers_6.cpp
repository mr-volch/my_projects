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
    const char *to_print[47] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct",
    "not", "or", "program", "read", "true", "break", "while", "goto",
    "for", "write", "fin", ";", ",", ":", ".", "=", "(", ")", "==", "<",
    ">", "+", "-", "*", "/", "<=", "!=", ">=", "num", "str", "id", "pointer", "{", "}", "PL", "PA", "PG", "PF"};
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
    void  set_type (type_of_lex t)  { t_lex = t; }
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

int struct_area = 1;

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
     Ident *fields;
     type_of_lex field_types[20];
     int f_top;
     int struct_number;
public:
    Ident() { declare = false; assign = false; str_val = NULL; name = NULL;
        //fields = new Ident[100], field_types = new int[100];
         f_top = 0;
         fields = NULL;}
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
    
    void set_struct_number(int n){struct_number = n;}
    int get_struct_number(){return struct_number;}

    char * get_str_value(){return str_val;}
    void change_str_value(char *n){
        if (str_val != NULL){
            delete []str_val;
        }
        str_val = new char [strlen(n)+1];
        memmove(str_val, n, strlen(n) + 1);
    }
    
    int put_field (const char *buf, type_of_lex cur_type_0);
    int get_f_top(){return f_top;}
    
    int field_search(const char *buf){
        for (int jil = 0; jil < f_top; jil++){
            if (strcmp(buf, fields[jil].get_name())){
                return 0;
            }
        }   
        return 1;
    }
             

    ~Ident(){
        if (str_val != NULL){
            delete []str_val;
        }
        
        if (fields != NULL){
            delete []fields;
        }

        delete []name;
    }
    
    void print_fields(){
        for (int ika = 0; ika < f_top; ika++){
            cout << fields[ika].get_name() << endl;
        }
    }
    
    char * get_field(int ihi){
        return fields[ihi].get_name();
    } 
    
    type_of_lex get_field_type(int ihi){
        return field_types[ihi];
    }
    
    
};

int Ident::put_field ( const char *buf, type_of_lex cur_type_0){
    if (fields == NULL){
        //cout << "A0" << endl;
        fields = new Ident[20];
    }
    for ( int j = 0; j < f_top; j++ ){
        if ( !strcmp ( buf, fields[j].get_name() ) ){
            return j;
        }
    }
    //cout << "A1" << endl;
    fields[f_top].put_name(buf);
    //cout << "A2" << endl;
    field_types[f_top] = cur_type_0;
    //cout << "A3" << endl;
    ++f_top;
    return f_top-1;
}


class Tabl_ident{
    Ident *p;
    int size;
    int top;
public:
    Tabl_ident ( int max_size ){
        p = new Ident [ size = max_size ];
        top = 0;
    }
    ~Tabl_ident () { delete []p;}
    int get_top() {return top;}
    Ident & operator[] ( int k ) { return p[k]; }
    int put ( const char *buf );
    //int put_field (const char *buf);
    int struct_search (const char *buf);
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

int Tabl_ident::struct_search ( const char *buf ){
    for ( int j = 0; j < top; j++ ){
        if ( !strcmp ( buf, p[j].get_name() ) ){
            return j;
        }
    }
    return -1;
}

class Scanner
{
    enum state { H, IDENT, DOT_IDENT, NUMB, PCOM, COM, STR, ALE, DELIM, NEQ};
    static const char *TW[];
    static type_of_lex  words[];
    static const char *TD[];
    static type_of_lex  dlms  [];
    state CS;
    FILE * fp;
    char c;
    int last_goto;
    int struct_num;
    int in_struct;
    type_of_lex curr_field_type;
    char buf [80];
    char tmp_buf[80];
    int  buf_top;
    int tmp_top;
    void clear(){
        buf_top = 0;
        tmp_top = 0;
        for ( int j = 0; j < 80; j++ ){
             buf[j] = '\0';
             tmp_buf[j] = '\0';
        }
    }

    void add(){
        buf [ buf_top++ ] = c;
        if (buf_top == 79){
            throw "too long id";
        }
    }
    
    void add_tmp(){
        buf[ tmp_top++ ] = c;
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
        struct_num = 0;
        in_struct = 0;
        CS = H;
        clear();
        gc();
    }
    
    void set_struct(int sopi){
        //cout << "SOPI: " << sopi << endl;
        struct_num = sopi;
    }
    
    void set_type(type_of_lex sopt){
        //cout << "SOPT: " << sopt << endl;
        curr_field_type = sopt;
    }

    ~Scanner(){
        pclose(fp);
    }
};

int struct_var = -1;
int after_struct = 0;
int in_ops = 0;

const char *Scanner::TW[] = {"", "and", "bool", "else", "if", "false", "int", "string", "struct", "not", "or", "program", "read", "true", "break", "while", "goto", "for", "write", NULL};
const char *Scanner::TD[] = {"", "@", ";", ",", ":", ".", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{", "}", NULL};

Lex_arr lex_collection;

Tabl_ident TID(100);
Tabl_ident TPD(100);
Tabl_ident TSD(100);
Tabl_ident TND(100);

type_of_lex Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BOOL, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING, LEX_STRUCT, LEX_NOT,
    LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_BREAK, LEX_WHILE, LEX_GOTO, LEX_FOR, LEX_WRITE, LEX_NULL};
type_of_lex Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN,
    LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_OPEN, LEX_CLOSE, LEX_NULL};


Lex Scanner::get_lex (){

    int d, j;
    char *cur;
    char *cur_tmp_1 = new char[160];
    for (int poj = 0; poj < 160; poj++){
        cur_tmp_1[poj] = '\0';
    }
    CS = H;
    do{
      switch(CS){
          case H:
              if (last_goto != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t'){
                  //cout << "MODIFIED-" << endl;
                  last_goto--;
              }
              
              if (in_struct != 0 && c != ' ' && c != '\n' && c != '\r' && c != '\t'){
                  //cout << "MODIFIED-" << endl;
                  in_struct--;
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
                  delete []cur_tmp_1;
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
              if ( isalpha(c) || isdigit(c) || (c == ':') || (c == '.')){
                  if (c == ':'){
                      if (j = look(buf, TW)){
                          delete []cur_tmp_1;
                          throw ("Error: code word as a pointer");
                      }

                      else{
                          j = TPD.put(buf);
                          delete []cur_tmp_1;
                          return Lex (LEX_POINTER, j);
                      }
                  }
                  
                  if (c == '.'){
                      if (in_ops){
                          CS = DOT_IDENT;
                      }
                      else{
                          delete []cur_tmp_1;
                          throw "wrong id name";
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
                  
                  else if ((!strcmp(buf, "struct")) && (struct_area)){
                      //cout << "INSTRUCT" << endl;
                      in_struct = 3;
                  }
                  
                  else if (((!strcmp(buf, "int")) || (!strcmp(buf, "bool"))
                       || (!strcmp(buf, "string"))) && (in_struct == 1)){
                       //cout << "OH MY " << buf << endl;
                       set_type(words[j]);
                       in_struct++;
                  }                
                  
                  //cout << "INKGHJHGJHG" << in_struct << endl;
                  delete []cur_tmp_1;
                  return Lex (words[j], j);
              }
              else{
                  //cout << "CUR GOTO" << last_goto << endl;
                  if (last_goto != 0){
                      j = TPD.put(buf);
                      delete []cur_tmp_1;
                      return Lex(LEX_POINTER, j);
                  }
                  
                  if (in_struct == 2){
                      j = TSD.put(buf);
                      set_struct(j);
                      delete []cur_tmp_1;
                      return Lex(LEX_ID, j);
                  }
                  
                  else if (in_struct == 1){
                      //cout << "BEST" << TSD.get_top() << endl;
                      
                      j = TSD[struct_num].put_field(buf, curr_field_type);
                      in_struct++;
                      //cout << "BESTBEST" << endl;
                      delete []cur_tmp_1;
                      return Lex(LEX_ID, j);
                  }
                  
                  if (struct_var != -1){
                      int first_j = -1;
                      j = TND.put(buf);
                      TND[j].set_struct_number(struct_var);    
                      for (int jbl = 0; jbl < TSD[struct_var].get_f_top(); jbl++){
                          //char cur_tmp_1[160];
                          //cout << "FRIENDLY0" << endl;
                          strcat(cur_tmp_1, buf);
                          //cout << "FRIENDLY1" << endl;
                          strcat(cur_tmp_1, ".");
                          //cout << "FRIENDLY2" << endl;
                          strcat(cur_tmp_1, TSD[struct_var].get_field(jbl));
                          j = TID.put(cur_tmp_1);
                          
                          for (int poj = 0; poj < 160; poj++){
                              cur_tmp_1[poj] = '\0';
                          }
                          if (first_j == -1){
                              first_j = j;
                          }
                      }
                      delete []cur_tmp_1;
                      return Lex (LEX_ID, first_j);
                  }
                  
                  if (in_ops){
                      j = TSD.struct_search(buf);
                      if (j==-1){
                          j = TID.put(buf);
                      }
                      
                      else{
                          j = TSD.put(buf);
                          in_ops = 2;
                      }
                  }
                  
                  else{ 
                      if (after_struct == 0){              
                          j = TID.put(buf);
                      }
                      else{
                          j = TSD.put(buf);
                      }
                      delete []cur_tmp_1;
                      return Lex (LEX_ID, j);
                  }
              }
              break;
          case DOT_IDENT:
              if ( isalpha(c) || isdigit(c) ){
                  add();
                  gc();
              }
              
              else {
                  j = TID.put(buf);
                  delete []cur_tmp_1;
                  return Lex (LEX_ID, j);
              }
              break;    
          case NUMB:
              if ( isdigit(c) ){
                  d = d * 10 + (c - '0'); gc();
              }
              else{
                  delete []cur_tmp_1;
                  return Lex ( LEX_NUM, d);
              }
              break;
          case STR:
              if (c == '"'){
                  gc();
                  cur[d] = '\0';
                  d++;
                  delete []cur_tmp_1;
                  return Lex ( LEX_STR, d, cur);
              }

              else if (c == '@'){
                  delete []cur;
                  delete []cur_tmp_1;
                  throw c;
              }
              if (d == 256){
                  delete []cur;
                  delete []cur_tmp_1;
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
                  delete []cur_tmp_1;
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
                  delete []cur_tmp_1;
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
                  delete []cur_tmp_1;
                  return Lex ( dlms[j], j);
              }
              else{
                  j = look ( buf, TD );
                  delete []cur_tmp_1;
                  return Lex ( dlms[j], j );
              }
              break;
          case NEQ:
              if (c == '='){
                  add();
                  gc();
                  j = look ( buf, TD );
                  delete []cur_tmp_1;
                  return Lex ( LEX_NEQ, j );
              }
              else{
                  delete []cur_tmp_1;
                  throw '!';
              }
              break;
          case DELIM:
              clear();
              add();
              if (j = look ( buf, TD)){
                  //cout << "CHECKING VERY " << dlms[j] << " " << in_struct << endl;
                  if (((dlms[j] == LEX_SEMICOLON) ||
                      (dlms[j] == LEX_COMMA) || (dlms[j] == LEX_OPEN)) && (in_struct ==1)){
                      in_struct++;
                  }
                  gc();
                  delete []cur_tmp_1;
                  return Lex ( dlms[j], j );
              }
              else{
                  delete []cur_tmp_1;
                  throw c;
              }
              break;
      }//end switch
    } while (true);
}


class Poliz
{
         Lex        * p;
         int        **br_mas;
         int        **point_mas;
         int         *init_point_mas;
         int          size;
         int          free;
         int       br_free;
         int        p_free;
public:
                      Poliz (int max_size)
                      {
                        p = new Lex [size = max_size];
                        br_mas = new int* [size];
                        point_mas = new int* [size];
                        for (int i = 0; i < size; i++){
                            br_mas[i] = new int[2];
                            point_mas[i] = new int[2];
                        }
                        
                        init_point_mas = new int[size];
                        cout << "get_top " << TPD.get_top() << endl;                        
                        free = 0;
                        br_free = 0;
                        p_free = 0;
                      }
                     ~Poliz() { 
                         delete [] p;
                         for (int i = 0; i < size; i++){
                             delete []br_mas[i];
                             delete []point_mas[i];
                         }
                         delete []br_mas;
                         delete []point_mas;
                         delete []init_point_mas;
                     }
         void        put_lex ( Lex l )
                      {
                        p [free] = l;
                        free++;
                      }
         void         put_lex ( Lex l, int place) {
            p [place] = l;
            //cout << place;
         }
         void         blank    () { free++; }
         
         void add_break(int cur_place, int cur_while){
             br_mas[br_free][0] = cur_place;
             br_mas[br_free][1] = cur_while;
             br_free++;
         }
         
         void init_break(int cur_place, int cur_while){
             for (int i = 0; i < br_free; i++){
                 if (br_mas[i][1] == cur_while){
                     put_lex(Lex(POLIZ_LABEL, cur_place), br_mas[i][0]);
                 }
             }
         } 
         
         void add_pointer (int cur_place, int cur_val){
             point_mas[p_free][0] = cur_place;
             point_mas[p_free][1] = cur_val;
             p_free++;
         }
         
         void init_pointer (int cur_place, int cur_val){
             cout << "initializing pointer " << cur_val << endl;
             init_point_mas[cur_val] = cur_place;
         }
         
         void make_pointers(){
             //cout << "make pointers, get_top: " << TPD.get_top() << "p_free " << p_free << endl;
             for (int i = 0; i < TPD.get_top(); i++){
                 for (int j = 0; j < p_free; j++){
                     if (point_mas[j][1] == i){
                         put_lex(Lex(POLIZ_LABEL, init_point_mas[i]), point_mas[j][0]);
                     }
                 }
             }
         } 
             
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
                          cout << i << ": " << p[i] << endl;
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
  if ( !is_empty() ){
    //cout << "POPPING: " << typeid(T).name() << endl;
    //cout << s[--top] << endl;
    return s[--top];
  }
  else
    throw "stack_is_empty";
}

class Parser
{
         Lex          curr_lex;
         type_of_lex  c_type;
         int          c_val;
         int          in_while;
         int          id_crash;
         int          struct_assign;
         Scanner      scan;
         Stack < int, 100 > st_int;
         Stack < type_of_lex, 100 >  st_lex;
         void         P();
         void         STRUCT();
         void         STRUCT0();
         void         FIELD();
         void         SV();
         void         D1();
         int          D2();
         void         V(type_of_lex);
         void         B();
         int         S();
         void          E();
         void         E0();
         void         E1();
         void         T();
         void         F();
         void         dec ( type_of_lex type);
         void         dec_special();
         void         dec_p();
         void         check_id ();
         void         check_p ();
         void         check_op ();
         void         check_not ();
         type_of_lex  eq_type ();
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
  cout << "printing POLIZ" << endl;
  prog.print();
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
    
    STRUCT();

    struct_area = 0;
    D1();
    cout << "launching operators" << endl;
    in_ops = 1;
    B();

    if (c_type == LEX_CLOSE){
        gl();
    }

    else{
        throw curr_lex;
    }

    if (c_type != LEX_FIN){
        throw curr_lex;
    }
    
    check_p();
    prog.make_pointers();
}

void Parser::STRUCT(){
    while (c_type == LEX_STRUCT){
        gl();
        STRUCT0();
        
        if (c_type == LEX_SEMICOLON){
            gl();
        }
        
        else{
            throw curr_lex;
        }
    }
}

void Parser::STRUCT0(){
    if (c_type == LEX_ID){
        Lex main_lex = curr_lex;
        gl();
        
        if (c_type != LEX_OPEN){
            throw curr_lex;
        }
        
        gl();
        while ((c_type == LEX_INT) || (c_type == LEX_STRING) || (c_type == LEX_BOOL)){
            FIELD();
            if (c_type != LEX_SEMICOLON){
                throw curr_lex;
            }
            
            gl();
        }
        
        if (c_type != LEX_CLOSE){
            throw curr_lex;
        }
        
        gl();
    }
    
    else{
        throw curr_lex;
    }
}
        
void Parser::FIELD(){
    gl();
    
    SV();

    while (c_type == LEX_COMMA){
        gl();
        SV();
    }
}

void Parser::SV(){
    if (c_type != LEX_ID){
        throw curr_lex;
    }
    gl();
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
    
    else if (c_type == LEX_STRUCT){
         after_struct = 1;
         gl();
         after_struct = 0;
         if (c_type == LEX_ID){
             struct_var = TSD.struct_search(TSD[c_val].get_name());
             if (struct_var == -1){
                 throw "Error: bad structure type";
             }
             
             gl();
             
             if (c_type != LEX_ID){
                 throw curr_lex;
             }
             
             for (int jbl = 0; jbl < TSD[struct_var].get_f_top(); jbl++){
                 st_int.push(c_val + jbl);
                 dec(TSD[struct_var].get_field_type(jbl));
             }
             
             gl();
             
             while (c_type == LEX_COMMA){
                 gl();
                 if (c_type != LEX_ID){
                     throw curr_lex;
                 }
                 for (int jbl = 0; jbl < TSD[struct_var].get_f_top(); jbl++){
                     st_int.push(c_val + jbl);
                     dec(TSD[struct_var].get_field_type(jbl));
                 }
             
                 gl();
             }
             
             struct_var = -1;
             return 1;
         }
         
         else{
             throw curr_lex;
         }
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
                
                else if (c_type == LEX_MINUS){
                    gl();
                    if (c_type == LEX_NUM){
                        prog.put_lex(Lex (LEX_ASSIGN));
                    }
                    
                    else{
                        throw curr_lex;
                    }
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
        gl();
        if (c_type == LEX_LPAREN){
            gl();
        }

        else{
            throw curr_lex;
        }
        
        in_while++;
        pl0=prog.get_free();
        E();
        eq_bool();
        
        pl1=prog.get_free();
        prog.blank();
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
        prog.init_break(prog.get_free(), in_while);
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
            prog.put_lex(Lex(LEX_WRITE));
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

        prog.add_break(prog.get_free(), in_while);
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
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


        //отладить на проверку на логический тип
        pl0 = prog.get_free();
        E();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        
        pl3 = prog.get_free();

        if (c_type == LEX_SEMICOLON){
            gl();
        }

        else{
            throw curr_lex;
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
        
        prog.put_lex(Lex(POLIZ_LABEL, pl0));
        prog.put_lex(POLIZ_GO);
        
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);

        S();
        prog.put_lex(Lex(POLIZ_LABEL, pl3));
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        
        prog.init_break(prog.get_free(), in_while);
        in_while--;
    }

    else if (c_type == LEX_GOTO){
        gl();

        if (c_type == LEX_POINTER){
            prog.add_pointer(prog.get_free(), c_val);
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO)); 
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
            cout << "closed sostav" << endl;
            gl();
        }

        else{
            throw curr_lex;
        }
    }

    /*else if (c_type == LEX_ID){
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
        
        if (c_type == LEX_SEMICOLON){
            gl();
        }
        
        else{
            throw curr_lex;
        }
    }*/

    else if (c_type == LEX_POINTER){
        //сделать полиз
        prog.init_pointer(prog.get_free(), c_val);
        dec_p();
        gl();

        if (c_type == LEX_COLON){
            gl();
            S();
        }

        else{
            throw curr_lex;
        }
        
    }
    
    else if ((c_type == LEX_ID) || (c_type == LEX_NUM) || (c_type == LEX_STR)
    || (c_type == LEX_FALSE) || (c_type == LEX_TRUE) || (c_type == LEX_NOT) || (c_type == LEX_LPAREN)){
        E();
        
        if (c_type == LEX_SEMICOLON){
            gl();
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

int first_of_e = 2;

void Parser::E(){
    id_crash = 0;
    E0();
    type_of_lex cur_t_lex_1 = LEX_NULL;
    
    int amount_eq = 0;
    //cout << "CUR LEX" << curr_lex << endl;
    while (c_type == LEX_ASSIGN){
        if (id_crash == 1){
            throw "error: trying to assign meaning to expression";
        }
        
        //cout << "POLIZ " << prog[prog.get_free()-1] << endl;
        prog[prog.get_free()-1].set_type(POLIZ_ADDRESS);
        //cout << "POLIZ " << prog[prog.get_free()-1] << endl;
        gl();
        E0();
        if (cur_t_lex_1 != LEX_NULL){
            st_lex.push(cur_t_lex_1);
        }
        cur_t_lex_1 = eq_type();
        amount_eq++;
    }
    
    while (amount_eq != 0){
        //cout << "POLIZ " << Lex(LEX_ASSIGN);
        prog.put_lex(Lex (LEX_ASSIGN));
        amount_eq--;
    }
}

void Parser::E0 () 
{
  E1();
  if ( c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
       c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
  {
    id_crash = 1;
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
    id_crash = 1;
    st_lex.push (c_type);
    gl();
    T();
    check_op();
  }
}
 
void Parser::T ()
{
  F();
  first_of_e--;
  while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND){
    id_crash = 1;
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
    if (in_ops == 2){
        if (first_of_e == 2){
            prog.put_lex(Lex(LEX_STRUCT));
        }
        
        else if (first_of_e == 1){
            first_of_e++;
        }
        
        else{
            throw "wrong expression with struct var";
        }

        prog.put_lex (Lex (LEX_ID, c_val));
        in_ops = 1;
        gl();        
    }
    
    else{
        check_id();
        prog.put_lex (Lex (LEX_ID, c_val));
        gl();
    }
  }
  else if ( c_type == LEX_NUM ) 
  {
    id_crash = 1;
    st_lex.push ( LEX_INT );
    prog.put_lex ( curr_lex );
    gl();
  }
  else if (c_type == LEX_STR){
    id_crash = 1;
    st_lex.push (LEX_STRING);
    prog.put_lex ( curr_lex );
    gl();
  }
  else if ( c_type == LEX_TRUE ) 
  {
    id_crash = 1;
    st_lex.push ( LEX_BOOL );
    prog.put_lex (Lex (LEX_TRUE, 1) );
    gl();
  }
  else if ( c_type == LEX_FALSE)
  {
    id_crash = 1;
    st_lex.push ( LEX_BOOL );
    prog.put_lex (Lex (LEX_FALSE, 0) );
    gl();
  }
  else if (c_type == LEX_NOT) 
  {
    id_crash = 1;
    gl(); 
    F(); 
    check_not();
  }
  else if ( c_type == LEX_LPAREN ) 
  {
    id_crash = 0;
    gl(); 
    E();
    if ( c_type == LEX_RPAREN){
      id_crash = 1;
      gl();
    }
    
    else{
        throw curr_lex;
    }
  }
  else if (c_type == LEX_MINUS){
      id_crash = 1;
      
      gl();
      
      if (c_type == LEX_ID){
          check_id();
          prog.put_lex (Lex (LEX_ID, c_val));
          gl();
      }
      
      else if (c_type == LEX_NUM){
          st_lex.push ( LEX_INT );
          //prog.put_lex ( curr_lex );
          gl();
      }
      
      else{
          throw curr_lex;
      }
  }

  else 
    throw curr_lex;
}
 
////////////////////////////////////////////////////////////////

void Parser::dec_p()
{
    if (TPD[c_val].get_declare()){
        throw "pointer twice";
    }
    else{
        TPD[c_val].put_declare();
    }
}

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
  if ( TID[c_val].get_declare() ){
      //cout << "printing type of id: " << TID[c_val].get_type() << endl;
      st_lex.push ( TID[c_val].get_type() );
  }
  else 
      throw "not declared";
}


void Parser::check_p ()
{
    for (int i = 0; i < (TPD.get_top()); i++){
        if ((TPD[i].get_declare()) == 0){
            throw "pointer not declared";
        }
    }
}
    
 
void Parser::check_op () 
{
  type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL, pt = LEX_STRING;
 
  t2 = st_lex.pop();
  op = st_lex.pop();
  t1 = st_lex.pop();
  //cout << "types: " << t1 << " " << t2 << endl;
  
  if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH){
    r = LEX_INT;
    if (op != LEX_PLUS){
        pt = LEX_INT;
    }
  }
  if (op == LEX_OR || op == LEX_AND){
    t = LEX_BOOL;
    pt = LEX_BOOL;
  }
  if (t1 == t2){
    if (t1 == t){
        st_lex.push(r);
    }
        
    //случай конкатиенации строк
    else if (t1 == pt){
        if (op == LEX_PLUS){
            st_lex.push(LEX_STRING);
        }
        else{
            st_lex.push(LEX_BOOL);
        }
    }
    
    else{
        //cout << t1 << ' ' << pt << ' ' << t << ' ' << op;
        throw "wrong operation";
    }
  }
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
 
type_of_lex Parser::eq_type () 
{
  type_of_lex tmp_t_lex_1;
  if (st_lex.pop() != (tmp_t_lex_1 = st_lex.pop())){
    //cout << "error on = with lexem of type" << tmp_t_lex_1 << " ";
    throw "wrong types are in =";
  }
  return tmp_t_lex_1;
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

class Executer
{
         Lex          pc_el;
public:
         void         execute ( Poliz & prog );
};
 
void Executer::execute ( Poliz & prog )
{
  Stack < int, 200 > args;
  Stack <int, 200> t_args;
  Stack < char *, 200> s_args;
  int i, j, a, index = 0, size = prog.get_free();
  char *ii;
  char **clear_arr = new char*[100];
  int clear_ind = 0;
  while ( index < size )
  {
    pc_el = prog [ index ];
    //cout << "INDEX " << index << endl;
    switch ( pc_el.get_type () )
    {
      case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
        args.push ( pc_el.get_value () );
        t_args.push(0);
        s_args.push(NULL);
        break;
      case LEX_STR:
        args.push(0);
        t_args.push(1);
        cout << "CUR: " << prog[index].get_str_value() << endl;

        s_args.push(prog[index].get_str_value());
        //cout << "CURRR: " << s_args.pop() << endl;
        //s_args.push(pc_el.get_str_value());
        break;
      case LEX_ID:
        i = pc_el.get_value ();
        if ( TID[i].get_assign () )
        {
          //cout << "LEX " << TID[i].get_type() << endl;
          if (TID[i].get_type() == LEX_STRING){
              args.push (0);
              if (TID[i].get_str_value() == NULL){
                  cout << "val 0" << endl;
              }
              s_args.push(TID[i].get_str_value());
              cout << "CURR: " << TID[i].get_str_value() << endl;
              t_args.push(1);
              break;
          }
              
          args.push ( TID[i].get_value () );
          s_args.push(NULL);
          t_args.push(0);
          break;
        }
        else
          throw "POLIZ: indefinite identifier";
      case LEX_NOT:
        args.push( !args.pop() );
        break;
      case LEX_OR:
        i = args.pop();
        t_args.pop();
        s_args.pop();
        args.push ( args.pop() || i );
        break;
      case LEX_AND:
        i = args.pop();
        t_args.pop();
        s_args.pop();
        args.push ( args.pop() && i );
        break;
      case POLIZ_GO:
        index = args.pop() - 1;
        t_args.pop();
        s_args.pop();
        break;
      case POLIZ_FGO:
        i = args.pop();
        if ( !args.pop() ) index = i-1;
        t_args.pop();
        t_args.pop();
        s_args.pop();
        s_args.pop();
        break;
      case LEX_WRITE:
        if (t_args.pop()){
            cout << "WRITING: " << endl;
            cout << s_args.pop() << endl;
            args.pop();
            //cout << "WRITING: " << endl;
            break;
        }
        cout << args.pop () << " n" <<  endl;
        s_args.pop();
        break;
      case LEX_READ:
        {
          //cout << "READING: " << endl;
          int is_s = 0;
          int k;
          char *cstr = new char[257];
          i = args.pop ();
          t_args.pop();
          s_args.pop();
          if ( TID[i].get_type () == LEX_INT )
          {
            cout << "Input int value for " << TID[i].get_name () << endl;
            cin >> k;
          }
          else if ( TID[i].get_type () == LEX_STRING){
              cout << "Input string value for " <<
                  TID[i].get_name() << endl;
              cin >> cstr;
              is_s = 1;
          }
          else
          {
            char j[20];
            rep:
            cout << "Input boolean value (true or false) for " << TID[i].get_name() << endl;
            cin >> j;
            if (!strcmp(j, "true"))
              k = 1;
            else
              if (!strcmp(j, "false"))
                k = 0;
              else
              {
                cout << "Error in input:true/false" << endl;
                goto rep;
              }
          }
          
          if (is_s){
              TID[i].change_str_value(cstr);
              delete []cstr;
              TID[i].put_value (0);
              TID[i].put_assign();
              break;
          }
           
          delete []cstr;   
          TID[i].put_value (k);
          TID[i].put_assign ();
          break;
        }
      case LEX_PLUS:
        if (t_args.pop()){
            char *bb = s_args.pop();
            char *aa = s_args.pop();
            
            char *cc = new char[strlen(aa) + strlen(bb) + 1];
            memmove(cc, aa, strlen(aa));
            memmove(cc+strlen(aa), bb, strlen(bb) + 1);
            s_args.push(cc);
            clear_arr[clear_ind++] = cc;
            args.pop();
            break;
        }
        args.push ( args.pop() + args.pop() );
        s_args.pop();
        break;
      case LEX_TIMES:
        args.push ( args.pop() * args.pop() );
        t_args.pop();
        s_args.pop();
        break;
      case LEX_MINUS:
        i = args.pop();
        args.push ( args.pop() - i );
        t_args.pop();
        s_args.pop();
        break;
      case LEX_SLASH:
        i = args.pop();
        if (!i)
        {
          args.push(args.pop() / i);
          t_args.pop();
          s_args.pop();
          break;
        }
        else
          throw "POLIZ:divide by zero";
      case LEX_EQ:
        if (t_args.pop()){
            a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            
            args.push(a==0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        args.push ( args.pop() == args.pop() );
        s_args.pop();
        break;
      case LEX_LSS:
        if (t_args.pop()){
            a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            
            args.push(a<0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        i = args.pop();
        args.push ( args.pop() < i );
        s_args.pop();
        break;
      case LEX_GTR:
        if (t_args.pop()){
            a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            args.push(a>0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        i = args.pop();
        args.push ( args.pop() > i );
        s_args.pop();
        break;
      case LEX_LEQ:
        if (t_args.pop()){
            a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            
            args.push(a<=0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        i = args.pop();
        args.push ( args.pop() <= i );
        s_args.pop();
        break;
      case LEX_GEQ:
        if (t_args.pop()){
            a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            args.push(a>=0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        i = args.pop();
        args.push ( args.pop() >= i );
        s_args.pop();
        break;
      case LEX_NEQ:
        if (t_args.pop()){
            int a = strcmp(s_args.pop(), s_args.pop());
            args.pop();
            args.pop();
            t_args.pop();
            args.push(a!=0);
            t_args.push(0);
            s_args.push(NULL);
            break;
        }
        i = args.pop();
        args.push ( args.pop() != i );
        s_args.pop();
        break;
      case LEX_ASSIGN:
        i = args.pop();
        j = args.pop();
        //cout << "INDEX: " << j << "STR: " << ii;
        if (!t_args.pop()){
            TID[j].put_value(i);
            TID[j].put_assign();
            t_args.pop();
            s_args.pop();
            s_args.pop();
            break;
        }
        
        //cout << "STRIG: " << endl;
        //cout << "STRING: " << s_args.pop() << endl;
        TID[j].put_value(0);
        TID[j].change_str_value(ii = s_args.pop());
        cout << "TID VAL: " << ii << endl; 
        s_args.pop();
        TID[j].put_assign();
        t_args.pop();
        break;
      default:
        cout << pc_el << endl;
        throw "POLIZ: unexpected elem";
    }//end of switch
    ++index;
  };//end of while
  cout << "Finish of executing!!!" << endl;
  
  for (int yip = 0; yip < clear_ind; yip++){
      delete []clear_arr[yip];
  }
  
  delete []clear_arr;
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
    cout << "start" << endl;
    Parser pars(argv[1]);
    Executer E;
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
        
        cout << "printing structs" << endl;
        for (int i = 0; i < (TSD.get_top()); i++){
            cout << TSD[i].get_name() << endl;
            cout << "fields: " << endl;
            TSD[i].print_fields();
        }
        
        cout << "printing struct vars" << endl;
        for (int i = 0; i < (TND.get_top()); i++){
            cout << TND[i].get_name() << endl;
        }
        
        int execute_id;
        cout << "DO YOU WANT TO EXECUTE PROGRAM (0/1)?" << endl;
        cin >> execute_id;
        cout << endl;
        
        while(execute_id){
            E.execute (pars.prog);
            cout << "DO YOU WANT TO EXECUTE PROGRAM ONE MORE TIME (0/1)?" << endl;
            cin >> execute_id;
            cout << endl;
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

//сделать динамическмй массив имен переменных структур
//сделать проверку в начале Е на то что id содержится в ней(таблице)
//если содержиться, то только присваивание переменных-структур друг другу
//дополнить присваивания в executerе для структур
