#include "syntax_analysys.h"

void  Parser::gl () {
    curr_lex  = scan.get_lex ();
    c_type    = curr_lex.get_type ();
    c_val     = curr_lex.get_value ();
}

void Parser::analyze () {
    gl ();
    P ();
    if (c_type != LEX_FIN)
        throw curr_lex;

    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    for ( Lex l : poliz )
        std::cout << l;
    std::cout << std::endl << "Yes!!!" << std::endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else
        throw curr_lex;
    D1 ();
    if ( c_type == LEX_SEMICOLON )
        gl ();
    else
        throw curr_lex;
    B ();
}

void Parser::D1 () {
    if ( c_type == LEX_VAR ) {
        gl ();
        D ();
        while ( c_type == LEX_COMMA ) {
            gl ();
            D ();
        }
    }
    else
        throw curr_lex;
}

void Parser::D () {
    if ( c_type != LEX_ID )
        throw curr_lex;
    else {
        st_int.push ( c_val ); //stack for declaring of ident
        gl ();
        while ( c_type == LEX_COMMA ) {
            gl ();
            if ( c_type != LEX_ID )
                throw curr_lex;
            else {
                st_int.push ( c_val );
                gl ();
            }
        }
        if ( c_type != LEX_COLON )
            throw curr_lex;
        else {
            gl ();
            if ( c_type == LEX_INT ) {
                dec ( LEX_INT );
                gl ();
            }
            else if ( c_type == LEX_BOOL ) {
                dec ( LEX_BOOL );
                gl ();
            }
            else if ( c_type == LEX_STRING ){
                dec ( LEX_STRING );
                gl ();
            }
            else
                throw curr_lex;
        }
    }
}

