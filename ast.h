#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    enum {
        //prvo preko enumeracije definišemo sve vrste čvorova koje AST može da ima, tj. one koje štampa i uzima u obzir
        N_INTEGER_CONST, N_DOUBLE_CONST, N_STRING_CONST, N_BOOL_CONST,
        N_IDENTIFIER, N_PLUS, N_MINUS, N_MULTIPLY, N_DIVIDE,
        N_MOD, N_LE, N_GE, N_LT, N_GT, N_EQ, N_NE, N_AND,
        N_OR, N_NOT, N_POW, N_ASSIGN, N_READ, N_WRITE,
        N_SKIP, N_IF, N_WHILE, N_FOR, N_BREAK, N_SEQUENCE,
        N_PROGRAM, N_LET, N_IN, N_END, N_RETURN,
    } type;
    union {
        //union nam omogućava da svaki čvor u AST - u sadrži različite tipove podataka
        //zavisno od vrste čvora, npr. konstante imaju vrijednosti int, double, string, a neki drugi čvorovi pokazivače na lijevi i desni element u operaciji
        int int_value;
        double double_value;
        char *string_value;
        //npr. ovaj struct je za binarne operacije, plus, minus, puta, ...
        struct {
            struct Node *left;
            struct Node *right;
        } binary_operation;
        //ovaj je za if, imamo uslov, then i else
        struct {
            struct Node *condition;
            struct Node *branch_then;
            struct Node *branch_else;
        } N_if;
        //while ima uslov i tijelo od while
        struct {
            struct Node *condition;
            struct Node *body;
        } N_while;
        //for ima inicijalizaciju (i = 0), uslov (i < n), povećanje (i++) i tijelo
        struct {
            struct Node *init;
            struct Node *condition;
            struct Node *body;
        } N_for;
        //unarne imaju samo sebe, nemaju lijevog i desnog
        struct {
            struct Node *node;
        } unary_operation;
        //čuvanje sekvenci - niza dva čvora, da bi se povezivale promjenljive međusobno
        //npr int x; int y; spajamo u jedan čvor, ili recimo naredbe x = 5; y = x + 1;
        //ako imamo više varijabli/naredbi, logično je da idemo dva po dva, prve dvije, pa druge dvije, ...
        struct {
            struct Node *nodes[2];
        } sequence;
        //program ima deklaracije i komande
        struct {
            struct Node *declarations;
            struct Node *commands;
        } N_program;
    };
} Node;

//definišemo deklaracije funkcija za kreiranje čvorova različitih tipova
Node *create_program(Node *declarations, Node *commands); //prvo program koji ima i LET i IN
Node *create_let(Node *declarations); //dio programa, osnovne komande, unutar LET idu deklaracije
Node *create_in(Node *commands); //unutar IN idu komande
Node *create_end(); //poslije END nemamo ništa

//konstantni čvorovi sa vrijednostima int_value, double_value, string_value,
//i identifikator koji ima ime promjenljive koju označava
Node *create_int_const(int value);
Node *create_double_const(double value);
Node *create_string_const(char *value);
Node *create_bool_const(int value);
Node *create_identifier(char *name);
Node *create_sequence(Node *first, Node *second);
//pravljenje sekvenci čvorova

//operatori i njihov tip
Node *create_binary_operator(int type, Node *left, Node *right);
Node *create_unary_operator(int type, Node *node);
 
//petlje
Node *create_if(Node *condition, Node *branch_then, Node *branch_else);
Node *create_while(Node *condition, Node *body);
Node *create_for(Node *init, Node *condition, Node *body);

//dodjela, čitanje i pisanje, break, skip, return
Node *create_assign(Node *left, Node *right);
Node *create_read(Node *node);
Node *create_write(Node *node);
Node *create_skip();
Node *create_break();
Node *create_return(Node *node); 

//štampanje AST, počevši od korijena
void print_ast_helper(Node *root, int depth, int current_level);
void print_ast(Node *root);

#endif 
