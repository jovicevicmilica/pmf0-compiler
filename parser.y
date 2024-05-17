%{
#include "location.h"
#include "parser.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char* s);
%}

%union {
    int intVal;
    double doubleVal;
    char* string;
}

%token INT DOUBLE BOOL STRING
%token LET IN END ASSIGN READ WRITE SKIP
%token IF THEN ELSE FI WHILE DO FOR BREAK RETURN
%token TRUE FALSE
%token INTEGER_CONST DOUBLE_CONST IDENTIFIER
%token LE GE LT GT EQ NE AND OR NOT POW
%token TO

%type <intVal> INTEGER_CONST
%type <doubleVal> DOUBLE_CONST
%type <string> IDENTIFIER

%locations

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left '+' '-'
%left '*' '/' '%'
%right POW
%right NOT
%left ELSE

%%

program:
    LET declarations IN command_sequence END {}
    ;

declarations:
    declaration
    | declarations declaration
    ;

declaration:
    type id_list '.'
    ;

id_list:
    IDENTIFIER
    | id_list ',' IDENTIFIER
    ;

type:
    INT
    | DOUBLE
    | BOOL
    | STRING
    ;

command_sequence:
    command
    | command_sequence command
    ;

command:
    SKIP ';'
    | IDENTIFIER ASSIGN expression ';'
    | IF expression THEN command_sequence ELSE command_sequence FI ';'
    | IF expression THEN command_sequence FI ';' %prec ELSE
    | WHILE expression DO command_sequence END ';'
    | FOR IDENTIFIER ASSIGN expression TO expression DO command_sequence END ';'
    | READ IDENTIFIER ';'
    | WRITE expression ';'
    ;

expression:
    INTEGER_CONST
    | DOUBLE_CONST
    | IDENTIFIER
    | TRUE
    | FALSE
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    | expression LE expression
    | expression GE expression
    | expression LT expression
    | expression GT expression
    | expression EQ expression
    | expression NE expression
    | expression AND expression
    | expression OR expression
    | expression POW expression
    | NOT expression
    | '(' expression ')'
    ;

%%

void yyerror(const char* s) {
    reportError(s, yylloc.first_line, yylloc.first_column);
}