void Parser::B () {
    if ( c_type == LEX_BEGIN ) {
        gl ();
        S ();
        while ( c_type == LEX_SEMICOLON ) {
            gl ();
            S ();
        }
        if ( c_type == LEX_END ) {
            gl ();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}

void Parser::S () {
    int pl0, pl1, pl2, pl3;

    if ( c_type == LEX_IF ) {
        gl ();
        E ();
        eq_bool ();
        pl2 = poliz.size();
        poliz.push_back ( Lex() );
        poliz.push_back ( Lex(POLIZ_FGO) );
        if ( c_type == LEX_THEN ) {
            gl ();
            S ();
            pl3 = poliz.size ();
            poliz.push_back ( Lex () );

            poliz.push_back ( Lex ( POLIZ_GO ) );
            poliz[pl2] = Lex ( POLIZ_LABEL, poliz.size() );

            if ( c_type == LEX_ELSE ) {
                gl ();
                S ();
                poliz[pl3] = Lex ( POLIZ_LABEL, poliz.size() );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end if
    else if ( c_type == LEX_CASE ){
        gl();
        E();
        eq_bool();
        if ( c_type == LEX_OF ){
            gl();
            V();

            gl();
            if ( c_type != LEX_END )
                throw curr_lex;
            else
                poliz.push_back(Lex(POLIZ_CASE_END));
        }

    }
    else if ( c_type == LEX_WHILE ) {
        pl0 = poliz.size ();
        gl ();
        E ();
        eq_bool ();
        pl1 = poliz.size ();
        poliz.push_back ( Lex () );
        poliz.push_back ( Lex (POLIZ_FGO) );
        if ( c_type == LEX_DO ) {
            gl();
            S();
            poliz.push_back ( Lex ( POLIZ_LABEL, pl0 ) );
            poliz.push_back ( Lex ( POLIZ_GO) );
            poliz[pl1] = Lex ( POLIZ_LABEL, poliz.size() );
        }
        else
            throw curr_lex;
    }//end while
    else if ( c_type == LEX_FOR ){
        gl();
        if ( c_type == LEX_ID ) {
            Lex InAddr(POLIZ_ADDRESS, c_val);
            Lex InVal(LEX_ID, c_val);
            poliz.push_back(InAddr);
            check_id(); //!
            gl();
            if (c_type == LEX_EQ){
                gl();
                E();
                eq_type(); //!
                eq_int();//!!!!!!
                poliz.push_back(Lex(LEX_ASSIGN));

                pl0 = poliz.size();
                poliz.push_back(Lex());
                poliz.push_back(Lex(POLIZ_GO));
                if (c_type == LEX_STEP){
                    pl1 = poliz.size();
                    poliz.push_back(InAddr);
                    poliz.push_back(InVal);
                    gl();
                    E();
                    eq_int();
                    poliz.push_back(Lex(LEX_PLUS));
                    if ( c_type == LEX_UNTIL ){
                        poliz.push_back(InVal);
                        gl();
                        E();
                        eq_int();

                        poliz.push_back(Lex(LEX_NEQ));
                        pl2 = poliz.size();
                        poliz.push_back(Lex());
                        poliz.push_back(Lex(POLIZ_FGO));
                        poliz[pl0] = Lex(POLIZ_GO, poliz.size());

                        if ( c_type == LEX_DO ){
                            gl();
                            st_loop.push(pl0);

                            S();
                            poliz.push_back ( Lex ( POLIZ_LABEL, pl1 ) );
                            poliz.push_back ( Lex ( POLIZ_GO) );
                            poliz[pl2] = Lex(POLIZ_GO, poliz.size());

                            st_loop.pop();
                        }
                        else
                            throw curr_lex;
                    }
                    else
                        throw curr_lex;
                }
                else
                    throw curr_lex;
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    } //end for
    else if ( c_type == LEX_CONTINUE ){
        if ( st_loop.empty() )
            throw curr_lex;
        else{
            poliz.push_back( Lex (POLIZ_LABEL, st_loop.top() ) );
            poliz.push_back( Lex (POLIZ_GO) );
        }
    } //end continue
    else if ( c_type == LEX_READ ) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            if ( c_type == LEX_ID ) {
                check_id_in_read ();
                poliz.push_back ( Lex( POLIZ_ADDRESS, c_val) );
                gl();
            }
            else
                throw curr_lex;
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex (LEX_READ) );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end read
    else if ( c_type == LEX_WRITE ) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            E ();
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex ( LEX_WRITE ) );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end write
    else if (c_type == LEX_WRITESTR) {
        gl ();
        if ( c_type == LEX_LPAREN ) {
            gl ();
            E ();
            if ( c_type == LEX_RPAREN ) {
                gl ();
                poliz.push_back ( Lex ( LEX_WRITESTR ) );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }
    else if ( c_type == LEX_ID ) {
        check_id ();
        poliz.push_back (Lex ( POLIZ_ADDRESS, c_val ) );
        gl();
        if ( c_type == LEX_ASSIGN ) {
            gl ();
            E ();
            eq_type ();
            poliz.push_back ( Lex ( LEX_ASSIGN ) );
        }
        else
            throw curr_lex;
    }//assign-end
    else
        B();
}

void Parser::V(){
    gl();
    V1();
    //POLIZ
    while ( c_type == LEX_COMMA ){
        gl();
        V1();
        //POLIZ
    }
}

void Parser::V1(){
    int pl0;
    gl();
    poliz.push_back(Lex(POLIZ_DUP));
    F();
    poliz.push_back(Lex(LEX_EQ));
    while ( c_type == LEX_COMMA ){
        gl();
        poliz.push_back(Lex(POLIZ_DUP));
        F();
        poliz.push_back(Lex(LEX_EQ));
        poliz.push_back(Lex(LEX_OR));
    }
    pl0 = poliz.size();
    poliz.push_back(LEX_NULL);
    poliz.push_back(Lex(POLIZ_FGO));
    if ( c_type == LEX_COLON ){
        gl();
        S();
    }
    else
        throw curr_lex;
    poliz[pl0] = Lex(POLIZ_LABEL, poliz.size());
}

void Parser::E () {
    E1 ();
    if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push ( c_type );
        gl ();
        E1 ();
        check_op ();
    }
}

void Parser::E1 () {
    T ();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push ( c_type );
        gl ();
        T ();
        check_op ();
    }
}

void Parser::T () {
    F ();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND || LEX_PERCENT ) {
        st_lex.push ( c_type );
        gl ();
        F ();
        check_op ();
    }
}

void Parser::F () {
    if ( c_type == LEX_ID ) {
        check_id ();
        poliz.push_back ( Lex ( LEX_ID, c_val ) );
        gl ();
    }
    else if ( c_type == LEX_NUM ) {
        st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_TRUE, 1) );
        gl ();
    }
    else if ( c_type == LEX_FALSE) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lex (LEX_FALSE, 0) );
        gl ();
    }
    else if ( c_type == LEX_NOT ) {
        gl ();
        F ();
        check_not ();
    }
    else if ( c_type == LEX_LPAREN ) {
        gl ();
        E ();
        if ( c_type == LEX_RPAREN)
            gl ();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}


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
