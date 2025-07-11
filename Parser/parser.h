#ifndef PARSER_H
#define PARSER_H

#define MAX_TOKENS 256
#define MAX_TOKEN_LEN 64

typedef enum {
    TOKEN_PALAVRA,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_ABRE_PARENTESE,
    TOKEN_FECHA_PARENTESE
} TokenTipo;

typedef struct {
    TokenTipo tipo;
    char valor[MAX_TOKEN_LEN]; // só usado para palavras
} Token;

// Função para tokenizar a string de entrada
int tokenizarExpressao(const char* expressao, Token tokens[], int maxTokens);

// Converte a lista de tokens para notação pós-fixa (RPN)
int converterParaRPN(Token tokens[], int numTokens, Token saida[], int maxTokens);

#endif // PARSER_H
