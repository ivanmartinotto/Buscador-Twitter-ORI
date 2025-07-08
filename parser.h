#ifndef PARSER_H
#define PARSER_H

#include "set.h"

typedef enum { OPER_PALAVRA, OPER_AND, OPER_OR, OPER_NOT } TipoNo;

typedef struct Expr {
    TipoNo tipo;
    char *palavra;
    struct Expr *esq;
    struct Expr *dir;
} Expr;

extern char *tokens[];
extern int num_tokens;
extern int pos;

void tokenize(char *entrada);
Expr* parse_expr();
SetNode* avaliar_expr(Expr *e, SetNode *conjunto_total);
void liberar_expr(Expr *e);

#endif
