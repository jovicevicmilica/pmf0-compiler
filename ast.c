#include "ast.h"

//funkcija za kreiranje novog čvora i postavljanje njegovog tipa
static Node *new_node(int type) {
    Node *node = CREATE_NODE();
    SET_NODE_TYPE(node, type);
    return node;
}

//kreiranje čvorova za ast

//prvo pravimo čvor za program, i pravimo LET i IN i dodjeljujemo im deklaracije i komande
Node *create_program(Node *declarations, Node *commands) {
    Node *node = new_node(N_PROGRAM);
    node->sequence.nodes[0] = create_let(declarations);
    node->sequence.nodes[1] = create_in(commands);
    return node;
}

//sada pravimo LET i IN na osnovu deklaracija i komandi koje su dodijeljene u programu
Node *create_let(Node *declarations) {
    Node *node = new_node(N_LET);
    node->sequence.nodes[0] = declarations;
    return node;
}

Node *create_in(Node *commands) {
    Node *node = new_node(N_IN);
    node->sequence.nodes[0] = commands;
    return node;
}

//END ima samo tip
Node *create_end() {
    return new_node(N_END);
}

//sada idu konstante sa tipovima
Node *create_int_const(int value) {
    Node *node = new_node(N_INTEGER_CONST);
    node->int_value = value;
    return node;
}

Node *create_double_const(double value) {
    Node *node = new_node(N_DOUBLE_CONST);
    node->double_value = value;
    return node;
}

Node *create_string_const(char *value) {
    Node *node = new_node(N_STRING_CONST);
    node->string_value = strdup(value);
    return node;
}

Node *create_bool_const(int value) {
    Node *node = new_node(N_BOOL_CONST);
    node->int_value = value;
    return node;
}

//identifikatori
Node *create_identifier(char *name) {
    Node *node = new_node(N_IDENTIFIER);
    node->string_value = strdup(name);
    return node;
}

//pravimo sekvence, ovo je neophodno kada imamo lijevu i desnu stranu
Node *create_sequence(Node *first, Node *second) {
    Node *node = new_node(N_SEQUENCE);
    node->sequence.nodes[0] = first;
    node->sequence.nodes[1] = second;
    return node;
}

//sada binarni i unarni operatori
Node *create_binary_operator(int type, Node *left, Node *right) {
    Node *node = new_node(type);
    node->binary_operation.left = left;
    node->binary_operation.right = right;
    return node;
}

Node *create_unary_operator(int type, Node *operand) {
    Node *node = new_node(type);
    node->unary_operation.node = operand;
    return node;
}

//petlje
Node *create_if(Node *condition, Node *branch_then, Node *branch_else) {
    Node *node = new_node(N_IF);
    node->N_if.condition = condition;
    node->N_if.branch_then = branch_then;
    node->N_if.branch_else = branch_else;
    return node;
}

Node *create_while(Node *condition, Node *body) {
    Node *node = new_node(N_WHILE);
    node->N_while.condition = condition;
    node->N_while.body = body;
    return node;
}

Node *create_for(Node *init, Node *condition, Node *body) {
    Node *node = new_node(N_FOR);
    node->N_for.init = init;
    node->N_for.condition = condition;
    node->N_for.body = body;
    return node;
}

Node *create_assign(Node *left, Node *right) {
    return create_binary_operator(N_ASSIGN, left, right);
}

Node *create_read(Node *operand) {
    Node *node = new_node(N_READ);
    node->unary_operation.node = operand;
    return node;
}

Node *create_write(Node *operand) {
    Node *node = new_node(N_WRITE);
    node->unary_operation.node = operand;
    return node;
}

Node *create_skip() {
    return new_node(N_SKIP);
}

Node *create_break() {
    return new_node(N_BREAK);
}

Node *create_return(Node *operand) {
    Node *node = new_node(N_RETURN);
    node->unary_operation.node = operand;
    return node;
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
        default: return "unknown_op";
    }
}

//štampa razmake za ast
void print_indent(int level) {
    for (int i = 0; i < level; ++i) {
        printf(" ");
    }
}

//štampa informacije o čvoru
void print_N_info(int current_level, const char* N_type, const char* value) {
    print_indent(current_level);
    if (value) { //ako postoji vrijednost, štampamo je unutar []
        printf("%d. nivo: %s [%s]\n", current_level, N_type, value);
    } else {
        printf("%d. nivo: %s\n", current_level, N_type);
    }
}

//pomoćna funkcija za štampanje ast
void print_ast_helper(Node *root, int depth, int current_level) {
    if (!root) return;

    const char *op_name = get_operator_name(root->type);
    char buffer[50];

    switch (root->type) {
        case N_INTEGER_CONST:
            snprintf(buffer, sizeof(buffer), "%d", root->int_value);
            print_N_info(current_level, "int konstanta", buffer);
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
            if (strcmp(root->string_value, "int") == 0 || strcmp(root->string_value, "double") == 0 || strcmp(root->string_value, "string") == 0 || strcmp(root->string_value, "bool") == 0) {
                print_N_info(current_level, "kljucna rijec", root->string_value);
            } else {
                print_N_info(current_level, "identifikator", root->string_value);
            }
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
            print_N_info(current_level, "FI", NULL);
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
            print_N_info(current_level, "SKIP", NULL);
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

//funkcija za oslobađanje memorije zauzete od čvorova
static void free_node(Node *node) {
    if (!node) return;

    switch (node->type) {
        case N_INTEGER_CONST:
        case N_DOUBLE_CONST:
        case N_BOOL_CONST:
            //nema se čega osloboditi
            break;
        case N_STRING_CONST:
        case N_IDENTIFIER:
            free(node->string_value);
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
        case N_POW:
        case N_ASSIGN:
            free_node(node->binary_operation.left);
            free_node(node->binary_operation.right);
            break;
        case N_NOT:
        case N_READ:
        case N_WRITE:
        case N_RETURN:
            free_node(node->unary_operation.node);
            break;
        case N_IF:
            free_node(node->N_if.condition);
            free_node(node->N_if.branch_then);
            free_node(node->N_if.branch_else);
            break;
        case N_WHILE:
            free_node(node->N_while.condition);
            free_node(node->N_while.body);
            break;
        case N_FOR:
            free_node(node->N_for.init);
            free_node(node->N_for.condition);
            free_node(node->N_for.body);
            break;
        case N_SEQUENCE:
            free_node(node->sequence.nodes[0]);
            free_node(node->sequence.nodes[1]);
            break;
        case N_PROGRAM:
            free_node(node->sequence.nodes[0]);
            free_node(node->sequence.nodes[1]);
            break;
        case N_LET:
        case N_IN:
            free_node(node->sequence.nodes[0]);
            break;
        case N_SKIP:
        case N_BREAK:
        case N_END:
            //nemamo čega da se oslobodimo
            break;
        default:
            fprintf(stderr, "Nepoznat tip: %d\n", node->type);
            break;
    }
    free(node);
}

//oslobađanje čitavog stabla (memorijski)
void free_ast(Node *root) {
    free_node(root);
}

//funkcija za štampanje ast, počevši od korijena
void print_ast(Node *root) {
    printf("\n--------------AST--------------\n");
    print_ast_helper(root, 100, 1);
    printf("-------------------------------\n");
    free_ast(root); //na kraju oslobađamo memoriju
}

