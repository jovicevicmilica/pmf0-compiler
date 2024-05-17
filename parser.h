#ifndef PARSER_H
#define PARSER_H

#include "location.h"

void yyerror(const char* s);
int yyparse();
int yylex();

#endif // PARSER_H
