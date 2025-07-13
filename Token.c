#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Set.h"          
#include "ArvoreAVL.h"   
#include "Token.h"

#define MAX_WORD_LEN 100

int tokeniza(const char* entrada, token tokens[], int max_tokens) {
    int i = 0; // índice do token
    int j = 0; // índice da string

    while (entrada[j] != '\0' && i < max_tokens) {
        // Ignora espaços manualmente
        if (entrada[j] == ' ') {
            j++;
            continue;
        }

        // Parênteses
        if (entrada[j] == '(') {
            tokens[i].tipo = 1;
            tokens[i].palavra = NULL;
            tokens[i].set = NULL;
            i++; j++;
            continue;
        }

        if (entrada[j] == ')') {
            tokens[i].tipo = 2;
            tokens[i].palavra = NULL;
            tokens[i].set = NULL;
            i++; j++;
            continue;
        }

        // Palavra ou operador (qualquer sequência de letras/números/_)
        if ((entrada[j] >= 'A' && entrada[j] <= 'Z') || 
            (entrada[j] >= 'a' && entrada[j] <= 'z') || 
            (entrada[j] >= '0' && entrada[j] <= '9') || 
            entrada[j] == '_') {
            
            char buffer[MAX_WORD_LEN];
            int k = 0;

            // Copia a palavra
            while ((entrada[j] >= 'A' && entrada[j] <= 'Z') || 
                   (entrada[j] >= 'a' && entrada[j] <= 'z') || 
                   (entrada[j] >= '0' && entrada[j] <= '9') || 
                   entrada[j] == '_') {
                if (k < MAX_WORD_LEN - 1) {
                    buffer[k++] = entrada[j];
                }
                j++;
            }
            buffer[k] = '\0';

            // Compara com os operadores (precisa estar em maiúsculo na entrada!)
            if (strcmp(buffer, "NOT") == 0) {
                tokens[i].tipo = 3;
                tokens[i].palavra = NULL;
            } else if (strcmp(buffer, "AND") == 0) {
                tokens[i].tipo = 4;
                tokens[i].palavra = NULL;
            } else if (strcmp(buffer, "OR") == 0) {
                tokens[i].tipo = 5;
                tokens[i].palavra = NULL;
            } else {
                tokens[i].tipo = 0;
                tokens[i].palavra = malloc(k + 1);
                if (tokens[i].palavra) {
                    for (int m = 0; m <= k; m++) {
                        tokens[i].palavra[m] = buffer[m];
                    }
                }
            }

            tokens[i].set = NULL;
            i++;
            continue;
        }
    }
    return i; // número de tokens
}

int substituiPorResultado(token tokens[], int n, int start, int end, Set* resultado) {
    tokens[start].tipo = 0;
    tokens[start].palavra = NULL;
    tokens[start].set = resultado;

    int nova_n = start + 1;
    for (int i = end + 1; i < n; i++) {
        tokens[nova_n++] = tokens[i];
    }
    return nova_n;
}

int achaParentesesMaisInterno(token tokens[], int n, int *start, int *end) {
    int ultimo_abre = -1;
    for (int i = 0; i < n; i++) {
        if (tokens[i].tipo == 1) ultimo_abre = i;
        if (tokens[i].tipo == 2 && ultimo_abre != -1) {
            *start = ultimo_abre;
            *end = i;
            return 1;
        }
    }
    return 0;
}

int temParenteses(token tokens[], int n) {
    for (int i = 0; i < n; i++) {
        if (tokens[i].tipo == 1 || tokens[i].tipo == 2) {
            return 1;
        }
    }  
    return 0;
}

Set* avaliaSemParenteses(token tokens[], int n) {
    Set* resultado = NULL;
    int i = 0;

    // inicializa resultado com o primeiro conjunto (pode ser palavra ou parêntese já substituído)
    if (n > 0 && tokens[0].tipo == 0) {
        resultado = tokens[0].set;
        i = 1;
    }

    // percorre os operadores restantes
    while (i < n) {
        // AND ou AND NOT
        if (tokens[i].tipo == 4) { // AND
            if (i + 2 < n && tokens[i + 1].tipo == 3 && tokens[i + 2].tipo == 0) {
                // A AND NOT B = A - B
                resultado = diferencaSet(resultado, tokens[i + 2].set);
                i += 3;
            } else {
                // A AND B
                resultado = resolveAND(resultado, tokens[i + 1].set);
                i += 2;
            }
        }
        // OR ou OR NOT
        else if (tokens[i].tipo == 5) { // OR
            if (i + 2 < n && tokens[i + 1].tipo == 3 && tokens[i + 2].tipo == 0) {
                Set* uni = uniaoSet(resultado, tokens[i + 2].set);
                resultado = diferencaSet(uni, tokens[i + 2].set);
                liberaSet(uni);
                i += 3;
            } else {
                // A OR B
                resultado = resolveOR(resultado, tokens[i + 1].set);
                i += 2;
            }
        } else {
            i++;
        }
    }

    return resultado;
}

Set* avaliaExpressao(token tokens[], int n) {
    int start, end;
    while (temParenteses(tokens, n)) {
        achaParentesesMaisInterno(tokens, n, &start, &end);
        int tamanho = end - start - 1;
        Set* resultado = avaliaSemParenteses(tokens + start + 1, tamanho);
        n = substituiPorResultado(tokens, n, start, end, resultado);
    }
    return avaliaSemParenteses(tokens, n);
}

void associaSetsAosTokens(token tokens[], int n, HashTable* hash) {
    for (int i = 0; i < n; i++) {
        if (tokens[i].tipo == 0 && tokens[i].palavra != NULL) {
            Node* no = buscaHashTable(hash, tokens[i].palavra);
            tokens[i].set = no ? no->offsets : criaSet();   // vazio se não achou
        }
    }
}
