#include "scanner.h"
#include "parser.tab.h"
#include "errors.h"
#include <stdio.h>

extern int yyparse();
extern FILE* yyin;

int main(int argc, char** argv) {
    if (argc > 1) { //otvaramo fajl
        yyin = fopen(argv[1], "r");
        if (!yyin) { //ako je neuspješno, greška
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    int parse_result = yyparse(); //parsiramo
    if (parse_result == 0) {
        printf("Uspjesno parsirano: 0\n");
    } else {
        printf("Neuspjesno parsiranje: 1\n");
    }

    return parse_result;
}
