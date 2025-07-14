#include "indexador.h"
#include "../Indice/indice_invertido.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

long* offsets = NULL;

void normalizarPalavra(char* str) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        unsigned char c = (unsigned char)str[i];
        if (isalnum(c)) {
            str[j++] = tolower(c);
        }
    }
    str[j] = '\0';
}

void processarTexto(const char* texto, int docID) {
    char copia[TAM_LINHA];
    strncpy(copia, texto, TAM_LINHA);
    copia[TAM_LINHA - 1] = '\0';

    char* token = strtok(copia, " ");
    char palavrasInseridas[512][TAM_TOKEN];
    int count = 0;

    while (token) {
        char palavra[TAM_TOKEN];
        strncpy(palavra, token, TAM_TOKEN);
        palavra[TAM_TOKEN - 1] = '\0';
        normalizarPalavra(palavra);

        int jaInserida = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(palavrasInseridas[i], palavra) == 0) {
                jaInserida = 1;
                break;
            }
        }

        if (!jaInserida && strlen(palavra) > 0) {
            strcpy(palavrasInseridas[count++], palavra);
            indexarPalavra(palavra, docID);
        }

        token = strtok(NULL, " ");
    }
}

int processarCSV(const char* caminhoCSV) {
    FILE* f = fopen(caminhoCSV, "r");
    if (!f) {
        perror("Erro abrindo CSV");
        return 0;
    }

    offsets = malloc(sizeof(long) * MAX_DOCS);
    int docID = 0;
    char linha[TAM_LINHA];

    while (fgets(linha, TAM_LINHA, f)) {
        offsets[docID] = ftell(f) - strlen(linha) - 1;

        // Cópia da linha para strtok
        char* copiaLinha = strdup(linha);
        if (!copiaLinha) continue;

        // ignora o ID
        strtok(copiaLinha, ",");
        
        // pula os zeros e uns antes do texto
        strtok(NULL, ",");
        
        // pega o texto
        char* texto = strtok(NULL, "\n");  

        if (texto) {
            processarTexto(texto, docID);
        }

        free(copiaLinha);
        docID++;
    }

    fclose(f);
    return docID;
}

void exibirTextoOriginal(const char* caminhoCSV, int docID) {
    FILE* f = fopen(caminhoCSV, "r");
    if (!f || !offsets) return;

    fseek(f, offsets[docID], SEEK_SET);
    char linha[TAM_LINHA];
    if (fgets(linha, TAM_LINHA, f)) {
        printf("%s", linha);
    }
    fclose(f);
}
