#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "set.h"
#include "parser.h"
#include "hash.h"

#define MAX_LINHA 2048
#define TAM_HASH 100003

// Gera conjunto com todos os RRNs do corpus (necessário para NOT)
SetNode* gerar_conjunto_total(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    SetNode *total = NULL;
    char linha[MAX_LINHA];
    int rrn = 0;
    while (fgets(linha, sizeof(linha), f)) {
        set_inserir(&total, rrn);
        rrn++;
    }

    fclose(f);
    return total;
}

// Recupera e imprime as postagens correspondentes aos RRNs
void exibir_postagens(const char *nome_arquivo, SetNode *resultados) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return;
    }

    int rrn_atual = 0;
    char linha[MAX_LINHA];

    while (fgets(linha, sizeof(linha), f)) {
        for (SetNode *p = resultados; p != NULL; p = p->prox) {
            if (p->valor == rrn_atual) {
                printf("%s", linha);
                break;
            }
        }
        rrn_atual++;
    }

    fclose(f);
}

int main() {
    const char *arquivo = "corpus.csv";

    // Cria a hash
    Hash *ha = criaHash(TAM_HASH);

    // Indexa o arquivo CSV na hash
    printf("Indexando postagens do arquivo %s...\n", arquivo);
    indexar_arquivo(ha, arquivo);

    // Gera conjunto total de RRNs
    SetNode *conjunto_total = gerar_conjunto_total(arquivo);

    // Loop de consulta
    while (1) {
        printf("\nDigite sua consulta booleana (ou 'sair'): ");
        char entrada[512];
        if (!fgets(entrada, sizeof(entrada), stdin)) break;

        if (strncmp(entrada, "sair", 4) == 0)
            break;

        num_tokens = 0;
        pos = 0;
        tokenize(entrada);
        Expr *raiz = parse_expr();

        if (!raiz) {
            printf("Consulta inválida.\n");
            continue;
        }

        // Avalia a árvore de expressão
        SetNode *resultados = avaliar_expr(raiz, conjunto_total, ha);

        if (resultados) {
            printf("Postagens encontradas:\n");
            exibir_postagens(arquivo, resultados);
            set_free(resultados);
        } else {
            printf("Nenhuma postagem encontrada.\n");
        }

        // Limpeza da árvore e tokens
        for (int i = 0; i < num_tokens; i++) {
            free(tokens[i]);
        }
        liberar_expr(raiz);
    }

    set_free(conjunto_total);
    liberaHash(ha);

    printf("Busca finalizada.\n");
    return 0;
}
