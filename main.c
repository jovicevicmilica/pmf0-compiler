#include "scanner.h"
#include "parser.tab.h"
#include "errors.h"

#include <stdio.h>

extern int yyparse();
extern FILE* yyin;

int main(int argc, char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    int parse_result = yyparse();
    if (parse_result == 0) {
        printf("0\n");
    } else {
        printf("1\n");
    }

    return parse_result;
}
