// https://computinglife.wordpress.com/2008/11/20/why-do-Hash-functions-use-prime-numbers/
// http://stackoverflow.com/questions/2624192/good-Hash-function-for-strings

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TabelaHash.h"

HashTable* criaHashTable(int TABLE_SIZE){
    HashTable* ha = (HashTable*) malloc(sizeof(HashTable));
    if(ha != NULL){
        ha->TABLE_SIZE = TABLE_SIZE;
        ha->lists = malloc(TABLE_SIZE * sizeof(Node*));
        // ha->lists = (LinkedList list) malloc(TABLE_SIZE * sizeof(LinkedList list));
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

//==============================================
int valorString(char *str){
    int i, valor = 7;
    int tam = strlen(str);
    for(i=0; i < tam; i++)
        valor = 31 * valor + (int) str[i];
    return (valor & 0x7FFFFFFF);
}

//HashTable Divisao
//int chave = valorString(nome);
//pos = chaveDivisao(chave, TABLE_SIZE)
int chaveDivisao(int chave, int TABLE_SIZE){
    return (chave & 0x7FFFFFFF) % TABLE_SIZE;
}

//==============================================
//HashTable Dobra
//int chave = valorString(nome);
//pos = chaveDobra(chave, TABLE_SIZE)
int chaveDobra(int chave, int TABLE_SIZE){
    int num_bits = 10;
    int parte1 = chave >> num_bits;
    int parte2 = chave & (TABLE_SIZE-1);
    return (parte1 ^ parte2);
}

//==============================================
//HashTable Multiplicão
//int chave = valorString(nome);
//pos = chaveDobra(chave, TABLE_SIZE)
int chaveMultiplicacao(int chave, int TABLE_SIZE){
    double A = 0.6180339887; // constante: 0 < A < 1
    double val = chave * A;
    val = val - (int) val;
    return (int) (TABLE_SIZE * val);
}

//==============================================
// Insere e busca sem tratamento de colis�o
//==============================================
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

//==============================================
// Insere e busca com tratamento de colisão: Endereçamento Aberto
//==============================================
int sondagemLinear(int pos, int i, int TABLE_SIZE){
    return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE;
}

int sondagemQuadratica(int pos, int i, int TABLE_SIZE){
    pos = pos + 2*i + 5*i*i;// HashTable + (c1 * i) + (c2 * i^2)
    return (pos & 0x7FFFFFFF) % TABLE_SIZE;
}

int duploHashTable(int H1, int chave, int i, int TABLE_SIZE){
    int H2 = chaveDivisao(chave,TABLE_SIZE-1) + 1;
    return ((H1 + i*H2) & 0x7FFFFFFF) % TABLE_SIZE;
}

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
