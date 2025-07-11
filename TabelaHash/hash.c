#include "hash.h"
#include <string.h>
#include <ctype.h>

#define TAM_POOL 1000000  // 1 milhão de PostList por bloco (ajustável)

WordEntry* hashTable[TABLE_SIZE];

typedef struct Pool {
    PostList* blocos;
    int usado;
    struct Pool* proximo;
} Pool;

static Pool* poolInicial = NULL;

PostList* alocarPostList() {
    if (!poolInicial || poolInicial->usado >= TAM_POOL) {
        // Aloca novo bloco
        Pool* novoPool = (Pool*)malloc(sizeof(Pool));
        novoPool->blocos = (PostList*)malloc(sizeof(PostList) * TAM_POOL);
        novoPool->usado = 0;
        novoPool->proximo = poolInicial;
        poolInicial = novoPool;
    }

    return &poolInicial->blocos[poolInicial->usado++];
}

// Função de hash djb2 (boa distribuição)
unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + tolower(c); // hash * 33 + c
    }

    return hash % TABLE_SIZE;
}

void inicializarHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// Cria uma cópia em heap da string (equivalente ao strdup)
char* copiaString(const char* str) {
    char* copia = (char*)malloc(strlen(str) + 1);
    if (copia) {
        strcpy(copia, str);
    }
    return copia;
}

// Verifica se o docID já está na lista
int docIDPresente(PostList* head, int docID) {
    while (head) {
        if (head->docID == docID)
            return 1;
        head = head->next;
    }
    return 0;
}

void adicionarDocID(PostList** lista, int docID) {
    if (*lista && (*lista)->docID == docID) return; // otimização mínima

    PostList* novo = alocarPostList();
    novo->docID = docID;
    novo->next = *lista;
    *lista = novo;
}

void inserirPalavra(const char* palavra, int docID) {
    unsigned long pos = hash(palavra);
    WordEntry* atual = hashTable[pos];

    // Procura a palavra na lista
    while (atual) {
        if (strcasecmp(atual->word, palavra) == 0) {
            // Palavra encontrada, adiciona docID se necessário
            if (!docIDPresente(atual->posts, docID)) {
                adicionarDocID(&atual->posts, docID);
            }
            return;
        }
        atual = atual->next;
    }

    // Palavra não encontrada — criar novo WordEntry
    WordEntry* nova = (WordEntry*)malloc(sizeof(WordEntry));
    nova->word = copiaString(palavra);
    nova->posts = NULL;
    nova->next = hashTable[pos];
    hashTable[pos] = nova;

    // Adiciona o docID na nova entrada
    adicionarDocID(&nova->posts, docID);
}

PostList* buscarPalavra(const char* palavra) {
    unsigned long pos = hash(palavra);
    WordEntry* atual = hashTable[pos];

    while (atual) {
        if (strcasecmp(atual->word, palavra) == 0) {
            return atual->posts;
        }
        atual = atual->next;
    }

    return NULL;
}

void liberarHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        WordEntry* atual = hashTable[i];
        while (atual) {
            WordEntry* temp = atual;

            // NÃO liberar a lista de PostList individualmente — está em blocos
            // apenas liberar a palavra
            free(temp->word);
            atual = atual->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }

    // Libera blocos do pool
    Pool* atual = poolInicial;
    while (atual) {
        Pool* temp = atual;
        free(atual->blocos);  // libera bloco de PostList
        atual = atual->proximo;
        free(temp);           // libera struct Pool
    }
    poolInicial = NULL;
}

