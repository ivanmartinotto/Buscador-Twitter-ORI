#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "parser.h"
#include "hash.h"
#define MAX_TOKENS 100

char *tokens[MAX_TOKENS];
int num_tokens = 0;
int pos = 0;

void tokenize(char *input) {
    char *p = strtok(input, " \n\r\t()");
    while (p && num_tokens < MAX_TOKENS) {
        tokens[num_tokens++] = strdup(p);
        p = strtok(NULL, " \n\r\t()");
    }

    // Também precisamos guardar os parênteses como tokens:
    for (int i = 0; input[i]; i++) {
        if (input[i] == '(' || input[i] == ')') {
            char *t = malloc(2);
            t[0] = input[i];
            t[1] = '\0';
            tokens[num_tokens++] = t;
        }
    }
}

Expr *parse_expr();

Expr *parse_factor() {
    if (pos >= num_tokens) return NULL;

    if (strcmp(tokens[pos], "(") == 0) {
        pos++;  // consume '(' 
        Expr *e = parse_expr();
        if (pos < num_tokens && strcmp(tokens[pos], ")") == 0)
            pos++;  // consume ')'
        return e;
    }

    if (strcmp(tokens[pos], "NOT") == 0) {
        pos++;
        Expr *e = malloc(sizeof(Expr));
        e->tipo = OPER_NOT;
        e->dir = NULL;
        e->esq = parse_factor();
        return e;
    }

    Expr *e = malloc(sizeof(Expr));
    e->tipo = OPER_PALAVRA;

    char palavra[256];
    strncpy(palavra, tokens[pos++], sizeof(palavra));
    limpar_palavra(palavra);

    e->palavra = strdup(palavra);
    e->esq = e->dir = NULL;
    return e;
}

Expr *parse_term() {
    Expr *left = parse_factor();
    while (pos < num_tokens && strcmp(tokens[pos], "AND") == 0) {
        pos++;
        Expr *e = malloc(sizeof(Expr));
        e->tipo = OPER_AND;
        e->esq = left;
        e->dir = parse_factor();
        left = e;
    }
    return left;
}

Expr *parse_expr() {
    Expr *left = parse_term();
    while (pos < num_tokens && strcmp(tokens[pos], "OR") == 0) {
        pos++;
        Expr *e = malloc(sizeof(Expr));
        e->tipo = OPER_OR;
        e->esq = left;
        e->dir = parse_term();
        left = e;
    }
    return left;
}

SetNode *avaliar_expr(Expr *e, SetNode *conjunto_total, Hash *ha) {
    if (!e) return NULL;

    if (e->tipo == OPER_PALAVRA) {
        Palavra temp;
        if (buscaHash(ha, e->palavra)) {
            return set_copiar(temp.rrns); // copia todos os rrns da palavra
        } else {
            return NULL;
        }
    }

    if (e->tipo == OPER_NOT) {
        SetNode *sub = avaliar_expr(e->esq, conjunto_total, ha);
        return set_not(conjunto_total, sub);
    }

    if (e->tipo == OPER_AND) {
        SetNode *a = avaliar_expr(e->esq, conjunto_total, ha);
        SetNode *b = avaliar_expr(e->dir, conjunto_total, ha);
        return set_and(a, b);
    }

    if (e->tipo == OPER_OR) {
        SetNode *a = avaliar_expr(e->esq, conjunto_total, ha);
        SetNode *b = avaliar_expr(e->dir, conjunto_total, ha);
        return set_or(a, b);
    }

    return NULL;
}

void liberar_expr(Expr *e) {
    if (!e) return;
    if (e->palavra) free(e->palavra);
    liberar_expr(e->esq);
    liberar_expr(e->dir);
    free(e);
}