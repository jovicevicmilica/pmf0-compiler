#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//ovdje idu potpune funkcije, prvo rezervišemo mjesto za čvor, dodijelimo im tip i vrijednost

//prvo pravimo čvor za program, i pravimo LET i IN i dodjeljujemo im deklaracije i komande 
Node *create_program(Node *declarations, Node *commands) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_PROGRAM;
    node->sequence.nodes[0] = create_let(declarations);
    node->sequence.nodes[1] = create_in(commands);
    return node;
}

//sada pravimo LET i IN na osnovu deklaracija i komandi koje su dodijeljene u programu
Node *create_let(Node *declarations) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_LET;
    node->sequence.nodes[0] = declarations;
    return node;
}

Node *create_in(Node *commands) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_IN;
    node->sequence.nodes[0] = commands;
    return node;
}

//END ima samo tip
Node *create_end() {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_END;
    return node;
}

//pravimo sekvence, ovo je neophodno kada imamo lijevu i desnu stranu
Node *create_sequence(Node *first, Node *second) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_SEQUENCE;
    node->sequence.nodes[0] = first;
    node->sequence.nodes[1] = second;
    return node;
}

//sada idu konstante sa tipovima
Node *create_int_const(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_INTEGER_CONST;
    node->int_value = value;
    return node;
}

Node *create_double_const(double value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_DOUBLE_CONST;
    node->double_value = value;
    return node;
}

Node *create_string_const(char *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_STRING_CONST;
    node->string_value = strdup(value);
    return node;
}

Node *create_bool_const(int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_BOOL_CONST;
    node->int_value = value;
    return node;
}

Node *create_identifier(char *name) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_IDENTIFIER;
    node->string_value = strdup(name);
    return node;
}

//sada binarni i unarni operatori
Node *create_binary_operator(int type, Node *left, Node *right) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = type;
    node->binary_operation.left = left;
    node->binary_operation.right = right;
    return node;
}

Node *create_unary_operator(int type, Node *node) {
    Node *N_new = (Node *)malloc(sizeof(Node));
    N_new->type = type;
    N_new->unary_operation.node = node;
    return N_new;
}

//petlje
Node *create_if(Node *condition, Node *branch_then, Node *branch_else) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_IF;
    node->N_if.condition = condition;
    node->N_if.branch_then = branch_then;
    node->N_if.branch_else = branch_else;
    return node;
}

Node *create_while(Node *condition, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_WHILE;
    node->N_while.condition = condition;
    node->N_while.body = body;
    return node;
}

Node *create_for(Node *init, Node *condition, Node *body) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_FOR;
    node->N_for.init = init;
    node->N_for.condition = condition;
    node->N_for.body = body;
    return node;
}

Node *create_assign(Node *left, Node *right) {
    return create_binary_operator(N_ASSIGN, left, right);
}

Node *create_read(Node *node) {
    Node *N_new = (Node *)malloc(sizeof(Node));
    N_new->type = N_READ;
    N_new->unary_operation.node = node;
    return N_new;
}

Node *create_write(Node *node) {
    Node *N_new = (Node *)malloc(sizeof(Node));
    N_new->type = N_WRITE;
    N_new->unary_operation.node = node;
    return N_new;
}

Node *create_skip() {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_SKIP;
    return node;
}

Node *create_break() {
    Node *node = (Node *)malloc(sizeof(Node));
    node->type = N_BREAK;
    return node;
}

Node *create_return(Node *node) {
    Node *N_new = (Node *)malloc(sizeof(Node));
    N_new->type = N_RETURN;
    N_new->unary_operation.node = node;
    return N_new;
}

//pomoćna funkcija koja vraća ime operatora na osnovu njegovog tipa, radi lakšeg štampanja AST
const char* get_operator_name(int type) {
    switch (type) {
        case N_PLUS: return "+";
        case N_MINUS: return "-";
        case N_MULTIPLY: return "*";
        case N_DIVIDE: return "/";
        case N_MOD: return "%";
        case N_LE: return "<=";
        case N_GE: return ">=";
        case N_LT: return "<";
        case N_GT: return ">";
        case N_EQ: return "==";
        case N_NE: return "!=";
        case N_AND: return "&&";
        case N_OR: return "||";
        case N_NOT: return "!";
        case N_POW: return "^";
        case N_ASSIGN: return ":=";
        default: return "unknown_op"; //ako je nepoznat operator, nepravilan
    }
}

//štampanje razmaka za AST
void print_indent(int level) {
    for (int i = 0; i < level; ++i) {
        printf(" ");
    }
}

//štampanje informacija o nivou
void print_N_info(int current_level, const char* N_type, const char* value) {
    print_indent(current_level);
    if (value) { //ako postoji vrijednost, štampamo je unutar []
        printf("%d. nivo: %s [%s]\n", current_level, N_type, value);
    } else {
        printf("%d. nivo: %s\n", current_level, N_type);
    }
}

