%{
    #define RED "\x1b[31m" //za greske
    #define RESET "\x1b[0m" //vracanje na normalno stanje
    int column = 1; //pratimo kolonu, posto je flex ne generise moramo je samostalno uvecavati
    int temp_column = 1; //pomocna kolona za nepravilne stringove
    int comment_nesting = 0; //brojac za ugnjezdene komentare
    int error_reported = 0; //isto za string, pamtimo da li je vec u vise redova

    #include <string.h>
    #define YY_USER_ACTION column += yyleng; //ovako ce nam se ova fja pozivati prije izvrsenja iceg drugog kod tokena, i time ne moramo
    //vrsiti sabiranje posebno u svakom dijelu, medjutim, ako je ne umanjimo posle u printu, vracace nam da je greska u poslednjem karakteru stringa, a ne prvom
    //sve zavisi do preferenci

    //dodajemo string state zato sto se moze desiti da je string nezatvoren, a da je u vise redova, i onda je sve to cjelokupno greska
%}

%x LINE_COMMENT
%x MULTILINE_COMMENT
%x STRING_STATE

%option yylineno
%option noyywrap 

%%

[ \t]+      { } //ne radimo nista, potrebno nam samo da se kolona uveca, a to radimo gore vec
\r          { column = 1; }
\n          { column = 1; } //yylineno ne moramo dirati, on se automatski azurira

"//"        { BEGIN(LINE_COMMENT); }
"/*"        { comment_nesting = 1; BEGIN(MULTILINE_COMMENT); }

"INT"       { printf("Tip: INT\n");    } //TIPOVI
"DOUBLE"    { printf("Tip: DOUBLE\n"); }
"BOOL"      { printf("Tip: BOOL\n");   }
"STRING"    { printf("Tip: STRING\n"); }

"LET"       { printf("Keyword: LET\n");    }
"IN"        { printf("Keyword: IN\n");     }
"END"       { printf("Keyword: END\n");    }
":="        { printf("Keyword: ASSIGN\n"); } //operator dodjele
"READ"      { printf("Keyword: READ\n");   }
"WRITE"     { printf("Keyword: WRITE\n");  }
"SKIP"      { printf("Keyword: SKIP\n");   }

"IF"        { printf("Keyword: IF\n");     } //IF I OSTALE KLJUCNE RIJECI
"THEN"      { printf("Keyword: THEN\n");   }
"ELSE"      { printf("Keyword: ELSE\n");   }
"FI"        { printf("Keyword: FI\n");     }
"WHILE"     { printf("Keyword: WHILE\n");  }
"DO"        { printf("Keyword: DO\n");     }
"FOR"       { printf("Keyword: FOR\n");    }
"BREAK"     { printf("Keyword: BREAK\n");  }
"RETURN"    { printf("Keyword: RETURN\n"); }
"TRUE"      { 
                //yylval.bval = 1;  
                printf("Keyword: TRUE\n"); 
            }

"FALSE"     { 
                //yylval.bval = 0;  
                printf("Keyword: FALSE\n");  
            }

0|[1-9][0-9]*                           {   //decimalni brojevi
                                            //yylval.num = atoi(yytext); 
                                        
                                            //return INTEGER_CONST; 
                                            printf("Token: INTEGER(%s)\n", yytext);
                                        } 

[a-zA-Z][a-zA-Z0-9_]{0,30}              {   //da ogranicimo duzinu identifikatora na 31 karakter maksimalno (prvi obavezan, onda 30 opcionalnih)
                                            //yylval.str = strdup(yytext); //ne treba nam za skener
                                            //pratimo kolonu za svaku promjenljivu
                                            //return IDENTIFIER; 
                                            printf("Token: IDENTIFIER(%s)\n", yytext);
                                        }

