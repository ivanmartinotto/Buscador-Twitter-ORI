#ifndef HASH_H
#define HASH_H

#include "set.h"

#define TAM_HASH 1000003 // aumentado para melhorar dispersao

typedef struct Palavra {
    char *palavra;
    SetNode *rrn;
    struct Palavra *prox;
} Palavra;

extern Palavra *tabela[TAM_HASH];

void limpar_palavra(char *str);
void inserir_palavra(const char *palavra, int rrn);
void indexar_arquivo(const char *nome_arquivo);
Palavra* buscar_na_hash(const char *palavra);

#endif