//funkcija za pomoćno štampanje AST
void print_ast_helper(Node *root, int depth, int current_level) {
    if (root == NULL) return; //ako nema korijena, ne vraćemo stablo
    const char *op_name = get_operator_name(root->type); //dobijemo tip operatora
    char buffer[50];

    switch (root->type) {
        case N_INTEGER_CONST:
            snprintf(buffer, sizeof(buffer), "%d", root->int_value); //smještamo vrijednost u bafer
            print_N_info(current_level, "int konstanta", buffer); //štampamo informacije o čvoru iz bafera
            break;

        case N_DOUBLE_CONST:
            snprintf(buffer, sizeof(buffer), "%lf", root->double_value);
            print_N_info(current_level, "double konstanta", buffer);
            break;

        case N_STRING_CONST:
            print_N_info(current_level, "string konstanta", root->string_value);
            break;

        case N_BOOL_CONST:
            print_N_info(current_level, "bool konstanta", root->int_value ? "true" : "false");
            break;

        case N_IDENTIFIER:
            if(strcmp(root->string_value, "int") == 0 || strcmp(root->string_value, "double") == 0 || strcmp(root->string_value, "string") == 0 || strcmp(root->string_value, "bool") == 0) {
                print_N_info(current_level, "kljucna rijec", root->string_value);
                break;
            }
            print_N_info(current_level, "identifikator", root->string_value);
            break;

        case N_PLUS:
        case N_MINUS:
        case N_MULTIPLY:
        case N_DIVIDE:
        case N_MOD:
        case N_LE:
        case N_GE:
        case N_LT:
        case N_GT:
        case N_EQ:
        case N_NE:
        case N_AND:
        case N_OR:
        case N_NOT:
        case N_POW:
        case N_ASSIGN:
            print_N_info(current_level, op_name, NULL);
            print_ast_helper(root->binary_operation.left, depth + 1, current_level + 1);
            print_ast_helper(root->binary_operation.right, depth + 1, current_level + 1);
            break;

        case N_IF:
            print_N_info(current_level, "IF", NULL);
            print_ast_helper(root->N_if.condition, depth + 1, current_level + 1);
            print_N_info(current_level + 1, "THEN", NULL);
            print_ast_helper(root->N_if.branch_then, depth + 2, current_level + 2);
            if (root->N_if.branch_else) {
                print_N_info(current_level + 1, "ELSE", NULL);
                print_ast_helper(root->N_if.branch_else, depth + 2, current_level + 2);
            }
            break;

        case N_WHILE:
            print_N_info(current_level, "WHILE", NULL);
            print_ast_helper(root->N_while.condition, depth + 1, current_level + 1);
            print_N_info(current_level + 1, "DO", NULL);
            print_ast_helper(root->N_while.body, depth + 2, current_level + 2);
            break;

        case N_FOR:
            print_N_info(current_level, "FOR", NULL);
            print_ast_helper(root->N_for.init, depth + 1, current_level + 1); //assign ne dodajemo, jer već imamo dio za assign
            print_N_info(current_level + 1, "TO", NULL);
            print_ast_helper(root->N_for.condition, depth + 2, current_level + 2);
            print_N_info(current_level + 1, "DO", NULL);
            print_ast_helper(root->N_for.body, depth + 2, current_level + 2); 
            break;

        case N_READ:
            print_N_info(current_level, "READ", NULL);
            print_ast_helper(root->unary_operation.node, depth, current_level + 1);
            break;

        case N_WRITE:
            print_N_info(current_level, "WRITE", NULL);
            print_ast_helper(root->unary_operation.node, depth, current_level + 1);
            break;

        case N_SKIP:
            print_N_info(current_level, "SKIP", NULL); //nema uvećavanja nivoa, nemaju sinove
            break;

        case N_BREAK:
            print_N_info(current_level, "BREAK", NULL);
            break;

        case N_SEQUENCE:
            //ne ide print ovdje, sekvence su sa lijevih i desnih strana, bespotrebno je
            print_ast_helper(root->sequence.nodes[0], depth, current_level); 
            print_ast_helper(root->sequence.nodes[1], depth, current_level); 
            break;

        case N_PROGRAM:
            print_N_info(current_level, "PROGRAM", NULL);
            print_ast_helper(root->sequence.nodes[0], depth, current_level + 1);
            print_ast_helper(root->sequence.nodes[1], depth, current_level + 1);
            print_N_info(current_level + 1, "END", NULL);
            break;

        case N_LET:
            print_N_info(current_level, "LET", NULL);
            print_ast_helper(root->sequence.nodes[0], depth + 1, current_level + 1);
            break;

        case N_IN:
            print_N_info(current_level, "IN", NULL);
            print_ast_helper(root->sequence.nodes[0], depth + 1, current_level + 1);
            break;

        case N_END:
            print_N_info(current_level, "END", NULL);
            break;

        default:
            print_N_info(current_level, "NEPOZNAT ČVOR", NULL);
            break;
    }
}

void print_ast(Node *root) {
    printf("\n------AST------\n\n");
    print_ast_helper(root, 100, 1);
    printf("\n");
}
