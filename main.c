#include "scanner.h"
#include "parser.tab.h"
#include "errors.h"
#include <stdio.h>

extern int yyparse(); //funkcija koju generiše bison za parsiranje
extern FILE* yyin; //pokazivač na fajl koji određuje ulazni tok podataka za parser

int main(int argc, char** argv) {
    if (argc > 1) { //otvaramo fajl, mora imati bar 1 argument jer inače nema fajla za čitanje 
        yyin = fopen(argv[1], "r");
        if (!yyin) { //ako je neuspješno, greška
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin; //ako nije zadato ime, koristi se standardni ulaz stdin
    }

    int parse_result = yyparse(); //parsiramo preko parsera koji smo napravili
    if (parse_result == 0) {
        printf("Uspjesno parsirano: 0\n");
    } else {
        printf("Neuspjesno parsiranje: 1\n");
    }

    return parse_result;
}
