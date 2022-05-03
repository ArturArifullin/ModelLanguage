#include "interpretation.h"


int main () {
    try {
        Interpretator I ( "../prog.txt" );
        I.interpretation ();
        return 0;
    }
    catch ( char c ) {
        std::cout << "unexpected symbol " << c << std::endl;
        return 1;
    }
    catch ( Lex l ) {
        std::cout << "unexpected lexeme" << l << std::endl;
        return 1;
    }
    catch ( const char *source ) {
        std::cout << source << std::endl;
        return 1;
    }
}