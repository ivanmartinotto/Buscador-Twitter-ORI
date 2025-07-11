#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>
#include <stdlib.h>

// Lista encadeada de docIDs (postagens que contêm a palavra)
typedef struct PostList {
    int docID;
    struct PostList* next;
} PostList;

// Estrutura de entrada para uma palavra da hash
typedef struct WordEntry {
    char* word;
    PostList* posts;
    struct WordEntry* next; // para colisões (encadeamento externo)
} WordEntry;

// Tamanho fixo da tabela hash (de preferência número primo)
#define TABLE_SIZE 10000007

// Tabela hash 
extern WordEntry* hashTable[TABLE_SIZE];

/// Inicializa a tabela hash (define todos os ponteiros como NULL)
void inicializarHash();

/// Insere uma palavra e seu docID associado na hash
/// Se a palavra já existir, apenas adiciona o docID (se ainda não estiver presente)
void inserirPalavra(const char* palavra, int docID);

/// Retorna a lista de docIDs associados a uma palavra, ou NULL se não existir
PostList* buscarPalavra(const char* palavra);

/// Libera toda a memória alocada pela tabela hash
void liberarHash();

#endif // TABELA_HASH_H
