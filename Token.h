#ifndef TOKEN_H
#define TOKEN_H

#include "Set.h"
#include "TabelaHash.h"

typedef struct {
    int tipo;         // 0 = palavra, 1 = (, 2 = ), 3 = NOT, 4 = AND, 5 = OR
    char* palavra;     // NULL para operadores e parênteses
    Set* set;         // Guarda resultado parcial da query
} token;

// Protótipos
Set* avaliaExpressao(token tokens[], int n);
int tokeniza(const char* entrada, token tokens[], int max_tokens);
int substituiPorResultado(token tokens[], int n, int start, int end, Set* resultado);
int achaParentesesMaisInterno(token tokens[], int n, int *start, int *end);
int temParenteses(token tokens[], int n);
Set* avaliaSemParenteses(token tokens[], int n);
void associaSetsAosTokens(token tokens[], int n, HashTable* hash);

#endif // TOKEN_H
