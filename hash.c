#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"

Palavra *tabela[TAM_HASH] = {NULL};

unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str) {
        h = (31 * h + *str++) % TAM_HASH;
    }
    return h;
}

void limpar_palavra(char *str) {
    int i, j = 0;
    for (i = 0; str[i]; i++) {
        if (isalpha((unsigned char)str[i]))
            str[j++] = tolower((unsigned char)str[i]);
    }
    str[j] = '\0';
}

void inserir_palavra(const char *palavra, int rrn) {
    unsigned int h = hash(palavra);
    Palavra *p = tabela[h];

    while (p) {
        if (strcmp(p->palavra, palavra) == 0)
            break;
        p = p->prox;
    }

    if (!p) {
        p = malloc(sizeof(Palavra));
        p->palavra = strdup(palavra);
        p->rrn = NULL;
        p->prox = tabela[h];
        tabela[h] = p;
    }

    set_inserir(&p->rrn, rrn); // sempre insere, não verifica duplicação
}

void indexar_arquivo(const char *nome_arq) {
    FILE *f = fopen(nome_arq, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo!");
        exit(1);
    }

    char linha[2048];
    int rrn = 0;

    while (fgets(linha, sizeof(linha), f)) {
        char *texto = strchr(linha, ',');
        if (!texto) continue;
        texto = strchr(texto + 1, ',');
        if (!texto) continue;
        texto++;

        char *ptr = texto;
        while (*ptr) {
            while (*ptr && !isalpha((unsigned char)*ptr)) ptr++;
            char palavra[256];
            int i = 0;
            while (*ptr && isalpha((unsigned char)*ptr) && i < 255)
                palavra[i++] = tolower((unsigned char)*ptr++);
            palavra[i] = '\0';

            if (i > 0)
                inserir_palavra(palavra, rrn);
        }

        rrn++;
    }

    fclose(f);
}

Palavra* buscar_na_hash(const char *palavra) {
    unsigned int h = hash(palavra);
    Palavra *p = tabela[h];
    while (p) {
        if (strcmp(p->palavra, palavra) == 0)
            return p;
        p = p->prox;
    }
    return NULL;
}
