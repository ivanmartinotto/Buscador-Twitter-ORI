#include "indexador.h"
#include "../Indice/indice_invertido.h"
#include <string.h>
#include <ctype.h>

#define TAM_TOKEN 128

long offsets[MAX_DOCS + 1] = {0};

// Remove pontuações e converte para minúsculo
void normalizarPalavra(char* str) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (isalnum((unsigned char)str[i])) {
            str[j++] = tolower(str[i]);
        }
    }
    str[j] = '\0';
}

// Quebra texto em palavras, insere cada uma na hash com docID
void processarTexto(const char* texto, int docID) {
    const char* p = texto;
    char palavra[TAM_TOKEN];
    int len = 0;

    while (*p) {
        if (isalnum((unsigned char)*p)) {
            if (len < TAM_TOKEN - 1)
                palavra[len++] = tolower(*p);
        } else {
            if (len > 0) {
                palavra[len] = '\0';
                indexarPalavra(palavra, docID);
                len = 0;
            }
        }
        p++;
    }

    if (len > 0) {
        palavra[len] = '\0';
        indexarPalavra(palavra, docID);
    }
}

int processarCSV(const char* caminhoCSV) {
    FILE* arquivo = fopen(caminhoCSV, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo CSV");
        return 0;
    }

    char linha[TAM_LINHA];
    int total = 1;

    // Cabeçalho
    fgets(linha, TAM_LINHA, arquivo);
    printf("Total de linhas processadas:");
    while (!feof(arquivo)) {
        long pos = ftell(arquivo);  // offset antes da leitura
        if (!fgets(linha, TAM_LINHA, arquivo)) break;

        int docID;
        char* campo1 = strtok(linha, ",");
        char* campo2 = strtok(NULL, ",");
        char* campo3 = strtok(NULL, "\n");

        if (campo1 && campo3) {
            docID = atoi(campo1);
            if (docID >= 1 && docID <= MAX_DOCS) {
                offsets[docID] = pos;  // salva a posição da linha
                processarTexto(campo3, docID);
                total++;
                printf("%d\n", total);
            }
        }
    }

    fclose(arquivo);
    return total;
}

void exibirTextoOriginal(const char* caminhoCSV, int docID) {
    if (docID < 1 || docID > MAX_DOCS || offsets[docID] == 0) {
        printf("[docID %d inválido ou não encontrado]\n", docID);
        return;
    }

    FILE* arquivo = fopen(caminhoCSV, "r");
    if (!arquivo) {
        perror("Erro ao reabrir o arquivo CSV");
        return;
    }

    fseek(arquivo, offsets[docID], SEEK_SET);

    char linha[TAM_LINHA];
    if (fgets(linha, TAM_LINHA, arquivo)) {
        char* campo1 = strtok(linha, ",");
        char* campo2 = strtok(NULL, ",");
        char* campo3 = strtok(NULL, "\n");

        if (campo3) {
            printf("[%d] %s\n", docID, campo3);
        }
    }

    fclose(arquivo);
}