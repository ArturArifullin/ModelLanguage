#include "syntax_analysys.h"
#ifndef GRAMMATICS_INTERPRETATION_H
#define GRAMMATICS_INTERPRETATION_H


class Executer {
public:
    void execute ( std::vector<Lex> & poliz );
};


class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};

#endif //GRAMMATICS_INTERPRETATION_H
