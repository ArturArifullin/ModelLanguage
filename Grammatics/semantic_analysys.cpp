#include "syntax_analysys.h"

void Parser::dec ( type_of_lex type ) {
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( TID[i].get_declare () )
            throw "twice";
        else {
            TID[i].put_declare ();
            TID[i].put_type ( type );
        }
    }
}

void Parser::check_id () {
    if ( TID[c_val].get_declare() )
        st_lex.push ( TID[c_val].get_type () );
    else
        throw "noFt declared";
}

void Parser::check_op () {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;

    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );

    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH || op == LEX_PERCENT )
        r = LEX_INT;
    if ( op == LEX_OR || op == LEX_AND )
        t = LEX_BOOL;
    if ( t1 == t2  &&  t1 == t )
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    poliz.push_back (Lex (op) );
}

void Parser::check_not () {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else
        poliz.push_back ( Lex (LEX_NOT) );
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        throw "wrong types are in expression";
    st_lex.pop();
}

void Parser::eq_bool () {
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
}

void Parser::eq_int () {
    if ( st_lex.top() != LEX_INT )
        throw "expression is not int";
    st_lex.pop();
}

void Parser::check_id_in_read () {
    if ( !TID [c_val].get_declare() )
        throw "not declared";
}
