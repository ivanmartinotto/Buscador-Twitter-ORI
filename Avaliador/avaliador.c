#include "avaliador.h"
#include "../Parser/parser.h"
#include "../Indice/indice_invertido.h"
#include <string.h>
#include <stdio.h>

#define MAX_PILHA 256

Set* avaliarExpressaoRPN(Token rpn[], int numTokens, int totalDocs) {
    Set* pilha[MAX_PILHA];
    int topo = -1;

    for (int i = 0; i < numTokens; i++) {
        Token token = rpn[i];

        if (token.tipo == TOKEN_PALAVRA) {
            // Busca a palavra na hash e cria um Set correspondente
            Set* s = getSetFromPalavra(token.valor, totalDocs);

            pilha[++topo] = s;
        } else if (token.tipo == TOKEN_NOT) {
            if (topo < 0) {
                fprintf(stderr, "Erro: NOT sem operando.\n");
                return NULL;
            }
            Set* op = pilha[topo--];
            Set* universo = universoSet(totalDocs);
            Set* result = diferenca(universo, op);
            liberarSet(op);
            liberarSet(universo);
            pilha[++topo] = result;
        } else if (token.tipo == TOKEN_AND || token.tipo == TOKEN_OR) {
            if (topo < 1) {
                fprintf(stderr, "Erro: operador binário com menos de 2 operandos.\n");
                return NULL;
            }
            Set* b = pilha[topo--];
            Set* a = pilha[topo--];
            Set* result = NULL;

            if (token.tipo == TOKEN_AND) {
                result = intersecao(a, b);
            } else if (token.tipo == TOKEN_OR) {
                result = uniao(a, b);
            }

            liberarSet(a);
            liberarSet(b);
            pilha[++topo] = result;
        }
    }

    if (topo != 0) {
        fprintf(stderr, "Erro: expressão mal formada.\n");
        return NULL;
    }

    return pilha[0]; // resultado final
}
