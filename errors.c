#include "errors.h"
#include <stdio.h>

#define RED "\x1b[31m"
#define RESET "\x1b[0m"

void reportError(const char* message, int line, int column) {
    fprintf(stderr, RED "Sintaksna greska na liniji %d, koloni %d\n" RESET, line, column);
}

void reportLexicalError(const char* message, int line, int column) {
    fprintf(stderr, RED "Leksicka greska: %s na liniji %d, koloni %d\n" RESET, message, line, column);
}
