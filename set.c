#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "set.h"

// Insere valor no conjunto
void set_inserir(SetNode **conjunto, int valor){
    SetNode *p = *conjunto;
    while (p){
        if (p->valor == valor) return;
        p = p->prox;
    }
    SetNode *novo = malloc(sizeof(SetNode));
    novo->valor = valor;
    novo->prox = *conjunto;
    *conjunto = novo;
}

// copia um conjunto
SetNode* set_copiar(SetNode *orig){
    SetNode *novo = NULL;
    while (orig){
        set_inserir(&novo, orig->valor);
        orig = orig->prox;
    }
    return novo;
}

// Interseção (AND)
SetNode* set_and(SetNode *a, SetNode *b) {
    SetNode *res = NULL;
    for (SetNode *pa = a; pa; pa = pa->prox) {
        for (SetNode *pb = b; pb; pb = pb->prox) {
            if (pa->valor == pb->valor) {
                set_inserir(&res, pa->valor);
                break;
            }
        }
    }
    return res;
}

// União (OR)
SetNode* set_or(SetNode *a, SetNode *b) {
    SetNode *res = set_copiar(a);
    for (SetNode *pb = b; pb; pb = pb->prox) {
        set_inserir(&res, pb->valor);
    }
    return res;
}

// Diferença (NOT A): conjunto total - A
SetNode* set_not(SetNode *total, SetNode *a) {
    SetNode *res = NULL;
    for (SetNode *pt = total; pt; pt = pt->prox) {
        bool encontrado = false;
        for (SetNode *pa = a; pa; pa = pa->prox) {
            if (pt->valor == pa->valor) {
                encontrado = true;
                break;
            }
        }
        if (!encontrado)
            set_inserir(&res, pt->valor);
    }
    return res;
}

// Exibir elementos do conjunto
void set_print(SetNode *s) {
    while (s) {
        printf("%d ", s->valor);
        s = s->prox;
    }
    printf("\n");
}

// Liberar memória
void set_free(SetNode *s) {
    while (s) {
        SetNode *tmp = s;
        s = s->prox;
        free(tmp);
    }
}

