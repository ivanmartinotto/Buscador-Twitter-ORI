#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

int prioridade(TokenTipo tipo) {
    switch (tipo) {
        case TOKEN_NOT: return 3;
        case TOKEN_AND: return 2;
        case TOKEN_OR:  return 1;
        default: return 0;
    }
}

int ehOperador(TokenTipo tipo) {
    return tipo == TOKEN_AND || tipo == TOKEN_OR || tipo == TOKEN_NOT;
}

// Tokeniza a expressão booleana
int tokenizarExpressao(const char* expressao, Token tokens[], int maxTokens) {
    int numTokens = 0;
    const char* p = expressao;

    while (*p && numTokens < maxTokens) {
        if (isspace(*p)) {
            p++;
        } else if (*p == '(') {
            tokens[numTokens++] = (Token){TOKEN_ABRE_PARENTESE, "("};
            p++;
        } else if (*p == ')') {
            tokens[numTokens++] = (Token){TOKEN_FECHA_PARENTESE, ")"};
            p++;
        } else {
            char buffer[MAX_TOKEN_LEN];
            int len = 0;

            while (*p && !isspace(*p) && *p != '(' && *p != ')' && len < MAX_TOKEN_LEN - 1) {
                buffer[len++] = *p++;
            }
            buffer[len] = '\0';

            if (strcasecmp(buffer, "AND") == 0) {
                tokens[numTokens++] = (Token){TOKEN_AND, ""};
            } else if (strcasecmp(buffer, "OR") == 0) {
                tokens[numTokens++] = (Token){TOKEN_OR, ""};
            } else if (strcasecmp(buffer, "NOT") == 0) {
                tokens[numTokens++] = (Token){TOKEN_NOT, ""};
            } else {
                tokens[numTokens].tipo = TOKEN_PALAVRA;
                strncpy(tokens[numTokens].valor, buffer, MAX_TOKEN_LEN);
                numTokens++;
            }
        }
    }

    return numTokens;
}

// Conversão para notação pós-fixa usando Shunting Yard
int converterParaRPN(Token tokens[], int numTokens, Token saida[], int maxTokens) {
    Token pilha[MAX_TOKENS];
    int topo = -1;
    int saidaIdx = 0;

    for (int i = 0; i < numTokens; i++) {
        Token t = tokens[i];

        if (t.tipo == TOKEN_PALAVRA) {
            saida[saidaIdx++] = t;
        } else if (ehOperador(t.tipo)) {
            while (topo >= 0 && ehOperador(pilha[topo].tipo) &&
                   prioridade(pilha[topo].tipo) >= prioridade(t.tipo)) {
                saida[saidaIdx++] = pilha[topo--];
            }
            pilha[++topo] = t;
        } else if (t.tipo == TOKEN_ABRE_PARENTESE) {
            pilha[++topo] = t;
        } else if (t.tipo == TOKEN_FECHA_PARENTESE) {
            while (topo >= 0 && pilha[topo].tipo != TOKEN_ABRE_PARENTESE) {
                saida[saidaIdx++] = pilha[topo--];
            }
            if (topo >= 0 && pilha[topo].tipo == TOKEN_ABRE_PARENTESE) {
                topo--; // descarta o parêntese
            }
        }
    }

    while (topo >= 0) {
        saida[saidaIdx++] = pilha[topo--];
    }

    return saidaIdx;
}
