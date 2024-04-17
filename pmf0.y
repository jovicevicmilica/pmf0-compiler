%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "pmf0.tab.h" 

    extern int yylex();
    extern char *yytext;
    extern int yylineno;

    void yyerror(const char *s) {
        fprintf(stderr, "Greska na liniji %d: %s\n", yylineno, s); //yylineno prati na kojoj liniji se desila greska
    }

    //u union - u:
    //num - za integere i heksadecimalne vrijednosti
    //dnum - za double vrijednosti
    //char* str - za stringove
%}

%union {
    int num; 
    double dnum;
    char* str;
}

%token <str> IDENTIFIER STRING_CONST
%token <num> INTEGER_CONST
%token <dnum> DOUBLE_CONST
%token TRUE FALSE 
%token IF ELSE WHILE FOR RETURN 
%token PLUS MINUS MULT DIV MOD
%token EQ NE LT LE GT GE
%token AND OR NOT
%token SEMICOLON LBRACKET RBRACKET
%token COMMA BACKSLASH DOT


%start program

%%

program:
    statements
    ;

statements:
    statement
    | statements statement
    ;

statement:
    IF LBRACKET expression RBRACKET statement
    | IF LBRACKET expression RBRACKET statement ELSE statement
    | WHILE LBRACKET expression RBRACKET statement
    | FOR LBRACKET expression SEMICOLON expression SEMICOLON expression RBRACKET statement
    | RETURN expression SEMICOLON
    | expression SEMICOLON
    ;

expression:
    INTEGER_CONST
    | DOUBLE_CONST
    | STRING_CONST
    | TRUE
    | FALSE
    | IDENTIFIER
    | expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | expression MOD expression
    | expression LE expression
    | expression GE expression
    | expression LT expression
    | expression GT expression
    | expression EQ expression
    | expression NE expression
    | expression AND expression
    | expression OR expression
    | NOT expression
    | LBRACKET expression RBRACKET
    ;

%%
int main() {
    if (yyparse() == 0) { //uspjesno
        printf("Ulaz je bio ispravan!\n");
    } else { //neuspjesno
        printf("Ulaz nije bio ispravan.\n");
    }
    return 0;
}