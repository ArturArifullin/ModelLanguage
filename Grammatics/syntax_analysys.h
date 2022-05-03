#include "lexical_analisys.h"
#include <vector>
#include <stack>
#ifndef GRAMMATICS_SYNTAX_ANALYSYS_H
#define GRAMMATICS_SYNTAX_ANALYSYS_H



template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    Lex          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    Scanner      scan;
    std::stack < int >           st_int;
    std::stack < type_of_lex >   st_lex; // for control errors like int + bool
    std::stack < int >           st_loop;// we use it for "continue"
    void  P();
    void  D1();
    void  D();
    void  B();
    void  V();
    void  V1();
    void  S();
    void  E();
    void  E1();
    void  T();
    void  F();
    void  dec ( type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  eq_int();
    void  check_id_in_read ();
    void  gl ();
public:
    std::vector <Lex> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

#endif //GRAMMATICS_SYNTAX_ANALYSYS_H
