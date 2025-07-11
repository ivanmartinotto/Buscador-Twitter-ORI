#ifndef INDICE_INVERTIDO_H
#define INDICE_INVERTIDO_H
#include "../Set/set.h"

#define MAX_PALAVRAS 1000000
#define MAX_DOCS     1600000

// Inicializa as estruturas de dicionário e índice
void inicializarIndice();

// Retorna o ID da palavra, inserindo no dicionário se necessário
int obterOuInserirPalavraID(const char* palavra);

// Indexa uma palavra (via ID) associando-a a um docID
void indexarPalavra(const char* palavra, int docID);

// Retorna um Set* com todos os docIDs associados à palavra
Set* getSetFromPalavra(const char* palavra, int totalDocs);

// Libera toda a memória usada
void liberarIndice();

#endif
