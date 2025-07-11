#ifndef INDEXADOR_H
#define INDEXADOR_H

#include <stdio.h>

#define MAX_DOCS 1600000
#define TAM_LINHA 4096

// Vetor de offsets: posição de cada linha no arquivo (índice = docID)
extern long offsets[MAX_DOCS + 1];  // docID vai de 1 até MAX_DOCS

// Lê e processa o arquivo CSV, construindo o índice invertido
// Retorna o número total de documentos lidos
int processarCSV(const char* caminhoCSV);

// Recupera e imprime o texto de uma linha original, dado o docID
void exibirTextoOriginal(const char* caminhoCSV, int docID);

#endif // INDEXADOR_H
