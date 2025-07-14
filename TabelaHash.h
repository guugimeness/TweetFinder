#ifndef TABELAHASH_H
#define TABELAHASH_H

#include "Set.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Estrutura que representa um nó da lista ligada para tratar colisões na tabela 
// hash
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
typedef struct Node{
    char* key;      
    Set* offsets;   // conjunto de endereços do arquivo onde a chave aparece
    struct Node *next;
} Node;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Estrutura que representa a tabela hash
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
typedef struct HashTable{
    int qtd;
    int TABLE_SIZE;
    Node **lists;       // array de ponteiros para o início de cada lista ligada
} HashTable;

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// HEADERS
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
HashTable* criaHashTable(int TABLE_SIZE);
void liberaHash(HashTable* ha);
int valorString(char *str);
int insereHashTable(HashTable* ha, Node *no);
Node* buscaHashTable(HashTable* ha, const char* key);

#endif // TABELAHASH_H