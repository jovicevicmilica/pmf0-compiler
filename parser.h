#ifndef PARSER_H
#define PARSER_H

#include "location.h"

void yyerror();
int yyparse();
int yylex();

#endif 
