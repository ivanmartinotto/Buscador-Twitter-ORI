#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "set.h"      // ou use #include "set.h" se modularizar
#include "parser.h"   // contém Expr, parser e avaliação
#include "hash.h"// função indexar_arquivo e tabela hash global

#define MAX_LINHA 1024

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

// Limpa tokens e árvore entre consultas
void limpar_tokens_e_arvore(char **tokens, int n, Expr *raiz) {
    for (int i = 0; i < n; i++) {
        free(tokens[i]);
    }
    liberar_expr(raiz);
}

int main() {
    const char *arquivo = "corpus_test.csv";

    // Passo 1: Indexa todas as palavras do corpus
    printf("Indexando postagens do arquivo %s...\n", arquivo);
    indexar_arquivo(arquivo);

    // Passo 2: Gera conjunto total de RRNs
    SetNode *conjunto_total = gerar_conjunto_total(arquivo);

    // Loop de busca
    while (1) {
        printf("\nDigite sua consulta booleana (ou 'sair'): ");
        char entrada[512];
        fgets(entrada, sizeof(entrada), stdin);

        if (strncmp(entrada, "sair", 4) == 0)
            break;

        // Prepara tokenização e parsing
        num_tokens = 0;
        pos = 0;
        tokenize(entrada);
        Expr *raiz = parse_expr();

        if (!raiz) {
            printf("Consulta inválida.\n");
            continue;
        }

        // Avalia a expressão
        SetNode *resultados = avaliar_expr(raiz, conjunto_total);

        if (resultados) {
            printf("Postagens encontradas:\n");
            exibir_postagens(arquivo, resultados);
            set_free(resultados);
        } else {
            printf("Nenhuma postagem encontrada.\n");
        }

        limpar_tokens_e_arvore(tokens, num_tokens, raiz);
    }

    set_free(conjunto_total);
    printf("Busca finalizada.\n");
    return 0;
}
