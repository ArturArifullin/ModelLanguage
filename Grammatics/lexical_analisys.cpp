#include "lexical_analisys.h"


const char *
        Scanner::TW    [] = { "", "and", "begin", "bool", "string", "do", "else", "end", "if", "false", "int", "not", "or", "program",
                              "read", "then", "true", "var", "while", "write", "case","of", "for", "continue", "string", "step", "until", "writestr", NULL };

const char *
        Scanner::TD    [] = { "@", ";", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "%", NULL };

int put ( const std::string & buf ){
    std::vector<Ident>::iterator k;

    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
    return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}

std::vector<std::string> TStr;

Lex Scanner::get_lex () {
    enum    state { H, IDENT, NUMB, COM, ALE, NEQ, STRING }; //states of a finite state machine
    int     d, j;
    std::string  buf;
    state   CS = H;
    do {
        gc ();
        switch ( CS ) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
                else if ( isalpha (c) ) {
                    buf.push_back (c);
                    CS  = IDENT;
                }
                else if ( isdigit (c) ) {
                    d   = c - '0';
                    CS  = NUMB;
                }
                else if ( c== '{' ) {
                    CS  = COM;
                }
                else if ( c == ':' || c == '<' || c == '>' ) {
                    buf.push_back (c);
                    CS  = ALE;
                }
                else if (c == '@')
                    return Lex ( LEX_FIN );
                else if (c == '!') {
                    buf.push_back (c);
                    CS  = NEQ;
                }
                else if (c == '"')
                    CS = STRING;
                else {
                    buf.push_back (c);
                    if ( ( j = look ( buf, TD) ) ){
                        return Lex ( (type_of_lex)( j + (int) LEX_FIN ), j ); //put +, -, etc as Lex
                    }
                    else
                        throw c;
                }
                break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c); // creating name of ident
                }
                else {
                    ungetc ( c, fp );
                    if ( (j = look ( buf, TW) ) ) {
                        return Lex ( (type_of_lex) j, j );
                    }
                    else {
                        j   = put ( buf );
                        return Lex ( LEX_ID, j );
                    }
                }
                break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' ); // creating numbber
                }
                else {
                    ungetc ( c, fp ); //maybe it is '+' or smth else, so we have to unget it
                    return Lex ( LEX_NUM, d ); //it's user's number
                }
                break;
            case COM:
                if ( c == '}' ) {
                    CS  = H;
                }
                else if ( c == '@' || c == '{' )
                    throw c;
                break;
            case ALE:
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {
                    ungetc ( c, fp );
                    j   = look ( buf, TD );
                    return Lex ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lex ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
            case STRING:
                while ( c != '"'){
                    if ( c == '@')
                        throw c;
                    buf.push_back(c);
                }
                if ( buf.empty() )
                    throw '!';
                TStr.push_back(buf);
                return Lex (LEX_STRING, reinterpret_cast<long>(TStr[TStr.size()-1].c_str()));
                break;
        } //end switch
    } while (true); // break just after creating lex2
}

std::ostream & operator<< ( std::ostream &s, Lex l ) {
    std::string t;
    if ( l.t_lex <= LEX_WRITE )
        t = Scanner::TW[l.t_lex];
    else if ( l.t_lex >= LEX_FIN && l.t_lex <= LEX_GEQ )
        t = Scanner::TD[ l.t_lex - LEX_FIN ];
    else if ( l.t_lex == LEX_NUM )
        t = "NUMB";
    else if ( l.t_lex == LEX_ID )
        t = TID[l.v_lex].get_name ();
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << std::endl;
    return s;
}