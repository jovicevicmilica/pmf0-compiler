%{
#include "location.h"
#include "parser.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

void yyerror(); //funkcija za štampanje sintaksnih grešaka
int syntax_error = 0; //dodajemo globalnu promjenljivu za praćenje grešaka, jer se stablo NE ŠTAMPA u slučaju greške

//definišemo union u kom su tipovi tokena, neki imaju tip, a neki nemaju
//takođe uvodimo tip node za sve djelove gramatike, kako bi omogućili štampanje AST
//sve potrebne djelove za AST pretvaramo u čvorove i na kraju štampamo
//ostatak tipova je za konstante, stringove i identifikatore

//navodimo tokene, i uz one koje imaju tip i njihov tip iz union
//zatim definišemo jesu li right ili left operatori
//program, deklaracije, komande i sl. će biti tipa node, jer predstavljaju čvorove u AST
%}

%union {
    int int_value;
    double double_value;
    char* string_value;
    Node *node;
}

%token <int_value> INTEGER_CONST
%token <double_value> DOUBLE_CONST
%token <string_value> IDENTIFIER STRING_CONST
%token PLUS MINUS MULTIPLY DIVIDE MOD LE GE LT GT EQ NE AND OR NOT POW
%token ASSIGN READ WRITE SKIP IF THEN ELSE FI WHILE DO FOR TO BREAK RETURN
%token STRING BOOL INT DOUBLE LPARENT RPARENT COMMA DOT SEMICOLON
%token TRUE FALSE LET IN END

%type <node> program declarations declaration id_list type command_sequence command break_loop expression

%locations

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right POW
%right NOT
%left ELSE

%%

program:
    LET declarations IN command_sequence END {
        if (syntax_error == 0) { //provjeravamo da li je bilo grešaka, jer se inače ne štampa stablo
            $$ = create_program($2, $4);
            print_ast($$);
        } else {
            $$ = NULL;
        }
    }
;

declarations:
    declaration {
        //kreiramo sekvencu deklaracija ako imamo samo jednu deklaraciju
        $$ = $1;
    }
    | declarations declaration {
        //dodajemo novu deklaraciju u sekvencu, ako ih je više!!! drugačije bi vraćalo recimo samo sve int - ove, ako je int prvi
        $$ = create_sequence($1, $2);
    }
;

declaration:
    type id_list DOT { $$ = create_sequence($1, $2); } //takođe pravimo sekvencu, ovo je slično kao pravilo iznad bez rekurzije, ako ovo
    //maknemo, samo se neće štampati promjenljive u LET - u, to možda i treba tako!
;

id_list:
    IDENTIFIER { $$ = create_identifier($1); } //pravimo zaseban identifikator, isto važi kao i iznad
    | id_list COMMA IDENTIFIER { $$ = create_sequence($1, create_identifier($3)); }
;

type: //tip promjenljivih, ključne riječi
    INT { $$ = create_identifier("int"); }
    | DOUBLE { $$ = create_identifier("double"); }
    | BOOL { $$ = create_identifier("bool"); }
    | STRING { $$ = create_identifier("string"); }
;

command_sequence: //rekurzivno obavljamo komande
    command { $$ = $1; }
    | command_sequence command { $$ = create_sequence($1, $2); }
;

command: //sve moguće komande, return možda nije obavezan
    SKIP SEMICOLON { $$ = create_skip(); }
    | RETURN expression SEMICOLON { $$ = create_return($2); }
    | IDENTIFIER ASSIGN expression SEMICOLON { $$ = create_assign(create_identifier($1), $3); }
    | IF expression THEN command_sequence ELSE command_sequence FI SEMICOLON { $$ = create_if($2, $4, $6); }
    | IF expression THEN command_sequence FI SEMICOLON %prec ELSE { $$ = create_if($2, $4, NULL); }
    | WHILE expression DO command_sequence break_loop END SEMICOLON { $$ = create_while($2, $4); }
    | FOR IDENTIFIER ASSIGN expression TO expression DO command_sequence break_loop END SEMICOLON {
        Node *init = create_assign(create_identifier($2), $4); //prvo pravimo assign
        $$ = create_for(init, $6, $8); //proslijeđujemo inicijalizaciju, uslov i tijelo
    }
    | READ IDENTIFIER SEMICOLON { $$ = create_read(create_identifier($2)); }
    | WRITE expression SEMICOLON { $$ = create_write($2); }
;

break_loop:
    BREAK SEMICOLON { $$ = create_break(); }
    | { $$ = NULL; }
;

expression: //svi mogući izrazi u komandama
    INTEGER_CONST { $$ = create_int_const($1); }
    | DOUBLE_CONST { $$ = create_double_const($1); }
    | STRING_CONST { $$ = create_string_const($1); }
    | IDENTIFIER { $$ = create_identifier($1); }
    | TRUE { $$ = create_bool_const(1); }
    | FALSE { $$ = create_bool_const(0); }
    | expression PLUS expression { $$ = create_binary_operator(N_PLUS, $1, $3); }
    | expression MINUS expression { $$ = create_binary_operator(N_MINUS, $1, $3); }
    | expression MULTIPLY expression { $$ = create_binary_operator(N_MULTIPLY, $1, $3); }
    | expression DIVIDE expression { $$ = create_binary_operator(N_DIVIDE, $1, $3); }
    | expression MOD expression { $$ = create_binary_operator(N_MOD, $1, $3); }
    | expression LE expression { $$ = create_binary_operator(N_LE, $1, $3); }
    | expression GE expression { $$ = create_binary_operator(N_GE, $1, $3); }
    | expression LT expression { $$ = create_binary_operator(N_LT, $1, $3); }
    | expression GT expression { $$ = create_binary_operator(N_GT, $1, $3); }
    | expression EQ expression { $$ = create_binary_operator(N_EQ, $1, $3); }
    | expression NE expression { $$ = create_binary_operator(N_NE, $1, $3); }
    | expression AND expression { $$ = create_binary_operator(N_AND, $1, $3); }
    | expression OR expression { $$ = create_binary_operator(N_OR, $1, $3); }
    | expression POW expression { $$ = create_binary_operator(N_POW, $1, $3); }
    | NOT expression { $$ = create_unary_operator(N_NOT, $2); }
    | LPARENT expression RPARENT { $$ = $2; }
;

%%

void yyerror() {
    extern char *yytext; //tekst koji je parser trenutno analizirao kada je greška nastala
    report_syntax_error(yytext, yylloc.first_line, yylloc.first_column);
    syntax_error = 1; //postavljamo promjenljivu na 1 u slučaju greške, da nam ne štampa stablo
}
