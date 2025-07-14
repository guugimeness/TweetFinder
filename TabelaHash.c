#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TabelaHash.h"

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Cria uma tabela hash vazia com um tamanho especificado (TABLE_SIZE)
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
HashTable* criaHashTable(int TABLE_SIZE){
    HashTable* ha = (HashTable*) malloc(sizeof(HashTable));
    if(ha != NULL){
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->lists = malloc(TABLE_SIZE * sizeof(Node*));
        if(ha->lists == NULL){
            free(ha);
            return NULL;
        }
        ha->qtd = 0;
        for(int i=0; i < ha->TABLE_SIZE; i++)
            ha->lists[i] = NULL; // lista vazia no bucket
    }
    return ha;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Desaloca a memória associada à tabela hash
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void liberaHashTable(HashTable* ha){
    if(ha != NULL){
        int i;
        for(i=0; i < ha->TABLE_SIZE; i++){
            Node* curr = ha->lists[i];
            while(curr){
                Node* temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(ha->lists);
        free(ha);
    }
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Calcula um valor inteiro a partir de uma string (usado para gerar chaves hash)
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int valorString(char *str){
    int i, valor = 7;
    int tam = strlen(str);
    for(i=0; i < tam; i++)
        valor = 31 * valor + (int) str[i];
    return (valor & 0x7FFFFFFF);
}

///--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Insere um nó na tabela hash (tratamento de colisão por encadeamento)
// Retorna 1 em caso de sucesso, 0 caso contrário
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int insereHashTable(HashTable* ha, Node *no){
    if(ha == NULL || no == NULL)
        return 0;

    int chave = valorString(no->key);
    int pos = chaveDivisao(chave, ha->TABLE_SIZE);

    // Insere no início da lista
    no->next = ha->lists[pos];
    ha->lists[pos] = no;
    ha->qtd++;
    return 1;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Busca um nó na tabela hash com base na chave (string)
// Retorna o nó se encontrado, NULL caso contrário
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
Node* buscaHashTable(HashTable* ha, const char* key){
    if(ha == NULL)
        return 0;

    int i, pos, newPos, chave;

    chave = valorString((char*) key);
    pos = chaveDivisao(chave, ha->TABLE_SIZE);

    Node* curr = ha->lists[pos];
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return curr;
        }
        curr = curr->next;
    }
    return 0;
}