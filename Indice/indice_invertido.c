#include "indice_invertido.h"
#include "../Set/set.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define TABLE_SIZE 1000003  // primo grande

typedef struct Palavra {
    char* texto;
    int id;
    struct Palavra* next; // colisões
} Palavra;

static Palavra* tabelaPalavras[TABLE_SIZE];
static char* idToPalavra[MAX_PALAVRAS];  // reverso
static int proximoID = 0;

// Índice invertido: palavraID → vetor de docIDs
static int* listas[MAX_PALAVRAS];
static int tamanhos[MAX_PALAVRAS];
static int capacidades[MAX_PALAVRAS];

unsigned long hashPalavra(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % TABLE_SIZE;
}

void inicializarIndice() {
    int i;
    for(i=0; i < TABLE_SIZE; i++){
        tabelaPalavras[i] = 0;
    }
    for(i=0; i < MAX_PALAVRAS; i++){
        listas[i] = 0;
        tamanhos[i] = 0;
        capacidades[i] = 0;
    }
    proximoID = 0;
}

int obterOuInserirPalavraID(const char* palavra) {
    unsigned long h = hashPalavra(palavra);
    Palavra* atual = tabelaPalavras[h];

    while (atual) {
        if (strcasecmp(atual->texto, palavra) == 0)
            return atual->id;
        atual = atual->next;
    }

    // Não encontrada — inserir
    Palavra* nova = malloc(sizeof(Palavra));
    nova->texto = strdup(palavra);
    nova->id = proximoID;
    nova->next = tabelaPalavras[h];
    tabelaPalavras[h] = nova;

    idToPalavra[proximoID] = nova->texto;

    return proximoID++;
}

void indexarPalavra(const char* palavra, int docID) {
    int id = obterOuInserirPalavraID(palavra);

    // Verifica capacidade
    if (capacidades[id] == 0) {
        capacidades[id] = 4;
        listas[id] = malloc(capacidades[id] * sizeof(int));
    } else if (tamanhos[id] >= capacidades[id]) {
        capacidades[id] *= 2;
        listas[id] = realloc(listas[id], capacidades[id] * sizeof(int));
    }

    // Evita duplicatas simples (último inserido)
    if (tamanhos[id] == 0 || listas[id][tamanhos[id] - 1] != docID)
        listas[id][tamanhos[id]++] = docID;
}

Set* getSetFromPalavra(const char* palavra, int totalDocs) {
    int id = obterOuInserirPalavraID(palavra);
    Set* s = criarSet(totalDocs);

    for (int i = 0; i < tamanhos[id]; i++) {
        adicionarElemento(s, listas[id][i]);
    }

    return s;
}

void liberarIndice() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Palavra* atual = tabelaPalavras[i];
        while (atual) {
            Palavra* temp = atual;
            free(temp->texto);
            atual = atual->next;
            free(temp);
        }
    }

    for (int i = 0; i < proximoID; i++) {
        free(listas[i]);
    }
}
