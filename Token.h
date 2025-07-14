#ifndef TOKEN_H
#define TOKEN_H

#include "Set.h"
#include "TabelaHash.h"

// Estrutura para tokenizar cada símbolo/palavra/operação da query
typedef struct {
    int tipo;         // 0 = palavra, 1 = (, 2 = ), 3 = NOT, 4 = AND, 5 = OR
    char* palavra;     // NULL para operadores e parênteses
    Set* set;         // Guarda resultado parcial da query
} token;

Set* avalia_expressao(token tokens[], int n);
int tokeniza(const char* entrada, token tokens[], int max_tokens);
int substitui_por_resultado(token tokens[], int n, int start, int end, Set* resultado);
int acha_parenteses_mais_interno(token tokens[], int n, int *start, int *end);
int tem_parenteses(token tokens[], int n);
Set* avalia_sem_parenteses(token tokens[], int n);
void associa_sets_aos_tokens(token tokens[], int n, HashTable* hash);

#endif // TOKEN_H
