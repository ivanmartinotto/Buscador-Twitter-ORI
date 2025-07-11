#include "set.h"
#include <stdio.h>
#include <string.h>

#define BITS_POR_BYTE 8

Set* criarSet(int size) {
    Set* set = (Set*)malloc(sizeof(Set));
    if (!set) return NULL;

    set->size = size;
    int numBytes = (size + BITS_POR_BYTE - 1) / BITS_POR_BYTE;
    set->bits = (unsigned char*)calloc(numBytes, sizeof(unsigned char));
    return set;
}

void liberarSet(Set* set) {
    if (set) {
        free(set->bits);
        free(set);
    }
}

void adicionarElemento(Set* set, int docID) {
    if (docID < 0 || docID >= set->size) return;

    int byteIndex = docID / BITS_POR_BYTE;
    int bitIndex = docID % BITS_POR_BYTE;

    set->bits[byteIndex] |= (1 << bitIndex);
}

int contemElemento(Set* set, int docID) {
    if (docID < 0 || docID >= set->size) return 0;

    int byteIndex = docID / BITS_POR_BYTE;
    int bitIndex = docID % BITS_POR_BYTE;

    return (set->bits[byteIndex] & (1 << bitIndex)) != 0;
}

Set* uniao(Set* a, Set* b) {
    if (a->size != b->size) return NULL;

    Set* result = criarSet(a->size);
    int numBytes = (a->size + BITS_POR_BYTE - 1) / BITS_POR_BYTE;

    for (int i = 0; i < numBytes; i++) {
        result->bits[i] = a->bits[i] | b->bits[i];
    }

    return result;
}

Set* intersecao(Set* a, Set* b) {
    if (a->size != b->size) return NULL;

    Set* result = criarSet(a->size);
    int numBytes = (a->size + BITS_POR_BYTE - 1) / BITS_POR_BYTE;

    for (int i = 0; i < numBytes; i++) {
        result->bits[i] = a->bits[i] & b->bits[i];
    }

    return result;
}

Set* diferenca(Set* a, Set* b) {
    if (a->size != b->size) return NULL;

    Set* result = criarSet(a->size);
    int numBytes = (a->size + BITS_POR_BYTE - 1) / BITS_POR_BYTE;

    for (int i = 0; i < numBytes; i++) {
        result->bits[i] = a->bits[i] & (~b->bits[i]);
    }

    return result;
}

Set* universoSet(int size) {
    Set* set = criarSet(size);
    int numBytes = (size + BITS_POR_BYTE - 1) / BITS_POR_BYTE;

    for (int i = 0; i < numBytes; i++) {
        set->bits[i] = 0xFF;
    }

    // Limpa os bits além do tamanho real
    int sobra = numBytes * BITS_POR_BYTE - size;
    if (sobra > 0) {
        set->bits[numBytes - 1] &= (0xFF >> sobra);
    }

    return set;
}

void imprimirSet(Set* set) {
    for (int i = 0; i < set->size; i++) {
        if (contemElemento(set, i)) {
            printf("%d ", i);
        }
    }
    printf("\n");
}
