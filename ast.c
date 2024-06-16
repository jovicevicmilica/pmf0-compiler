#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

//ovdje idu potpune funkcije, prvo rezervišemo mjesto za čvor, dodijelimo im tip i vrijednost

//prvo pravimo čvor za program, i pravimo LET i IN i dodjeljujemo im deklaracije i komande 
ASTNode *create_program(ASTNode *declarations, ASTNode *commands) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->sequence.nodes[0] = create_let(declarations);
    node->sequence.nodes[1] = create_in(commands);
    return node;
}

//sada pravimo LET i IN na osnovu deklaracija i komandi koje su dodijeljene u programu
ASTNode *create_let(ASTNode *declarations) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_LET;
    node->sequence.nodes[0] = declarations;
    return node;
}

ASTNode *create_in(ASTNode *commands) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IN;
    node->sequence.nodes[0] = commands;
    return node;
}

//END ima samo tip
ASTNode *create_end() {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_END;
    return node;
}

//pravimo sekvence, ovo je neophodno kada imamo lijevu i desnu stranu
ASTNode *create_sequence(ASTNode *first, ASTNode *second) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_SEQUENCE;
    node->sequence.nodes[0] = first;
    node->sequence.nodes[1] = second;
    return node;
}

//sada idu konstante sa tipovima
ASTNode *create_int_const(int value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_INTEGER_CONST;
    node->intVal = value;
    return node;
}

ASTNode *create_double_const(double value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_DOUBLE_CONST;
    node->doubleVal = value;
    return node;
}

ASTNode *create_string_const(char *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_STRING_CONST;
    node->stringVal = strdup(value);
    return node;
}

ASTNode *create_bool_const(int value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_BOOL_CONST;
    node->intVal = value;
    return node;
}

ASTNode *create_identifier(char *name) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->stringVal = strdup(name);
    return node;
}

//sada binarni i unarni operatori
ASTNode *create_binary_operator(int type, ASTNode *left, ASTNode *right) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = type;
    node->binary.left = left;
    node->binary.right = right;
    return node;
}

ASTNode *create_unary_operator(int type, ASTNode *node) {
    ASTNode *newNode = (ASTNode *)malloc(sizeof(ASTNode));
    newNode->type = type;
    newNode->unary.node = node;
    return newNode;
}

//petlje
ASTNode *create_if(ASTNode *cond, ASTNode *thenBranch, ASTNode *elseBranch) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->ifNode.cond = cond;
    node->ifNode.thenBranch = thenBranch;
    node->ifNode.elseBranch = elseBranch;
    return node;
}

ASTNode *create_while(ASTNode *cond, ASTNode *body) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->whileNode.cond = cond;
    node->whileNode.body = body;
    return node;
}

ASTNode *create_for(ASTNode *init, ASTNode *cond, ASTNode *incr, ASTNode *body) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    node->forNode.init = init;
    node->forNode.cond = cond;
    node->forNode.incr = incr;
    node->forNode.body = body;
    return node;
}

ASTNode *create_assign(ASTNode *left, ASTNode *right) {
    return create_binary_operator(NODE_ASSIGN, left, right);
}

ASTNode *create_read(ASTNode *node) {
    ASTNode *newNode = (ASTNode *)malloc(sizeof(ASTNode));
    newNode->type = NODE_READ;
    newNode->unary.node = node;
    return newNode;
}

ASTNode *create_write(ASTNode *node) {
    ASTNode *newNode = (ASTNode *)malloc(sizeof(ASTNode));
    newNode->type = NODE_WRITE;
    newNode->unary.node = node;
    return newNode;
}

ASTNode *create_skip() {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_SKIP;
    return node;
}

ASTNode *create_break() {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_BREAK;
    return node;
}

ASTNode *create_return(ASTNode *node) {
    ASTNode *newNode = (ASTNode *)malloc(sizeof(ASTNode));
    newNode->type = NODE_RETURN;
    newNode->unary.node = node;
    return newNode;
}

//pomoćna funkcija koja vraća ime operatora na osnovu njegovog tipa, radi lakšeg štampanja AST
const char* get_operator_name(int type) {
    switch (type) {
        case NODE_PLUS: return "+";
        case NODE_MINUS: return "-";
        case NODE_MULTIPLY: return "*";
        case NODE_DIVIDE: return "/";
        case NODE_MOD: return "%";
        case NODE_LE: return "<=";
        case NODE_GE: return ">=";
        case NODE_LT: return "<";
        case NODE_GT: return ">";
        case NODE_EQ: return "==";
        case NODE_NE: return "!=";
        case NODE_AND: return "&&";
        case NODE_OR: return "||";
        case NODE_NOT: return "!";
        case NODE_POW: return "^";
        case NODE_ASSIGN: return ":=";
        default: return "unknown_op";
    }
}

//štampanje razmaka za AST
void print_indent(int level) {
    for (int i = 0; i < level; ++i) {
        printf(" ");
    }
}

