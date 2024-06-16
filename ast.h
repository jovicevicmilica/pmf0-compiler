#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ASTNode {
    enum {
        //prvo preko enumeracije definišemo sve vrste čvorova koje AST može da ima, tj. one koje štampa i uzima u obzir
        NODE_INTEGER_CONST, NODE_DOUBLE_CONST, NODE_STRING_CONST, NODE_BOOL_CONST,
        NODE_IDENTIFIER, NODE_PLUS, NODE_MINUS, NODE_MULTIPLY, NODE_DIVIDE,
        NODE_MOD, NODE_LE, NODE_GE, NODE_LT, NODE_GT, NODE_EQ, NODE_NE, NODE_AND,
        NODE_OR, NODE_NOT, NODE_POW, NODE_ASSIGN, NODE_READ, NODE_WRITE,
        NODE_SKIP, NODE_IF, NODE_WHILE, NODE_FOR, NODE_BREAK, NODE_SEQUENCE,
        NODE_PROGRAM, NODE_LET, NODE_IN, NODE_END, NODE_RETURN,
    } type;
    union {
        //union nam omogućava da svaki čvor u AST - u sadrži različite tipove podataka
        //zavisno od vrste čvora, npr. konstante imaju vrijednosti int, double, string, a neki drugi čvorovi pokazivače na lijevi i desni element u operaciji
        int intVal;
        double doubleVal;
        char *stringVal;
        //npr. ovaj struct je za binarne operacije, plus, minus, puta, ...
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } binary;
        //ovaj je za if, imamo uslov, then i else
        struct {
            struct ASTNode *cond;
            struct ASTNode *thenBranch;
            struct ASTNode *elseBranch;
        } ifNode;
        //while ima uslov i tijelo od while
        struct {
            struct ASTNode *cond;
            struct ASTNode *body;
        } whileNode;
        //for ima inicijalizaciju (i = 0), uslov (i < n), povećanje (i++) i tijelo
        struct {
            struct ASTNode *init;
            struct ASTNode *cond;
            struct ASTNode *incr;
            struct ASTNode *body;
        } forNode;
        //unarne imaju samo sebe, nemaju lijevog i desnog
        struct {
            struct ASTNode *node;
        } unary;
        //čuvanje sekvenci - niza dva čvora, da bi se povezivale promjenljive međusobno
        //npr int x; int y; spajamo u jedan čvor, ili recimo naredbe x = 5; y = x + 1;
        //ako imamo više varijabli/naredbi, logično je da idemo dva po dva, prve dvije, pa druge dvije, ...
        struct {
            struct ASTNode *nodes[2];
        } sequence;
        //program ima deklaracije i komande
        struct {
            struct ASTNode *declarations;
            struct ASTNode *commands;
        } programNode;
    };
} ASTNode;

//definišemo deklaracije funkcija za kreiranje čvorova različitih tipova
ASTNode *create_program(ASTNode *declarations, ASTNode *commands); //prvo program koji ima i LET i IN
ASTNode *create_let(ASTNode *declarations); //dio programa, osnovne komande, unutar LET idu deklaracije
ASTNode *create_in(ASTNode *commands); //unutar IN idu komande
ASTNode *create_end(); //poslije END nemamo ništa

//konstantni čvorovi sa vrijednostima intVal, doubleVal, stringVal,
//i identifikator koji ima ime promjenljive koju označava
ASTNode *create_int_const(int value);
ASTNode *create_double_const(double value);
ASTNode *create_string_const(char *value);
ASTNode *create_bool_const(int value);
ASTNode *create_identifier(char *name);
ASTNode *create_sequence(ASTNode *first, ASTNode *second);
//pravljenje sekvenci čvorova

//operatori i njihov tip
ASTNode *create_binary_operator(int type, ASTNode *left, ASTNode *right);
ASTNode *create_unary_operator(int type, ASTNode *node);
 
//petlje
ASTNode *create_if(ASTNode *cond, ASTNode *thenBranch, ASTNode *elseBranch);
ASTNode *create_while(ASTNode *cond, ASTNode *body);
ASTNode *create_for(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body);

//dodjela, čitanje i pisanje, break, skip, return
ASTNode *create_assign(ASTNode *left, ASTNode *right);
ASTNode *create_read(ASTNode *node);
ASTNode *create_write(ASTNode *node);
ASTNode *create_skip();
ASTNode *create_break();
ASTNode *create_return(ASTNode *node); 

//štampanje AST, počevši od korijena
void print_ast_helper(ASTNode *root, int depth, int current_level);
void print_ast(ASTNode *root);

#endif 
