%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <math.h>

    void yyerror(const char *s);
    int yylex();

    //u union - u:
    //num - za integere i heksadecimalne vrijednosti
    //dnum - za double vrijednosti
    //char* str - za stringove
%}

%union {
    int num; 
    double dnum;
    char* str;
    int bval; 
}

%token <str> IDENTIFIER STRING_CONST
%token <num> INTEGER_CONST
%token <dnum> DOUBLE_CONST
%token <bval> TRUE FALSE

%token LET IN END
%token ASSIGN READ WRITE SKIP
%token IF THEN ELSE FI
%token FOR WHILE DO BREAK RETURN 
%token INT DOUBLE BOOL STRING

%token PLUS MINUS MULT DIV MOD POW
%token EQ NE LT LE GT GE
%token AND OR NOT

%token SEMICOLON COMMA LBRACKET RBRACKET DOT

%type <num> int_expression
%type <dnum> double_expression
%type <str> string_expression
%type <bval> bool_expression

%right ASSIGN
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT UNARY_MINUS

%start program

%%

program:
    LET declarations IN command_sequence END
    ;

declarations:
    /*prazna*/
    | declarations type ident_decl
    ;

type:
    INT { }
    | DOUBLE { }
    | BOOL { }
    | STRING { }
    ;

ident_decl:
    IDENTIFIER
    | ident_decl COMMA IDENTIFIER
    ;

command_sequence:
    command
    | command_sequence command
    ;

command:
    SKIP SEMICOLON
    | IDENTIFIER ASSIGN expression SEMICOLON
    | IF expression THEN command_sequence ELSE command_sequence FI
    | WHILE expression DO command_sequence END
    | READ IDENTIFIER SEMICOLON
    | WRITE expression SEMICOLON
    ;

expression:
    int_expression
    | double_expression
    | bool_expression
    | string_expression
    ;

int_expression:
    INTEGER_CONST
    | int_expression '+' int_expression { /* provjera tipova i sabiranje */ }
    | int_expression '-' int_expression { /* provjera tipova i oduzimanje */ }
    | int_expression '*' int_expression { /* provjera tipova i množenje */ }
    | int_expression '/' int_expression { /* provjera tipova i dijeljenje */ }
    | int_expression '%' int_expression { /* provjera tipova i mod */ }
    | '-' int_expression %prec UNARY_MINUS { /* unarni minus za int */ }
    ;

double_expression:
    DOUBLE_CONST
    | double_expression '+' double_expression { /* provjera tipova i sabiranje */ }
    | double_expression '-' double_expression { /* provjera tipova i oduzimanje */ }
    | double_expression '*' double_expression { /* provjera tipova i množenje */ }
    | double_expression '/' double_expression { /* provjera tipova i dijeljenje */ }
    | double_expression '%' double_expression { /* provjera tipova i mod */ }
    | '-' double_expression %prec UNARY_MINUS { /* unarni minus za int */ }
    ;

bool_expression:
    TRUE
    | FALSE
    | bool_expression AND bool_expression { /* logičko i */ }
    | bool_expression OR bool_expression  { /* logičko ili */ }
    | NOT bool_expression %prec NOT { /* negacija */ }
    | expression '==' expression { /* jednakost, provjeriti tipove */ }
    | expression '!=' expression { /* nejednakost, provjeriti tipove */ }
    ;

string_expression:
    STRING_CONST
    | IDENTIFIER
    | string_expression '+' string_expression { /* konkatenacija stringova */ }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Greska na liniji %d, koloni %d: %s\n", yylineno, column, s); //yylineno prati na kojoj liniji se desila greska
}

int main() {
    if (yyparse() == 0) { //uspjesno
        printf("Ulaz je bio ispravan!\n");
    } else { //neuspjesno
        printf("Ulaz nije bio ispravan.\n");
    }
    return 0;
}