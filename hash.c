#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hash.h"

Hash* criaHash(int TABLE_SIZE){
    Hash* ha = (Hash*) malloc(sizeof(Hash));
    if(ha != NULL){
        int i;
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->buckets = (struct Palavra**) malloc(TABLE_SIZE * sizeof(struct Palavra*));
        if(ha->buckets == NULL){
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        for(i=0; i < ha->TABLE_SIZE; i++)
            ha->buckets[i] = NULL;
    }
    return ha;
}

void liberaHash(Hash* ha){
    if(ha != NULL){
        int i;
        for(i=0; i < ha->TABLE_SIZE; i++){
            if(ha->buckets[i] != NULL)
                free(ha->buckets[i]);
        }
        free(ha->buckets);
        free(ha);
    }
}

int valorString(char *str){
    int i, valor = 7;
    int tam = strlen(str);
    for(i=0; i < tam; i++)
        valor = 31 * valor + (int) str[i];
    return (valor & 0x7FFFFFFF);
}

int chaveDivisao(int chave, int TABLE_SIZE){
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

// Na inserção, verifica se a palavra já existe no bucket
int insereHash(Hash *ha, char *palavra, int rrn) {
    if (ha == NULL || palavra == NULL)
        return 0;

    int chave = valorString(palavra);
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);

    Palavra *atual = ha->buckets[pos];
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0) {
            set_inserir(&atual->rrns, rrn);  // Palavra já existe, adiciona rrn
            return 1;
        }
        atual = atual->prox;
    }

    // Se não existir, cria novo nó Palavra
    Palavra *nova = malloc(sizeof(Palavra));
    nova->palavra = strdup(palavra);
    nova->rrns = NULL;
    set_inserir(&nova->rrns, rrn);  // Insere primeiro rrn

    nova->prox = ha->buckets[pos];
    ha->buckets[pos] = nova;

    ha->qtd++;
    return 1;
}

// Busca uma palavra na hash
Palavra* buscaHash(Hash *ha, char *palavra) {
    if (ha == NULL || palavra == NULL)
        return NULL;

    int chave = valorString(palavra);
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);

    Palavra *atual = ha->buckets[pos];
    while (atual != NULL) {
        if (strcmp(atual->palavra, palavra) == 0)
            return atual;
        atual = atual->prox;
    }

    return NULL;
}

void limpar_palavra(char *str) {
    int i, j = 0;
    for (i = 0; str[i]; i++) {
        if (isalnum((unsigned char)str[i]))
            str[j++] = tolower((unsigned char)str[i]);
    }
    str[j] = '\0';
}

void indexar_arquivo(Hash *ha, const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo CSV");
        exit(1);
    }

    char linha[2048];
    int rrn = 0;

    while (fgets(linha, sizeof(linha), f)) {
        // Localiza o campo do texto (terceira vírgula)
        char *texto = strchr(linha, ',');
        if (!texto) continue;
        texto = strchr(texto + 1, ',');
        if (!texto) continue;
        texto++; // avança além da segunda vírgula

        // Extrai as buckets
        char *ptr = texto;
        while (*ptr) {
            // Pula separadores
            while (*ptr && !isalnum((unsigned char)*ptr)) ptr++;

            // Extrai palavra
            char palavra[256];
            int i = 0;
            while (*ptr && isalnum((unsigned char)*ptr) && i < 255)
                palavra[i++] = tolower((unsigned char)*ptr++);
            palavra[i] = '\0';

            if (strlen(palavra) > 0) {
                insereHash(ha, palavra, rrn);
            }
        }

        rrn++;
    }

    fclose(f);
}
