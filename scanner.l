%{
#include "location.h"
#include "ast.h"
#include "parser.tab.h"
#include "errors.h"
#include "scanner.h"

int column = 1; //pratimo kolonu, posto je flex ne generise moramo je samostalno uvecavati
int temp_column = 1; //pomocna kolona za nepravilne stringove
int comment_nesting = 0; //brojac za ugnjezdene komentare
int error_reported = 0; //isto za string, pamtimo da li je vec u vise redova

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = column; \
    yylloc.last_column = column + yyleng - 1; \
    column += yyleng;
//ovako ce nam se ova fja pozivati prije izvrsenja iceg drugog kod tokena, i time ne moramo
//vrsiti sabiranje posebno u svakom dijelu, medjutim, ako je ne umanjimo posle u printu, vracace nam da je greska u poslednjem karakteru stringa, a ne prvom
//sve zavisi do preferenci

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
%}

%x LINE_COMMENT
%x MULTILINE_COMMENT
%x STRING_STATE

%option yylineno

%%

[ \t]+      { } //ne radimo nista, potrebno nam samo da se kolona uveca, a to radimo gore vec
\r          { column = 1; }
\n          { column = 1; } //yylineno ne moramo dirati, on se automatski azurira

"//"        { BEGIN(LINE_COMMENT); }
"/*"        { comment_nesting = 1; BEGIN(MULTILINE_COMMENT); }

"INT"       { return INT; } //TIPOVI
"DOUBLE"    { return DOUBLE; }
"BOOL"      { return BOOL; }
"STRING"    { return STRING; }

"LET"       { return LET; }
"IN"        { return IN; }
"END"       { return END; }
":="        { return ASSIGN; } //OPERATOR DODJELE
"READ"      { return READ; }
"WRITE"     { return WRITE; }
"SKIP"      { return SKIP; }

"IF"        { return IF; } //IF I OSTALE KLJUCNE RIJECI
"THEN"      { return THEN; }
"ELSE"      { return ELSE; }
"FI"        { return FI; }
"WHILE"     { return WHILE; }
"DO"        { return DO; }
"FOR"       { return FOR; }
"BREAK"     { return BREAK; }
"RETURN"    { return RETURN; }
"TRUE"      { return TRUE; }
"FALSE"     { return FALSE; }

0|[1-9][0-9]*                           { yylval.intVal = atoi(yytext); return INTEGER_CONST; }

[a-zA-Z][a-zA-Z0-9_]{0,30}              { yylval.stringVal = strdup(yytext); return IDENTIFIER; }

[a-zA-Z][a-zA-Z0-9_]{30}[^ \t\r\n]+     { reportLexicalError("Neispravan format identifikatora (duzina prelazi 31)", yylineno, column - yyleng); }

(0x|0X)(0|[1-9A-Fa-f][0-9A-Fa-f]*)      { yylval.intVal = strtol(yytext, NULL, 16); return INTEGER_CONST; }

(0x|0X)[0-9A-Fa-f]+[^ \t\r\n0-9A-Fa-f]+ { reportLexicalError("Neispravan format heksadecimalne konstante (ima nedozvoljene cifre)", yylineno, column - yyleng); }

[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?        { yylval.doubleVal = atof(yytext); return DOUBLE_CONST; }

\.[0-9]+([eE][+-]?[0-9]+)?              { reportLexicalError("Neispravan format double konstante (nema vodecu cifru)", yylineno, column - yyleng); }

[0-9]+[eE][+-]?[0-9]+                   { reportLexicalError("Neispravan format double konstante (eksponent bez tacke)", yylineno, column - yyleng); }

[0-9]+(\.[0-9]*)?\.[0-9]+               { reportLexicalError("Neispravan format double konstante (visak tacaka)", yylineno, column - yyleng); }

[0-9]+\.[0-9]*[eE][+-]?                 { reportLexicalError("Neispravan format double konstante (eksponent bez cifara posle znaka)", yylineno, column - yyleng); }

\"[^"\n]*\"                             { yylval.stringVal = strdup(yytext); return STRING_CONST; }

\"                                      { BEGIN(STRING_STATE); temp_column = column; error_reported = 0; }

[_][a-zA-Z0-9_]+                        { reportLexicalError("Neispravan format identifikatora (ne moze poceti sa '_')", yylineno, column - yyleng); }

"<="    { return LE; } //OPERATORI
">="    { return GE; }
"<"     { return LT; }
">"     { return GT; }
"=="    { return EQ; }
"!="    { return NE; }
"&&"    { return AND; }
"||"    { return OR; }
"!"     { return NOT; }
"^"     { return POW; }

"+"     { return PLUS; }
"-"     { return MINUS; }
"*"     { return MULTIPLY; }
"/"     { return DIVIDE; }
"%"     { return MOD; }
";"     { return SEMICOLON; }
","     { return COMMA; }
"."     { return DOT; }
"("     { return LPARENT; }
")"     { return RPARENT; }

.           { reportLexicalError("Neocekivani karakter", yylineno, column - yyleng); }

<LINE_COMMENT>{
"\n"        { column = 1; BEGIN(INITIAL); }
.           { }
}

<MULTILINE_COMMENT>{
"/*"        { comment_nesting++; reportLexicalError("Nije dozvoljeno ugnjezdjavanje komentara", yylineno, column - yyleng); }
"*/"        { if(--comment_nesting == 0) {} BEGIN(INITIAL); }
.           { }
"\n"        { column = 1; }
<<EOF>>     { if(comment_nesting > 0) reportLexicalError("Nezatvoren viselinijski komentar", yylineno, column - yyleng); BEGIN(INITIAL); }
}

<STRING_STATE>{
\\\"        { } //escaped quote, nista posebno ne radimo
\\\n        { } //escaped newline
\n          { if(error_reported == 0) { reportLexicalError("String u vise redova", yylineno, temp_column - yyleng); error_reported = 1; } column = 1; }
\"          { yylval.stringVal = strdup(yytext); BEGIN(INITIAL); return STRING; }
<<EOF>>     { reportLexicalError("Nezatvoren string na kraju fajla", yylineno, column - yyleng); BEGIN(INITIAL); }
.           { }
}

%%

int yywrap() {
    return 1;
}
