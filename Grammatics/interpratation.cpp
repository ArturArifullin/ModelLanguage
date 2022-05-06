#include "interpretation.h"

struct argument {
    int int_v;
    std::string str_v = "";
    bool str = false;
    void defalted(){
        str_v = "";
        str = false;
    }
};

void Executer:: execute ( std::vector<Lex> & poliz ) {
    Lex pc_el;
    std::stack < argument > args;
    std::stack < argument > dup_stack; //just for case...of...end
    long i, j, index = 0, size = poliz.size();
    argument x , y;
    bool dup = false; int dup_v;
    argument arg_x;
    while ( index < size ) {
        pc_el = poliz [ index ]; Lex pc_el_pr;
        if (index >= 0)
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                arg_x.defalted();
                arg_x.int_v = pc_el.get_value ();
                args.push (arg_x);
                break;

            case LEX_STRING:
                arg_x.str = true;
                arg_x.str_v = pc_el.get_str();
                args.push(arg_x);
                break;

            case POLIZ_DUP:
                if ( dup_stack.empty() )
                    throw  "POLIZ: unexpected elem";
                else{
                    args.push(dup_stack.top());
                }
                break;

            case POLIZ_CASE_END:
                dup_stack.pop();
                dup = false;
                break;

            case LEX_ID:
                i = pc_el.get_value ();
                if ( TID[i].get_assign () ) {
                    arg_x.defalted();
                    arg_x.int_v = TID[i].get_value ();
                    if (TID[i].probably_string!="") {
                        arg_x.str = true;
                        arg_x.str_v = TID[i].probably_string;
                    }
                    args.push (arg_x);
                    break;
                }
                else
                    throw "POLIZ: indefinite identifier";

            case LEX_NOT:
                from_st ( args, x );
                arg_x.defalted();
                arg_x.int_v = !x.int_v;
                args.push (arg_x);
                break;

            case LEX_OR:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v||y.int_v;
                args.push (arg_x);
                break;

            case LEX_AND:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v && y.int_v;
                args.push (arg_x);
                break;

            case POLIZ_GO:
                from_st ( args, x );
                index = x.int_v - 1;
                break;

            case POLIZ_FGO:
                from_st ( args, x );
                from_st ( args, y );
                if ( !y.int_v )
                    index = x.int_v - 1;
                break;

            case LEX_WRITE:
                x.defalted();
                from_st ( args, x );
                j = x.int_v;
                if ( !x.str )
                    std::cout << j << std::endl;
                else
                    std::cout << x.str_v << std::endl;
                break;


            case LEX_READ: {
                long k;
                std::string str;
                from_st(args, x);
                i = x.int_v;
                if (TID[i].get_type() == LEX_INT) {
                    std::cout << "Input int value for" << TID[i].get_name() << std::endl;
                    std::cin >> k;
                } else if (TID[i].get_type() == LEX_STRING) {
                    std::cout << "Input string for" << TID[i].get_name() << std::endl;
                    std::cin >> str;
                } else {
                    std::string j;
                    while (1) {
                        std::cout << "Input boolean value (true or false) for" << TID[i].get_name() << std::endl;
                        std::cin >> j;
                        if (j != "true" && j != "false") {
                            std::cout << "Error in input:true/false" << std::endl;
                            continue;
                        }
                        k = (j == "true") ? 1 : 0;
                        break;
                    }
                }
                TID[i].put_value(k);
                TID[i].put_string(str);
                TID[i].put_assign();
                break;
            }

            case LEX_PLUS:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v + y.int_v;
                args.push (arg_x);
                break;

            case LEX_TIMES:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v * y.int_v;
                args.push (arg_x);
                break;

            case LEX_MINUS:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v - x.int_v;
                args.push (arg_x);
                break;

            case LEX_PERCENT:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v % x.int_v;
                args.push (arg_x);
                break;

            case LEX_SLASH:
                from_st ( args, x );
                from_st ( args, y );
                if (!x.int_v) {
                    arg_x.defalted();
                    arg_x.int_v = x.int_v / y.int_v;
                    args.push ( arg_x );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";

            case LEX_EQ:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v == y.int_v;
                args.push (arg_x);
                break;

            case LEX_LSS:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v < x.int_v;
                args.push (arg_x);
                break;

            case LEX_GTR:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v > x.int_v;
                args.push (arg_x);
                break;

            case LEX_LEQ:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v <= x.int_v;
                args.push (arg_x);
                break;

            case LEX_GEQ:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = y.int_v >= x.int_v;
                args.push (arg_x);
                break;

            case LEX_NEQ:
                from_st ( args, x );
                from_st ( args, y );
                arg_x.defalted();
                arg_x.int_v = x.int_v != y.int_v;
                args.push (arg_x);
                break;

            case LEX_ASSIGN: //Error here!!!
                from_st ( args, x );
                from_st ( args, y );
                j = y.int_v;
                TID[j].put_value (x.int_v);
                TID[j].put_assign ();
                if (x.str)
                    TID[j].put_string(x.str_v);
                break;

            case POLIZ_CASE_START:
                from_st(args, x);
                dup_stack.push(x);
                break;

            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    //std::cout << "Finish of executing!!!" << std::endl;
}

void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}
