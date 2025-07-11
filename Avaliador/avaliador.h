#ifndef AVALIADOR_H
#define AVALIADOR_H

#include "../Parser/parser.h"
#include "../Set/set.h"

// Avalia a expressão RPN e retorna um Set com os docIDs que satisfazem a busca
Set* avaliarExpressaoRPN(Token rpn[], int numTokens, int totalDocs);

#endif // AVALIADOR_H
