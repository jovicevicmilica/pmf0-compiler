%{
    #include "pmf0.tab.h" //povezemo sa bisonom
    #include <string.h>
%}

%option yylineno 

%x LINE_COMMENT
%x MULTILINE_COMMENT

%%

[ \t\n]+               { } 
"//"[^"\n"]*           { BEGIN(LINE_COMMENT); }
"/*"                   { BEGIN(MULTILINE_COMMENT); }

"IF"                   { return IF; } //if i ostale kljucne rijeci
"ELSE"                 { return ELSE; }
"WHILE"                { return WHILE; }
"FOR"                  { return FOR; }
"RETURN"               { return RETURN; }
"TRUE"                 { return TRUE; }
"FALSE"                { return FALSE; }

[a-zA-Z][a-zA-Z0-9_]{0,30} { yylval.str = strdup(yytext); return IDENTIFIER; } //da ogranicimo duzinu identifikatora na 31 karakter maksimalno (prvi obavezan, onda 30 opcionalnih)

0|[1-9][0-9]*          { yylval.num = atoi(yytext); return INTEGER_CONST; } //decimalni brojevi
(0x|0X)[1-9A-Fa-f][0-9A-Fa-f]*       { yylval.num = strtol(yytext, NULL, 16); return INTEGER_CONST; } //heksadecimalni brojevi

[0-9]+"."[0-9]*        { yylval.dnum = atof(yytext); return DOUBLE_CONST; }
[0-9]+("."[0-9]*)?[eE][-+]?[0-9]+ { yylval.dnum = atof(yytext); return DOUBLE_CONST; } //floating point brojevi

\"[^"\n]*\"            { yylval.str = strdup(yytext); return STRING_CONST; } //stringovi

"+"                    { return PLUS; } //operatori
"-"                    { return MINUS; }
"*"                    { return MULT; }
"/"                    { return DIV; }
"%"                    { return MOD; }

"\\\\"                 { return BACKSLASH; }
"<="                   { return LE; }
">="                   { return GE; }
"<"                    { return LT; }
">"                    { return GT; }
"=="                   { return EQ; }
"!="                   { return NE; }
"&&"                   { return AND; }
"||"                   { return OR; }
"!"                    { return NOT; }
";"                    { return SEMICOLON; }
","                    { return COMMA; }
"."                    { return DOT; }
"("                    { return LBRACKET; }
")"                    { return RBRACKET; }

.                      { return yytext[0]; } 

<LINE_COMMENT>{
"\n"    { BEGIN(INITIAL); } 
.       { } 
}

<MULTILINE_COMMENT>{ 
"\n"    { }
"*/"    { BEGIN(INITIAL); } 
.       { } 
}

%%

int yywrap() {
    return 1;
}