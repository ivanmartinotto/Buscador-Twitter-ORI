#ifndef SET_H
#define SET_H

#include <stdlib.h>

typedef struct {
    unsigned char* bits;
    int size;  // número de bits
} Set;

/// Cria um novo conjunto vazio com `size` bits
Set* criarSet(int size);

/// Libera a memória usada por um Set
void liberarSet(Set* set);

/// Adiciona um elemento (docID) ao conjunto
void adicionarElemento(Set* set, int docID);

/// Verifica se um elemento está no conjunto (retorna 1 ou 0)
int contemElemento(Set* set, int docID);

/// União: result = set1 OR set2
Set* uniao(Set* set1, Set* set2);

/// Interseção: result = set1 AND set2
Set* intersecao(Set* set1, Set* set2);

/// Diferença: result = set1 AND NOT set2
Set* diferenca(Set* set1, Set* set2);

/// Cria um conjunto com todos os bits ligados (para operação NOT)
Set* universoSet(int size);

/// Imprime os elementos presentes no conjunto
void imprimirSet(Set* set);

#endif // SET_H
