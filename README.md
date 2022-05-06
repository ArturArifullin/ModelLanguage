# ModelLanguage
###### Development of a model programming language using the theory of formal grammars
###### There are following steps of interpetating of model-languge programm: 
###### 1. lexical analysys ( in the project there is class Lex and class Scanner in the lexical_analysys.h, etc)
###### 2. syntax analysys  ( there is file, called synrax_analysys.h, which consists class Parser; methods S(), E(), etc. we use for syntax analysys )
###### 3. semantix analysys 
###### 4. generation program in the internal language (there is intepretation.h, etc.) 
###### 5. run previous program 
##### For lexical analysys we use this tables implemented by a constant array of rows, TW - key words, TD - key signs:
```
const char *
        Scanner::TW    [] = { "", "and", "begin", "bool", "string", "do", "else", "end", "if", "false", "int", "not", "or", "program",
                              "read", "then", "true", "var", "while", "write", "case","of", "for", "continue", "string", "step", "until", NULL };

const char *
        Scanner::TD    [] = { "@", ";", ",", ":", ":=", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "%", NULL };
```
##### For syntax analysys we use following grammar:
```
P  -> programm D1; B@
D1 -> var D {,D}
D  -> I {,I}:[int|bool|string]
B  -> begin S {;S} end
S  -> I:=E | if E then S else S | while E do S | B | read(I) | write(E)| case E of V end | for I = E step E until E do S | continue
V  -> V1 {,V1}
V1 -> F {,F}: S
E  -> E1 [ = | > | < | >= | <= | != ] E1 | E1
E1 -> T { [ + | - | or ] T }
T  -> F { [ * | / | and | % ] F ] }
F  -> I | N | L | not F | (E)
L  -> true | false 
I  -> a | b | ... | z | Ia | ... | Iz | I1 | ... | I9 | I0
N  -> 0 | ... | 9 | N0 | ... | N9
```
#### Testing of interpetator
###### Write your program to file called "prog.txt"
###### Example of test program. Please, pay attention to delimiters between operators. There is not any delimiter before "end"
```
program
    var q, k: string,
    a, b, i, j : int,
    bo: bool;
begin
    read(a);
    read(b);
    if ( a > b ) then
        write(a)
    else
        write(b);
    read(q);
    write(q)
end@
```
###### Other example:
```
program
    var q, k: string,
    a, b, i, j : int,
    bo: bool;
begin

    a := 2;
    case a+8 of
        1, 10: write(1),
        3, 5, 7 , 9: write(2)
    end;

    bo := true and false or true;
    write(bo);

    for i = 1 step 1 until 5 do
        for j = 1 step 2 until 9 do
            case i + j of
                1: write("one"),
                2: write("two"),
                3: write("three"),
                4: write("four"),
                5: for a = 1 step 1 until 5 do
                    write(a)
            end;


    a := 1; b:= 10;
    q := "Hi"; k := "Hello";
    case a+b of
        10: write("?"),
        11: case a+b+i of
                10: write(k),
                16, 17: write(q)
            end
    end;

    write("Bye")
end@
```

