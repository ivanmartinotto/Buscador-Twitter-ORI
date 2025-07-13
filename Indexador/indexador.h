#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <stdio.h>

#define MAX_DOCS 1600000
#define TAM_LINHA 4096
#define TAM_TOKEN 128

// Ponteiro dinâmico para offsets
extern long* offsets;

// Lê e processa o arquivo CSV, construindo o índice invertido
int processarCSV(const char* caminhoCSV);

// Recupera e imprime o texto de uma linha original, dado o docID
void exibirTextoOriginal(const char* caminhoCSV, int docID);

// Função para normalização de palavras
void normalizarPalavra(char* str);

#endif // INDEXADOR_H
