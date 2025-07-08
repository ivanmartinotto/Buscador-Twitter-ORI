#ifndef SET_H
#define SET_H

typedef struct SetNode {
    int valor;
    struct SetNode *prox;
} SetNode;

void set_inserir(SetNode **conjunto, int valor);
SetNode* set_copiar(SetNode *orig);
SetNode* set_and(SetNode *a, SetNode *b);
SetNode* set_or(SetNode *a, SetNode *b);
SetNode* set_not(SetNode *total, SetNode *a);
void set_print(SetNode *s);
void set_free(SetNode *s);

#endif
