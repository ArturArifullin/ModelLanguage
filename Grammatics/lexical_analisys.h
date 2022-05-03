#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>
#ifndef GRAMMATICS_LEXICAL_ANALISYS_H
#define GRAMMATICS_LEXICAL_ANALISYS_H


enum type_of_lex {
    LEX_NULL,                                                                                   /* 0*/
    LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,        /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,  /*18*/
    LEX_CASE, LEX_OF, LEX_FOR, LEX_CONTINUE, LEX_STRING,  LEX_STEP, LEX_UNTIL, LEX_WRITESTR,    /*26*/
    LEX_FIN,                                                                                    /*27*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   /*35*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_PERCENT, /*44*/
    LEX_NUM,                                                                                    /*45*/
    LEX_ID,                                                                                     /*46*/ //user's ident
    POLIZ_LABEL,                                                                                /*47*/
    POLIZ_ADDRESS,                                                                              /*48*/
    POLIZ_GO,                                                                                   /*49*/
    POLIZ_FGO,                                                                                  /*50*/
    POLIZ_DUP,                                                                                  /*51*/
    POLIZ_CASE_END,                                                                             /*52*/
};

class Lex {
    type_of_lex   t_lex;
    long           v_lex;
public:
    Lex ( type_of_lex t = LEX_NULL, int v = 0 ): t_lex (t), v_lex (v)  { } // LEX_NULL - empty lex
    type_of_lex  get_type () const {
        return t_lex;
    }
    long get_value () const {
        return v_lex;
    }
    friend std::ostream & operator<< ( std::ostream &s, Lex l ); // just for debugging
};

class Ident { // during lex analysis, we'll have table of ident (TID), which we can use in future
    std::string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    long          value;
    std::string probably_string;
public:
    Ident() {
        declare = false;
        assign  = false;
    }
    bool operator== ( const std::string& s ) const {
        return name == s;
    }
    Ident ( const std::string n ) {
        name    = n;
        declare = false;
        assign  = false;
    }
    std::string get_name () const {
        return name;
    }
    bool get_declare () const {
        return declare;
    }
    void put_declare () {
        declare   = true;
    }
    type_of_lex get_type () const {
        return type;
    }
    void put_type ( type_of_lex t ) {
        type      = t;
    }
    bool get_assign () const {
        return assign;
    }
    void put_assign () {
        assign    = true;
    }
    long  get_value () const {
        return value;
    }
    void put_value ( long v ) {
        value     = v;
    }
    void string_console(){
        std::getline(std::cin, probably_string);
    }
    const char* get_link_str(){
        return probably_string.c_str();
    }
};

int put ( const std::string & buf );
extern std::vector<Ident> TID;

class Scanner {
    FILE * fp; //fiie, which has text of programm
    char   c;
    int look ( const std::string buf, const char ** list ) { //looking for "buf" in TW
        int i = 0;
        while ( list[i] ) {
            if ( buf == list[i] )
                return i;
            ++i;
        }
        return 0;
    }
    void gc () {
        c = fgetc (fp); // read following sympol into private char c
    }
public:
    static const char * TW [], * TD [];
    Scanner ( const char * program ) {
        if ( !(fp = fopen ( program, "r" )) )
            throw  "can’t open file" ;
    }
    Lex get_lex ();
};

//@ - symbol which use as end of a finite state machine

#endif //GRAMMATICS_LEXICAL_ANALISYS_H
