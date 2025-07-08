#ifndef HASH_H
#define HASH_H

#include "set.h"

// Cada palavra tem um conjunto de RRNs onde aparece
typedef struct Palavra {
    char *palavra;
    SetNode *rrns;            // Lista de RRNs (set)
    struct Palavra *prox;     // Próximo elemento da lista encadeada do bucket
} Palavra;

// Tabela hash genérica
typedef struct Hash {
    int TABLE_SIZE;
    int qtd;
    Palavra **buckets;        // Vetor de ponteiros para listas de Palavra
} Hash;

// Cria e libera a hash
Hash* criaHash(int TABLE_SIZE);
void liberaHash(Hash* ha);

// Funções de hashing
int valorString(char *str);
int chaveDivisao(int chave, int TABLE_SIZE);

// Inserção e busca
int insereHash(Hash* ha, char *palavra, int rrn);
Palavra* buscaHash(Hash* ha, char *palavra);

void limpar_palavra(char *str);
void indexar_arquivo(Hash *ha, const char *nome_arquivo);

#endif