//štampanje informacija o nivou
void print_node_info(int current_level, const char* node_type, const char* value) {
    print_indent(current_level);
    if (value) { //ako postoji vrijednost, štampamo je unutar []
        printf("%d. nivo: %s [%s]\n", current_level, node_type, value);
    } else {
        printf("%d. nivo: %s\n", current_level, node_type);
    }
}

//funkcija za pomoćno štampanje AST
void print_ast_helper(ASTNode *root, int depth, int current_level) {
    if (root == NULL) return; //ako nema korijena, ne vraćemo stablo
    const char *op_name = get_operator_name(root->type); //dobijemo tip operatora
    char buffer[50];

    switch (root->type) {
        case NODE_INTEGER_CONST:
            snprintf(buffer, sizeof(buffer), "%d", root->intVal); //smještamo vrijednost u bafer
            print_node_info(current_level, "int konstanta", buffer); //štampamo informacije o čvoru iz bafera
            break;

        case NODE_DOUBLE_CONST:
            snprintf(buffer, sizeof(buffer), "%lf", root->doubleVal);
            print_node_info(current_level, "double konstanta", buffer);
            break;

        case NODE_STRING_CONST:
            print_node_info(current_level, "string konstanta", root->stringVal);
            break;

        case NODE_BOOL_CONST:
            print_node_info(current_level, "bool konstanta", root->intVal ? "true" : "false");
            break;

        case NODE_IDENTIFIER:
            print_node_info(current_level, "identifikator", root->stringVal);
            break;

        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MULTIPLY:
        case NODE_DIVIDE:
        case NODE_MOD:
        case NODE_LE:
        case NODE_GE:
        case NODE_LT:
        case NODE_GT:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_NOT:
        case NODE_POW:
        case NODE_ASSIGN:
            print_node_info(current_level, op_name, NULL);
            print_ast_helper(root->binary.left, depth + 1, current_level + 1);
            print_ast_helper(root->binary.right, depth + 1, current_level + 1);
            break;

        case NODE_IF:
            print_node_info(current_level, "IF", NULL);
            print_ast_helper(root->ifNode.cond, depth + 1, current_level + 1);
            print_node_info(current_level + 1, "THEN", NULL);
            print_ast_helper(root->ifNode.thenBranch, depth + 2, current_level + 2);
            if (root->ifNode.elseBranch) {
                print_node_info(current_level + 1, "ELSE", NULL);
                print_ast_helper(root->ifNode.elseBranch, depth + 2, current_level + 2);
            }
            break;

        case NODE_WHILE:
            print_node_info(current_level, "WHILE", NULL);
            print_ast_helper(root->whileNode.cond, depth + 1, current_level + 1);
            print_node_info(current_level + 1, "DO", NULL);
            print_ast_helper(root->whileNode.body, depth + 2, current_level + 2);
            break;

        case NODE_FOR:
            print_node_info(current_level, "FOR", NULL);
            print_ast_helper(root->forNode.init, depth + 1, current_level + 1);
            print_ast_helper(root->forNode.cond, depth + 1, current_level + 1);
            if (root->forNode.incr) {
                print_node_info(current_level + 1, "INCR", NULL);
                print_ast_helper(root->forNode.incr, depth + 2, current_level + 2);
            }
            print_node_info(current_level + 1, "DO", NULL);
            print_ast_helper(root->forNode.body, depth + 2, current_level + 2);
            break;

        case NODE_READ:
            print_node_info(current_level, "READ", NULL);
            print_ast_helper(root->unary.node, depth, current_level + 1);
            break;

        case NODE_WRITE:
            print_node_info(current_level, "WRITE", NULL);
            print_ast_helper(root->unary.node, depth, current_level + 1);
            break;

        case NODE_SKIP:
            print_node_info(current_level, "SKIP", NULL);
            break;

        case NODE_BREAK:
            print_node_info(current_level, "BREAK", NULL);
            break;

        case NODE_SEQUENCE:
            //ne ide print ovdje, sekvence su sa lijevih i desnih strana, bespotrebno je
            print_ast_helper(root->sequence.nodes[0], depth, current_level); 
            print_ast_helper(root->sequence.nodes[1], depth, current_level); 
            break;

        case NODE_PROGRAM:
            print_node_info(current_level, "PROGRAM", NULL);
            print_ast_helper(root->sequence.nodes[0], depth, current_level + 1);
            print_ast_helper(root->sequence.nodes[1], depth, current_level + 1);
            print_node_info(current_level + 1, "END", NULL);
            break;

        case NODE_LET:
            print_node_info(current_level, "LET", NULL);
            print_ast_helper(root->sequence.nodes[0], depth + 1, current_level + 1);
            break;

        case NODE_IN:
            print_node_info(current_level, "IN", NULL);
            print_ast_helper(root->sequence.nodes[0], depth + 1, current_level + 1);
            break;

        case NODE_END:
            print_node_info(current_level, "END", NULL);
            break;

        default:
            print_node_info(current_level, "NEPOZNAT ČVOR", NULL);
            break;
    }
}

void print_ast(ASTNode *root) {
    printf("------AST------\n");
    print_ast_helper(root, 100, 1);
}