[a-zA-Z][a-zA-Z0-9_]{30}[^ \t\r\n]+     { printf(RED "Greska: Neispravan format identifikatora (duzina prelazi 31): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); } //umanjimo column za yyleng zbog yy_user_actiona koji nam racuna kolone

(0x|0X)(0|[1-9A-Fa-f][0-9A-Fa-f]*)      {   //heksadecimalni brojevi
                                            //yylval.num = strtol(yytext, NULL, 16); 
                                            
                                            //return INTEGER_CONST;
                                            printf("Token: HEXINTEGER(%s)\n", yytext);
                                        }

(0x|0X)[0-9A-Fa-f]+[^ \t\r\n0-9A-Fa-f]+ {  printf(RED "Greska: Neispravan format heksadecimalne konstante (ima nedozvoljene cifre): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); }

[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?        { 
                                            //yylval.dnum = atof(yytext); 
                                            
                                            //return DOUBLE_CONST;
                                            printf("Token: DOUBLE(%s)\n", yytext);
                                        }

\.[0-9]+([eE][+-]?[0-9]+)?              { printf(RED "Greska: Neispravan format double konstante (nema vodecu cifru): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); }

[0-9]+[eE][+-]?[0-9]+                   { printf(RED "Greska: Neispravan format double konstante (eksponent bez tacke): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); }

[0-9]+(\.[0-9]*)?\.[0-9]+               { printf(RED "Greska: Neispravan format double konstante (visak tacaka): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); }

[0-9]+\.[0-9]*[eE][+-]?                 { printf(RED "Greska: Neispravan format double konstante (eksponent bez cifara poslije znaka): '%s' na liniji %d, koloni %d\n" RESET, yytext, yylineno, column - yyleng); }

\"[^"\n]*\"                             { printf("Token: STRING(%s)\n", yytext); }

\"                                      { BEGIN(STRING_STATE); temp_column = column; error_reported = 0; }

[_][a-zA-Z0-9_]+                        { printf(RED "Greska: Neispravan format identifikatora (ne moze poceti sa '_') na liniji %d, koloni %d\n" RESET, yylineno, column - yyleng); }

"<="        { printf("Token: LE(<=)\n");        } //OPERATORI
">="        { printf("Token: GE(>=)\n");        }
"<"         { printf("Token: LT(<)\n");         }
">"         { printf("Token: GT(>)\n");         }
"=="        { printf("Token: EQ(==)\n");        }
"!="        { printf("Token: NE(!=)\n");        }
"&&"        { printf("Token: AND(&&)\n");       }
"||"        { printf("Token: OR(||)\n");        }
"!"         { printf("Token: NOT(!)\n");        }
"^"         { printf("Token: POW(^)\n");        }
          
"+"         { printf("Token: PLUS(+)\n");       } 
"-"         { printf("Token: MINUS(-)\n");      }
"*"         { printf("Token: MULTIPLY(*)\n");   }
"/"         { printf("Token: DIVIDE(/)\n");     }
"%"         { printf("Token: MODULO(%%)\n");    }
"\\"        { printf("Token: BACKSLASH(\\)\n"); }

";"         { printf("Token: SEMICOLON(;)\n");  }
","         { printf("Token: COMMA(,)\n");      }
"."         { printf("Token: DOT(.)\n");        }
"("         { printf("Token: LBRACKET(()\n");   }
")"         { printf("Token: RBRACKET())\n");   }

.           { printf(RED "Greska: Neocekivani karakter '%c' na liniji %d, koloni %d\n" RESET, yytext[0], yylineno, column - yyleng); }

<LINE_COMMENT>{
"\n"        { printf("Comment: LINE\n"); column = 1; BEGIN(INITIAL); } //ugnjezdavanje jednolinijskih valjda ne treba? jer cim pocne // tretira se kao komentar, nije naglaseno
.           {  } 
}

<MULTILINE_COMMENT>{ 
"/*"        { comment_nesting++; printf(RED "Greska: Nije dozvoljeno ugnjezdavanje komentara na liniji %d, koloni %d\n" RESET, yylineno, column - yyleng); } //brojanje ugnjezdenja
"*/"        { 
                if(--comment_nesting == 0) {
                    printf("Comment: MULTILINE\n");
                }

                BEGIN(INITIAL);
            }
.           { } //ignorisemo sve u komentaru osim novih linija i kraj komentara
"\n"        { column = 1; }
<<EOF>>     {
                if(comment_nesting > 0) printf(RED "Greska: Nezatvoren viselinijski komentar na liniji %d, koloni %d\n" RESET, yylineno, column - yyleng);
                BEGIN(INITIAL);
            }
}

<STRING_STATE>{
\\\"        { } //escaped quote, nista posebno ne radimo
\\\n        { } //escaped newline
\n          { 
                if (error_reported == 0) {
                    printf(RED "Greska: String u vise redova na liniji %d, koloni %d\n" RESET, yylineno, temp_column - yyleng); 
                    error_reported = 1;
                }
                column = 1;
            } //string u vise redova, nepravilno
\"          { BEGIN(INITIAL); } //kraj stringa, vracanje na pocetno stanje
<<EOF>>     { printf(RED "Greska: Nezatvoren string na kraju fajla, na liniji %d, koloni %d\n" RESET, yylineno, column - yyleng); BEGIN(INITIAL); } //string koji nije zatvoren prije kraja fajla
.           { } 
}

%%

int main() {
    yylex();

    return 0;
}