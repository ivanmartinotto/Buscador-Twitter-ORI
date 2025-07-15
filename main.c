#include "Indexador/indexador.h"
#include "Parser/parser.h"
#include "Avaliador/avaliador.h"
#include "Indice/indice_invertido.h"
#include "Set/set.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Eduardo Sanzovo Quaggio 813641
// Gustavo Cesar Bento Laurindo 821402
// Ivan Fernando Rizzi Villalba 821478

int main() {
    printf("Aguarde, indexando os dados.\n");
    inicializarIndice();

    const char* caminho = "corpus.csv";
    int totalDocs = processarCSV(caminho);
    printf("Total de postagens indexadas: %d\n", totalDocs);

    while (1) {
        char expressao[512];
        printf("\nDigite uma expressao (ou 'sair'): ");
        fgets(expressao, sizeof(expressao), stdin);
        if (strncmp(expressao, "sair", 4) == 0) break;

        Token tokens[256], rpn[256];
        int ntokens = tokenizarExpressao(expressao, tokens, 256);
        int nrpn = converterParaRPN(tokens, ntokens, rpn, 256);

        Set* resultado = avaliarExpressaoRPN(rpn, nrpn, totalDocs);

        if(resultado != NULL){
            printf("\nPostagens encontradas:\n");
            for (int i = 0; i < totalDocs; i++) {
                if (contemElemento(resultado, i)) {
                    exibirTextoOriginal(caminho, i);
                }
            }
        }

        liberarSet(resultado);
    }

    printf("Programa encerrado.\n");
    liberarIndice();
    return 0;
}

